//
//  PBExpression.h
//  Pbind
//
//  Created by galen on 15/2/26.
//  Copyright (c) 2015年 galen. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 This class is used to parse the expressions configure in Plist.
 
 @discussion Each expression is composed by `[binding flag][unary op][tag][key][multi op][right value]`.
 
 The [flag] is optional, supports:
 
 * '='      -> oneway binding, notifys view to update while the data source value changed
 * '=='     -> duplex binding, notifys each other to update on the other value changed.
 
 If was not defined, just map the data to the view.
 
 The [unary op] is optional, supports:
 
 * '!'      -> logic not
 * '-'      -> negative
 * '~'      -> animated while setting the value for the key of the owner
 
 The [tag] is required, declares the binding data source, supports:
 
 * '$'      -> value for the key of the fetching data
 * '$[0-9]' -> value for the key of the fetching data's element at the index
 * '.'      -> value for the key of the owner view
 * '.$'     -> value for the key of the owner view data
 * '@^'     -> value for the key of the owner view controller
 * '@xx.'   -> value for the key of the alias view (by [view setAlias:@"xx"])
 * '@>'     -> value for the key of the owner form
 * '>'      -> text for the owner form's key-named input
 * '>$'     -> value for the owner form's key-named input
 * '>!'     -> error of the owner form's key-named input
 * '#.'     -> the action state of the default action store
 * '#$'     -> the action state data of the default action store
 
 The [multi op] is optional, supports:
 
 * '+', '-', '*', '/'                       -> arithmetic operators
 * '>', '<', '=', '==', '>=', '<=', '!='    -> comparision operators
 * '?:', '? * : *'                          -> conditional operators
 
 The [right value] is optional, follows by [multi op] only, and only supports constant now.
 
 */
@interface PBExpression : NSObject
{
    struct {
        // unary operators
        unsigned int unaryNot:2;    // '!', '!!' before variable
        unsigned int negative:1;    // '-' before variable
        unsigned int animated:1;    // '~'
        
        // arithmetic operators
        unsigned int plus:1;        // '+'
        unsigned int minus:1;       // '-' after variable
        unsigned int times:1;       // '*'
        unsigned int divide:1;      // '/'
        
        // conditional operators
        unsigned int test:1;        // '?'
        unsigned int unaryTest:1;   // '?:'
        
        // flags
        unsigned int onewayBinding:1;   // '='
        unsigned int duplexBinding:1;   // '=='
        
        // comparision operators
        unsigned int lesser:1;      // '<'
        unsigned int greater:1;     // '>'
        unsigned int equal:2;       // '=', '=='
        
        unsigned int multiNot:1;    // '!' after variable

        // tags
        unsigned int mapToData:1;               // '$'
        unsigned int mapToOwnerView:1;          // '.'
        unsigned int mapToOwnerViewData:1;      // '.$'
        
        unsigned int mapToFormFieldText:1;      // '>'
        unsigned int mapToFormFieldValue:1;     // '>$'
        unsigned int mapToFormFieldError:1;     // '>!'
        unsigned int mapToForm:1;               // '@>'
        
        unsigned int mapToActiveController:1;   // '@^'
        unsigned int mapToAliasView:1;          // '@alias.'
        
        unsigned int mapToActionState:1;        // '#.'
        unsigned int mapToActionStateData:1;    // '#$' <==> '#.data'
        
        unsigned int dataTag:8;                 // '0-9' for multi data, other for user-defined tag. Default is 0xFF(unset).
    } _flags;
    
    NSString *_format;
    NSString *_variable;    // the key of the data source
    NSString *_rvalue;      // right value, accepts constants only
    NSString *_rvalueForTrue; // for `?:' expression, value after '?', accepts constants only
    NSString *_alias;
    
    NSString *_bindingKeyPath;  // the owner's keyPath binding with `_variable'
    __unsafe_unretained id _bindingOwner;
    __unsafe_unretained id _bindingData;
}

+ (instancetype)expressionWithString:(NSString *)aString;

/**
 The parent expression, usually is an instance of PBMutableExpression.
 
 @discussion On binding mode, use to notify the parent expression re-calculated.
 
 Example: suppose current expression is `$name` and parent is `%(hello %@!),$name`, 
 if the `$name` changed to be 'someone', then the parent value should be 'hello someone!'.
 */
@property (nonatomic, strong) PBExpression *parent;

- (instancetype)initWithString:(NSString *)aString;

- (id)valueWithData:(id)data;
- (id)valueWithData:(id)data target:(id)target;
- (id)valueWithData:(id)data target:(id)target context:(UIView *)context;
- (id)valueWithData:(id)data keyPath:(NSString *)keyPath target:(id)target context:(UIView *)context;

- (void)bindData:(id)data toTarget:(id)target forKeyPath:(NSString *)targetKeyPath inContext:(UIView *)context;
- (void)mapData:(id)data toTarget:(id)target forKeyPath:(NSString *)targetKeyPath inContext:(UIView *)context;

/**
 The expression text, for debugger output.
 */
- (NSString *)stringValue;

/**
 Unobserve the target.

 @param target the observed target
 */
- (void)unbind:(id)target forKeyPath:(NSString *)keyPath;

@end
