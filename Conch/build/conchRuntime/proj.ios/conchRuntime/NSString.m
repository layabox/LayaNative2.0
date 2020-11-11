/**
 @file			NSString.m
 @brief
 @author		wyw
 @version		1.0
 @date			2012_12_17
 @company       JoyChina
 */
#import "NSString.h"

@implementation NSString (OAURLEncodingAdditions)

- (NSString *)URLEncodedString
{
    NSString *result = (__bridge_transfer NSString *)CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault,
                                                                           (__bridge CFStringRef)self,
                                                                           NULL,
                                                                           CFSTR("!*'();:@&=+$,/?%#[]"),
                                                                           kCFStringEncodingUTF8);
    return result;
}

- (NSString*)URLDecodedString
{
    NSString *result = (__bridge_transfer NSString *)CFURLCreateStringByReplacingPercentEscapesUsingEncoding(kCFAllocatorDefault,
                                                                                           (__bridge CFStringRef)self,
                                                                                           CFSTR(""),
                                                                                           kCFStringEncodingUTF8);
    return result;
}
@end