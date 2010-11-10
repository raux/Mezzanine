//� Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _uiscreen_cpp
#define _uiscreen_cpp

#include "uiscreen.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uibutton.h"
#include "internalGorilla.h.cpp"

namespace phys
{
    UIScreen::UIScreen(const String& name, Gorilla::Screen* GScreen, UIManager* manager)
        : Name(name)
    {
        GorillaScreen = GScreen;
        Manager = manager;
    }

    UIScreen::~UIScreen()
    {
        Manager->GetSilverbackPointer()->destroyScreen(GorillaScreen);
    }

    String& UIScreen::GetName()
    {
        return Name;
    }

    void UIScreen::SetVisible(bool Visible)
    {
        GorillaScreen->setVisible(Visible);
    }

    bool UIScreen::IsVisible()
    {
        return GorillaScreen->isVisible();
    }

    void UIScreen::Show()
    {
        GorillaScreen->show();
    }

    void UIScreen::Hide()
    {
        GorillaScreen->hide();
    }

    UILayer* UIScreen::CreateLayer(const String& Name, Whole Zorder)
    {
        Gorilla::Layer* layer = GorillaScreen->createLayer(Zorder);
        UILayer* physlayer = new UILayer(Name, layer, this->GorillaScreen, Manager);
        Layers.push_back(physlayer);
        return physlayer;
    }

    UILayer* UIScreen::GetLayer(const String& Name)
    {
        for ( std::vector<UILayer*>::iterator it = Layers.begin() ; it != Layers.end() ; it++ )
        {
            if ( Name == (*it)->GetName() )
            {
                UILayer* Layer = (*it);
                return Layer;
            }
        }
        return 0;
    }

    UILayer* UIScreen::GetLayer(Whole Index)
    {
        return Layers[Index];
    }

    Whole UIScreen::GetNumLayers()
    {
        return Layers.size();
    }

    void UIScreen::DestroyLayer(UILayer* Layer)
    {
        if(Layers.empty())
            return;
        for( std::vector<UILayer*>::iterator it = Layers.begin() ; it != Layers.end() ; it++ )
        {
            if( Layer == (*it) )
            {
                delete (*it);
                Layers.erase(it);
                return;
            }
        }
    }

    UI::Button* UIScreen::GetButtonMouseIsOver()
    {
        for( Whole x=0 ; x != Layers.size() ; x++ )
        {
            if( Layers[x]->GetVisible() )
            {
                UI::Button* button = Layers[x]->GetButtonMouseIsOver();
                if(button)
                {
                    return button;
                }
            }
        }
        return 0;
    }
}

#endif
