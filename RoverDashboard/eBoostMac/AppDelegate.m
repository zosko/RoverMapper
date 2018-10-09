//
//  AppDelegate.m
//  eBoostMac
//
//  Created by Bosko Petreski on 11/7/13.
//  Copyright (c) 2013 Bosko Petreski. All rights reserved.
//

#import "AppDelegate.h"
#import "MLHudAlert.h"
#import <Quartz/Quartz.h>

@implementation NSBezierPath (BezierPathQuartzUtilities)
// This method works only in OS X v10.2 and later.
- (CGPathRef)quartzPath
{
    int i, numElements;
    
    // Need to begin a path here.
    CGPathRef           immutablePath = NULL;
    
    // Then draw the path elements.
    numElements = [self elementCount];
    if (numElements > 0)
    {
        CGMutablePathRef    path = CGPathCreateMutable();
        NSPoint             points[3];
        BOOL                didClosePath = YES;
        
        for (i = 0; i < numElements; i++)
        {
            switch ([self elementAtIndex:i associatedPoints:points])
            {
                case NSMoveToBezierPathElement:
                    CGPathMoveToPoint(path, NULL, points[0].x, points[0].y);
                    break;
                    
                case NSLineToBezierPathElement:
                    CGPathAddLineToPoint(path, NULL, points[0].x, points[0].y);
                    didClosePath = NO;
                    break;
                    
                case NSCurveToBezierPathElement:
                    CGPathAddCurveToPoint(path, NULL, points[0].x, points[0].y,
                                          points[1].x, points[1].y,
                                          points[2].x, points[2].y);
                    didClosePath = NO;
                    break;
                    
                case NSClosePathBezierPathElement:
                    CGPathCloseSubpath(path);
                    didClosePath = YES;
                    break;
            }
        }
        
        // Be sure the path is closed or Quartz may not do valid hit detection.
        if (!didClosePath)
            CGPathCloseSubpath(path);
        
        immutablePath = CGPathCreateCopy(path);
        CGPathRelease(path);
    }
    
    return immutablePath;
}
@end

@implementation AppDelegate{
    double timerCompass;
    double previousX,previousY;
}

-(void)applicationDidFinishLaunching:(NSNotification *)aNotification{
    bleShield = [[BLE alloc] init];
    [bleShield controlSetup];
    bleShield.delegate = self;
    timerCompass = [NSDate date].timeIntervalSince1970;
    
    timerCompass = NSDate.date.timeIntervalSince1970;
    
    previousX = self.window.frame.size.width / 2;
    previousY = self.window.frame.size.height / 2;
}
#pragma mark - TableViewDelegates
-(NSInteger)numberOfRowsInTableView:(NSTableView *)tableView{
    return arrayPeripetals.count;
}
-(id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
    CBPeripheral *p = arrayPeripetals[row];
    return @{@"name":[NSString stringWithFormat:@"%@",p.name],@"uuid":[NSString stringWithFormat:@"%@",p.identifier.UUIDString]}[tableColumn.identifier];
}

#pragma mark - IBActions
-(IBAction)onBtnSearchDevices:(id)sender{
    [btnConnect setTitle:@"Searching"];
    arrayPeripetals = [NSMutableArray new];
    if (bleShield.activePeripheral){
        [btnConnect setTitle:@"Disconnect"];
        [[bleShield CM] cancelPeripheralConnection:[bleShield activePeripheral]];
        return;
    }
    if (bleShield.peripherals){
        bleShield.peripherals = nil;
    }
    [bleShield findBLEPeripherals:3];
    [NSTimer scheduledTimerWithTimeInterval:3.0 target:self selector:@selector(connectionTimer:) userInfo:nil repeats:NO];
}
-(IBAction)onBtnConnectDevice:(id)sender{
    [listDevices orderOut:nil];
    [NSApp endSheet:listDevices];
    
    [bleShield connectPeripheral:arrayPeripetals[[tblDevices selectedRow]]];
}

#pragma mark - Custom Functions
-(void)connectionTimer:(NSTimer *)timer{
    if(bleShield.peripherals.count > 0){
        for(int i=0;i<bleShield.peripherals.count;i++){
            CBPeripheral *p = [bleShield.peripherals objectAtIndex:i];
            [arrayPeripetals addObject:p];
        }
        [tblDevices reloadData];
        [NSApp beginSheet:listDevices modalForWindow:self.window modalDelegate:self didEndSelector:NULL contextInfo:nil];
    }
    else{
        [NSApp beginSheet:listDevices modalForWindow:self.window modalDelegate:self didEndSelector:NULL contextInfo:nil];
        [MLHudAlert alertWithWindow:self.window type:MLHudAlertTypeWarn message:@"Devices not found"];
    }
}
-(void)sendDataToBLE:(NSString *)str{
    NSLog(@"SEND: %@",str);
    NSString *sendData = [NSString stringWithFormat:@"%@", str];
    NSData *data = [sendData dataUsingEncoding:NSUTF8StringEncoding];
    [bleShield write:data];
}

#pragma mark - BLE Delegates
-(void)bleDidReceiveData:(unsigned char *)data length:(int)length{
    NSData *dataRecv = [NSData dataWithBytes:data length:length];
    NSString *stringRecv = [[NSString alloc] initWithData:dataRecv encoding:NSUTF8StringEncoding];
    
//    if([NSDate date].timeIntervalSince1970 - timerCompass < 1){
//        return;
//    }
    timerCompass = [NSDate date].timeIntervalSince1970;
    
    printf("%s",stringRecv.UTF8String);
    
    if([stringRecv containsString:@"c"]){
        //[self drawColision];
    }
    else{
        [self drawRoverPoint:[stringRecv doubleValue]];
    }
}
-(void)bleDidDisconnect{
    [btnConnect setTitle:@"Search Device"];
}
-(void)bleDidConnect{
    [btnConnect setTitle:@"Disconnect"];
}
-(void)bleDidUpdateRSSI:(NSNumber *)rssi{
    
}

#pragma mark - CustomFunction
-(double) degreesToRadians:(double)degrees{
    return (degrees * M_PI) / 180;
}
-(void)drawColision{
    CAShapeLayer *shapeLayer = [CAShapeLayer layer];
    shapeLayer.strokeColor = [[NSColor redColor] CGColor];
    shapeLayer.fillColor = [[NSColor redColor] CGColor];
    [shapeLayer setPath:[[NSBezierPath bezierPathWithOvalInRect:CGRectMake(previousX, previousY, 5, 5)] quartzPath]];
    [self.window.contentView.layer addSublayer:shapeLayer];
}
-(void)drawRoverPoint:(double)degrees{
    int step = 1;
    [carArrow setFrameRotation:degrees];
    NSBezierPath *path = [NSBezierPath bezierPath];
    [path moveToPoint:CGPointMake(previousX, previousY)];
    
    previousX = previousX + (step * cos([self degreesToRadians:degrees]));
    previousY = previousY + (step * sin([self degreesToRadians:degrees]));
    
    [path lineToPoint:CGPointMake(previousX,previousY)];
    
    CAShapeLayer *shapeLayer = [CAShapeLayer layer];
    shapeLayer.path = [path quartzPath];
    shapeLayer.strokeColor = [[NSColor redColor] CGColor];
    shapeLayer.lineWidth = 2;
    shapeLayer.fillColor = [[NSColor clearColor] CGColor];
    [self.window.contentView.layer addSublayer:shapeLayer];
    [[self.window contentView] setWantsLayer:YES];
}

@end
