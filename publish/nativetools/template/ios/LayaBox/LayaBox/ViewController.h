#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <conchRuntime.h>

@interface ViewController : GLKViewController
{
@public
    
    GLKView*                    m_pGLKView;
    
    EAGLContext*                m_pGLContext;
    
    conchRuntime*               m_pConchRuntime;
}
+(ViewController*)GetIOSViewController;
-(id)init;
@end

