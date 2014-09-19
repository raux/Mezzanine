// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _graphicstexturemanager_cpp
#define _graphicstexturemanager_cpp

#include "Graphics/texturemanager.h"

#include <Ogre.h>

namespace Mezzanine
{
    template<> Graphics::TextureManager* Singleton<Graphics::TextureManager>::SingletonPtr = NULL;

    namespace Graphics
    {
        TextureManager::TextureManager()
        {

        }

        TextureManager::TextureManager(XML::Node& XMLNode)
        {

        }

        TextureManager::~TextureManager()
        {

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void TextureManager::Initialize()
            { this->Initialized = true; }

        void TextureManager::Deinitialize()
            { this->Initialized = false; }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType TextureManager::GetInterfaceType() const
            { return ManagerBase::MT_TextureManager; }

        String TextureManager::GetImplementationTypeName() const
            { return "DefaultTextureManager"; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultTextureManagerFactory Methods

        DefaultTextureManagerFactory::DefaultTextureManagerFactory()
            {  }

        DefaultTextureManagerFactory::~DefaultTextureManagerFactory()
            {  }

        String DefaultTextureManagerFactory::GetManagerTypeName() const
            { return "DefaultTextureManager"; }

        ManagerBase* DefaultTextureManagerFactory::CreateManager(NameValuePairMap& Params)
        {
            if( TextureManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return TextureManager::GetSingletonPtr();
            }else return new TextureManager();
        }

        ManagerBase* DefaultTextureManagerFactory::CreateManager(XML::Node& XMLNode)
        {
            if( TextureManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return TextureManager::GetSingletonPtr();
            }else return new TextureManager(XMLNode);
        }

        void DefaultTextureManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
            { delete ToBeDestroyed; }
    }//Graphics
}//Mezzanine


#endif
