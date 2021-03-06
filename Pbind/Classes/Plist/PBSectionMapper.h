//
//  PBSectionMapper.h
//  Pbind
//
//  Created by galen on 15/2/17.
//  Copyright (c) 2015年 galen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "PBRowMapper.h"

@interface PBSectionMapper : PBRowMapper

@property (nonatomic, strong) NSString *title;

/**
 Hide the last cell's separator view.
 
 @discussion this is only used for a grouped PBTableView. We've done something tricky here:
 In the selector `cellForRowAtIndexPath', we check if the index path is the last row of the section.
 If it is, then find the subview who's frame is bottom to the cell, and set it's alpha to 0.
 */
@property (nonatomic, assign) BOOL hidesLastSeparator;

/**
 The mappers for each row(UITableViewCell) of the section.
 
 @discussion each element will be parsed from NSDictionay to PBRowMapper.
 */
@property (nonatomic, strong) NSArray *rows;

/**
 The data for the section.
 */
@property (nonatomic, strong) id data;

/**
 The distinct row(UITableViewCell) mapper for the section.
 
 @discussion the dictionary here will be parsed to a PBRowMapper.
 */
@property (nonatomic, strong) id row;

/**
 The empty row used to create a placeholder cell to display while the section data is nil.
 
 @discussion the dictionary here will be parsed to a PBRowMapper.
 */
@property (nonatomic, strong) id emptyRow;

/**
 The footer view of the section.
 
 @discussion the dictionary here will be parsed to a PBRowMapper.
 */
@property (nonatomic, strong) id footer;

/**
 The row count for the section.
 
 @discussion
 
 - If rows was specified, return the rows count
 - If row was specified, return the data count
 - If emptyRow was specified and the data is empty, return 1
 */
@property (nonatomic, assign, readonly) NSInteger rowCount;

#pragma mark - UICollectionView

/**
 The distinct item(UICollectionViewCell) mapper for the section.
 
 @discussion the dictionary here will be parsed to a PBRowMapper.
 */
@property (nonatomic, strong) id item;

/**
 The mappers for each item(UICollectionViewCell) of the section.
 
 @discussion each element will be parsed from NSDictionay to PBRowMapper.
 */
@property (nonatomic, strong) NSArray *items;

/**
 The section inset.
 
 @discussion this is set to following method in the UICollectionViewDelegateLayout delegate:
 
 - collectionView:layout:insetForSectionAtIndex:
 
 */
@property (nonatomic, assign) UIEdgeInsets inset;

/**
 The minimum inter item spacing size.
 
 @discussion this is set to following method in the UICollectionViewDelegateLayout delegate:
 
 - width -> collectionView:layout:minimumInteritemSpacingForSectionAtIndex:
 - height -> collectionView:layout:collectionViewLayoutminimumLineSpacingForSectionAtIndex:
 
 */
@property (nonatomic, assign) CGSize inner;

@end
