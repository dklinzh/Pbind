//
//  PBFormController.h
//  Pbind
//
//  Created by galen on 15/4/11.
//  Copyright (c) 2015年 galen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PBForm.h"
#import "PBViewController.h"

@interface PBFormController : PBViewController<PBFormDelegate>

@property (nonatomic, strong) PBForm *form;

@end
