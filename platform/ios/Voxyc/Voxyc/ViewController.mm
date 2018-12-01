//
//  ViewController.m
//  Voxyc
//
//  Created by Dimitri Lozovoy on 11/26/18.
//
//

#import "ViewController.h"
#import "../../../../app/main.h"
#import "../../../../engine/Engine2.h"
#import "IOSGUI.h"

@interface ViewController () {
}

@property (strong, nonatomic) EAGLContext *context;

@end

@implementation ViewController

@synthesize context = _context;

- (void)dealloc
{
    [self shutdownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)viewDidLoad {
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to initialize OpenGL ES context");
        exit(1);
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    g_viewController = self;
    g_iosGUI = [[IOSGUI alloc] init];
    
    [self initGL];
}

- (void)initGL {
    [EAGLContext setCurrentContext:self.context];
    [((GLKView *) self.view) bindDrawable];
    
    StartApp(0);
    LoadApp(0);
}

- (void)shutdownGL {
    [EAGLContext setCurrentContext:self.context];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    AppTick();
    Draw(0);
}

- (void) touchesBegan:(NSSet *)touches
            withEvent:(UIEvent *)event {
    NSUInteger touchCount = [touches count];
    NSUInteger tapCount = [[touches anyObject] tapCount];
    
    NSArray *arrTouches = [touches allObjects];
    
    CGFloat x1 = 0;
    CGFloat y1 = 0;
    CGFloat x2 = 0;
    CGFloat y2 = 0;
    
    for (int i = 0; i < touchCount; i++)
    {
        UITouch *touch = [arrTouches objectAtIndex:i];
        CGPoint point = [touch locationInView:self.view];
        
        if (i == 0)
        {
            x1 = point.x;
            y1 = point.y;
        }
        if (i == 1)
        {
            x2 = point.x;
            y2 = point.y;
        }
    };
    
    g_engine2->touchEvent((int)touchCount, 1, x1, y1, 1, x2, y2, -1);
}

- (void) touchesMoved:(NSSet *)touches
            withEvent:(UIEvent *)event {
    NSUInteger touchCount = [touches count];
    NSUInteger tapCount = [[touches anyObject] tapCount];
    
    NSArray *arrTouches = [touches allObjects];
    
    CGFloat x1 = 0;
    CGFloat y1 = 0;
    CGFloat x2 = 0;
    CGFloat y2 = 0;
    
    for (int i = 0; i < touchCount; i++)
    {
        UITouch *touch = [arrTouches objectAtIndex:i];
        CGPoint point = [touch locationInView:self.view];
        
        if (i == 0)
        {
            x1 = point.x;
            y1 = point.y;
        }
        if (i == 1)
        {
            x2 = point.x;
            y2 = point.y;
        }
    };
    
    g_engine2->touchEvent((int)touchCount, 3, x1, y1, 3, x2, y2, -1);
}

- (void) touchesEnded:(NSSet *)touches
            withEvent:(UIEvent *)event {
    NSUInteger touchCount = [touches count];
    NSUInteger tapCount = [[touches anyObject] tapCount];
    
    NSArray *arrTouches = [touches allObjects];
    
    CGFloat x1 = 0;
    CGFloat y1 = 0;
    CGFloat x2 = 0;
    CGFloat y2 = 0;
    
    for (int i = 0; i < touchCount; i++)
    {
        UITouch *touch = [arrTouches objectAtIndex:i];
        CGPoint point = [touch locationInView:self.view];
        
        if (i == 0)
        {
            x1 = point.x;
            y1 = point.y;
        }
        if (i == 1)
        {
            x2 = point.x;
            y2 = point.y;
        }
    };
    
    g_engine2->touchEvent((int)touchCount, 4, x1, y1, 4, x2, y2, -1);
}

@end
