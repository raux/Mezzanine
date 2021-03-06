// © Copyright 2010 - 2017 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _uiscrollbar_cpp
#define _uiscrollbar_cpp

#include "UI/scrollbar.h"
#include "UI/button.h"
#include "UI/pagedcontainer.h"
#include "UI/screen.h"
#include "UI/uimanager.h"

#include "Input/metacode.h"

#include <algorithm>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Scrollbar Methods

        const String Scrollbar::TypeName                        = "Scrollbar";
        const EventNameType Scrollbar::EventScrollValueChanged = "ScrollValueChanged";

        Scrollbar::Scrollbar(Screen* Parent) :
            PageProvider(Parent),
            Scroller(NULL),
            ScrollBack(NULL),
            UpLeftButton(NULL),
            DownRightButton(NULL),
            ChildLock(NULL),
            IncrementDistance(0.01),
            ScrollerSize(1.0),
            AutoHideScroll(false)
            {  }

        Scrollbar::Scrollbar(const String& RendName, Screen* Parent) :
            PageProvider(RendName,Parent),
            Scroller(NULL),
            ScrollBack(NULL),
            UpLeftButton(NULL),
            DownRightButton(NULL),
            ChildLock(NULL),
            IncrementDistance(0.01),
            ScrollerSize(1.0),
            AutoHideScroll(false)
            { this->AddSubscriptionTable(Scrollbar::EventScrollValueChanged); }

        Scrollbar::Scrollbar(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            PageProvider(RendName,RendRect,Parent),
            Scroller(NULL),
            ScrollBack(NULL),
            UpLeftButton(NULL),
            DownRightButton(NULL),
            ChildLock(NULL),
            IncrementDistance(0.01),
            ScrollerSize(1.0),
            AutoHideScroll(false)
            { this->AddSubscriptionTable(Scrollbar::EventScrollValueChanged); }

        Scrollbar::~Scrollbar()
            {  }

        Boole Scrollbar::HandleInputImpl(const Input::MetaCode& Code)
        {
            if( this->ChildLock != NULL ) {
                if( Code.IsDeviceButton() && Code.GetMetaValueAsButtonState() == Input::BUTTON_DOWN && !( this->ChildLock->IsOnStandby() ) ) {
                    if( this->ChildLock == this->ScrollBack ) {
                        return this->_ScrollBackScroll(ParentScreen->GetMouseHitPosition());
                    }else if( this->ChildLock == this->UpLeftButton ) {
                        return this->_ButtonScroll(this->UpLeftButton);
                    }else if( this->ChildLock == this->DownRightButton ) {
                        return this->_ButtonScroll(this->DownRightButton);
                    }
                }else if( Code.GetCode() == Input::MOUSEHORIZONTAL ) {
                    Vector2 Delta( static_cast<Real>(Code.GetMetaValue()),0 );
                    return this->_MouseScroll(Delta);
                }else if( Code.GetCode() == Input::MOUSEVERTICAL ) {
                    Vector2 Delta( 0,static_cast<Real>(Code.GetMetaValue()) );
                    return this->_MouseScroll(Delta);
                }
            }else if( Code.GetCode() == Input::MOUSEWHEELVERTICAL || Code.GetCode() == Input::MOUSEWHEELHORIZONTAL ) {
                return this->_MouseWheelScroll(Code.GetMetaValue());
            }
            return false;
        }

        void Scrollbar::SubscribeToChildEvents()
        {
            this->Scroller->Subscribe(Button::EventActivated,SubscriberType(this,
                                      [this](EventPtr Args){ this->_NotifyEvent(Args); } ) );
            this->Scroller->Subscribe(Button::EventDeactivated,SubscriberType(this,
                                      [this](EventPtr Args){ this->_NotifyEvent(Args); } ) );
            this->ScrollBack->Subscribe(Button::EventActivated,SubscriberType(this,
                                        [this](EventPtr Args){ this->_NotifyEvent(Args); } ) );
            this->ScrollBack->Subscribe(Button::EventDeactivated,SubscriberType(this,
                                        [this](EventPtr Args){ this->_NotifyEvent(Args); } ) );
            if( this->UpLeftButton != NULL ) {
                this->UpLeftButton->Subscribe(Button::EventActivated,SubscriberType(this,
                                              [this](EventPtr Args){ this->_NotifyEvent(Args); } ) );
                this->UpLeftButton->Subscribe(Button::EventDeactivated,SubscriberType(this,
                                              [this](EventPtr Args){ this->_NotifyEvent(Args); } ) );
            }
            if( this->DownRightButton != NULL ) {
                this->DownRightButton->Subscribe(Button::EventActivated,SubscriberType(this,
                                                 [this](EventPtr Args){ this->_NotifyEvent(Args); } ) );
                this->DownRightButton->Subscribe(Button::EventDeactivated,SubscriberType(this,
                                                 [this](EventPtr Args){ this->_NotifyEvent(Args); } ) );
            }
        }

        Real Scrollbar::GetScrollRange() const
        {
            return ( this->GetUpperScrollLimit() - this->GetLowerScrollLimit() );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void Scrollbar::SetIncrementDistance(const Real& IncDist)
            { this->IncrementDistance = IncDist; }

        Real Scrollbar::GetIncrementDistance() const
            { return this->IncrementDistance; }

        void Scrollbar::SetAutoHide(Boole AutoHide)
            { this->AutoHideScroll = AutoHide; }

        Boole Scrollbar::GetAutoHide() const
            { return this->AutoHideScroll; }

        ///////////////////////////////////////////////////////////////////////////////
        // Fetch Methods

        Button* Scrollbar::GetScroller() const
            { return this->Scroller; }

        Button* Scrollbar::GetUpLeftButton() const
            { return this->UpLeftButton; }

        Button* Scrollbar::GetDownRightButton() const
            { return this->DownRightButton; }

        Button* Scrollbar::GetScrollBack() const
            { return this->ScrollBack; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Scrollbar::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeProperties(SelfRoot);
            XML::Node PropertiesNode = SelfRoot.AppendChild( Scrollbar::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("IncrementDistance").SetValue( this->GetIncrementDistance() ) &&
                PropertiesNode.AppendAttribute("ScrollerSize").SetValue( this->GetScrollerSize() ) &&
                PropertiesNode.AppendAttribute("AutoHideScroll").SetValue( this->AutoHideScroll ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",Scrollbar::GetSerializableName() + "Properties",true);
            }
        }

        void Scrollbar::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Widget::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( Scrollbar::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("IncrementDistance");
                    if( !CurrAttrib.Empty() )
                        this->SetIncrementDistance( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("ScrollerSize");
                    if( !CurrAttrib.Empty() )
                        this->SetScrollerSize( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("AutoHideScroll");
                    if( !CurrAttrib.Empty() )
                        this->SetAutoHide( CurrAttrib.AsBool() );
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (Scrollbar::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Scrollbar::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String Scrollbar::GetSerializableName()
        {
            return Scrollbar::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        void Scrollbar::_OnScrollValueChanged(const Real OldValue, const Real NewValue)
        {
            if( this->Container != NULL ) {
                this->Container->UpdateVisibleChildren();
            }

            ScrollValueChangedEventPtr Args = std::make_shared<ScrollValueChangedEvent>(EventScrollValueChanged,this->Name,OldValue,NewValue);
            this->DispatchEvent(EventScrollValueChanged,&SubscriberType::operator(),Args);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void Scrollbar::_NotifyEvent(EventPtr Args)
        {
            WidgetEventPtr WidArgs = std::static_pointer_cast<WidgetEvent>(Args);
            Widget* EventWidget = this->ParentScreen->GetWidget(WidArgs->WidgetName);
            if( EventWidget == NULL )
                return;

            if( EventWidget == this->Scroller )
            {
                if( WidArgs->EventName == Button::EventActivated && ChildLock == NULL )
                {
                    // Obtain the lock
                    this->ChildLock = this->Scroller;
                }
                else if( WidArgs->EventName == Button::EventDeactivated )
                {
                    // Release the lock
                    this->ChildLock = NULL;
                }
            }
            else if( EventWidget == this->ScrollBack )
            {
                if( WidArgs->EventName == Button::EventActivated && ChildLock == NULL )
                {
                    // Obtain the lock
                    this->ChildLock = this->ScrollBack;
                    this->_ScrollBackScroll(ParentScreen->GetMouseHitPosition());
                }
                else if( WidArgs->EventName == Button::EventDeactivated )
                {
                    // Release the lock
                    this->ChildLock = NULL;
                }
            }
            else if( EventWidget == this->UpLeftButton )
            {
                if( WidArgs->EventName == Button::EventActivated && ChildLock == NULL )
                {
                    // Obtain the lock
                    this->ChildLock = this->UpLeftButton;
                    this->_ButtonScroll(this->UpLeftButton);
                }
                else if( WidArgs->EventName == Button::EventDeactivated )
                {
                    // Release the lock
                    this->ChildLock = NULL;
                }
            }
            else if( EventWidget == this->DownRightButton )
            {
                if( WidArgs->EventName == Button::EventActivated && ChildLock == NULL )
                {
                    // Obtain the lock
                    this->ChildLock = this->DownRightButton;
                    this->_ButtonScroll(this->DownRightButton);
                }
                else if( WidArgs->EventName == Button::EventDeactivated )
                {
                    // Release the lock
                    this->ChildLock = NULL;
                }
            }
        }
    }//UI
}//Mezzanine

#endif
