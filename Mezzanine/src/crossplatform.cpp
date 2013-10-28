//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _crossplatform_cpp
#define _crossplatform_cpp
///////////////////////////////////////////////////////////////////////////////
//this is the place where ALL of the non-multiplatform code goes.

//Internal includes
#include "crossplatform.h"
#include "entresol.h"
#include "Graphics/graphicsmanager.h"
#include "UI/uimanager.h"
#include "Graphics/gamewindow.h"
#include "Threading/systemcalls.h"

//External includes
#include <Ogre.h>
#include "SDL.h"


#include <dirent.h> //Included with gcc/mingw will likely need replacement with msvc

#include <stdio.h>

//Selective Includes
#ifdef WINDOWS
    #include <windows.h>
	#include <cstdlib>//for sleep
    #include <Winuser.h>
	#include "SDL_syswm.h" //for the needed commands
    #include <direct.h> // for _getcwd
#else
	#include <unistd.h>//for sleep and getcwd
    #include <sys/time.h>
#endif

namespace Mezzanine
{
    namespace crossplatform
    {
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

        MaxInt GetTimeStamp()
            { return Mezzanine::GetTimeStamp(); }

        Whole GetTimeStampResolution()
            { return Mezzanine::GetTimeStampResolution(); }

        Whole MEZZ_LIB GetCPUCount()
            { return Mezzanine::GetCPUCount(); }

    }//crossplatform
}//Mezzanine

#endif
