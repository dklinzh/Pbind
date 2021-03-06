//
//  Pbind+API.h
//  Pbind
//
//  Created by Galen Lin on 16/8/31.
//  Copyright © 2016年 galen. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface Pbind : NSObject

/**
 The width pixel of the sketch provided by UI designer.
 */
+ (void)setSketchWidth:(CGFloat)sketchWidth;

/**
 The scaled value calculated by the scale of `sketchWidth' and current device screen width.
 */
+ (CGFloat)valueScale;

/**
 Add a resources bundle who contains `*.plist', `*.xib', `*.png'.
 
 @discussion: The added bundle will be inserted to the top of `allResourcesBundles', 
 so the resources inside it will consider to be load by first order.
 
 */
+ (void)addResourcesBundle:(NSBundle *)bundle;

/**
 Returns all the loaded resources bundles.
 */
+ (NSArray<NSBundle *> *)allResourcesBundles;

/**
 Reload all the views who are using the `plist`.

 @param plistPath the path of the plist
 */
+ (void)reloadViewsOnPlistUpdate:(NSString *)plist;

/**
 Reload all the views who are using the API(`action`).

 @param action the action of the API
 */
+ (void)reloadViewsOnAPIUpdate:(NSString *)action;

@end

/**
 Calculate the scaled value.
 
 @discussion Round the decimal value as follow:
 
 * <= 0.3   downto  0
 * >= 0.7   upto    1
 * 0.4-0.6  as      0.5
 
 */
UIKIT_STATIC_INLINE CGFloat PBValueByScale(CGFloat value, CGFloat scale) {
    if (value == 0) {
        return 0;
    }
    
    value *= scale;
    if (value < 0) {
        return value;
    }
    
    int integer = (int) value;
    int decimal = (value - integer) * 10;
    if (decimal <= 3) {
        return integer;
    } else if (decimal >= 7) {
        return integer + 1.f;
    } else {
        return integer + .5f;
    }
}

UIKIT_STATIC_INLINE CGFloat PBValue2(CGFloat value, CGFloat sketchWidth) {
    CGFloat scale = [Pbind valueScale];
    if (sketchWidth != 0) {
        scale = [UIScreen mainScreen].bounds.size.width / sketchWidth;
    }
    return PBValueByScale(value, scale);
}

UIKIT_STATIC_INLINE CGFloat PBValue(CGFloat value) {
    return PBValueByScale(value, [Pbind valueScale]);
}

UIKIT_STATIC_INLINE CGPoint PBPoint(CGPoint point) {
    return CGPointMake(PBValue(point.x),
                       PBValue(point.y));
}

UIKIT_STATIC_INLINE CGPoint PBPoint2(CGPoint point, CGFloat sketchWidth) {
    return CGPointMake(PBValue2(point.x, sketchWidth),
                       PBValue2(point.y, sketchWidth));
}

UIKIT_STATIC_INLINE CGPoint PBPointMake(CGFloat x, CGFloat y) {
    return CGPointMake(PBValue(x), PBValue(y));
}

UIKIT_STATIC_INLINE CGSize PBSize(CGSize size) {
    return CGSizeMake(PBValue(size.width),
                      PBValue(size.height));
}

UIKIT_STATIC_INLINE CGSize PBSize2(CGSize size, CGFloat sketchWidth) {
    return CGSizeMake(PBValue2(size.width, sketchWidth),
                      PBValue2(size.height, sketchWidth));
}

UIKIT_STATIC_INLINE CGSize PBSizeMake(CGFloat w, CGFloat h) {
    return CGSizeMake(PBValue(w), PBValue(h));
}

UIKIT_STATIC_INLINE CGRect PBRect(CGRect rect) {
    return CGRectMake(PBValue(rect.origin.x),
                      PBValue(rect.origin.y),
                      PBValue(rect.size.width),
                      PBValue(rect.size.height));
}

UIKIT_STATIC_INLINE CGRect PBRect2(CGRect rect, CGFloat sketchWidth) {
    return CGRectMake(PBValue2(rect.origin.x, sketchWidth),
                      PBValue2(rect.origin.y, sketchWidth),
                      PBValue2(rect.size.width, sketchWidth),
                      PBValue2(rect.size.height, sketchWidth));
}

UIKIT_STATIC_INLINE CGRect PBRectMake(CGFloat x, CGFloat y, CGFloat w, CGFloat h) {
    return CGRectMake(PBValue(x), PBValue(y), PBValue(w), PBValue(h));
}

UIKIT_STATIC_INLINE UIEdgeInsets PBEdgeInsets(UIEdgeInsets insets) {
    return UIEdgeInsetsMake(PBValue(insets.top),
                            PBValue(insets.left),
                            PBValue(insets.bottom),
                            PBValue(insets.right));
}

