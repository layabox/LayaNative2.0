#import "NavViewController.h"

@implementation NavViewController

- (void)viewDidLoad{
    [super viewDidLoad];
    self.navigationBar.hidden = TRUE;
}
- (BOOL)shouldAutorotate{
    return [[self.viewControllers lastObject]shouldAutorotate];
}
- (UIInterfaceOrientationMask)supportedInterfaceOrientations{
    return [[self.viewControllers lastObject]supportedInterfaceOrientations];
    //return [conchConfig GetInstance]->m_nOrientationType;
}
- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation{
    return [[self.viewControllers lastObject] preferredInterfaceOrientationForPresentation];
}
@end
