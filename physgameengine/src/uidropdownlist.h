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
#ifndef _uidropdownlist_h
#define _uidropdownlist_h

#include "uiwidget.h"
#include "enumerations.h"

namespace phys
{
    namespace UI
    {
        class ListBox;
        class Button;
        class Caption;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class DropDownList
        /// @headerfile uidropdownlist.h
        /// @brief A widget that displays one selection from a list that can have it's visibility toggled.
        /// @details Note: When getting or setting either the position or size of this widget, you are altering
        /// the caption and button only, and not the list.  The list will be moved and resized appropriately, automatically.
        ///////////////////////////////////////
        class DropDownList : public Widget
        {
            protected:
                Caption* Selection;
                Button* ListToggle;
                UI::ListBox* SelectionList;
                bool ToggleActivated;
                /// @brief Internal construction function.
                void ConstructDropDownList(const RenderableRect& Rect, const Whole& Glyph, const UI::ScrollbarStyle& ScrollStyle);
                /// @brief For use with widget update/automation.
                virtual void Update(bool Force = false) = 0;
            public:
                /// @brief Class constructor.
                /// @param name The Name for the Widget.
                /// @param Rect The renderable rect representing the position and size of ths widget.
                /// @param LineHeight The lineheight you want the text to have.
                /// If the Rect passed in is relative, this will expect LineHeight to be relative as well.
                /// @param ScrollStyle The style of the scrollbar you want for this List Box.  See Scrollbar
                /// class for more information.
                /// @param parent The parent layer that created this widget.
                DropDownList(const String& name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle, Layer* parent);
                /// @brief Class constructor.
                /// @param name The Name for the Widget.
                /// @param Rect The renderable rect representing the position and size of ths widget.
                /// @param Glyph The Glyph index to be applied to all text in this widget.  Must be valid.
                /// @param ScrollStyle The style of the scrollbar you want for this List Box.  See Scrollbar
                /// class for more information.
                /// @param parent The parent layer that created this widget.
                DropDownList(const String& name, const RenderableRect& Rect, const Whole& Glyph, const UI::ScrollbarStyle& ScrollStyle, Layer* parent);
                /// @brief Class destructor.
                virtual ~DropDownList();
                /// @brief Sets the visibility of this widget.
                /// @param visible Bool determining whether or not this widget should be visible.
                virtual void SetVisible(bool visible);
                /// @brief Gets the visibility of this widget.
                /// @return Returns a bool representing the visibility of this widget.
                virtual bool IsVisible();
                /// @brief Forces this widget to be shown.
                virtual void Show();
                /// @brief Forces this widget to hide.
                virtual void Hide();
                /// @brief Checks to see if the current mouse position is over this widget.
                /// @return Returns a bool value, true if the mouse is over this widget, false if it's not.
                virtual bool CheckMouseHover();
                /// @brief Sets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this widget.
                virtual void SetPosition(const Vector2& Position);
                /// @brief Gets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this widget.
                virtual Vector2 GetPosition();
                /// @brief Sets the pixel position of this widget.
                /// @param Position A vector2 representing the pixel position of this widget.
                virtual void SetActualPosition(const Vector2& Position);
                /// @brief Sets the pixel position of this widget.
                /// @return Returns a vector2 representing the pixel position of this widget.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this widget.
                virtual void SetSize(const Vector2& Size);
                /// @brief Gets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this widget.
                virtual Vector2 GetSize();
                /// @brief Sets the pixel size of this widget.
                /// @param Size A vector2 representing the pixel size of this widget.
                virtual void SetActualSize(const Vector2& Size);
                /// @brief Sets the pixel size of this widget.
                /// @return Returns a vector2 representing the pixel size of this widget.
                virtual Vector2 GetActualSize();
        };//dropdownlist
    }//ui
}//phys

#endif
