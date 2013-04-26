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
#ifndef _graphicsmanager_cpp
#define _graphicsmanager_cpp

#include "eventmanager.h"
#include "eventrendertime.h"
#include "graphicsmanager.h"
#include "cameramanager.h"
#include "resourcemanager.h"
#include "uimanager.h"
#include "camera.h"
#include "cameramanager.h"
#include "crossplatform.h"
#include "Graphics/viewport.h"
#include "stringtool.h"
#include "entresol.h"

#include <SDL.h>
#include <Ogre.h>

#ifdef MEZZ_BUILD_DIRECTX9_SUPPORT
#include "OgreD3D9Plugin.h"
#endif
#ifdef MEZZ_BUILD_DIRECTX11_SUPPORT
#include "OgreD3D11Plugin.h"
#endif
#ifdef MEZZ_BUILD_OPENGLES2_SUPPORT
#include "OgreGLES2Plugin.h"
#endif
#ifdef MEZZ_BUILD_OPENGLES_SUPPORT
#include "OgreGLESPlugin.h"
#endif
#ifdef MEZZ_BUILD_OPENGL_SUPPORT
#include "OgreGLPlugin.h"
#endif

#include <cstdlib>

namespace Mezzanine
{
    template<> GraphicsManager* Singleton<GraphicsManager>::SingletonPtr = 0;

    ///////////////////////////////////////////////////////////////////////////
    // Creation and Deletion functions
    ///////////////////////////////////
    GraphicsManager::GraphicsManager()
        : OgreBeenInitialized(false),
          CurrRenderSys(Graphics::RS_OpenGL2)
    {
        Construct();
        this->AutoGenFiles = false;
    }

    GraphicsManager::GraphicsManager(XML::Node& XMLNode)
        : OgreBeenInitialized(false),
          CurrRenderSys(Graphics::RS_OpenGL2)
    {
        Construct();

        XML::Attribute CurrAttrib;
        String PathPreset;
        // Get whether or not to autogen the directory path and settings file.
        XML::Node AutoGenNode = XMLNode.GetChild("AutoCreateSettings");
        if(!AutoGenNode.Empty())
        {
            CurrAttrib = AutoGenNode.GetAttribute("Auto");
            if(!CurrAttrib.Empty())
                AutoGenPath = AutoGenFiles = StringTools::ConvertToBool( CurrAttrib.AsString() );
        }
        // Get preset path to default to if a path is not provided.
        XML::Node PathNode = XMLNode.GetChild("SettingsPath");
        if(!PathNode.Empty())
        {
            CurrAttrib = PathNode.GetAttribute("Path");
            if(!CurrAttrib.Empty())
                PathPreset = CurrAttrib.AsString();

            if(!PathPreset.empty())
                SetSettingsFilePath(PathPreset);
        }
        // Get the files to be loaded, and load them.
        XML::Node FilesNode = XMLNode.GetChild("SettingsFiles");
        if(!FilesNode.Empty())
        {
            for( XML::NodeIterator SetFileIt = FilesNode.begin() ; SetFileIt != FilesNode.end() ; ++SetFileIt )
            {
                String FileName, FilePath, FileGroup;
                // Get the filename to load
                CurrAttrib = (*SetFileIt).GetAttribute("FileName");
                if(!CurrAttrib.Empty())
                    FileName = CurrAttrib.AsString();
                // Get the path
                CurrAttrib = (*SetFileIt).GetAttribute("Path");
                if(!CurrAttrib.Empty())
                    FilePath = CurrAttrib.AsString();
                else
                {
                    CurrAttrib = (*SetFileIt).GetAttribute("Group");
                    if(!CurrAttrib.Empty())
                        FileGroup = CurrAttrib.AsString();
                }

                if(FilePath.empty())
                {
                    if(FileGroup.empty()) LoadSettings(FileName);
                    else LoadSettingsFromGroup(FileName,FileGroup);
                }
                else LoadSettings(FileName,FilePath);
            }
        }
        /// @todo This is currently necessary because a render window of some kind needs to exist for the loading
        /// of resources that occurs later in world construction (when this is constructed by the world, which this
        /// assumes.  If possible this should be removed, to keep construction more flexible.
        InitOgreRenderSystem();
    }


