/**
 @file			NSString.h
 @brief         能够转换utf8编码用的，从js传入的符号必须通过这个类转换才好用，否则@ ＃ 这个都不认
 @author		wyw
 @version		1.0
 @date			2012_12_17
 @company       JoyChina
 */
#import <Foundation/Foundation.h>

@interface NSString (OAURLEncodingAdditions)


- (NSString *)URLEncodedString;


- (NSString *)URLDecodedString;


@end
