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
#ifndef _audiooalssoundscapemanager_h
#define _audiooalssoundscapemanager_h

#include "Audio/soundscapemanager.h"
#include "uidgenerator.h"

// OALS forward declares
#ifndef OALS_STRUCTS_DECLARED
struct ALCcontext;
#endif //OALS_STRUCTS_DECLARED

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            class Listener;
            class SoundProxy;
            class SoundScapeManager;

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the work unit for updating audio buffers as necessary for audio playback.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB BufferUpdate3DWorkUnit : public Audio::iBufferUpdate3DWorkUnit
            {
            protected:
                /// @internal
                /// @brief A pointer to the manager this work unit is processing.
                OALS::SoundScapeManager* TargetManager;
                /// @internal
                /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
                /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
                BufferUpdate3DWorkUnit(const BufferUpdate3DWorkUnit& Other);
                /// @internal
                /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
                /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
                BufferUpdate3DWorkUnit& operator=(const BufferUpdate3DWorkUnit& Other);
            public:
                /// @brief Class constructor.
                /// @param Target The InputManager this work unit will process during the frame.
                BufferUpdate3DWorkUnit(OALS::SoundScapeManager* Target);
                /// @brief Class destructor.
                virtual ~BufferUpdate3DWorkUnit();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief This does any required updating of audio buffers belonging to sound proxies in this manager.
                /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
                virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
            };//BufferUpdate3DWorkUnit

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the base manager class for audio being played in a 3D environment.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB SoundScapeManager : public Audio::SoundScapeManager
            {
            public:
                /// @brief Basic container type for OALS context storage by this class.
                typedef std::vector<ALCcontext*>                    ContextContainer;
                /// @brief Iterator type for OALS context instances stored by this class.
                typedef ContextContainer::iterator                  ContextIterator;
                /// @brief Const Iterator type for OALS context instances stored by this class.
                typedef ContextContainer::const_iterator            ConstContextIterator;
                /// @brief Basic container type for @ref OALS::Listener storage by this class.
                typedef std::vector<OALS::Listener*>                ListenerContainer;
                /// @brief Iterator type for @ref OALS::Listener instances stored by this class.
                typedef ListenerContainer::iterator                 ListenerIterator;
                /// @brief Const Iterator type for @ref OALS::Listener instances stored by this class.
                typedef ListenerContainer::const_iterator           ConstListenerIterator;
                /// @brief Basic container type for @ref OALS::SoundProxy storage by this class.
                typedef std::vector<OALS::SoundProxy*>              ProxyContainer;
                /// @brief Iterator type for @ref OALS::SoundProxy instances stored by this class.
                typedef ProxyContainer::iterator                    ProxyIterator;
                /// @brief Const Iterator type for @ref OALS::SoundProxy instances stored by this class.
                typedef ProxyContainer::const_iterator              ConstProxyIterator;

                /// @brief A String containing the name of this manager implementation.
                static const String ImplementationName;
                /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
                static const ManagerBase::ManagerType InterfaceType;
            protected:
                friend class BufferUpdate3DWorkUnit;

                /// @brief Generator responsible for creating unique IDs for @ref OALS::Listener and @ref OALS::SoundProxy instances.
                UIDGenerator<UInt32> ProxyIDGen;
                /// @brief Container storing all OALS context instances.
                ContextContainer Contexts;
                /// @brief Container storing all @ref OALS::Listener instances.
                ListenerContainer Listeners;
                /// @brief Container storing all @ref OALS::SoundProxy instances.
                ProxyContainer Proxies;

                /// @brief The workunit this will use to complete its buffer updates.
                OALS::BufferUpdate3DWorkUnit* BufferUpdate3DWork;
                /// @brief Can be used for thread safe logging and other thread specific resources.
                Threading::DefaultThreadSpecificStorage::Type* ThreadResources;

                /// @brief Creates a new context for use by objects in this manager.
                ALCcontext* CreateContext();
                /// @brief Destroys an existing context owned by this manager.
                void DestroyContext(ALCcontext* Context);
                /// @brief Destroys all contexts owned by this manager.
                void DestroyAllContexts();
            public:
                /// @brief Class constructor.
                /// @param Creator The parent world that is creating the manager.
                SoundScapeManager(World* Creator);
                /// @brief XML constructor.
                /// @param Creator The parent world that is creating the manager.
                /// @param XMLNode The node of the xml document to construct from.
                SoundScapeManager(World* Creator, const XML::Node& XMLNode);
                /// @brief Class destructor.
                virtual ~SoundScapeManager();

                ///////////////////////////////////////////////////////////////////////////////
                // Proxy Creation

                /// @copydoc Audio::SoundScapeManager::CreateListener()
                iListener* CreateListener();
                /// @copydoc Audio::SoundScapeManager::CreateListener(XML::Node&)
                iListener* CreateListener(const XML::Node& SelfRoot);

                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16)
                Audio::SoundProxy* CreateSoundProxy(const UInt16 Type);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, DataStreamPtr, const Audio::Encoding)
                Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, DataStreamPtr Stream, const Audio::Encoding Encode);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, DataStreamPtr, const UInt32, const Audio::BitConfig)
                Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, const String&, const String&)
                Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, const String& FileName, const String& Group);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, const String&, Char8*, const UInt32, const Audio::Encoding)
                Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, const String&, Char8*, const UInt32, const UInt32, const Audio::BitConfig)
                Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(XML::Node&)
                Audio::SoundProxy* CreateSoundProxy(const XML::Node& SelfRoot);

                /// @copydoc EntityComponentManager::CreateComponent(const XML::Node&)
                EntityComponent* CreateComponent(const XML::Node& SelfRoot);

                ///////////////////////////////////////////////////////////////////////////////
                // General Proxy Management

                /// @copydoc EntityComponentManager::GetComponentByID(const UInt32) const
                EntityComponent* GetComponentByID(const UInt32 ID) const;

                /// @copydoc EntityComponentManager::GetNumComponents() const
                UInt32 GetNumComponents() const;
                /// @copydoc EntityComponentManager::GetNumComponents(const UInt32
                UInt32 GetNumComponents(const UInt32 Types) const;
                /// @copydoc EntityComponentManager::GetComponents() const
                EntityComponentManager::ComponentVec GetComponents() const;

                /// @copydoc EntityComponentManager::DestroyComponent(EntityComponent*)
                void DestroyComponent(EntityComponent* ToBeDestroyed);
                /// @copydoc EntityComponentManager::DestroyAllComponents(const UInt32)
                void DestroyAllComponents(const UInt32 Types);
                /// @copydoc EntityComponentManager::DestroyAllComponents()
                void DestroyAllComponents();

                ///////////////////////////////////////////////////////////////////////////////
                // Specific Proxy Management

                /// @brief Gets a Listener instance by index.
                /// @param Index The index of the Listener to be retrieved.
                /// @return Returns a pointer to the Listener at the specified index.
                OALS::Listener* GetListener(const UInt32 Index) const;
                /// @brief Gets the number of Listener instances in this manager.
                /// @return Returns a UInt32 representing the number of Listener instances contained in this manager.
                UInt32 GetNumListeners() const;
                /// @brief Deletes a Listener.
                /// @param ToBeDestroyed A pointer to the Listener you want deleted.
                void DestroyListener(OALS::Listener* ToBeDestroyed);
                /// @brief Deletes all stored Listener instances.
                void DestroyAllListeners();

                /// @brief Gets an SoundProxy instance by index.
                /// @param Index The index of the SoundProxy to be retrieved.
                /// @return Returns a pointer to the SoundProxy at the specified index.
                OALS::SoundProxy* GetSoundProxy(const UInt32 Index) const;
                /// @brief Gets the number of SoundProxy instances in this manager.
                /// @return Returns a UInt32 representing the number of SoundProxy instances contained in this manager.
                UInt32 GetNumSoundProxies() const;
                /// @brief Deletes a SoundProxy.
                /// @param ToBeDestroyed A pointer to the SoundProxy you want deleted.
                void DestroySoundProxy(OALS::SoundProxy* ToBeDestroyed);
                /// @brief Deletes all stored SoundProxy instances.
                void DestroyAllSoundProxies();

                #ifndef SWIG
                /// @brief Gets an iterator to the first Sound Proxy in this manager.
                /// @return Returns an iterator to the first Sound Proxy being stored by this manager.
                ProxyIterator BeginSoundProxy();
                /// @brief Gets an iterator to one passed the last Sound Proxy in this manager.
                /// @return Returns an iterator to one passed the last Sound Proxy being stored by this manager.
                ProxyIterator EndSoundProxy();
                /// @brief Gets a const iterator to the first Sound Proxy in this manager.
                /// @return Returns a const iterator to the first Sound Proxy being stored by this manager.
                ConstProxyIterator BeginSoundProxy() const;
                /// @brief Gets a const iterator to one passed the last Sound Proxy in this manager.
                /// @return Returns a const iterator to one passed the last Sound Proxy being stored by this manager.
                ConstProxyIterator EndSoundProxy() const;
                #endif

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc WorldManager::Pause(const UInt32)
                virtual void Pause(const UInt32 PL);

                /// @copydoc WorldManager::Initialize()
                virtual void Initialize();
                /// @copydoc ManagerBase::Deinitialize()
                virtual void Deinitialize();

                /// @copydoc Audio::SoundScapeManager::GetBufferUpdate3DWork()
                virtual iBufferUpdate3DWorkUnit* GetBufferUpdate3DWork();

                ///////////////////////////////////////////////////////////////////////////////
                // Type Identifier Methods

                /// @copydoc ManagerBase::GetImplementationTypeName()
                virtual String GetImplementationTypeName() const;
            };//SoundScapeManager
        }//OALS
    }//Audio
}//Mezzanine

#endif