    GraphicsManager::~GraphicsManager()
    {
        if(AutoGenFiles)
            SaveAllSettings();

        DestroyAllGameWindows(false);

        UInt32 InitSDLSystems = SDL_WasInit(0);
        if( SDL_INIT_VIDEO | InitSDLSystems )
        {
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }

        /// @todo This is commented out due to an issue with the ogre shutdown sequence.
        /// Ogre will eventually shut these plugins down, but not until after the lifetime of this manager.
        /*for (std::vector<Ogre::Plugin*>::iterator Iter = RenderSystems.begin();
             Iter != RenderSystems.end();
             Iter++)
        {
            Ogre::Root::getSingletonPtr()->uninstallPlugin(*Iter);
            delete *Iter;
        }//*/
    }

    void GraphicsManager::Construct()
    {
        UInt32 InitSDLSystems = SDL_WasInit(0);
        if( (SDL_INIT_VIDEO & InitSDLSystems) == 0 )
        {
            if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0 )
                { MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Video/Windowing, SDL Error: ") + SDL_GetError()); }
        }

        SDL_DisplayMode DeskMode;
        SDL_GetDesktopDisplayMode(0,&DeskMode);
        DesktopSettings.RenderWidth = DeskMode.w;
        DesktopSettings.RenderHeight = DeskMode.h;
        DesktopSettings.RefreshRate = DeskMode.refresh_rate;

        this->Priority = 70;
        this->FrameDelay = 0;
    }

    void GraphicsManager::InitOgreRenderSystem()
    {
        if(!OgreBeenInitialized)
        {
            Ogre::Root* OgreCore = Ogre::Root::getSingletonPtr();
            Ogre::Plugin* CurrentRenderSystem = 0;

            #ifdef MEZZ_BUILD_DIRECTX9_SUPPORT
            CurrentRenderSystem = new Ogre::D3D9Plugin;
            RenderSystems.push_back(CurrentRenderSystem);
            OgreCore->installPlugin(CurrentRenderSystem);
            RenderSystemTypes.push_back(Graphics::RS_DirectX9);
            #endif
            #ifdef MEZZ_BUILD_DIRECTX11_SUPPORT
            CurrentRenderSystem = new Ogre::D3D11Plugin;
            RenderSystems.push_back(CurrentRenderSystem);
            OgreCore->installPlugin(CurrentRenderSystem);
            RenderSystemTypes.push_back(Graphics::RS_DirectX11);
            #endif
            #ifdef MEZZ_BUILD_OPENGLES2_SUPPORT
            CurrentRenderSystem = new Ogre::GLES2Plugin;
            RenderSystems.push_back(CurrentRenderSystem);
            OgreCore->installPlugin(CurrentRenderSystem);
            RenderSystemTypes.push_back(Graphics::RS_OpenGLES2);
            #endif
            #ifdef MEZZ_BUILD_OPENGLES_SUPPORT
            CurrentRenderSystem = new Ogre::GLESPlugin;
            RenderSystems.push_back(CurrentRenderSystem);
            OgreCore->installPlugin(CurrentRenderSystem);
            RenderSystemTypes.push_back(Graphics::RS_OpenGLES1);
            #endif
            #ifdef MEZZ_BUILD_OPENGL_SUPPORT
            CurrentRenderSystem = new Ogre::GLPlugin;
            RenderSystems.push_back(CurrentRenderSystem);
            OgreCore->installPlugin(CurrentRenderSystem);
            RenderSystemTypes.push_back(Graphics::RS_OpenGL2);
            #endif

            if(RenderSystems.size()==1)
            {
                Ogre::RenderSystem* temp = OgreCore->getRenderSystemByName( GetRenderSystemName(RenderSystemTypes[0]) );
                OgreCore->setRenderSystem( OgreCore->getRenderSystemByName( GetRenderSystemName(RenderSystemTypes[0]) ) );
            }
            else
            {
                Ogre::RenderSystem* temp = OgreCore->getRenderSystemByName( GetRenderSystemName(CurrRenderSys) );
                OgreCore->setRenderSystem( OgreCore->getRenderSystemByName( GetRenderSystemName(CurrRenderSys) ) );
            }

            OgreCore->initialise(false,"");
            OgreBeenInitialized = true;

            PrimaryGameWindow = new Graphics::GameWindow("Primary",1,1,Graphics::GameWindow::WF_Hidden);
        }
    }


