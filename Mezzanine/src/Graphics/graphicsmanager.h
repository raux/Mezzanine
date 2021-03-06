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
#ifndef _graphicsmanager_h
#define _graphicsmanager_h

#include "Graphics/windowsettings.h"
#include "Graphics/graphicsenumerations.h"
#include "Graphics/gamewindow.h"

#include "Threading/dagframescheduler.h"

#include "entresolmanager.h"
#include "entresolmanagerfactory.h"
#include "eventpublisher.h"
#include "objectsettings.h"
#include "singleton.h"

namespace Ogre
{
    class Timer;
    class RenderWindow;
    class Viewport;
    class Plugin;
}

struct SDL_Surface;
struct SDL_Window;

namespace Mezzanine
{
    namespace Graphics
    {
        // Used by the scripting language binder to help create bindings for this class. SWIG does know to creation template instances
        #ifdef SWIG
        %template(SingletonGraphicsManager) Singleton<GraphicsManager>;
        #endif

        class GraphicsManager;

        /// @brief This does the main loop processing for required to make the Graphics Manager function
        class MEZZ_LIB RenderWorkUnit : public Threading::MonopolyWorkUnit
        {
        protected:
            /// @brief The GraphicsManager this will work with
            GraphicsManager* TargetManager;

            /// @brief Private copy constructor to prevent useless copying of this,
            RenderWorkUnit(const RenderWorkUnit&) {}
            /// @brief Private assignment operator to prevent useless assignment of this,
            void operator=(RenderWorkUnit) {}

        public:
            /// @brief Create a GraphicsWorkUnit
            /// @param WhichGraphicsManager This is the Manager that this Work unit must work with.
            RenderWorkUnit(GraphicsManager* Target);
            /// @brief virtual deconstructor
            virtual ~RenderWorkUnit();

            /// @brief Once The graphics is properly multithread, this will set the amount of threads it should use
            /// @param AmountToUse Currently Ignored.
            virtual void UseThreads(const Whole& AmountToUse);
            /// @brief Get the amount of threads this will attempt to sue
            /// @return 1, this will return 1 until this Ogre threading is implemented.
            virtual Whole UsingThreadCount();
            /// @brief This does any required update of the Graphical Scene graph and REnders one frame
            virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
        };//RenderWorkUnit

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is intended to store basic graphics setting for the user.
        /// @details This stores x/y resolution, fullscreen and in the future other
        /// settings. This is intended to make it easy for developers to pass/move around
        /// complex graphics settings. We hope to eventually include other items like
        /// shader settings, rendering API, and maybe other settings too.
        ///////////////////////////////////////
        class MEZZ_LIB GraphicsManager: public EntresolManager, public ObjectSettingsHandler, public Singleton<GraphicsManager>
        {
        public:
            /// @brief Basic container type for @ref GameWindow storage by this class.
            using GameWindowContainer = std::vector<GameWindow*>;
            /// @brief Iterator type for @ref GameWindow instances stored by this class.
            using GameWindowIterator = GameWindowContainer::iterator;
            /// @brief Const Iterator type for @ref GameWindow instances stored by this class.
            using ConstGameWindowIterator = GameWindowContainer::const_iterator;
            /// @brief Basic container type for storing the detected supported Fullscreen resolutions on the current system.
            using ResolutionContainer = std::vector<Resolution>;
            /// @brief Iterator type for stored supported resolutions.
            using ResolutionIterator = ResolutionContainer::iterator;
            /// @brief Const Iterator type for stored supported resolutions.
            using ConstResolutionIterator = ResolutionContainer::const_iterator;
            /// @brief Basic container type for registered RenderSystem type storage by this class.
            using RenderSystemTypeContainer = std::vector<RenderSystem>;
            /// @brief Convenience type for the publisher of events fired by this manager.
            using EventPublisherType = WindowEventPublisher;

            /// @brief A String containing the name of this manager implementation.
            static const String ImplementationName;
            /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
            static const ManagerBase::ManagerType InterfaceType;

            /// @brief Identifier for the Event that is raised when a Window is created.
            /// @details Events with this name are of type: WindowEvent.
            static const EventNameType EventWindowCreated;
            /// @brief Identifier for the Event that is raised when a Window is destroyed.
            /// @details Events with this name are of type: WindowEvent.  The Event will be dispatched prior to
            /// the window being destroyed, however once the event dispatch is complete it will be promptly
            /// destroyed.  So any subscribers reacting to the destruction must be immediate if they need the
            /// window intact.
            static const EventNameType EventWindowDestroyed;
        protected:
            friend class RenderWorkUnit;

