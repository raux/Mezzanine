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
#ifndef _graphicsrenderableproxy_h
#define _graphicsrenderableproxy_h

/// @file
/// @brief This file contains the declaration for the base class from which graphics proxies inherit.

#include "entityproxy.h"
#include "axisalignedbox.h"

namespace Ogre
{
    class MovableObject;
    class SceneNode;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class SceneManager;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base proxy class for world proxies wrapping functionality of the graphics subsystem.
        ///////////////////////////////////////
        class MEZZ_LIB RenderableProxy : public EntityProxy
        {
        protected:
            /// @brief A pointer to the internal object storing the proxy transform.
            Ogre::SceneNode* GraphicsNode;
            /// @brief This is a pointer to the scene manager that created and owns this proxy.
            SceneManager* Manager;
            /// @brief This is a bitmask identifying this objects type when being rendered.  Used for advanced visibility configuration.
            UInt32 VisibilityMask;
            /// @brief This is a bitmask identifying this objects type when being queried.  Used for advanced query configuration.
            UInt32 QueryMask;
            /// @brief This stores whether the proxy is currently in the graphics world or not.
            Boole InWorld;
        public:
            /// @brief XML-assist Constructor.
            /// @param Creator A pointer to the manager that created this proxy.
            RenderableProxy(SceneManager* Creator);
            /// @brief Normal Constructor.
            /// @param ID The unique ID assigned to this proxy.
            /// @param Creator A pointer to the manager that created this proxy.
            RenderableProxy(const UInt32 ID, SceneManager* Creator);
            /// @brief Class destructor.
            virtual ~RenderableProxy();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets this proxies AABB.
            /// @note This will only return valid values if this proxy is in the world.  A proxy outside of the world has no AABB.
            /// @return Returns an AxisAlignedBox containing the AABB of this graphics proxy.
            virtual AxisAlignedBox GetAABB() const;

            /// @copydoc EntityProxy::Activate()
            virtual void Activate() override;
            /// @copydoc EntityProxy::Deactivate()
            virtual void Deactivate() override;
            /// @copydoc EntityProxy::IsActivated() const
            virtual Boole IsActivated() const override;

            /// @copydoc EntityComponent::GetCreator() const
            virtual EntityComponentManager* GetCreator() const override;

            ///////////////////////////////////////////////////////////////////////////////
            // RenderableProxy Properties

            /// @brief Sets whether or not this proxy is visible.
            /// @param Visible True to allow this proxy to render, false otherwise.
            virtual void SetVisible(const Boole Visible);
            /// @brief Gets whether or not this proxy is visible.
            /// @return Returns true if this proxy is being rendered, false otherwise.
            virtual Boole GetVisible() const;
            /// @brief Sets whether or not this proxy can cast a shadow.
            /// @param CastShadows True to allow this proxy to cast a shadow, false to prevent it from casting a shadow.
            virtual void SetCastShadows(const Boole CastShadows);
            /// @brief Gets whether or not this proxy can cast a shadow.
            /// @return Returns true if this proxy can cast a shadow, false otherwise.
            virtual Boole GetCastShadows() const;
            /// @brief Gets whether or not this proxy can be rendered with a shadow casted on it.
            /// @return Returns true if this proxy can receive shadows, false otherwise.
            virtual Boole GetReceiveShadows() const;

            /// @brief Sets which types of lights will affect this proxy.
            /// @param Mask A bitmask used to indicate which types of lights will be applied to this proxy.
            virtual void SetLightMask(const UInt32 Mask);
            /// @brief Gets which types of lights will affect this proxy.
            /// @return Returns a bitmask indicating the types of lights that will affect this proxies rendering.
            virtual UInt32 GetLightMask() const;
            /// @brief Sets the bitmask that will be used to determine if this object should be visible when rendering.
            /// @remarks This bitmask is compared against the bitmask you provide to a viewport for what should be visible during rendering.
            /// @param Mask The bitmask to be applied.
            virtual void SetVisibilityMask(const UInt32 Mask);
            /// @brief Gets the bitmask that will be used to determine if this object should be visible when rendering.
            /// @remarks This bitmask is compared against the bitmask you provide to a viewport for what should be visible during rendering.
            /// @return Returns a bitmask describing the type of object this will be treated as when rendering.
            virtual UInt32 GetVisibilityMask() const;
            /// @brief Sets the bitmesk that will be used to determine if this object should be counted in scene queries.
            /// @remarks This bitmask is compared against the bitmask you provide when performing a scene query from the scenemanager.
            /// @param Mask The bitmask to be applied.
            virtual void SetQueryMask(const UInt32 Mask);
            /// @brief Gets the bitmask that will be used to determine if this object should be counted in scene queries.
            /// @remarks This bitmask is compared against the bitmask you provide when performing a scene query from the scenemanager.
            /// @return Returns a bitmask describing the type of object this will be treated as when discovered in scene queries.
            virtual UInt32 GetQueryMask() const;