    String GraphicsManager::GetObjectRootNodeName() const
    {
        return "DefaultGraphicsManagerSettings";
    }

    void GraphicsManager::AppendCurrentSettings(XML::Node& SettingsRootNode)
    {
        // Create the Group node to be returned
        XML::Node CurrentSettings = SettingsRootNode.AppendChild("Current");
        // Create and initialize the rendersystem settings
        XML::Node RenderSystemNode = CurrentSettings.AppendChild("RenderSystem");
        RenderSystemNode.AppendAttribute("Name").SetValue( GetShortenedRenderSystemName(CurrRenderSys) );
        // Create and initialize the window configuration
        for( GameWindowIterator WinIt = BeginGameWindow() ; WinIt != EndGameWindow() ; ++WinIt )
        {
            XML::Node WindowConfigNode = CurrentSettings.AppendChild("GameWindow");
            WindowConfigNode.AppendAttribute("Caption").SetValue( (*WinIt)->GetWindowCaption() );
            WindowConfigNode.AppendAttribute("Width").SetValue( StringTools::ConvertToString( (*WinIt)->GetRenderWidth() ) );
            WindowConfigNode.AppendAttribute("Height").SetValue( StringTools::ConvertToString( (*WinIt)->GetRenderHeight() ) );
            WindowConfigNode.AppendAttribute("Fullscreen").SetValue( StringTools::ConvertToString( (*WinIt)->GetFullscreen() ) );
            WindowConfigNode.AppendAttribute("Hidden").SetValue( StringTools::ConvertToString( (*WinIt)->IsHidden() ) );
            WindowConfigNode.AppendAttribute("Vsync").SetValue( StringTools::ConvertToString( (*WinIt)->VsyncEnabled() ) );
            WindowConfigNode.AppendAttribute("Resizeable").SetValue( StringTools::ConvertToString( (*WinIt)->BorderIsResizeable() ) );
            WindowConfigNode.AppendAttribute("Borderless").SetValue( StringTools::ConvertToString( (*WinIt)->IsBorderless() ) );
            WindowConfigNode.AppendAttribute("FSAA").SetValue( StringTools::ConvertToString( (*WinIt)->GetFSAALevel() ) );
            /// @todo Currently the maximized setting does nothing in the gamewindow.  If it gets implemented, so does this.
            //WindowConfigNode.AppendAttribute("Maximized").SetValue( (*WinIt)-> );//
            for( Graphics::GameWindow::ViewportIterator VPIt = (*WinIt)->BeginViewport() ; VPIt != (*WinIt)->EndViewport() ; ++VPIt )
            {
                XML::Node ViewportConfigNode = WindowConfigNode.AppendChild("Viewport");
                ViewportConfigNode.AppendAttribute("ZOrder").SetValue( (*VPIt)->GetZOrder() );
                ViewportConfigNode.AppendAttribute("Position").SetValue( StringTools::ConvertToString( Vector2((*VPIt)->GetLeft(),(*VPIt)->GetTop()) ) );
                ViewportConfigNode.AppendAttribute("Size").SetValue( StringTools::ConvertToString( Vector2((*VPIt)->GetWidth(),(*VPIt)->GetHeight()) ) );
            }
        }
    }

