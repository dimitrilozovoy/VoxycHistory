//
//  IOSGUI.cpp
//  Voxyc
//
//  Created by Dimitri Lozovoy on 11/29/18.
//
//

#import "IOSGUI.h"
#import "Globals.hpp"

IOSGUI *g_iosGUI;
GLKViewController *g_viewController;

@implementation IOSGUI

UIAlertController* alert;

std::vector<std::string> listMenu;
bool listMenuShown = false;

std::vector<IOSDialogPart> dialogParts;

- (void) clearListMenu
{
        listMenu.clear();
}

- (void) addListMenuOption:(std::string) title withDesc:(std::string) desc
{
    listMenu.push_back(title);
}


- (void) showListMenuInDialog:(NSString*) title withOptions:(NSString*) options
{
    [self addListMenuOption:"Cancel" withDesc:""];
    
    alert = [UIAlertController alertControllerWithTitle:title
                                                                   message:@"Menu"
                                                            preferredStyle:UIAlertControllerStyleAlert];
    
//    UITableView *alertTableView;
    
//    CGRect rect;
//    rect = CGRectMake(0, 0, 272, 422);

//    [alert setPreferredContentSize:rect.size];
    
//    alertTableView  = [[UITableView alloc]initWithFrame:rect];
//    alertTableView.delegate = self;
//    alertTableView.dataSource = self;
//    alertTableView.tableFooterView = [[UIView alloc]initWithFrame:CGRectZero];
//    [alertTableView setSeparatorStyle:UITableViewCellSeparatorStyleSingleLine];
//    [alert.view addSubview:alertTableView];
//    [alert.view bringSubviewToFront:alertTableView];
    [alert.view setUserInteractionEnabled:YES];
//    [alertTableView setUserInteractionEnabled:YES];
//    [alertTableView setAllowsSelection:YES];
    
    for (int i = 0; i < listMenu.size(); i++)
    {
        NSString *nsText = [NSString stringWithFormat:@"%s", listMenu[i].c_str()];
        UIAlertAction* action = [UIAlertAction actionWithTitle:nsText style:UIAlertActionStyleDefault
                                                          handler:^(UIAlertAction * action) {
                                                              g_common.extraStrings["listmenuoptionclicked"] = listMenu[i];
                                                          }];
    
        [alert addAction:action];
    }
    
//    NSLayoutConstraint *constraint = [NSLayoutConstraint constraintWithItem:alert.view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationLessThanOrEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1 constant:g_viewController.view.frame.size.height * 0.4f];
//    [alert.view addConstraint:constraint];
    
    [g_viewController presentViewController:alert animated:YES completion:nil];
    
    listMenuShown = true;    
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {

/*    if (!listMenuShown)
        return 0;
    
    return listMenu.size();*/
    
    return 0;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
/*    if (!listMenuShown)
        return nullptr;
    
    NSString *nsText = [NSString stringWithFormat:@"%s", listMenu[indexPath.item].c_str()];
    
    static NSString *CellIdentifier = nsText;
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    cell.textLabel.text = nsText;
    
    return cell;*/
    
    return nullptr;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {

/*    if (!listMenuShown)
        return;
    
    if (indexPath.item == listMenu.size() - 1)
    {
        [alert dismissViewControllerAnimated:YES completion:nil];
        return;
    }

    g_common.extraStrings["listmenuoptionclicked"] = listMenu[indexPath.item];
    
    [alert dismissViewControllerAnimated:YES completion:nil];*/
}

@end

