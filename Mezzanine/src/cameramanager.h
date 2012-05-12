//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _CameraManager_h
#define _CameraManager_h

#include "datatypes.h"
#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"
#include "quaternion.h"
#include "ray.h"
#include "vector3.h"

namespace Ogre
{
    class Camera;
    class SceneNode;
    class Viewport;
}

namespace Mezzanine
{
    class Camera;
    class CameraController;
    class SceneManager;
    class GraphicsManager;
    namespace Internal
    {
        class SceneManagerData;
    }

    // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
    #ifdef SWIG
    %template(SingletonCameraManager) Singleton<CameraManager>;
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// @class CameraManager
    /// @headerfile cameramanager.h
    /// @brief This is the manager class for all camera functions.
    /// @details This class contains all the functionality of the use and manipulation of the camera. @n
    /// All functions that manipulate the camera will default to the default camera, so if you only use
    /// one camera you should never have to name the camera you want to use. @n
    /// This class should only be created after the SceneManager has been created.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB CameraManager : public ManagerBase, public Singleton<CameraManager>
    {
        protected:
            friend class Internal::SceneManagerData;
            friend class GraphicsManager;
            friend class World;
            friend class Camera;

            Camera* DefaultCamera;
            std::vector< Camera* > Cameras;
            std::map< Camera* , CameraController* > CameraControllers;
            Camera* FindCamera(const String& Name);
            /// @internal
            /// @brief Used to reference the appropriate scene
            Mezzanine::SceneManager* SManager;
        public:
            /// @brief Class Constructor.
            CameraManager();
#ifdef MEZZXML
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            CameraManager(xml::Node& XMLNode);
#endif
            /// @brief Class Destructor.
            virtual ~CameraManager();

            /// @brief Creates a camera.
            /// @remarks This function will autogenerate the name for the camera.
            /// @return Returns a pointer to the created camera.
            Camera* CreateCamera();
            /// @brief Creates a camera.
            /// @param Name The name to be assigned to the created camera.
            /// @return Returns a pointer to the created camera.
            Camera* CreateCamera(const String& Name);
            /// @brief Gets an already created camera by name.
            /// @return Returns a pointer to the camera of the specified name.
            Camera* GetCamera(const String& Name);
            /// @brief Gets an already created camera by index.
            /// @return Returns a pointer to the camera at the specified index.
            Camera* GetCamera(const Whole& Index);
            /// @brief Gets the number of cameras created and stored in this manager.
            /// @return Returns the number of cameras this manager is storing.
            Whole GetNumCameras();
            /// @brief Destroy's all stored camera's.
            void DestroyAllCameras();

            /// @brief Gets a camera controller if it exists, otherwise creates it.
            /// @param Controlled The camera that will be controlled by the controller returned.
            /// @return Returns a pointer to the created or retrieved camera controller for the camera.
            CameraController* GetOrCreateCameraController(Camera* Controlled);
            /// @brief Destroys a cameracontroller.
            /// @param ToBeDestroyed Pointer to the cameracontrolled you want destroyed.
            void DestroyCameraController(CameraController* ToBeDestroyed);
            /// @brief Destroys a cameracontroller by camera.
            /// @param ControlledCam The camera who's controller will be destroyed.  This doesn't do anything to the camera.
            void DestroyCameraController(Camera* ControlledCam);
            /// @brief Destroys all camera controllers being stored in this manager.
            void DestroyAllCameraControllers();

            ///////////////////////////////////////////////////////////////////////////////
            //Inherited from ManagerBase

            /// @copydoc ManagerBase::Initialize()
            virtual void Initialize();
            /// @copydoc ManagerBase::DoMainLoopItems()
            virtual void DoMainLoopItems();
            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;
    };//CameraManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DefaultCameraManagerFactory
    /// @headerfile cameramanager.h
    /// @brief A factory responsible for the creation and destruction of the default cameramanager.
    ///////////////////////////////////////
    class MEZZ_LIB DefaultCameraManagerFactory : public ManagerFactory
    {
        public:
            /// @brief Class constructor.
            DefaultCameraManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultCameraManagerFactory();

            /// @copydoc ManagerFactory::GetManagerTypeName()
            String GetManagerTypeName() const;
            /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
            ManagerBase* CreateManager(NameValuePairList& Params);
#ifdef MEZZXML
            /// @copydoc ManagerFactory::CreateManager(xml::Node&)
            ManagerBase* CreateManager(xml::Node& XMLNode);
#endif
            /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
            void DestroyManager(ManagerBase* ToBeDestroyed);
    };//DefaultCameraManagerFactory
}//Mezzanine
#endif
