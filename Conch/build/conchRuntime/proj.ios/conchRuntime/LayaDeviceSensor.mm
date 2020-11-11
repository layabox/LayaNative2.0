
#import "LayaDeviceSensor.h"
#import <CoreMotion/CMGyro.h>
#import "../../../../source/conch/JCScriptRuntime.h"

static const double UPDATE_INTERVAL = 1.0 / 60.0;
static const double G = 9.80665;
void handleDeviceMotionEvent(float ax, float ay, float az, float agx, float agy, float agz, float ra, float rb, float rg, float interval)
{
    DeviceMotionEvent e;
    e.nType = E_DEVICEMOTION;
    e.ax = ax;
    e.ay = ay;
    e.az = az;
    e.agx = agx;
    e.agy = agy;
    e.agz = agz;
    e.ra = ra;
    e.rb = rb;
    e.rg = rg;
    e.interval = interval;
    strncpy(e.type, "devicemotion", 256);
    JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
}
double rad2deg(double rad)
{
return (rad * 180 * M_1_PI);
}
void handleDeviceOrientationEvent(double pitch, double roll, double yaw)
{
    // Compose the raw motion data to an intermediate ZXY-based 3x3 rotation
    // matrix (R) where [z=attitude.yaw, x=attitude.pitch, y=attitude.roll]
    // in the form:
    //
    //   /  R[0]   R[1]   R[2]  \
    //   |  R[3]   R[4]   R[5]  |
    //   \  R[6]   R[7]   R[8]  /
    
    double cX = cos(pitch);
    double cY = cos(roll);
    double cZ = cos(yaw);
    double sX = sin(pitch);
    double sY = sin(roll);
    double sZ = sin(yaw);
    
    double R[] = {
        cZ * cY - sZ * sX * sY,
        - cX * sZ,
        cY * sZ * sX + cZ * sY,
        cY * sZ + cZ * sX * sY,
        cZ * cX,
        sZ * sY - cZ * cY * sX,
        - cX * sY,
        sX,
        cX * cY
    };
    
    // Compute correct, normalized values for DeviceOrientation from
    // rotation matrix (R) according to the angle conventions defined in the
    // W3C DeviceOrientation specification.
    
    double zRot;
    double xRot;
    double yRot;
    
    if (R[8] > 0) {
        zRot = atan2(-R[1], R[4]);
        xRot = asin(R[7]);
        yRot = atan2(-R[6], R[8]);
    } else if (R[8] < 0) {
        zRot = atan2(R[1], -R[4]);
        xRot = -asin(R[7]);
        xRot += (xRot >= 0) ? -M_PI : M_PI;
        yRot = atan2(R[6], -R[8]);
    } else {
        if (R[6] > 0) {
            zRot = atan2(-R[1], R[4]);
            xRot = asin(R[7]);
            yRot = -M_PI_2;
        } else if (R[6] < 0) {
            zRot = atan2(R[1], -R[4]);
            xRot = -asin(R[7]);
            xRot += (xRot >= 0) ? -M_PI : M_PI;
            yRot = -M_PI_2;
        } else {
            zRot = atan2(R[3], R[0]);
            xRot = (R[7] > 0) ? M_PI_2 : -M_PI_2;
            yRot = 0;
        }
    }

    // Rotation around the Z axis (pointing up. normalized to [0, 360] deg).
    double alpha = rad2deg(zRot > 0 ? zRot : (M_PI * 2 + zRot));
    
    if (alpha >= 0 && alpha <= 270){
        alpha += 90;
    }
    else{
        alpha -= 270;
    }
    
    // Rotation around the X axis (top to bottom).
    double beta  = rad2deg(xRot);
    // Rotation around the Y axis (side to side).
    double gamma = rad2deg(yRot);
    
    
    //NSLog(@"device orientation  alpha[%f] beta[%f] gama[%f] ",alpha,beta,gamma);
    
    DeviceOrientationEvent e;
    e.nType = E_DEVICEORIENTATION;
    e.ra = alpha;
    e.rb = beta;
    e.rg = gamma;
    strncpy(e.type, "deviceorientation", 256);
    JCScriptRuntime::s_JSRT->dispatchInputEvent(e);
}

@interface LayaDeviceSensor()
@property(nonatomic,strong) CMMotionManager *motionManager;
@property(nonatomic,strong) NSTimer* updateTimer;
@property(nonatomic,assign) BOOL active;
@end

@implementation LayaDeviceSensor

+ (instancetype) GetInstance
{
    static LayaDeviceSensor *_instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[LayaDeviceSensor alloc] init];
    });
    return _instance;
}

- (instancetype) init
{
    if( (self = [super init]) ) {
        self.motionManager = [[CMMotionManager alloc] init];
        self.active = NO;
    }
    return self;
}
- (void) active:(BOOL) active
{
    self.active = active;
}

- (void) start
{
    if (!self.active || self.updateTimer != nil)
        return;

    if (self.motionManager.deviceMotionAvailable){
        self.motionManager.deviceMotionUpdateInterval = UPDATE_INTERVAL;
        [self.motionManager startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXTrueNorthZVertical];
    }
    else{
        NSLog(@"This device do not support deviceMotion");
    }
    self.updateTimer = [NSTimer scheduledTimerWithTimeInterval:UPDATE_INTERVAL target:self selector:@selector(update) userInfo:nil repeats:YES];
}

- (void) stop
{
    if (!self.active || self.updateTimer == nil)
        return;
    
    [self.motionManager stopDeviceMotionUpdates];
    [self.updateTimer invalidate];
    self.updateTimer = nil;
}

- (void) update
{
    if (self.motionManager.deviceMotionAvailable){
        
        double ax = self.motionManager.deviceMotion.userAcceleration.x * G;
        double ay = self.motionManager.deviceMotion.userAcceleration.y * G;
        double az = self.motionManager.deviceMotion.userAcceleration.z * G;
        double agx = (self.motionManager.deviceMotion.userAcceleration.x + self.motionManager.deviceMotion.gravity.x) * G;
        double agy = (self.motionManager.deviceMotion.userAcceleration.y + self.motionManager.deviceMotion.gravity.y) * G;
        double agz = (self.motionManager.deviceMotion.userAcceleration.z + self.motionManager.deviceMotion.gravity.z) * G;
        double ra = self.motionManager.deviceMotion.rotationRate.x * 180 * M_1_PI;
        double rb = self.motionManager.deviceMotion.rotationRate.y * 180 * M_1_PI;
        double rg = self.motionManager.deviceMotion.rotationRate.z * 180 * M_1_PI;
        double interval = 1000 * UPDATE_INTERVAL;
    
        
        //NSLog(@"acceleration x[%f] y[%f] z[%f]",ax,ay,az);
        //NSLog(@"accelerationIncludingGravity x[%f] y[%f] z[%f]",agx,agy,agz);
        //NSLog(@"rotationRate alpha[%f] beta[%f] gama[%f]",ra,rb,rg);
        //NSLog(@"interval [%f]",interval);
        
        handleDeviceMotionEvent( ax,  ay,  az,  agx,  agy,  agz,  ra,  rb,  rg,  interval);
        
        
        handleDeviceOrientationEvent(self.motionManager.deviceMotion.attitude.pitch,self.motionManager.deviceMotion.attitude.roll,self.motionManager.deviceMotion.attitude.yaw);
    }
}
@end

