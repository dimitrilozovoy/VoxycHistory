//
//  IOSGUI.hpp
//  Voxyc
//
//  Created by Dimitri Lozovoy on 11/29/18.
//
//

#ifndef IOSGUI_hpp
#define IOSGUI_hpp

#include <stdio.h>
#include <string>
#import <Foundation/Foundation.h>
#import "GLKit/GLKit.h"

class IOSGUI
{
public:
    void showListMenuInDialog(std::string title, std::string options);

};

extern IOSGUI g_iosGUI;
extern GLKViewController *g_viewController;

#endif /* IOSGUI_hpp */
