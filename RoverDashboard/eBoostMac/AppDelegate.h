//
//  AppDelegate.h
//  eBoostMac
//
//  Created by Bosko Petreski on 11/7/13.
//  Copyright (c) 2013 Bosko Petreski. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "BLE.h"

@interface AppDelegate : NSObject <NSApplicationDelegate,BLEDelegate,NSTableViewDataSource,NSTableViewDelegate>{
    IBOutlet NSButton *btnConnect;

    BLE *bleShield;
    NSMutableArray *arrayPeripetals;
    
    IBOutlet NSTableView *tblDevices;
    IBOutlet NSWindow *listDevices;
    
    IBOutlet NSImageView *carArrow;
}

@property (assign) IBOutlet NSWindow *window;

@end
