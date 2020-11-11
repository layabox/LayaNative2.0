#import "IAPManager.h"
#define __MAX_GAME_ID_SIZE     32

typedef void (^IAPProductsResponseBlock)(SKProductsRequest* request , SKProductsResponse* response);
typedef void (^IAPcheckReceiptCompleteResponseBlock)(NSString* response,NSError* error);

@interface IAPManager()<NSURLConnectionDataDelegate>
@property (nonatomic,copy) IAPProductsResponseBlock requestProductsBlock;
@property (nonatomic,copy) IAPcheckReceiptCompleteResponseBlock checkReceiptCompleteBlock;
@property (nonatomic,assign) id<JCIapProcessCtrlDelegate> delegate;
@property (nonatomic,strong) NSString* gameAppID;
@property (nonatomic,strong) SKProductsRequest *request;
@property (nonatomic,strong) NSString *strPlateform;
@property (nonatomic,assign) BOOL isProcessing;
@property (nonatomic,strong) NSMutableData* receiptRequestData;
@end


@implementation IAPManager

+ (IAPManager*) getInstance{
    static IAPManager* iap = nil;
    if (iap == nil){
        iap = [IAPManager alloc];
    }
    return iap;
}

- (id)initWithGameID:(NSString*)gameID andDelegate:(id<JCIapProcessCtrlDelegate>)delegate{
    if ((self = [super init])) {
        NSAssert(gameID != nil,@"[IAP] gameID can not be nil");
        NSAssert(gameID.length <= __MAX_GAME_ID_SIZE,@"gameID is too long");
        self.gameAppID = [NSString stringWithString:gameID];
        NSAssert(delegate != nil, @"[IAP] delegate can not be nil");
        self.delegate = delegate;
        self.strPlateform = [NSString stringWithFormat:@"%@,%@,%@", [[UIDevice currentDevice] model], [[UIDevice currentDevice] systemName],[[UIDevice currentDevice] systemVersion]];
        NSLog(@"[IAP]plateform info: %@\n", self.strPlateform);
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        self.isProcessing = NO;
    }
    return self;
}

- (void)dealloc
{
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
}

- (void)requestProducts:(NSSet *)productIdentifiers onCompletion:(IAPProductsResponseBlock)completion {
    self.request = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    self.request.delegate = self;
    self.requestProductsBlock = completion;
    [self.request start];
   
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    self.request = nil;
    if(self.requestProductsBlock) {
        self.requestProductsBlock (request,response);
    }
}
//-------------------------------------------------------------------------------
- (void)completeTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"[IAP] SKPaymentTransactionStatePurchased");
    [self doVerifyTransaction:transaction];
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"[IAP] SKPaymentTransactionStateRestored");
    [self doVerifyTransaction:transaction];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"[IAP] Transaction error: %@ %ld", transaction.error.localizedDescription,(long)transaction.error.code);
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    NSLog(@"[IAP] SKPaymentTransactionStateFailed");
    NSMutableDictionary *paymentmulti =(NSMutableDictionary *)[NSKeyedUnarchiver unarchiveObjectWithData:transaction.payment.requestData];
    NSString* _order_id = [paymentmulti objectForKey:@"order_id"];
    self.isProcessing = NO;
    [self.delegate onFailed:transaction.payment.productIdentifier quantity:transaction.payment.quantity orderID:_order_id];
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
            default:
                break;
        }
    }
}