            /// @brief A container storing all the game windows created by this manager.
            GameWindowContainer GameWindows;
            /// @brief A container of strings storing all the detected supported resolutions on the current hardware.
            ResolutionContainer SupportedResolutions;
            /// @brief A container of strings storing all the detected names of video devices on the current hardware.
            StringVector SupportedDevices;
            /// @brief A publisher for notifying when the system graphics configuration changes.
            EventPublisherType GraphicsPublisher;
            /// @brief A struct storing the dimensions of the desktop on the current hardware.
            WindowSettings DesktopSettings;
            /// @brief A pointer to the hidden window storing the context and render resources.
            GameWindow* PrimaryWindow;

            /// @brief The work unit that does all the rendering.
            RenderWorkUnit* RenderWork;
            /// @brief Can be used for thread safe logging and other thread Specific resources.
            Threading::DefaultThreadSpecificStorage::Type* ThreadResources;

            /// @brief A RenderSystem enum value storing the RenderSystem type currently in use.
            Graphics::RenderSystem CurrRenderSys;
            /// @brief Stores whether the internal graphics subsystem has been initialized.
            Boole OgreBeenInitialized;

            /// @brief Construct the manager and set sane defaults.
            void Construct();

            /// @brief Pumps the internal event queue of graphics events.
            void PumpInternalEvents();
            /// @brief Initializes the internal graphics subsystem with the currently set configuration.
            void InitOgreRenderSystem();

            /// @copydoc ObjectSettingsHandler::GetObjectRootNodeName() const
            virtual String GetObjectRootNodeName() const;
            /// @copydoc ObjectSettingsHandler::AppendCurrentSettings(XML::Node&)
            virtual void AppendCurrentSettings(XML::Node& SettingsRootNode);
            /// @copydoc ObjectSettingsHandler::ApplySettingGroupImpl(ObjectSettingGroup*)
            virtual void ApplySettingGroupImpl(ObjectSettingGroup* Group);
        public:
            /// @brief Basic constructor.
            /// @details This creates a basic Graphics Settings with resolution 640x480 with fullscreen set to false.
            GraphicsManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            GraphicsManager(const XML::Node& XMLNode);
            /// @brief Class Destructor.
            virtual ~GraphicsManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Window Management

            /// @brief Creates a new GameWindow to be rendered to.
            /// @param WindowCaption The caption to be set in the window title bar.
            /// @param Width The desired width in pixels.
            /// @param Height The desired height in pixels.
            /// @param Flags Additional misc parameters, see GameWindow class for more info.
            /// @return Returns a pointer to the created window.
            GameWindow* CreateGameWindow(const String& WindowCaption, const Whole Width, const Whole Height, const Whole Flags);
            /// @brief Gets a GameWindow by index.
            /// @param Index The index of the window to retrieve.
            /// @return Returns a pointer to the GameWindow at the specified index.
            GameWindow* GetGameWindow(const Whole Index) const;
            /// @brief Gets a GameWindow by it's unique ID.
            /// @param WinID The ID of the Window to retrieve.
            /// @return Returns a pointer to the GameWindow with the specified ID.
            GameWindow* GetGameWindowByID(const UInt32 WinID) const;
            /// @brief Gets a GameWindow by it's caption text.
            /// @note If multiple windows have the same caption, the first one found will be returned.
            /// @param Caption The caption of the window to retrieve.
            /// @return Returns a pointer to the GameWindow with the specified caption, or NULL if no GameWindows with that caption exist.
            GameWindow* GetGameWindowByCaption(const String& Caption) const;
            /// @brief Gets the number of GameWindows within this manager.
            /// @return Returns a Whole representing the number of GameWindow instances within this manager.
            Whole GetNumGameWindows() const;
            /// @brief Destroys a created GameWindow by index.
            /// @param WindowIndex The index of the window to be destroyed.
            void DestroyGameWindow(GameWindow* ToBeDestroyed);
            /// @brief Destroys every GameWindow created.
            /// @param ExcludePrimary Whether or not you want to spare the primary window created.
            void DestroyAllGameWindows(Boole ExcludePrimary = true);
            /// @brief Gets the primary(first) GameWindow.
            /// @return Returns a pointer to the primary GameWindow.
            GameWindow* GetPrimaryWindow();

            /// @brief Gets an iterator to the first GameWindow stored in this manager.
            GameWindowIterator BeginGameWindow();
            /// @brief Gets an iterator to one passed the last GameWindow stored in this manager.
            GameWindowIterator EndGameWindow();
            /// @brief Gets a const iterator to the first GameWindow stored in this manager.
            ConstGameWindowIterator BeginGameWindow() const;
            /// @brief Gets a const iterator to one passed the last GameWindow stored in this manager.
            ConstGameWindowIterator EndGameWindow() const;

            ///////////////////////////////////////////////////////////////////////////////
            // RenderSystem Management

