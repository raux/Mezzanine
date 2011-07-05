//� Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _uilayer_cpp
#define _uilayer_cpp

#include "uilayer.h"
#include "uimanager.h"
#include "uitextbutton.h"
#include "uirectangle.h"
#include "uicaption.h"
#include "uiwidget.h"
#include "uicheckbox.h"
#include "uibuttonlistbox.h"
#include "uimarkuptext.h"
#include "uilistbox.h"
#include "uiscreen.h"
#include "uiscrollbar.h"
#include "uilinelist.h"
#include "uiwindow.h"
#include "uimenu.h"
#include "uispinner.h"
#include "uiscrolledcellgrid.h"
#include "uipagedcellgrid.h"

#include "graphicsmanager.h"
#include "world.h"

#include "internalGorilla.h.cpp"

namespace phys
{
    namespace UI
    {
        Layer::Layer(const String& name, Gorilla::Layer* GLayer, Screen* PScreen)
            : Name(name),
              GorillaLayer(GLayer),
              Parent(PScreen)
        {
            Manager = World::GetWorldPointer()->GetUIManager();
        }

        Layer::~Layer()
        {
            while(!Buttons.empty())
            {
                Button* button = Buttons.back();
                delete button;
                Buttons.pop_back();
            }
            while(!Rectangles.empty())
            {
                Rectangle* rectangle = Rectangles.back();
                delete rectangle;
                Rectangles.pop_back();
            }
            while(!Captions.empty())
            {
                Caption* caption = Captions.back();
                delete caption;
                Captions.pop_back();
            }
            while(!MarkupTexts.empty())
            {
                MarkupText* markup = MarkupTexts.back();
                delete markup;
                MarkupTexts.pop_back();
            }
            while(!LineLists.empty())
            {
                LineList* linelist = LineLists.back();
                delete linelist;
                LineLists.pop_back();
            }
            while(!Widgets.empty())
            {
                Widget* widget = Widgets.back();
                delete widget;
                Widgets.pop_back();
            }
            Parent->GetGorillaScreen()->destroy(GorillaLayer);
        }

        String& Layer::GetName()
        {
            return Name;
        }

        Screen* Layer::GetParent()
        {
            return Parent;
        }

        void Layer::SetVisible(bool Visable)
        {
            GorillaLayer->setVisible(Visable);
        }

        bool Layer::IsVisible()
        {
            return GorillaLayer->isVisible() && Parent->IsVisible();
        }

        void Layer::Show()
        {
            GorillaLayer->show();
        }

        void Layer::Hide()
        {
            GorillaLayer->hide();
        }

        Button* Layer::CreateButton(ConstString& Name, const Vector2& Position, const Vector2& Size)
        {
            Button* button = new Button(Name, Position, Size, this);
            Buttons.push_back(button);
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    button->BindActivationKeyOrButton(Codes->at(X));
            }
            return button;
        }