    void GraphicsManager::ApplySettingGroupImpl(ObjectSettingGroup* Group)
    {
        for( ObjectSettingSetContainer::SubSetIterator SubSetIt = Group->SubSetBegin() ; SubSetIt != Group->SubSetEnd() ; ++SubSetIt )
        {
            String CurrSettingValue;
            if( "RenderSystem" == (*SubSetIt)->GetName() )
            {
                Graphics::RenderSystem RenderSys = Graphics::RS_OpenGL2;
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Name");
                if( GetShortenedRenderSystemName(Graphics::RS_DirectX9) == CurrSettingValue )
                    RenderSys = Graphics::RS_DirectX9;
                else if( GetShortenedRenderSystemName(Graphics::RS_DirectX11) == CurrSettingValue )
                    RenderSys = Graphics::RS_DirectX11;
                else if( GetShortenedRenderSystemName(Graphics::RS_OpenGL2) == CurrSettingValue )
                    RenderSys = Graphics::RS_OpenGL2;
                else if( GetShortenedRenderSystemName(Graphics::RS_OpenGLES1) == CurrSettingValue )
                    RenderSys = Graphics::RS_OpenGLES1;
                else if( GetShortenedRenderSystemName(Graphics::RS_OpenGLES2) == CurrSettingValue )
                    RenderSys = Graphics::RS_OpenGLES2;

                if(!OgreBeenInitialized)
                {
                    SetRenderSystem(CurrRenderSys,true);
                }else{
                    /// @todo May want to make some other data member so that people can accurately get what is set now, instead of what will be set.
                    Entresol::GetSingletonPtr()->Log("WARNING: Attempting to apply new RenderSystem settings after the GraphicsManager has been initialized.  "
                                                  "These Settings will be applied the next time settings are loaded during manager construction if current settings are saved.");
                    CurrRenderSys = RenderSys;
                }
            }
            else if( "GameWindow" == (*SubSetIt)->GetName() )
            {
                String WinCaption("Mezzanine Window");
                Whole WinWidth = 800;
                Whole WinHeight = 600;
                Whole WinFlags = 0;

                // Get the caption.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Caption");
                if(!CurrSettingValue.empty())
                    WinCaption = CurrSettingValue;
                // Get the width.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Width");
                if(!CurrSettingValue.empty())
                    WinWidth = StringTools::ConvertToUInt32(CurrSettingValue);
                // Get the height.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Height");
                if(!CurrSettingValue.empty())
                    WinHeight = StringTools::ConvertToUInt32(CurrSettingValue);
                // Get fullscreen.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Fullscreen");
                if(!CurrSettingValue.empty())
                {
                    if(StringTools::ConvertToBool(CurrSettingValue))
                        WinFlags = (WinFlags | Graphics::GameWindow::WF_Fullscreen);
                }
                // Get hidden.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Hidden");
                if(!CurrSettingValue.empty())
                {
                    if(StringTools::ConvertToBool(CurrSettingValue))
                        WinFlags = (WinFlags | Graphics::GameWindow::WF_Hidden);
                }
                // Get vsync.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Vsync");
                if(!CurrSettingValue.empty())
                {
                    if(StringTools::ConvertToBool(CurrSettingValue))
                        WinFlags = (WinFlags | Graphics::GameWindow::WF_VsyncEnabled);
                }
                // Get resizable.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Resizeable");
                if(!CurrSettingValue.empty())
                {
                    if(StringTools::ConvertToBool(CurrSettingValue))
                        WinFlags = (WinFlags | Graphics::GameWindow::WF_Resizeable);
                }
                // Get maximized.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Maximized");
                if(!CurrSettingValue.empty())
                {
                    if(StringTools::ConvertToBool(CurrSettingValue))
                        WinFlags = (WinFlags | Graphics::GameWindow::WF_Maximized);
                }
                // Get borderless.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Borderless");
                if(!CurrSettingValue.empty())
                {
                    if(StringTools::ConvertToBool(CurrSettingValue))
                        WinFlags = (WinFlags | Graphics::GameWindow::WF_Borderless);
                }
                // Get the FSAA level
                CurrSettingValue = (*SubSetIt)->GetSettingValue("FSAA");
                if(!CurrSettingValue.empty())
                {
                    switch (StringTools::ConvertToUInt32(CurrSettingValue))
                    {
                        case 2:
                            WinFlags = (WinFlags | Graphics::GameWindow::WF_FSAA_2);
                            break;
                        case 4:
                            WinFlags = (WinFlags | Graphics::GameWindow::WF_FSAA_4);
                            break;
                        case 8:
                            WinFlags = (WinFlags | Graphics::GameWindow::WF_FSAA_8);
                            break;
                        case 16:
                            WinFlags = (WinFlags | Graphics::GameWindow::WF_FSAA_16);
                            break;
                    }
                }
                // Finally, construct the window.
                Graphics::GameWindow* CurrWindow = CreateGameWindow(WinCaption,WinWidth,WinHeight,WinFlags);
                // Set up the viewports
                for( ObjectSettingSetContainer::SubSetIterator VPIt = (*SubSetIt)->SubSetBegin() ; VPIt != (*SubSetIt)->SubSetEnd() ; ++VPIt )
                {
                    if( "Viewport" == (*VPIt)->GetName() )
                    {
                        Integer ZO = 0;
                        Vector2 Position(1,1);
                        Vector2 Size(0,0);

                        CurrSettingValue = (*VPIt)->GetSettingValue("ZOrder");
                        if(!CurrSettingValue.empty())
                            ZO = StringTools::ConvertToInteger( CurrSettingValue );
                        CurrSettingValue = (*VPIt)->GetSettingValue("Position");
                        if(!CurrSettingValue.empty())
                            Position = StringTools::ConvertToVector2( CurrSettingValue );
                        CurrSettingValue = (*VPIt)->GetSettingValue("Size");
                        if(!CurrSettingValue.empty())
                            Size = StringTools::ConvertToVector2( CurrSettingValue );

                        Graphics::Viewport* CurrViewport = CurrWindow->CreateViewport(NULL,ZO);
                        CurrViewport->SetDimensions(Position.X,Position.Y,Size.X,Size.Y);
                    }// if - Viewport
                }// for - Viewports
            }// if - RS || GameWindow
        }// for - SubSets
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Window Management

    Graphics::GameWindow* GraphicsManager::CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags)
    {
        if(!OgreBeenInitialized) InitOgreRenderSystem();

        Graphics::GameWindow* NewWindow = new Graphics::GameWindow(WindowCaption,Width,Height,Flags);
        GameWindows.push_back(NewWindow);
        return NewWindow;
    }