UIKIT_STATIC_INLINE UIEdgeInsets PBEdgeInsets2(UIEdgeInsets insets, CGFloat sketchWidth) {
    return UIEdgeInsetsMake(PBValue2(insets.top, sketchWidth),
                            PBValue2(insets.left, sketchWidth),
                            PBValue2(insets.bottom, sketchWidth),
                            PBValue2(insets.right, sketchWidth));
}

UIKIT_STATIC_INLINE UIEdgeInsets PBEdgeInsetsMake(CGFloat t, CGFloat l, CGFloat b, CGFloat r) {
    return UIEdgeInsetsMake(PBValue(t), PBValue(l), PBValue(b), PBValue(r));
}

UIKIT_STATIC_INLINE NSURL *PBResourceURL(NSString *resource, NSString *extension) {
    NSArray *preferredBundles = [Pbind allResourcesBundles];
    for (NSBundle *bundle in preferredBundles) {
        NSURL *url = [bundle bundleURL];
        if ([url isFileURL]) {
            // If the resource is in file system, while something recently added to the bundle,
            // the bundle would not recognize it, which means:
            //      [bundle URLForResource:resource withExtension:extension]
            // return nil.
            // So we had to manually check if the file exists and return the correct URL.
            NSString *name = [NSString stringWithFormat:@"%@.%@", resource, extension];
            NSString *path = [[bundle bundlePath] stringByAppendingPathComponent:name];
            if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
                return [NSURL URLWithString:name relativeToURL:url];
            }
        } else {
            url = [bundle URLForResource:resource withExtension:extension];
            if (url != nil) {
                return url;
            }
        }
    }
    return nil;
}

UIKIT_STATIC_INLINE NSDictionary *PBPlist(NSString *plistName) {
    NSURL *url = PBResourceURL(plistName, @"plist");
    if (url == nil) {
        return nil;
    }
    return [NSDictionary dictionaryWithContentsOfURL:url];
}

/**
 Find the image with name in any resources bundles.

 @param imageName the image name
 @return the first found image
 */
UIKIT_STATIC_INLINE UIImage *PBImage(NSString *imageName) {
    if (imageName == nil) {
        return nil;
    }
    
    NSArray *preferredBundles = [Pbind allResourcesBundles];
    UIImage *image = nil;
    for (NSBundle *bundle in preferredBundles) {
        image = [UIImage imageNamed:imageName inBundle:bundle compatibleWithTraitCollection:nil];
        if (image != nil) {
            break;
        }
    }
    return image;
}

/**
 Get the localized string for key in any resources bundles.

 @param key the key in the default string table: `Localizable.strings`
 @return the first found localized string for the key
 */
UIKIT_STATIC_INLINE NSString *PBLocalizedString(NSString *key) {
    if (key == nil) {
        return nil;
    }
    
    NSArray *preferredBundles = [Pbind allResourcesBundles];
    NSString *localeString = nil;
    for (NSBundle *bundle in preferredBundles) {
        localeString = [bundle localizedStringForKey:key value:nil table:nil];
        if (localeString != nil) {
            break;
        }
    }
    return localeString;
}

UIKIT_STATIC_INLINE UINib *PBNib(NSString *nibName) {
    if (nibName == nil) {
        return nil;
    }
    
    NSArray *preferredBundles = [Pbind allResourcesBundles];
    UINib *nib = nil;
    for (NSBundle *bundle in preferredBundles) {
        if ([bundle pathForResource:nibName ofType:@"nib"] != nil) {
            nib = [UINib nibWithNibName:nibName bundle:bundle];
            break;
        }
    }
    return nib;
}

/**
 Look up the visible controller from the given controller

 @param controller current controller

 @return the visible controller
 */
UIKIT_STATIC_INLINE UIViewController *PBVisibleController(UIViewController *controller) {
    UIViewController *presentedController = [controller presentedViewController];
    if (presentedController != nil) {
        return PBVisibleController(presentedController);
    }
    
    if ([controller isKindOfClass:[UINavigationController class]]) {
        return PBVisibleController([(id)controller topViewController]);
    }
    
    if ([controller isKindOfClass:[UITabBarController class]]) {
        return PBVisibleController([(id)controller selectedViewController]);
    }
    
    return controller;
}

/**
 Look up the visible controller for the application

 @return the top visible controller
 */
UIKIT_STATIC_INLINE UIViewController *PBTopController() {
    UIViewController *rootController = [[UIApplication sharedApplication].delegate window].rootViewController;
    return PBVisibleController(rootController);
}
