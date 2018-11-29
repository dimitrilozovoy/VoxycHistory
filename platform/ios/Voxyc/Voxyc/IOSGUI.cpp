//
//  IOSGUI.cpp
//  Voxyc
//
//  Created by Dimitri Lozovoy on 11/29/18.
//
//

#include "IOSGUI.hpp"

IOSGUI g_iosGUI;
GLKViewController *g_viewController;

void IOSGUI::showListMenuInDialog(std::string title, std::string options)
{
    NSString *nsTitle = [NSString stringWithFormat:@"%s", title.c_str()];
    
    UIAlertController* alert = [UIAlertController alertControllerWithTitle:nsTitle
                                                                   message:@"This is an alert."
                                                            preferredStyle:UIAlertControllerStyleAlert];
    
    UITableView *alertTableView;
    
    CGRect rect;
    rect = CGRectMake(0, 0, 272, 250);
    [alert setPreferredContentSize:rect.size];
    
/*    alertTableView  = [[UITableView alloc]initWithFrame:rect];
    alertTableView.delegate = self;
    alertTableView.dataSource = self;
    alertTableView.tableFooterView = [[UIView alloc]initWithFrame:CGRectZero];
    [alertTableView setSeparatorStyle:UITableViewCellSeparatorStyleSingleLine];
    [alertTableView setTag:kAlertTableViewTag];
    [controller.view addSubview:alertTableView];
    [controller.view bringSubviewToFront:alertTableView];
    [controller.view setUserInteractionEnabled:YES];
    [alertTableView setUserInteractionEnabled:YES];
    [alertTableView setAllowsSelection:YES];*/
    
    UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
                                                          handler:^(UIAlertAction * action) {}];
    
    [alert addAction:defaultAction];
    [g_viewController presentViewController:alert animated:YES completion:nil];
}
