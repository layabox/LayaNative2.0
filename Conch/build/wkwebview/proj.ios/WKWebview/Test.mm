#import <Foundation/Foundation.h>
#import "Test.h"
#import <LayaWKWebview.h>
@implementation Test

+(void)test:(NSString*)json
{
    NSError* error = nil;
    NSData* jsonData = [json dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary* dict = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&error];
    NSLog(@"OC: %@", [dict objectForKey:@"value"]);
    {
        NSDictionary* dic = [NSDictionary dictionaryWithObject:@"Hello JS!" forKey:@"value"];
        NSData* jsonData = [NSJSONSerialization dataWithJSONObject:dic options:0 error:&error];
        NSString* jsonStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
        [[LayaWKWebview GetLayaWKWebview] callbackToJSWithClass:self.class methodName:@"test:" ret:jsonStr];
    }
}


@end
