//
//  YKScreenshotter.m
//  ScreenshotTest

#import "LayaScreenShotter.h"


@interface LayaScreenShotter ()

@property (nonatomic, strong) dispatch_queue_t screenImgQueue;

@end

@implementation LayaScreenShotter

+ (instancetype)sharedInstance
{
    static LayaScreenShotter *_instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[LayaScreenShotter alloc] init];
    });
    return _instance;
}

- (instancetype)init
{
    if (self = [super init]) {
        self.screenImgQueue = dispatch_queue_create("LayaScreenShotter.queue", DISPATCH_QUEUE_SERIAL);
        dispatch_set_target_queue(self.screenImgQueue, dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_HIGH, 0));
    }
    return self;
}

- (void)getScreenImageWithCompletionBlock:(void (^)(UIImage *))completionBlock
{
    dispatch_async(self.screenImgQueue, ^{
    
        CGSize screenSize = [UIApplication sharedApplication].delegate.window.bounds.size;
        CGFloat screenScale = [UIScreen mainScreen].scale;
        
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();

        CGContextRef bitmapContext = CGBitmapContextCreate(NULL, screenSize.width * screenScale, screenSize.height * screenScale, 8, screenSize.width * screenScale * 4, colorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
        CGContextScaleCTM(bitmapContext, screenScale, screenScale);
        CGAffineTransform flipVertical = CGAffineTransformMake(1, 0, 0, -1, 0, screenSize.height);
        CGContextConcatCTM(bitmapContext, flipVertical);
        
        dispatch_semaphore_t sema = dispatch_semaphore_create(0);
        dispatch_async(dispatch_get_main_queue(), ^{
    
            UIGraphicsPushContext(bitmapContext);
            
            for (UIWindow *window in [UIApplication sharedApplication].windows) {
                [window drawViewHierarchyInRect:CGRectMake(0.0, 0.0, screenSize.width, screenSize.height) afterScreenUpdates:NO];
            }
            
            UIGraphicsPopContext();
            dispatch_semaphore_signal(sema);
        });
        dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER);
    
        CGImageRef imgRef = CGBitmapContextCreateImage(bitmapContext);
        UIImage *screenImage = [UIImage imageWithCGImage:imgRef];
        
        CGImageRelease(imgRef);
        CGContextRelease(bitmapContext);
        CGColorSpaceRelease(colorSpace);
        
        if (completionBlock) {
            dispatch_async(dispatch_get_main_queue(), ^{
                completionBlock(screenImage);
            });
        }
    });
    
}
+ (unsigned char *) convertUIImageToBitmapRGBA8:(UIImage *) image length:(int*)len {
    
    CGImageRef imageRef = image.CGImage;
    
    // Create a bitmap context to draw the uiimage into
    CGContextRef context = [self newBitmapRGBA8ContextFromImage:imageRef];
    
    if(!context) {
        return NULL;
    }
    
    size_t width = CGImageGetWidth(imageRef);
    size_t height = CGImageGetHeight(imageRef);
    
    CGRect rect = CGRectMake(0, 0, width, height);
    
    // Draw image into the context to get the raw image data
    CGContextDrawImage(context, rect, imageRef);
    
    // Get a pointer to the data
    unsigned char *bitmapData = (unsigned char *)CGBitmapContextGetData(context);
    
    // Copy the data and release the memory (return memory allocated with new)
    size_t bytesPerRow = CGBitmapContextGetBytesPerRow(context);
    size_t bufferLength = bytesPerRow * height;
    
    unsigned char *newBitmap = NULL;
    
    if(bitmapData) {
        *len = sizeof(unsigned char) * bytesPerRow * height;
        newBitmap = new unsigned char[sizeof(unsigned char) * bytesPerRow * height];
        
        if(newBitmap) {	// Copy the data
            for(int i = 0; i < bufferLength; ++i) {
                newBitmap[i] = bitmapData[i];
            }
        }
        
        free(bitmapData);
        
    } else {
        NSLog(@"Error getting bitmap pixel data\n");
    }
    
    CGContextRelease(context);
    
    return newBitmap;	
}

+ (CGContextRef) newBitmapRGBA8ContextFromImage:(CGImageRef) image {
    CGContextRef context = NULL;
    CGColorSpaceRef colorSpace;
    uint32_t *bitmapData;
    
    size_t bitsPerPixel = 32;
    size_t bitsPerComponent = 8;
    size_t bytesPerPixel = bitsPerPixel / bitsPerComponent;
    
    size_t width = CGImageGetWidth(image);
    size_t height = CGImageGetHeight(image);
    
    size_t bytesPerRow = width * bytesPerPixel;
    size_t bufferLength = bytesPerRow * height;
    
    colorSpace = CGColorSpaceCreateDeviceRGB();
    
    if(!colorSpace) {
        NSLog(@"Error allocating color space RGB\n");
        return NULL;
    }
    
    // Allocate memory for image data
    bitmapData = (uint32_t *)malloc(bufferLength);
    
    if(!bitmapData) {
        NSLog(@"Error allocating memory for bitmap\n");
        CGColorSpaceRelease(colorSpace);
        return NULL;
    }
    
    //Create bitmap context
    context = CGBitmapContextCreate(bitmapData,
                                    width,
                                    height,
                                    bitsPerComponent,
                                    bytesPerRow,
                                    colorSpace,
                                    kCGImageAlphaPremultipliedLast);	// RGBA
    
    if(!context) {
        free(bitmapData);
        NSLog(@"Bitmap context not created");
    }
    
    CGColorSpaceRelease(colorSpace);
    
    return context;	
}

@end