    Graphics::GameWindow* GraphicsManager::GetGameWindow(const Whole& Index)
    {
        return GameWindows.at(Index);
    }

    Whole GraphicsManager::GetNumGameWindows()
    {
        return GameWindows.size();
    }

    void GraphicsManager::DestroyGameWindow(Graphics::GameWindow* ToBeDestroyed)
    {
        for ( std::vector<Graphics::GameWindow*>::iterator it = GameWindows.begin() ; it != GameWindows.end() ; it++ )
        {
            if ( ToBeDestroyed == (*it) )
            {
                delete ToBeDestroyed;
                GameWindows.erase(it);
                return;
            }
        }
    }

    void GraphicsManager::DestroyAllGameWindows(bool ExcludePrimary)
    {
        for(GameWindowContainer::reverse_iterator Iter = GameWindows.rbegin(); Iter!=GameWindows.rend(); Iter++)
            { delete *Iter; }
        GameWindows.clear();

        if(ExcludePrimary)
            { }
        else
        {
            delete PrimaryGameWindow;
            PrimaryGameWindow = 0;
        }
    }

    Graphics::GameWindow* GraphicsManager::GetPrimaryGameWindow()
    {
        return PrimaryGameWindow;
    }

    GraphicsManager::GameWindowIterator GraphicsManager::BeginGameWindow()
    {
        return GameWindows.begin();
    }

    GraphicsManager::GameWindowIterator GraphicsManager::EndGameWindow()
    {
        return GameWindows.end();
    }

    GraphicsManager::ConstGameWindowIterator GraphicsManager::BeginGameWindow() const
    {
        return GameWindows.begin();
    }

