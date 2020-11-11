
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface UIFloatPanel : UIImageView
{
    GLKView* m_pGLView;
    UIButton* m_pRefreshBtn;
    UIButton* m_pBackBtn;
}
-(id)initWithParentView:(GLKView*)pGLView;
@end