            /// @brief Sets the distance at which the proxy will stop rendering.
            /// @note Passing in zero will remove distance checking for this object when rendering.
            /// @param Distance The distance in world units from the camera when the proxy will stop being rendered.
            virtual void SetRenderDistance(const Real Distance);
            /// @brief Gets the distance at which the proxy will stop rendering.
            /// @return Returns a Real representing the max distance from the camera at which the proxy will be rendered.
            virtual Real GetRenderDistance() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Methods

            /// @copydoc EntityProxy::SetTransform(const Transform&)
            virtual void SetTransform(const Transform& Trans) override;
            /// @copydoc EntityProxy::SetTransform(const Vector3&,const Quaternion&)
            virtual void SetTransform(const Vector3& Loc, const Quaternion& Ori) override;
            /// @copydoc EntityProxy::GetTransform() const
            virtual Transform GetTransform() const override;

            /// @copydoc EntityProxy::SetLocation(const Vector3&)
            virtual void SetLocation(const Vector3& Loc) override;
            /// @copydoc EntityProxy::SetLocation(const Real, const Real, const Real)
            virtual void SetLocation(const Real X, const Real Y, const Real Z) override;
            /// @copydoc EntityProxy::GetLocation() const
            virtual Vector3 GetLocation() const override;
            /// @copydoc EntityProxy::SetOrientation(const Quaternion&)
            virtual void SetOrientation(const Quaternion& Ori) override;
            /// @copydoc EntityProxy::SetOrientation(const Real, const Real, const Real, const Real)
            virtual void SetOrientation(const Real X, const Real Y, const Real Z, const Real W) override;
            /// @copydoc EntityProxy::GetOrientation() const
            virtual Quaternion GetOrientation() const override;
            /// @copydoc EntityProxy::SetScale(const Vector3&)
            virtual void SetScale(const Vector3& Sc) override;
            /// @copydoc EntityProxy::SetScale(const Real, const Real, const Real)
            virtual void SetScale(const Real X, const Real Y, const Real Z) override;
            /// @copydoc EntityProxy::GetScale() const
            virtual Vector3 GetScale() const override;

            /// @copydoc EntityProxy::Translate(const Vector3&)
            virtual void Translate(const Vector3& Trans) override;
            /// @copydoc EntityProxy::Translate(const Real, const Real, const Real)
            virtual void Translate(const Real X, const Real Y, const Real Z) override;
            /// @copydoc EntityProxy::Yaw(const Real)
            virtual void Yaw(const Real Angle) override;
            /// @copydoc EntityProxy::Pitch(const Real)
            virtual void Pitch(const Real Angle) override;
            /// @copydoc EntityProxy::Roll(const Real)
            virtual void Roll(const Real Angle) override;
            /// @copydoc EntityProxy::Rotate(const Vector3&, const Real)
            virtual void Rotate(const Vector3& Axis, const Real Angle) override;
            /// @copydoc EntityProxy::Rotate(const Quaternion&)
            virtual void Rotate(const Quaternion& Rotation) override;
            /// @copydoc EntityProxy::Scale(const Vector3&)
            virtual void Scale(const Vector3& Scale) override;
            /// @copydoc EntityProxy::Scale(const Real, const Real, const Real)
            virtual void Scale(const Real X, const Real Y, const Real Z) override;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc EntityProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const override;
            /// @copydoc EntityProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot) override;

            /// @copydoc EntityProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const override;
            /// @copydoc EntityProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal node in the scenegraph for this proxy.
            /// @return Returns a pointer to the scenenode storing the transform data of this proxy.
            virtual Ogre::SceneNode* _GetGraphicsNode() const;
            /// @internal
            /// @brief Accessor for the internal graphics object.
            /// @return Returns a pointer to the internal object of this proxy.
            virtual Ogre::MovableObject* _GetBaseGraphicsObject() const = 0;
            /// @internal
            /// @brief Automatically performs a cast from an internal graphics object to a Mezzanine object.
            /// @param ToCast A pointer to the internal object being casted.
            /// @return Returns a pointer to the RenderableProxy used to represent the internal graphics object.
            static RenderableProxy* _Upcast(const Ogre::MovableObject* ToCast);
        };//RenderableProxy
    }//Graphics
}//Mezzanine

#endif