    GraphicsManager::ConstGameWindowIterator GraphicsManager::EndGameWindow() const
    {
        return GameWindows.end();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // RenderSystem Management

    void GraphicsManager::SetRenderSystem(const Graphics::RenderSystem& RenderSys, bool InitializeRenderSystem)
    {
        if(!OgreBeenInitialized) CurrRenderSys = RenderSys;
        else { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to set RenderSystem after graphics has been initialized.  This is not supported."); }

        if(InitializeRenderSystem)
            InitOgreRenderSystem();
    }

    Graphics::RenderSystem GraphicsManager::GetCurrRenderSystem()
    {
        return CurrRenderSys;
    }

    String GraphicsManager::GetRenderSystemName(const Graphics::RenderSystem& RenderSys)
    {
        switch(RenderSys)
        {
            case Graphics::RS_DirectX9: return "Direct3D9 Rendering Subsystem"; break;
            case Graphics::RS_DirectX11: return "Direct3D11 Rendering Subsystem"; break;
            case Graphics::RS_OpenGL2: return "OpenGL Rendering Subsystem"; break;  /// @todo This will likely have to change when other OGL systems are implemented
            //case Graphics::RS_OpenGL3: return ""; break;  Not yet implemented
            //case Graphics::RS_OpenGL4: return ""; break;  Not yet implemented
            case Graphics::RS_OpenGLES1: return "OpenGL ES 1.x Rendering Subsystem"; break;
            case Graphics::RS_OpenGLES2: return "OpenGL ES 2.x Rendering Subsystem"; break;
        }
        return "";
    }

    String GraphicsManager::GetShortenedRenderSystemName(const Graphics::RenderSystem& RenderSys)
    {
        switch(RenderSys)
        {
            case Graphics::RS_DirectX9: return "Direct3D9"; break;
            case Graphics::RS_DirectX11: return "Direct3D11"; break;
            case Graphics::RS_OpenGL2: return "OpenGL"; break;  /// @todo This will likely have to change when other OGL systems are implemented
            //case Graphics::RS_OpenGL3: return ""; break;  Not yet implemented
            //case Graphics::RS_OpenGL4: return ""; break;  Not yet implemented
            case Graphics::RS_OpenGLES1: return "OpenGLES1.x"; break;
            case Graphics::RS_OpenGLES2: return "OpenGLES2.x"; break;
        }
        return "";
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Query Methods

    const StringVector* GraphicsManager::GetSupportedResolutions()
    {
        return &SupportedResolutions;
    }

    const StringVector* GraphicsManager::GetSupportedDevices()
    {
        return &SupportedDevices;
    }

    const WindowSettings& GraphicsManager::GetDesktopSettings()
    {
        return DesktopSettings;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility Methods

    void GraphicsManager::ResetRenderTimer()
    {
        this->RenderTimer->reset();
    }

    void GraphicsManager::RenderOneFrame()
    {
        Entresol* TheEntresol = Entresol::GetSingletonPtr();
        #ifdef MEZZDEBUG
        TheEntresol->Log("Rendering the World.");
        #endif
        Ogre::Root::getSingleton().renderOneFrame();
        if( !GetPrimaryGameWindow()->_GetOgreWindowPointer()->isVisible() )
            Ogre::Root::getSingleton().clearEventTimes();
        #ifdef MEZZDEBUG
        TheEntresol->Log("Finished Rendering");
        #endif
    }

    void GraphicsManager::SwapAllBuffers(bool WaitForVsync)
    {
        for( Whole X = 0 ; X < GetNumGameWindows() ; X++ )
            GetGameWindow(X)->_GetOgreWindowPointer()->swapBuffers(false);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // SubSystem Initialization

    bool GraphicsManager::HasSDLBeenInitialized()
    {
        return SDL_WasInit(SDL_INIT_VIDEO);
    }

    bool GraphicsManager::HasOgreBeenInitialized()
    {
        return OgreBeenInitialized;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited from ManagerBase

    void GraphicsManager::Initialize()
    {
        if(!OgreBeenInitialized)
            InitOgreRenderSystem();
        this->RenderTimer = new Ogre::Timer();

        Ogre::ConfigOptionMap& CurrentRendererOptions = Ogre::Root::getSingleton().getRenderSystem()->getConfigOptions();
        for( Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin() ;
            configItr != CurrentRendererOptions.end() ; configItr++)
        {
            if( (configItr)->first == "Video Mode" )
            {
                for( Whole X = 0 ; X < (configItr)->second.possibleValues.size() ; X++ )
                {
                    String NewRes = (configItr)->second.possibleValues[X];
                    StringTools::RemoveDuplicateWhitespaces(NewRes);
                    StringTools::Trim(NewRes);
                    SupportedResolutions.push_back(NewRes);
                }
                continue;
            }
            if( (configItr)->first == "Rendering Device" )
            {
                for( Whole Y = 0 ; Y < (configItr)->second.possibleValues.size() ; Y++ )
                    SupportedDevices.push_back((configItr)->second.possibleValues[Y]);
                continue;
            }
        }

        if(AutoGenFiles)
            SaveAllSettings();

        Initialized = true;
    }

    void GraphicsManager::DoMainLoopItems()
    {
        Ogre::WindowEventUtilities::messagePump();
        RenderOneFrame();

        //Do Time Calculations to Determine Rendering Time
        this->TheEntresol->SetFrameTime( this->RenderTimer->getMilliseconds() );
        this->RenderTimer->reset();
        if(this->TheEntresol->GetTargetFrameTime() > this->TheEntresol->GetFrameTime()){
            FrameDelay++;
        }else if(this->TheEntresol->GetTargetFrameTime() == this->TheEntresol->GetFrameTime()){
        }else{
            if (0<FrameDelay){
                FrameDelay--;
            }else{
                FrameDelay=0;
            }
        }
        crossplatform::WaitMilliseconds( FrameDelay );
    }

    bool GraphicsManager::PostMainLoopItems()
    {
        EventManager::GetSingletonPtr()->AddEvent(new EventRenderTime(this->TheEntresol->GetFrameTime()));
        return ManagerBase::PostMainLoopItems();
    }

    ManagerBase::ManagerType GraphicsManager::GetInterfaceType() const
        { return ManagerBase::GraphicsManager; }

    String GraphicsManager::GetImplementationTypeName() const
        { return "DefaultGraphicsManager"; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultGraphicsManagerFactory Methods

    DefaultGraphicsManagerFactory::DefaultGraphicsManagerFactory()
    {
    }

    DefaultGraphicsManagerFactory::~DefaultGraphicsManagerFactory()
    {
    }

    String DefaultGraphicsManagerFactory::GetManagerTypeName() const
    {
        return "DefaultGraphicsManager";
    }

    ManagerBase* DefaultGraphicsManagerFactory::CreateManager(NameValuePairList& Params)
    {
        if(GraphicsManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return GraphicsManager::GetSingletonPtr();
        }else{
            if(Params.empty()) return new GraphicsManager();
            else
            {
                Whole Width = 800;
                Whole Height = 600;
                bool FullScreen = false;
                for( NameValuePairList::iterator ParIt = Params.begin() ; ParIt != Params.end() ; ++ParIt )
                {
                    String Lower = (*ParIt).first;
                    StringTools::ToLowerCase(Lower);
                    if( "width" == Lower )
                    {
                        Width = StringTools::ConvertToUInt32( (*ParIt).second );
                    }
                    else if( "height" == Lower )
                    {
                        Height = StringTools::ConvertToUInt32( (*ParIt).second );
                    }
                    else if( "fullscreen" == Lower )
                    {
                        FullScreen = StringTools::ConvertToBool( (*ParIt).second );
                    }
                }
                return new GraphicsManager();
            }
        }
    }

    ManagerBase* DefaultGraphicsManagerFactory::CreateManager(XML::Node& XMLNode)
    {
        if(GraphicsManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return GraphicsManager::GetSingletonPtr();
        }else return new GraphicsManager(XMLNode);
    }

    void DefaultGraphicsManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
    {
        delete ToBeDestroyed;
    }
}//Mezzanine

#endif
