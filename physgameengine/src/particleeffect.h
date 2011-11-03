//� Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _particleeffect_h
#define _particleeffect_h

#include "attachable.h"
#include "crossplatformexport.h"
#include "datatypes.h"
#include "quaternion.h"
#include "xml.h"

/// @file particleeffect.cpp
/// @brief Contains the declaration of the ParticleEffect class

namespace Ogre
{
    class ParticleSystem;
    class SceneNode;
}

namespace phys
{
    namespace internal
    {
        struct ParticleEffectInternalData;
    }

    class SceneManager;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ParticleEffect
    /// @headerfile particleeffect.h
    /// @brief This class is responsible for creating visual particle effects, such as rain, smoke, sparks, and explosions.
    /// @details Particle effects are loaded from particle scripts which are contained in particle files(*.particle). @n
    /// All particle effects are created attached to a world node. The world Node provides the Nagivation functionality of for this
    ///////////////////////////////////////
    class PHYS_LIB ParticleEffect : public Attachable
    {
        private:
            /// @internal
            /// @brief The internal data for the particle effect
            internal::ParticleEffectInternalData *Pie;
        public:

            ///////////////////////////////////////////////////////////////////////////////
            // Construction

            /// @brief Standard initialization constructor.
            /// @param Name The name of this particle effect.
            /// @param Template Name of the particle script to be used in creating this particle effect.
            /// @param manager Pointer to the manager that this particle effect is to be used in.
            ParticleEffect(const String& Name, const String& Template, SceneManager* manager);

            /// @internal
            /// @brief Internal constructor.
            /// @details This constructor should not be called on manually.
            /// @param System Pointer to the Ogre ParticleSystem this class is based on.
            /// @param Template Name of the particle script to be used in creating this particle effect.
            /// @param manager Pointer to the manager that this particle effect is to be used in.
            ParticleEffect(Ogre::ParticleSystem* System, const String& Template, SceneManager* manager);

            /// @brief Class destructor.
            virtual ~ParticleEffect();

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited From Attachable

            /// @copydoc phys::Attachable::GetName()
            ConstString& GetName() const;

            /// @copydoc phys::Attachable::GetAttachableType()
            virtual Attachable::AttachableElement GetAttachableType() const;

            /// @copydoc phys::Attachable::SetLocation(const Vector3& Vec)
            virtual void SetLocation(const Vector3& Vec);

            /// @copydoc phys::Attachable::GetLocation()
            virtual Vector3 GetLocation() const;

            /// @copydoc phys::Attachable::SetOrientation(const Quaternion& Orientation)
            virtual void SetOrientation(const Quaternion& Orientation);

            /// @copydoc phys::Attachable::GetOrientation()
            virtual Quaternion GetOrientation() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Particle Functionality

            /// @brief Adds this particle effect to the world, allowing it to render.
            /// @details If the Effect is already in the world, this does nothing.
            void AddToWorld();

            /// @brief Removes this particle effect from the world, preventing it from rendering.
            /// @details If the Effect is not in the world, this does nothing
            void RemoveFromWorld();

            /// @brief Gets whether or not this effect is in the world.
            /// @return True if in the world, false otherwise.
            bool IsInWorld() const;

            /// @brief The name of the template used to create this.
            /// @return A reaference to a ConstString that has the name.
            ConstString& GetTemplate() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Functions

            /// @internal
            /// @brief Gets pointers to the internal ogre structures for this attachable.
            /// @return Returns an AttachableData struct with the internal data.
            virtual AttachableData GetAttachableData() const;
    };//ParticleEffect
}//phys

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML

/// @brief Serializes the passed phys::ParticleEffect to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the phys::ParticleEffect to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& PHYS_LIB operator << (std::ostream& stream, const phys::ParticleEffect& Ev);

/// @brief Deserialize a phys::ParticleEffect
/// @param stream The istream to get the xml from to (re)make the phys::ParticleEffect.
/// @param Ev the phys::ParticleEffect to be deserialized.
/// @return this returns the ostream, advanced past the phys::ParticleEffect that was recreated onto Ev.
/// @warning This does not the pointer to the scene manager. When a scene manager is serialized, this data is implicitly stored by the ParticleEffects location in the xml hierarchy, this is used instead. The Name of the manager is stored for possible future use.
std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ParticleEffect& Ev);

/// @brief Set all values of a phys::ParticleEffect from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the phys::ParticleEffect.
/// @param Ev the phys::ParticleEffect to be reset.
/// @return This returns thexml::Node that was passed in.
/// @warning This does not attempt to de-serialize the name or template of the ParticleEffect. These is not currently changeable after the creation of a ParticleEffect. However, the ParticleEffectmanager will correctly create name ParticleEffect upon creation then deserialize the rest of the ParticleEffect.
/// @warning This does not throw an exception if the ParticleEffect could not be attached to the appropriate worldnode. It is assumed that the worldnode will be able to adjust the pointer on this if it is deserialized second.
phys::xml::Node& PHYS_LIB operator >> (const phys::xml::Node& OneNode, phys::ParticleEffect& Ev);

#endif


#endif
