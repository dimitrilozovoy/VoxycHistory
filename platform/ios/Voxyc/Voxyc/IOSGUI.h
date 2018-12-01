//
//  IOSGUI.hpp
//  Voxyc
//
//  Created by Dimitri Lozovoy on 11/29/18.
//
//

#ifndef IOSGUI_hpp
#define IOSGUI_hpp

#import <Foundation/Foundation.h>
#import "GLKit/GLKit.h"
#import <string>
#import <vector>

typedef struct IOSDialogPart
{
    std::string caption = "";
    std::string value = "";
    std::string extraKey = "";
};

@interface IOSGUI <UITableViewDelegate> : NSObject

- (void) clearListMenu;
- (void) addListMenuOption:(std::string) title withDesc:(std::string) desc;
- (void) showListMenuInDialog:(NSString*) title withOptions:(NSString*) options;

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView;
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section;
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath;
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath;

@end

extern IOSGUI *g_iosGUI;
extern GLKViewController *g_viewController;

#endif /* IOSGUI_hpp */
