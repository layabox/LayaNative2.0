#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@protocol JCIapProcessCtrlDelegate <NSObject>
@required
- (void)onSuccess:(NSString *)p_pProductionIdentifier quantity:(NSInteger)p_iNumber orderID:(NSString *)p_pszOrderID;
- (void)onFailed:(NSString *)p_pProductionIdentifier quantity:(NSInteger)p_iNumber orderID:(NSString *)p_pszOrderID;
@end

@interface IAPManager : NSObject<SKProductsRequestDelegate,SKPaymentTransactionObserver>
+ (IAPManager*) getInstance;
-(void)recharge:(NSString*) jsonParam;
- (id)initWithGameID:(NSString*)gameID andDelegate:(id<JCIapProcessCtrlDelegate>)delegate;
@end