//-------------------------------------------------------------------------------
-(void)recharge:(NSString*) jsonParam
{
    if (self.isProcessing == YES){
        self.isProcessing = NO;
        NSLog(@"[IAP] another payments is processing!\n");
        return;
    }

    self.isProcessing = YES;
    
    
    NSLog(@"[IAP] this is MarketAppStore method pay param=%@",jsonParam);
    if(jsonParam==NULL)
    {
        self.isProcessing = NO;
        NSLog(@"[IAP] recharge param is nil!\n");
        [self.delegate onFailed:@"nill" quantity:0 orderID:@"nill"];
        return;
    }
    NSData* pJsonData = [jsonParam dataUsingEncoding:NSUTF8StringEncoding];
    NSError* pError;
    NSDictionary* pJson = [NSJSONSerialization JSONObjectWithData:pJsonData options:kNilOptions error:&pError];
    if( !pError )
    {
        NSString* sProductID = [pJson objectForKey:@"product_id"];
        int nNum = [[pJson objectForKey:@"amount"] intValue];
        NSString* sOrderID = [pJson objectForKey:@"order_id"];
        NSString* sCallbackURL = [pJson objectForKey:@"callback_uri"];
        NSLog(@"[IAP] buyProps: product_id=%@, amount=%d, order_id=%@,callbackurl=%@",sProductID, nNum, sOrderID,sCallbackURL );
        
        if( nNum <= 0 || nil == sProductID || nil == sOrderID || nil == sCallbackURL)
        {
            self.isProcessing = NO;
            NSLog(@"[IAP] param error!\n");
            [self.delegate onFailed:sProductID quantity:nNum orderID:sOrderID];
            return;
        }
        
        if([SKPaymentQueue canMakePayments] == NO)
        {
            self.isProcessing = NO;
            NSLog(@"[IAP] can not make payments!\n");
            [self.delegate onFailed:sProductID quantity:nNum orderID:sOrderID];
            return;
        }
        
        NSArray* transactions = [SKPaymentQueue defaultQueue].transactions;
        if (transactions.count > 0)
        {
            //检测是否有未完成的交易
            SKPaymentTransaction* transaction = [transactions firstObject];
            if (transaction.transactionState == SKPaymentTransactionStatePurchased)
            {
                NSLog(@"[IAP] process uncompleteed transaction");
                [self doVerifyTransaction:transaction];
            }
        }

        NSLog(@"[IAP] 开始向苹果请求产品信息");
        [self requestProducts:[[NSSet alloc] initWithObjects:sProductID, nil] onCompletion:^(SKProductsRequest* request,SKProductsResponse* response)
         {
             NSLog(@"[IAP] 苹果返回产品信息。");
             if(response.products.count > 0 ) {
                 
                 SKProduct* product =response.products[0];
                 NSMutableString *resOutput = [[NSMutableString alloc] init];
                 [resOutput appendString:@"[IAP] IAP Response Info\n"];
                 [resOutput appendFormat:@"%@,%@,%@,%@,%@\n",
                  product.localizedTitle,         // 本地化标题
                  product.localizedDescription,   // 本地化描述
                  product.price,                  // 价格
                  product.priceLocale,            // 地域
                  product.productIdentifier];     // 产品标识符
                 NSLog(@"%@",resOutput);
                 
                 SKMutablePayment *payment = [SKMutablePayment paymentWithProduct:product];
                 
                 
                 NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
                 [dict setObject:sOrderID forKey:@"order_id"];
                 [dict setObject:sCallbackURL forKey:@"backurl"];
                 [dict setObject:self.gameAppID forKey:@"gameid"];
                 NSData* pData = [NSKeyedArchiver archivedDataWithRootObject:dict];
                 
                 payment.requestData = pData;
                 payment.quantity = nNum;

                 [[SKPaymentQueue defaultQueue] addPayment:payment];
             }
             else{
                 self.isProcessing = NO;
                 NSLog(@"[IAP] invalid product!\n");
                 [self.delegate onFailed:sProductID quantity:nNum orderID:sOrderID];
             }
             // 错误的产品 ID 回馈
             NSMutableString *resOutput = [[NSMutableString alloc] init];
             for( NSString *invalidProductId in response.invalidProductIdentifiers )
             {
                 [resOutput appendFormat:@"[IAP] Invalid product id: %@\n",invalidProductId];
             }
             NSLog(@"%@",resOutput);
         }];
    }
    else
    {
        self.isProcessing = NO;
        NSLog(@"[IAP] buyProps:parser json error");
        [self.delegate onFailed:@"nill" quantity:0 orderID:@"nill"];
    }
}

