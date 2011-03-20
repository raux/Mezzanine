//© Copyright 2010 BlackTopp Studios Inc.
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

//Internal includes
#include "crossplatform.h"
#include "world.h"


//External includes
#include <Ogre.h>
#include "SDL.h"

#include <stdio.h>

//Selective Includes
#ifdef WINDOWS
	#include <cstdlib>//for sleep
    #include <Winuser.h>
	#include "SDL_syswm.h" //for the needed commands
    #include <direct.h> // for _getcwd
#else
	#include <unistd.h>//for sleep and getcwd
    #include "graphicsmanager.h"
#endif

namespace phys
{
    namespace crossplatform
    {
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
			#ifdef MACOSX
				return "data/macosx/plugins.cfg";
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
			#ifdef MACOSX
				return "data/macosx/settings.cfg";
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
        void* GetSDLOgreBinder(SDL_Window* window, size_t& winGlContext)
        {
            Ogre::NameValuePairList *misc = new Ogre::NameValuePairList();
            #ifdef WINDOWS
                SDL_SysWMinfo wmInfo;
                SDL_VERSION(&wmInfo.version);
                if(SDL_GetWindowWMInfo(window,&wmInfo))
                {
                    size_t winHandle = reinterpret_cast<size_t>(wmInfo.info.win.window);
                    //size_t winGlContext = reinterpret_cast<size_t>(wmInfo.hglrc);

                    (*misc)["externalWindowHandle"] = Ogre::StringConverter::toString(winHandle);
                    (*misc)["externalGLContext"] = Ogre::StringConverter::toString(winGlContext);
                    (*misc)["externalGLControl"] = Ogre::String("True");
                }else{
                    World::GetWorldPointer()->LogAndThrow("Failed to create SDL Binder.");
                }
            #else
                (*misc)["currentGLContext"] = Ogre::String("True");
            #endif

            return misc;
        }

        void WaitMilliseconds(const Whole &WaitTime)
        {
            #ifdef WINDOWS
                Sleep(WaitTime);
            #else
                usleep(1000*WaitTime);
            #endif
        }

        void RenderPhysWorld(Ogre::RenderWindow* TheOgreWindow, SDL_Window* SDLWindow)
        {
            World* TheWorld = World::GetWorldPointer();
            TheWorld->Log("Rendering the World.");
            #ifndef WINDOWS
                Ogre::Root::getSingleton()._fireFrameStarted();
                Ogre::Root::getSingleton()._fireFrameRenderingQueued();
                TheOgreWindow->update(true);
                Ogre::Root::getSingleton()._fireFrameEnded();
                if( !TheOgreWindow->isVisible() )
                    Ogre::Root::getSingleton().clearEventTimes();
            #else
                if(TheOgreWindow->isActive())
                {
                    Ogre::Root::getSingleton().renderOneFrame();
                    SDL_GL_SwapWindow(SDLWindow);
                }else if( !TheOgreWindow->isActive() && TheOgreWindow->isVisible()){
                    TheOgreWindow->update(false);
                    SDL_GL_SwapWindow(SDLWindow);
                }else{
                    TheWorld->Log("Aborted Rendering, target is not active");
                    // clear timings to allow smooth alt-tabbing action.
                    Ogre::Root::getSingleton().clearEventTimes();
                }
            #endif
            TheWorld->Log("Finished Rendering");
        }

        String GetPlatform()
        {
            #ifdef LINUX
                return "Linux";
            #endif
            #ifdef WINDOWS
                return "Windows";
            #endif
			#ifdef MACOSX
				return "MacOSX";
			#endif
        }

        void SanitizeWindowedRes(const Whole& Width, const Whole& Height, Whole& ActualWidth, Whole& ActualHeight)
        {
            #ifdef WINDOWS
            RECT rc;
            SetRect(&rc, 0, 0, Width, Height);
            AdjustWindowRect(&rc, WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW, false);
            ActualWidth = Width - ((rc.right - rc.left) - Width);
            ActualHeight = Height - ((rc.bottom - rc.top) - Height);
            #else
            ActualWidth = Width;
            ActualHeight = Height;
            #endif
        }

        String GetWorkingDir()
        {
            char cCurrentPath[FILENAME_MAX];
            //char cCurrentPath[MAXPATHLEN];
            #ifdef WINDOWS
            String Results (_getcwd(cCurrentPath,sizeof(cCurrentPath)));
            #else
            String Results (getcwd(cCurrentPath,sizeof(cCurrentPath)));
            #endif
            return Results;
        }


    }
}

#endif
