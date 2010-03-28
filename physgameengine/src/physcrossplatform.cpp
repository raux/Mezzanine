//© Copyright 2010 Joseph Toppi and John Blackwood
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
#ifndef _crossplatform_cpp
#define _crossplatform_cpp
///////////////////////////////////////////////////////////////////////////////
//this is the place where ALL of the non-multiplatform code goes.

//////////////////////
// If it cannot go here, at least describe it here
// 1: log(size_t) in the phys world is windows only there are items related to this in physworld.cpp and .h
//



///////////////////////////////////////////////////////////////////////////////
//Additional Includes

#include "physcrossplatform.h"
#include "physworld.h"
#include <Ogre.h>
#include "SDL.h"

//Selective Includes
#ifdef WINDOWS
	#include <cstdlib>//for sleep
	#include "SDL_syswm.h" //for the needed commands
#else
	#include <unistd.h>//for sleep
#endif


///////////////////////////////////////////////////////////////////////////////
//returns: The appropriate string, which contains a path, to the correct
//"plugins.cfg" file
string GetPluginsDotCFG()
{
	#ifdef LINUX
		return "data/linux/plugins.cfg";
	#endif
	#ifdef WINDOWS
		return "data/windows/plugins.cfg";
	#endif

}

///////////////////////////////////////////////////////////////////////////////
//returns: The appropriate string, which contains a path, to the correct
//"settings.cfg" file
string GetSettingsDotCFG()
{
	#ifdef LINUX
		return "data/linux/settings.cfg";
	#endif
	#ifdef WINDOWS
		return "data/windows/settings.cfg";
	#endif
}

string GetDataDirectory()
{
	return "data/common/";
}

///////////////////////////////////////////////////////////////////////////////
//This returns a named parameter list with valid settings to use Ogre rendering
//on a pre-existing SDL context
//void* is always an ogre NameValuePairList
void* GetSDLOgreBinder()
{
	Ogre::NameValuePairList *misc = new Ogre::NameValuePairList();
	#ifdef WINDOWS
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWMInfo(&wmInfo);

		size_t winHandle = reinterpret_cast<size_t>(wmInfo.window);
		size_t winGlContext = reinterpret_cast<size_t>(wmInfo.hglrc);

		(*misc)["externalWindowHandle"] = Ogre::StringConverter::toString(winHandle);
		(*misc)["externalGLContext"] = Ogre::StringConverter::toString(winGlContext);
		(*misc)["externalGLControl"] = Ogre::String("True");
	#else
		(*misc)["currentGLContext"] = Ogre::String("True");
	#endif

	return misc;
}

void WaitMilliseconds(PhysWhole WaitTime)
{
	#ifdef WINDOWS
		Sleep(WaitTime);
	#else
		usleep(1000*WaitTime);
	#endif
}

void RenderPhysWorld(PhysWorld *TheWorld)
{
    //TheWorld->OgreRoot->renderOneFrame();
    #ifndef WINDOWS
       TheWorld->OgreGameWindow->update(true);
    #else
       TheWorld->OgreRoot->renderOneFrame();
       SDL_GL_SwapBuffers();
    #endif
}

#endif
