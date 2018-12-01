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
std::vector<IOSDialogPart> dialogParts;

- (void) clearListMenu
{
        listMenu.clear();
}

- (void) addListMenuOption:(std::string) title withDsec:(std::string) desc
{
    listMenu.push_back(title);
}


- (void) showListMenuInDialog:(NSString*) title withOptions:(NSString*) options
{
    alert = [UIAlertController alertControllerWithTitle:title
                                                                   message:@"This is an alert."
                                                            preferredStyle:UIAlertControllerStyleAlert];
    
    UITableView *alertTableView;
    
    CGRect rect;
    rect = CGRectMake(0, 0, 272, 380);

    [alert setPreferredContentSize:rect.size];
    
    alertTableView  = [[UITableView alloc]initWithFrame:rect];
    alertTableView.delegate = self;
    alertTableView.dataSource = self;
    alertTableView.tableFooterView = [[UIView alloc]initWithFrame:CGRectZero];
    [alertTableView setSeparatorStyle:UITableViewCellSeparatorStyleSingleLine];
    [alert.view addSubview:alertTableView];
    [alert.view bringSubviewToFront:alertTableView];
    [alert.view setUserInteractionEnabled:YES];
    [alertTableView setUserInteractionEnabled:YES];
    [alertTableView setAllowsSelection:YES];
    
    UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
                                                          handler:^(UIAlertAction * action) {}];
    
    [alert addAction:defaultAction];
    [g_viewController presentViewController:alert animated:YES completion:nil];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
    return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
    return listMenu.size();
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    NSString *nsText = [NSString stringWithFormat:@"%s", listMenu[indexPath.item].c_str()];

    cell.textLabel.text = nsText;
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {

    g_common.extraStrings["listmenuoptionclicked"] = listMenu[indexPath.item];
    
    [alert dismissViewControllerAnimated:YES completion:nil];
}

@end