            /// @brief Sets the render system to be used.
            /// @remarks This will only work prior to a window being created/graphics manager being initialized.  The internal structures to be built need
            /// to know what rendersystem to build for.  Additionally this cannot be swapped/changed at runtime.  If called after a window has been made this will throw an exception.
            /// @param RenderSys The Render system to be used.
            /// @param InitializeRenderSystem Whether to immediately initialize the rendersystem afterwords.
            void SetRenderSystem(const Graphics::RenderSystem RenderSys, Boole InitializeRenderSystem = false);
            /// @brief Gets the current rendersystem being used.
            /// @remarks This does not return a pointer or any other kind of accessor to the actual rendersystem structure.  If you need that, then we're doing something wrong.
            /// @return Returns an enum value coresponding to the render system being used.
            Graphics::RenderSystem GetCurrRenderSystem();
            /// @brief Gets the name of the provided render system.
            /// @param RenderSys The rendersystem to get the name of.
            /// @return Returns a string containing the name of the provided render system.
            String GetRenderSystemName(const Graphics::RenderSystem RenderSys);
            /// @brief Gets a short hand name of the provided render system.
            /// @param RenderSys The rendersystem to get the name of.
            /// @return Returns a string containing the shortened name of the provided render system.
            String GetShortenedRenderSystemName(const Graphics::RenderSystem RenderSys);

            ///////////////////////////////////////////////////////////////////////////////
            // Query Methods

            /// @brief Gets a vector containing all the resolutions supported by this render system on the current hardware.
            /// @details This vector is populated when the manager gets initialized.  Calling on it before then will give you an empty vector.
            /// @return Returns a Const Pointer to the vector storing all the supported resolutions.
            const ResolutionContainer& GetSupportedResolutions() const;
            /// @brief Gets a vector containing all the devices supported by this render system on the current hardware.
            /// @details This vector is populated when the manager gets initialized.  Calling on it before then will give you an empty vector.
            /// @return Returns a Const Pointer to the vector storing all the supported devices.
            const StringVector& GetSupportedDevices() const;
            /// @brief Gets the desktop display settings.
            /// @param Returns a WindowSettings struct with the desktop display settings.
            const WindowSettings& GetDesktopSettings() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Renders one frame of the scene.
            void RenderOneFrame();
            /// @brief Swaps all the buffers of all GameWindows.
            /// @param WaitForVsync Whether or not the buffer should swap after the vsync interval is completed.
            void SwapAllBuffers(Boole WaitForVsync);

            /// @copydoc ManagerBase::Initialize()
            /// @details Added a GraphicWorkUnit to the WorkScheduler on the Entresol as a
            /// Monopoly, Initializes the Render device and rendering system if that has
            /// not already been done, if configured to will save all graphics configuration
            /// files.
            virtual void Initialize();
            /// @copydoc ManagerBase::Deinitialize()
            virtual void Deinitialize();

            /// @brief Gets the work unit responsible for performing the graphics render of all scenes.
            /// @return Returns a pointer to the RenderWorkUnit being used by this manager.
            RenderWorkUnit* GetRenderWork();
            /// @brief Gets the publisher for the Graphics system events.
            /// @return Returns a reference to the publisher that will dispatch Graphics events.
            EventPublisherType& GetGraphicsPublisher();
            /// @brief Gets the publisher for the Graphics system events.
            /// @return Returns a const reference to the publisher that will dispatch Graphics events.
            const EventPublisherType& GetGraphicsPublisher() const;

            ///////////////////////////////////////////////////////////////////////////////
            // SubSystem Initialization

            /// @brief Gets whether or not SDL has been started.
            /// @return Returns a Boole indicating whether or not SDL has been initialized yet.
            Boole HasSDLBeenInitialized();
            /// @brief Gets whether or not Ogre has been started.
            /// @return Returns a Boole indicating whether or not Ogre has been initialized yet.
            Boole HasOgreBeenInitialized();

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;
        };//GraphicsManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A factory responsible for the creation and destruction of the default graphicsmanager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultGraphicsManagerFactory : public EntresolManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultGraphicsManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultGraphicsManagerFactory();

            /// @copydoc ManagerFactory::GetManagerImplName()
            String GetManagerImplName() const;
            /// @copydoc ManagerFactory::GetManagerType() const
            ManagerBase::ManagerType GetManagerType() const;

            /// @copydoc EntresolManagerFactory::CreateManager(const NameValuePairList&)
            EntresolManager* CreateManager(const NameValuePairList& Params);
            /// @copydoc EntresolManagerFactory::CreateManager(const XML::Node&)
            EntresolManager* CreateManager(const XML::Node& XMLNode);
            /// @copydoc EntresolManagerFactory::DestroyManager(EntresolManager*)
            void DestroyManager(EntresolManager* ToBeDestroyed);
        }; // DefaultGraphicsManagerFactory
    } // Graphics namespace
} // Mezzanine
#endif
