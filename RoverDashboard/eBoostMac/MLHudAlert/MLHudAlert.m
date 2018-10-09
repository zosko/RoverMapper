#import "MLHudAlert.h"
#import <QuartzCore/QuartzCore.h>

@interface MLHudAlert()

@property (nonatomic, assign) MLHudAlertType alertType;
@property (nonatomic, strong) IBOutlet NSImageView *iconView;
@property (nonatomic, strong) IBOutlet NSTextField *messageField;
@property (nonatomic, strong) IBOutlet NSProgressIndicator *loadingIndicator;

@end

@implementation MLHudAlert

#define kWindowAlpha 0.86
#define kCornerRadius 13


static MLHudAlert *_staticHudAlert;

+ (void)alertWithWindow:(NSWindow *)aWindow type:(MLHudAlertType)type message:(NSString *)message {
    if (!_staticHudAlert) {
        _staticHudAlert = [[self alloc] init];
    }
    
    NSString *iconName = @"MLHudAlertInfo";
    _staticHudAlert.alertType = type;
    switch (type) {
        case MLHudAlertTypeError:
            iconName = @"MLHudAlertError";
            break;
        case MLHudAlertTypeInfo:
            iconName = @"MLHudAlertInfo";
            break;
        case MLHudAlertTypeSuccess:
            iconName = @"MLHudAlertSuccess";
            break;
        case MLHudAlertTypeWarn:
            iconName = @"MLHudAlertWarn";
            break;
        default:
            break;
    }
    
    if (type == MLHudAlertTypeLoading) {
        [_staticHudAlert.iconView setHidden:YES];
        [_staticHudAlert.loadingIndicator setHidden:NO];
        [_staticHudAlert.loadingIndicator startAnimation:nil];
    }
    else {
        [_staticHudAlert.loadingIndicator setHidden:YES];
        [_staticHudAlert.iconView setHidden:NO];
        _staticHudAlert.iconView.image = [NSImage imageNamed:iconName];
    }
    
    _staticHudAlert.messageField.stringValue = message ?: @"";
    
    // Add to parent window and show it
    if (_staticHudAlert.window.parentWindow) {
        [_staticHudAlert.window.parentWindow removeChildWindow:_staticHudAlert.window];
    }
    [aWindow addChildWindow:_staticHudAlert.window ordered:NSWindowAbove];
    [_staticHudAlert showWindow:aWindow];
}

+ (void) dismisWindow {
    [_staticHudAlert dismiss:nil];
}

- (void) updateWindowPosition {
    if (!self.window.parentWindow) {
        return;
    }
    NSRect parentRect = self.window.parentWindow.frame;
    [self.window setFrameOrigin:NSMakePoint(NSMidX(parentRect) - NSWidth(self.window.frame) / 2, NSMidY(parentRect) - NSHeight(self.window.frame) / 2)];
}

- (void) showWindow:(id)sender {
    self.window.alphaValue = 0;
    [super showWindow:sender];
    [self updateWindowPosition];
    [[self.window animator] setAlphaValue:kWindowAlpha];
    
    if (self.alertType != MLHudAlertTypeLoading) {
        [dismissTimer invalidate];
        dismissTimer = [NSTimer scheduledTimerWithTimeInterval:2 target:self selector:@selector(dismiss:) userInfo:nil repeats:NO];
        
        if (clickEvent) {
            [NSEvent removeMonitor:clickEvent];
        }
        clickEvent = [NSEvent addLocalMonitorForEventsMatchingMask:(NSLeftMouseDownMask | NSRightMouseDownMask) handler:^NSEvent *(NSEvent *e) {
            if (!NSPointInRect(NSEvent.mouseLocation, self.window.frame)) {
                if (clickEvent) {
                    [NSEvent removeMonitor:clickEvent];
                    clickEvent = nil;
                }

                [self dismiss:nil];
            }
            return e;
        }];
    }    
}

- (void) dismiss: (id) sender {
    [[self.window animator] setAlphaValue:0.0];
}

- (id)init {
    self = [super init];
    if (self) {
        //[self loadNibNamed:@"MLHudAlert" owner:self];
        [[NSBundle mainBundle] loadNibNamed:@"MLHudAlert" owner:self topLevelObjects:nil];
    }
    return self;
}


- (void)awakeFromNib {
    [(MLHudAlertWindow *)self.window setCornerRadius:kCornerRadius];
    [self.messageField setTextColor:[NSColor whiteColor]];
    [self.messageField.cell setBackgroundStyle:NSBackgroundStyleLowered];
    [self.iconView setImageScaling:NSImageScaleAxesIndependently];
}

- (void)mouseDown:(NSEvent *)theEvent {
    if (self.alertType != MLHudAlertTypeLoading) {
        [self dismiss:nil];
    }
}

@end
