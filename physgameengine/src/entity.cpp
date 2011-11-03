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
#ifndef _entity_cpp
#define _entity_cpp

#include "entity.h"
#include "scenemanager.h"

#include <Ogre.h>

namespace phys
{
    namespace internal
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class EntityInternalData
        /// @brief This class stores the internal data of an Entity.
        ///////////////////////////////////////
        struct EntityInternalData
        {
            /// @internal
            /// @brief Pointer to the ogre Entity from which this class gets it's functionality.
            Ogre::Entity* OgreEntity;

            /// @internal
            /// @brief Pointer to the ogre Scenenode to which this object is attached.
            Ogre::SceneNode* OgreNode;

            /// @internal
            /// @brief Pointer to the manager that created this class.
            SceneManager* Manager;

            /// @internal
            /// @brief Creates all the necessary data structures for the Entity.
            EntityInternalData(const String& Name, const String& MeshName, const String& Group, SceneManager* manager)
            {
                this->Manager = manager;
                Ogre::SceneManager* OgreManager = Manager->GetGraphicsWorldPointer();
                OgreEntity = OgreManager->createEntity(Name,MeshName,Group);
                OgreNode = OgreManager->createSceneNode();
                OgreManager->getRootSceneNode()->addChild(OgreNode);
            }

            /// @internal
            /// @brief Cleans up after the Entity.
            ~EntityInternalData()
            {
                Manager->GetGraphicsWorldPointer()->destroyEntity(OgreEntity);
                Manager->GetGraphicsWorldPointer()->destroySceneNode(OgreNode);
            }
        };//EntityInternalData
    }//internal

    Entity::Entity(const String& Name, const String& MeshName, const String& Group, SceneManager* manager)
    {
        EID = new internal::EntityInternalData(Name,MeshName,Group,manager);
    }

    Entity::~Entity()
    {
        delete EID;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited From Attachable

    ConstString& Entity::GetName() const
        { return this->EID->OgreEntity->getName(); }

    Attachable::AttachableElement Entity::GetAttachableType() const
        { return Attachable::Entity; }

    void Entity::SetLocation(const Vector3& Vec)
        { this->EID->OgreNode->setPosition(Vec.GetOgreVector3()); }

    Vector3 Entity::GetLocation() const
        { return Vector3(this->EID->OgreNode->getPosition()); }

    void Entity::SetOrientation(const Quaternion& Orientation)
        { this->EID->OgreNode->setOrientation(Orientation.GetOgreQuaternion()); }

    Quaternion Entity::GetOrientation() const
        { return Quaternion(this->EID->OgreNode->getOrientation()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Entity Functionality

    void Entity::SetScale(const Vector3& Scale)
    {
        this->EID->OgreNode->setScale(Scale.GetOgreVector3());
    }

    Vector3 Entity::GetScale() const
    {
        return Vector3(this->EID->OgreNode->getScale());
    }

    void Entity::AddToWorld()
    {
        if(!IsInWorld())
            { this->EID->OgreNode->attachObject(this->EID->OgreEntity); }
    }

    void Entity::RemoveFromWorld()
    {
        if(IsInWorld())
            { this->EID->OgreNode->detachObject(this->EID->OgreEntity); }
    }

    bool Entity::IsInWorld() const
        { return this->EID->OgreEntity->getParentSceneNode() == this->EID->OgreNode; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Functions

    AttachableData Entity::GetAttachableData() const
    {
        AttachableData Data;
        Data.OgreMovable = EID->OgreEntity;
        Data.OgreNode = EID->OgreNode;
        Data.Type = Attachable::Entity;
        return Data;
    }
}//phys

#endif
