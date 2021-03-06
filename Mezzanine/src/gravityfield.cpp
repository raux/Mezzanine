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
#ifndef _gravityfield_cpp
#define _gravityfield_cpp

#include "gravityfield.h"

#include "Physics/rigidproxy.h"
#include "Physics/ghostproxy.h"
#include "Physics/physicsmanager.h"

#include "entresol.h"
#include "world.h"
#include "serialization.h"
#include "exception.h"

namespace Mezzanine
{
    GravityField::GravityField(World* TheWorld) :
        AreaEffect(TheWorld)
        {  }

    GravityField::GravityField(const EntityID& EntID, World* TheWorld) :
        AreaEffect(EntID,TheWorld)
        {  }

    GravityField::GravityField(const EntityID& EntID, Physics::CollisionShape* Shape, World* TheWorld) :
        AreaEffect(EntID,Shape,TheWorld)
        {  }

    GravityField::GravityField(const XML::Node& SelfRoot, World* TheWorld) :
        AreaEffect(TheWorld)
        { this->ProtoDeSerialize(SelfRoot); }

    GravityField::~GravityField()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Mezzanine::EntityType GravityField::GetEntityType() const
        { return Mezzanine::ET_AreaEffectGravityField; }

    void GravityField::ApplyEffect()
    {
        AreaEffect::ApplyEffect();
        for( ObjectIterator AddedIt = this->AddedObjects.begin() ; AddedIt != this->AddedObjects.end() ; ++AddedIt )
        {
            const ComponentContainer& OtherComponents = (*AddedIt)->GetComponents();
            for( ConstComponentIterator CompIt = OtherComponents.begin() ; CompIt != OtherComponents.end() ; ++CompIt )
            {
                if( (*CompIt)->GetComponentType() == Mezzanine::CT_Physics_RigidProxy ) {
                    Physics::RigidProxy* RigProx = static_cast<Physics::RigidProxy*>( *CompIt );
                    RigProx->SetGravity(this->Grav);
                }
            }
        }

        if( !RemovedObjects.empty() ) {
            const Vector3 WorldGravity = static_cast<Physics::PhysicsManager*>( this->ParentWorld->GetManager(ManagerBase::MT_PhysicsManager) )->GetWorldGravity();

            for( ObjectIterator RemovedIt = this->RemovedObjects.begin() ; RemovedIt != this->RemovedObjects.end() ; ++RemovedIt )
            {
                const ComponentContainer& OtherComponents = (*RemovedIt)->GetComponents();
                for( ConstComponentIterator CompIt = OtherComponents.begin() ; CompIt != OtherComponents.end() ; ++CompIt )
                {
                    if( (*CompIt)->GetComponentType() == Mezzanine::CT_Physics_RigidProxy ) {
                        Physics::RigidProxy* RigProx = static_cast<Physics::RigidProxy*>( *CompIt );
                        RigProx->SetGravity(WorldGravity);
                    }
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // GravityField Properties

    void GravityField::SetFieldGravity(const Vector3& Gravity)
        { this->Grav = Gravity; }

    Vector3 GravityField::GetFieldGravity() const
        { return this->Grav; }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void GravityField::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        this->AreaEffect::ProtoSerializeProperties(SelfRoot);

        XML::Node PropertiesNode = SelfRoot.AppendChild( Entity::GetSerializableName() + "Properties" );

        if( PropertiesNode.AppendAttribute("Version").SetValue("1") )
        {
            XML::Node GravityNode = PropertiesNode.AppendChild("Gravity");
            this->GetFieldGravity().ProtoSerialize( GravityNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",Entity::GetSerializableName() + "Properties",true);
        }
    }

    void GravityField::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        this->AreaEffect::ProtoDeSerializeProperties(SelfRoot);

        XML::Node PropertiesNode = SelfRoot.GetChild( Entity::GetSerializableName() + "Properties" );

        if( !PropertiesNode.Empty() ) {
            if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                XML::Node GravityNode = PropertiesNode.GetChild("Gravity").GetFirstChild();
                if( !GravityNode.Empty() ) {
                    Vector3 Gravity(GravityNode);
                    this->SetFieldGravity(Gravity);
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (Entity::GetSerializableName() + "Properties" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Entity::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
        }
    }

    String GravityField::GetDerivedSerializableName() const
        { return GravityField::GetSerializableName(); }

    String GravityField::GetSerializableName()
        { return "GravityField"; }

    ///////////////////////////////////////////////////////////////////////////////
    // GravityFieldFactory Methods

    GravityFieldFactory::GravityFieldFactory()
        {  }

    GravityFieldFactory::~GravityFieldFactory()
        {  }

    String GravityFieldFactory::GetTypeName() const
        { return GravityField::GetSerializableName(); }

    GravityField* GravityFieldFactory::CreateGravityField(const EntityID& EntID, World* TheWorld)
        { return new GravityField(EntID,TheWorld); }

    GravityField* GravityFieldFactory::CreateGravityField(const EntityID& EntID, Physics::CollisionShape* AEShape, World* TheWorld)
        { return new GravityField(EntID,AEShape,TheWorld); }

    GravityField* GravityFieldFactory::CreateGravityField(const XML::Node& XMLNode, World* TheWorld)
        { return static_cast<GravityField*>( this->CreateEntity(XMLNode,TheWorld) ); }

    Entity* GravityFieldFactory::CreateEntity(const EntityID& EntID, World* TheWorld, const NameValuePairMap& Params)
        { return new GravityField(EntID,TheWorld); }

    Entity* GravityFieldFactory::CreateEntity(const XML::Node& XMLNode, World* TheWorld)
        { return new GravityField(XMLNode,TheWorld); }

    void GravityFieldFactory::DestroyEntity(Entity* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
