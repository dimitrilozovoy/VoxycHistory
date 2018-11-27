//
//  ViewController.m
//  Voxyc
//
//  Created by Dimitri Lozovoy on 11/26/18.
//
//

#import "ViewController.h"
#import "../../../../app/main.h"

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
    
    [self initGL];
}

- (void)initGL {
    [EAGLContext setCurrentContext:self.context];
    [((GLKView *) self.view) bindDrawable];
    
    StartApp(0);
}

- (void)shutdownGL {
    [EAGLContext setCurrentContext:self.context];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
//    glClearColor(0, 1, 0, 1);
//    glClear(GL_COLOR_BUFFER_BIT);

    AppTick();
    Draw(0);
}

@end
