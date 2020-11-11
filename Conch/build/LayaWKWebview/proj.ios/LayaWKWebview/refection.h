/**
 @file
 @brief
 @author
 @version		1.0
 @date
 @company       LayaBox
 */
#import <objc/NSObject.h>
#import <objc/objc.h>
#import <Foundation/NSString.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSInvocation.h>
#import "LayaWKWebview.h"

@interface Reflection : NSObject
-(id)init:(LayaWKWebview*)webview;
-(void)clearReflectionObjects;
-(NSString*)callMethod:(int)objid className:(NSString*)cls methodName:(NSString*)method param:(NSString*)param;
-(void)callbackToJSWithClass:(Class)cls methodName:(NSString*)name ret:(NSObject*)retObj;
-(void)callbackToJSWithClassName:(NSString*)cls methodName:(NSString*)name ret:(NSObject*)retObj;
-(void)callbackToJSWithObject:(id)obj methodName:(NSString*)name ret:(NSObject*)retObj;
@end
