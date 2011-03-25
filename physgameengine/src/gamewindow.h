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
#ifndef _gamewindow_h
#define _gamewindow_h

#include "graphicssettings.h"

namespace Ogre
{
    class RenderWindow;
}

struct SDL_Window;

namespace phys
{
    class Viewport;
    class Camera;
    class GraphicsManager;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class GameWindow
    /// @headerfile gamewindow.h
    /// @brief This class is for creating and managing game windows.
    /// @details
    ///////////////////////////////////////
    class PHYS_LIB GameWindow
    {
        public:
            /// @enum WindowFlags
            /// @brief A listing of potential options for configuring a game window during construction.
            /// @details Brief descriptions of the values are as follows: @n
            /// WF_Fullscreen: This enables fullscreen on the window. @n
            /// WF_Hidden: This hides the window so that it isn't visible. @n
            /// WF_VsyncEnabled: This enables vsync for the window. @n
            /// WF_FSAA_2: Enables Fullscreen Anti-Aliasing level 2 for the window. @n
            /// WF_FSAA_4: Enables Fullscreen Anti-Aliasing level 4 for the window. @n
            /// WF_FSAA_8: Enables Fullscreen Anti-Aliasing level 8 for the window. @n
            /// WF_FSAA_16: Enables Fullscreen Anti-Aliasing level 16 for the window. @n
            /// WF_Resizeable: Creates a window with resizable borders, otherwise it is fixed size. @n
            /// WF_Minimized: Minimizes the window to the task bar immediately after construction. @n
            /// WF_Maximized: Maximizes the window immediately after construction. @n
            /// WF_Borderless: Removes all window decorations from the window(titlebar, borders, etc.). @n
            enum WindowFlags
            {
                WF_Fullscreen = 1,
                WF_Hidden = 2,
                WF_VsyncEnabled = 4,
                WF_FSAA_2 = 8,
                WF_FSAA_4 = 16,
                WF_FSAA_8 = 32,
                WF_FSAA_16 = 64,
                WF_Resizeable = 128,
                WF_Minimized = 256,
                WF_Maximized = 512,
                WF_Borderless = 1024,
            };
        protected:
            Ogre::RenderWindow* OgreWindow;
            SDL_Window* SDLWindow;
            GraphicsSettings Settings;

            std::vector< Viewport* > Viewports;

            GraphicsManager* Manager;

            void CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags);
            void CorrectViewportAndCamera(const Whole& Index);
            int IsLargerThenDesktop(const Whole& Width, const Whole& Height);
        public:
            /// @brief Class constructor.
            /// @param WindowCaption The caption to be set in the window titlebar.
            /// @param Width The desired width in pixels.
            /// @param Height The desired height in pixels.
            /// @param Flags Additional misc parameters, see WindowFlags enum for more info.
            GameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags);

            /// @brief Class destructor.
            ~GameWindow();

            /// @brief Creates an additional Viewport within a created render window.
            /// @param VeiwportCamera The camera that is to be attached to this Viewport.
            /// @param ZOrder The render order of this viewport relative to other viewports in this window.
            /// 0 means it'll use the current viewport count to determine the ZOrder.
            Viewport* CreateViewport(Camera* ViewportCamera, const Whole& ZOrder = 0);

            /// @brief Gets a viewport by index.
            /// @return Returns a pointer to the viewport requested.
            Viewport* GetViewport(const Whole& Index);

            /// @brief Gets the number of viewports within this window.
            /// @return Returns a Whole representing the number of viewports within this window.
            Whole GetNumViewports();

            /// @brief Destroys a viewport within this window.
            /// @param ToBeDestroyed The viewport that will be destroyed.
            void DestroyViewport(Viewport* ToBeDestroyed);

            /// @brief Gets the current window settings.
            /// @param Returns a GraphicsSettings struct with the current window settings.
            const GraphicsSettings& GetSettings();

            /// @brief Set the Fullscreen Setting
            /// @details Set the Fullscreen Setting
            /// @param Fullscreen This accepts a bool. True for fullscreen, false for windowed
            void setFullscreen(const bool &Fullscreen);

            /// @brief Gets the Fullscreen Setting
            /// @details Gets the Fullscreen Setting
            /// @return This returns a bool, true if fullscreen is set, false otherwise
            bool getFullscreen() const;

            /// @brief Sets the Height.
            /// @details Set the Render Height inside the window in windowed mode, set the resolution of the screen in fullscreen
            /// @param Height This accepts a Whole.
            void setRenderHeight(const Whole &Height);

            /// @brief Gets the Height of the Rendering Area
            /// @details Gets the Height of the Rendering Area
            /// @return This returns the Height of the Rendering Area
            Whole getRenderHeight() const;

            /// @brief Sets the Width.
            /// @details Set the Render Width inside the window in windowed mode, set the resolution of the screen in fullscreen
            /// @param Width This accepts a Whole.
            void setRenderWidth(const Whole &Width);

            /// @brief Gets the Width of the Rendering Area
            /// @details Gets the Width of the Rendering Area
            /// @return This returns the Width of the Rendering Area
            Whole getRenderWidth() const;

            /// @brief Changes the X and Y Resolution at the same time
            /// @details This should be useful in situations where it is not possible to update the width and height separately.
            /// @param Width The new desired Width for the rendering area as a whole number
            /// @param Height The new desired Width for the rendering area as a whole number
            void setRenderResolution(const Whole &Width, const Whole &Height);

            /// @brief Changes the X Resolution, Y Resolution, and fullscreen at the same time
            /// @details This should be useful in situations where it is not possible to update all of the options separately.
            void setRenderOptions(const GraphicsSettings& NewSettings);

            /// @brief Gets the FPS based on the last frame rendered.
            /// @details This essentially determines how many frames could be rendered if all
            /// frames within a second rendered at the same speed.
            /// @return Returns a Real representing the framerate.
            Real GetLastFPS();

            /// @brief Gets the Average FPS.
            /// @return Returns a Real representing the average framerate.
            Real GetAverageFPS();

            /// @brief Gets the Best FPS.
            /// @return Returns a Real representing the best framerate.
            Real GetBestFPS();

            /// @brief Gets the Worst FPS.
            /// @return Returns a Real representing the worst framerate.
            Real GetWorstFPS();

            /// @brief Gets the shortest amount of time it's taken to render a frame.
            /// @return Returns a Real representing the best time for rendering a frame.
            Real GetBestFrameTime();

            /// @brief Gets the longest amount of time it's taken to render a frame.
            /// @return Returns a Real representing the worst time for rendering a frame.
            Real GetWorstFrameTime();

            /// @internal
            /// @brief This will get a pointer to the Ogre RenderWindow.
            /// @return Returns a pointer to the Ogre RenderWindow class in use.
            Ogre::RenderWindow* GetOgreWindowPointer();

            /// @internal
            /// @brief This will get a pointer to the SDL Window.
            /// @return Returns a pointer to the SDL Window class in use.
            SDL_Window* GetSDLWindowPointer();
    };//gamewindow
}//phys

#endif