        TextButton* Layer::CreateTextButton(ConstString& Name, const Vector2& Position, const Vector2& Size, const Whole& Glyph, ConstString& Text)
        {
            TextButton* tbutton = new TextButton(Name, Position, Size, Glyph, Text, this);
            Buttons.push_back(tbutton);
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    tbutton->BindActivationKeyOrButton(Codes->at(X));
            }
            return tbutton;
        }

        TextButton* Layer::CreateTextButton(ConstString& Name, const Vector2& Position, const Vector2& Size, const Real& LineHeight, ConstString& Text)
        {
            TextButton* tbutton = new TextButton(Name, Position, Size, LineHeight, Text, this);
            Buttons.push_back(tbutton);
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    tbutton->BindActivationKeyOrButton(Codes->at(X));
            }
            return tbutton;
        }

        Button* Layer::GetButton(ConstString& Name)
        {
            for ( std::vector<Button*>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    Button* button = (*it);
                    return button;
                }
            }
            return 0;
        }

        Button* Layer::GetButton(const Whole& Index)
        {
            return Buttons[Index];
        }

        Whole Layer::GetNumButtons()
        {
            return Buttons.size();
        }

        void Layer::DestroyButton(Button* ToBeDestroyed)
        {
            for ( std::vector<Button*>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    Buttons.erase(it);
                    return;
                }
            }
        }

        Rectangle* Layer::CreateRectangle(const Vector2& Position, const Vector2& Size)
        {
            Rectangle* rectangle = new Rectangle(Position,Size, this);
            Rectangles.push_back(rectangle);
            return rectangle;
        }

        Rectangle* Layer::GetRectangle(const Whole& Index)
        {
            return Rectangles[Index];
        }

        Whole Layer::GetNumRectangles()
        {
            return Rectangles.size();
        }

        void Layer::DestroyRectangle(Rectangle* ToBeDestroyed)
        {
            for ( std::vector<Rectangle*>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    Rectangles.erase(it);
                    return;
                }
            }
        }

        Caption* Layer::CreateCaption(ConstString& Name, const Vector2& Position, const Vector2& Size, const Whole& Glyph, const String& Text)
        {
            Caption* caption = new Caption(Name,Position,Size,Glyph,Text,this);
            Captions.push_back(caption);
            return caption;
        }

        Caption* Layer::CreateCaption(ConstString& Name, const Vector2& Position, const Vector2& Size, const Real& LineHeight, const String& Text)
        {
            Caption* caption = new Caption(Name,Position,Size,LineHeight,Text,this);
            Captions.push_back(caption);
            return caption;
        }

        Caption* Layer::GetCaption(ConstString& Name)
        {
            for ( std::vector<Caption*>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    Caption* caption = (*it);
                    return caption;
                }
            }
            return 0;
        }

        Caption* Layer::GetCaption(const Whole& Index)
        {
            return Captions[Index];
        }

        Whole Layer::GetNumCaptions()
        {
            return Captions.size();
        }

        void Layer::DestroyCaption(Caption* ToBeDestroyed)
        {
            for ( std::vector<Caption*>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    Captions.erase(it);
                    return;
                }
            }
        }

        MarkupText* Layer::CreateMarkupText(ConstString& Name, const Vector2& Position, const Whole& Glyph, const String& Text)
        {
            MarkupText* markup = new MarkupText(Name,Position,Glyph,Text,this);
            MarkupTexts.push_back(markup);
            return markup;
        }

        MarkupText* Layer::CreateMarkupText(ConstString& Name, const Vector2& Position, const Real& LineHeight, const String& Text)
        {
            MarkupText* markup = new MarkupText(Name,Position,LineHeight,Text,this);
            MarkupTexts.push_back(markup);
            return markup;
        }

        MarkupText* Layer::GetMarkupText(ConstString& Name)
        {
            for ( std::vector<MarkupText*>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    MarkupText* markup = (*it);
                    return markup;
                }
            }
            return 0;
        }

        MarkupText* Layer::GetMarkupText(const Whole& Index)
        {
            return MarkupTexts[Index];
        }

        Whole Layer::GetNumMarkupTexts()
        {
            return MarkupTexts.size();
        }

        void Layer::DestroyMarkupText(MarkupText* ToBeDestroyed)
        {
            for ( std::vector<MarkupText*>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    MarkupTexts.erase(it);
                    return;
                }
            }
        }

        LineList* Layer::CreateLineList()
        {
            LineList* linelist = new LineList(this);
            LineLists.push_back(linelist);
            return linelist;
        }

        LineList* Layer::GetLineList(const Whole& Index)
        {
            return LineLists[Index];
        }

        Whole Layer::GetNumLineLists()
        {
            return LineLists.size();
        }

        void Layer::DestroyLineList(LineList* ToBeDestroyed)
        {
            for ( std::vector<LineList*>::iterator it = LineLists.begin() ; it != LineLists.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    LineLists.erase(it);
                    return;
                }
            }
        }

        void Layer::AddWidget(Widget* ToAdd)
        {
            Widgets.push_back(ToAdd);
        }

        Widget* Layer::GetWidget(ConstString& Name)
        {
            for ( std::vector<Widget*>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    Widget* widget = (*it);
                    return widget;
                }
            }
            return 0;
        }

        Widget* Layer::GetWidget(Whole Index)
        {
            return Widgets[Index];
        }

        Whole Layer::GetNumWidgets()
        {
            return Widgets.size();
        }

        void Layer::DestroyWidget(Widget* ToBeDestroyed)
        {
            for ( std::vector<Widget*>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    Widgets.erase(it);
                    return;
                }
            }
        }

        Scrollbar* Layer::CreateScrollbar(ConstString& Name, const Vector2& Position, const Vector2& Size, const UI::ScrollbarStyle& Style)
        {
            Scrollbar* Scroll = new Scrollbar(Name,Position,Size,Style,this);
            Widgets.push_back(Scroll);
            return Scroll;
        }

        CheckBox* Layer::CreateCheckBox(ConstString& Name, const Vector2& Position, const Vector2& Size, const Whole& Glyph, const String &LabelText)
        {
            CheckBox* Check = new CheckBox(Name,Position,Size,Glyph,LabelText,this);
            Widgets.push_back(Check);
            return Check;
        }

        ButtonListBox* Layer::CreateButtonListBox(ConstString& Name, const Vector2& Position, const Vector2& Size, const Real& ScrollbarWidth, const UI::ScrollbarStyle& ScrollStyle)
        {
            ButtonListBox* BLB = new ButtonListBox(Name,Position,Size,ScrollbarWidth,ScrollStyle,this);
            Widgets.push_back(BLB);
            return BLB;
        }

        ListBox* Layer::CreateListBox(ConstString& Name, const Vector2& Position, const Vector2& Size, const Real& ScrollbarWidth, const UI::ScrollbarStyle& ScrollStyle)
        {
            ListBox* LB = new ListBox(Name,Position,Size,ScrollbarWidth,ScrollStyle,this);
            Widgets.push_back(LB);
            return LB;
        }

        Window* Layer::CreateWidgetWindow(ConstString& Name, const Vector2& Position, const Vector2& Size)
        {
            Window* Win = new Window(Name,Position,Size,this);
            Widgets.push_back(Win);
            return Win;
        }

        Menu* Layer::CreateMenu(ConstString& Name, const Vector2& Position, const Vector2& Size)
        {
            Menu* Men = new Menu(Name,Position,Size,this);
            Widgets.push_back(Men);
            return Men;
        }

        Spinner* Layer::CreateSpinner(ConstString& Name, const Vector2& Position, const Vector2& Size, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            Spinner* Spn = new Spinner(Name,Position,Size,SStyle,GlyphHeight,this);
            Widgets.push_back(Spn);
            return Spn;
        }

        ScrolledCellGrid* Layer::CreateScrolledCellGrid(ConstString& Name, const Vector2& Position, const Vector2& Size, const Real& Thickness, const UI::ScrollbarStyle& Style)
        {
            ScrolledCellGrid* SCG = new ScrolledCellGrid(Name,Position,Size,Thickness,Style,this);
            Widgets.push_back(SCG);
            return SCG;
        }

        PagedCellGrid* Layer::CreatePagedCellGrid(ConstString& Name, const Vector2& Position, const Vector2& Size, const Vector2& SpnPosition, const Vector2& SpnSize, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            PagedCellGrid* PCG = new PagedCellGrid(Name,Position,Size,SpnPosition,SpnSize,SStyle,GlyphHeight,this);
            Widgets.push_back(PCG);
            return PCG;
        }

        Button* Layer::CheckButtonMouseIsOver()
        {
            if(Buttons.empty())
                return 0;
            if(!IsVisible())
                return 0;
            Button* button = NULL;
            for( std::vector<Button*>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                button = (*it);
                if(button->IsVisible())
                {
                    if(button->CheckMouseHover())
                    {
                        return button;
                    }
                }
            }
            return 0;
        }

        Widget* Layer::CheckWidgetMouseIsOver()
        {
            if(Widgets.empty())
                return 0;
            if(!IsVisible())
                return 0;
            Widget* widget = NULL;
            for( std::vector<Widget*>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                widget = (*it);
                if(widget->IsVisible())
                {
                    if(widget->CheckMouseHover())
                    {
                        return widget;
                    }
                }
            }
            return 0;
        }

        void Layer::ViewportUpdate(const Vector2& OldViewportSize)
        {
            for ( std::vector<Button*>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it)->UpdateDimensions();
            }
            for ( std::vector<Rectangle*>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it)->UpdateDimensions();
            }
            for ( std::vector<Caption*>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it)->UpdateDimensions();
            }
            for ( std::vector<MarkupText*>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it)->UpdateDimensions();
            }
            //for ( std::vector<LineList*>::iterator it = LineLists.begin() ; it != LineLists.end() ; it++ )
            //{
            //    (*it)->
            //}
            for( std::vector<Widget*>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it)->UpdateDimensions(OldViewportSize);
            }
        }

        Gorilla::Layer* Layer::GetGorillaLayer()
        {
            return GorillaLayer;
        }
    }//ui
}//phys

#endif