// 向验证服务器发起验证请求
- (void)doVerifyTransaction: (SKPaymentTransaction *)transaction
{
    if (transaction == nil)
        return;
    
    
    if(NULL==transaction.payment.requestData)
    {
        self.isProcessing = NO;
        NSLog(@"[IAP] transDoVerifyTransaction  requestData is null");
        [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
        [self.delegate onFailed:@"nill" quantity:0 orderID:@"nill"];
        return;
    }
    NSMutableDictionary *paymentmulti =(NSMutableDictionary *)[NSKeyedUnarchiver unarchiveObjectWithData:transaction.payment.requestData];//
    NSString* _order_id = [paymentmulti objectForKey:@"order_id"];
    NSString* _backurl = [paymentmulti objectForKey:@"backurl"];
    NSString* _gameid = [paymentmulti objectForKey:@"gameid"];
    NSLog(@"[IAP] order:%@backurl:%@gameid:%@",_order_id,_backurl,_gameid);
    
    NSString* pszBase64 = [transaction.transactionReceipt base64Encoding];
    if(NULL==_order_id||NULL==_backurl||NULL==_gameid||NULL==self.strPlateform||NULL==transaction.payment.productIdentifier||NULL==transaction.transactionIdentifier)
    {
        self.isProcessing = NO;
        NSLog(@"[IAP] doVerifyTransaction something is NULL");
        [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
        [self.delegate onFailed:@"nill" quantity:0 orderID:@"nill"];
        return;
    }

    NSMutableString *sendParam = [[NSMutableString alloc] init];
    [sendParam appendFormat:@"plateform=%@",self.strPlateform];
    [sendParam appendFormat:@"&order-id=%@",_order_id];
    [sendParam appendFormat:@"&game-id=%@",_gameid];
    [sendParam appendFormat:@"&production-id=%@",transaction.payment.productIdentifier];
    [sendParam appendFormat:@"&transaction-id=%@",transaction.transactionIdentifier];
    [sendParam appendFormat:@"&receipt-data=%@",pszBase64];
    NSData* data = [sendParam dataUsingEncoding:NSUTF8StringEncoding];
    NSLog(@"[IAP] pruchase backurl =  %@",_backurl);
    NSLog(@"[IAP] pruchase param =  %@",sendParam);

    NSInteger amount = transaction.payment.quantity;
    [self checkReceipt:_backurl withData:data onCompletion:^(NSString* response,NSError* error){
        if (error == nil){
            
            NSLog(@"[IAP] web result =  %@",response);
            NSData* pJsonData = [response dataUsingEncoding:NSUTF8StringEncoding];
            NSError* pError;
            NSDictionary* pJson = [NSJSONSerialization JSONObjectWithData:pJsonData options:kNilOptions error:&pError];
            if( !pError ){
                NSString* resultCode = [pJson objectForKey:@"error-code"];
                int nCode = [resultCode intValue];
                if (nCode == 0){
                    NSLog(@"[IAP] verify success!");
                    self.isProcessing = NO;
                    [self.delegate onSuccess:transaction.payment.productIdentifier quantity:amount orderID:_order_id];

                }
                else{
                    NSLog(@"[IAP] verify err code = %@ info = %@",resultCode,[pJson objectForKey:@"error-info"]);
                    self.isProcessing = NO;
                    [self.delegate onFailed:transaction.payment.productIdentifier quantity:amount orderID:_order_id];
                }
                
            }
            else{
                self.isProcessing = NO;
                NSLog(@"[IAP] An error happened while serializing the JSON data.");
                [self.delegate onFailed:transaction.payment.productIdentifier quantity:amount orderID:_order_id];
            }
            [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
        }
        else{
            self.isProcessing = NO;
            [self.delegate onFailed:transaction.payment.productIdentifier quantity:amount orderID:_order_id];
        }
    }];
}
- (void)checkReceipt:(NSString*)url withData:(NSData*)receiptData onCompletion:(IAPcheckReceiptCompleteResponseBlock)completion
{
    self.checkReceiptCompleteBlock = completion;
    NSURL* requestURL = [NSURL URLWithString:url];
    NSMutableURLRequest* req = [[NSMutableURLRequest alloc] initWithURL:requestURL];
    [req setHTTPMethod:@"POST"];
    [req setHTTPBody:receiptData];
    
    NSURLConnection *conn = [[NSURLConnection alloc] initWithRequest:req delegate:self];
    if(conn) {
        self.receiptRequestData = [[NSMutableData alloc] init];
    } else {
        NSError* error = nil;
        NSMutableDictionary* errorDetail = [[NSMutableDictionary alloc] init];
        [errorDetail setValue:@"Can't create connection" forKey:NSLocalizedDescriptionKey];
        error = [NSError errorWithDomain:@"IAPManagerError" code:100 userInfo:errorDetail];
        if(self.checkReceiptCompleteBlock) {
            self.checkReceiptCompleteBlock(nil,error);
        }
    }
}

-(void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
    NSLog(@"[IAP] Cannot transmit receipt data. %@",[error localizedDescription]);
    if(self.checkReceiptCompleteBlock) {
        self.checkReceiptCompleteBlock(nil,error);
    }
}

-(void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response {
    [self.receiptRequestData setLength:0];
}

-(void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
    [self.receiptRequestData appendData:data];
}

-(void)connectionDidFinishLoading:(NSURLConnection *)connection {
    NSString *response = [[NSString alloc] initWithData:self.receiptRequestData encoding:NSUTF8StringEncoding];
    if(self.checkReceiptCompleteBlock) {
        self.checkReceiptCompleteBlock(response,nil);
    }
}
@end
