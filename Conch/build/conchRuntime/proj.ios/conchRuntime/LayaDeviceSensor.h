
#import<CoreMotion/CoreMotion.h>

@interface LayaDeviceSensor : NSObject

+ (instancetype) GetInstance;
- (instancetype) init;
- (void) stop;
- (void) start;
- (void) active:(BOOL) active;
@end
