
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@interface LayaScreenShotter : NSObject

+ (instancetype)sharedInstance;

- (void)getScreenImageWithCompletionBlock:(void(^)(UIImage *screenImage))completionBlock;
+ (unsigned char *) convertUIImageToBitmapRGBA8:(UIImage *) image length:(int*)len;
@end
