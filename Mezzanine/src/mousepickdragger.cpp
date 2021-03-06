// � Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _mousepickdragger_cpp
#define _mousepickdragger_cpp

#include "mousepickdragger.h"
#include "rayquery.h"
#include "entity.h"
#include "entityproxy.h"

#include "Input/mouse.h"

#include "Physics/physicsmanager.h"
#include "Physics/genericsixdofconstraint.h"
#include "Physics/point2pointconstraint.h"
#include "Physics/rigidproxy.h"
#include "Physics/softproxy.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // PlaneDragger Methods

    PlaneDragger::PlaneDragger(const Plane& ThePlane) :
        DragPlane(ThePlane),
        SavedActivationState(Physics::AS_Active),
        Dragger(NULL),
        LastTarget(NULL)
        {  }

    Physics::GenericSixDofConstraint* PlaneDragger::CreateDragger(Physics::RigidProxy* Target, const Transform& LocalTrans)
    {
        Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( Target->GetCreator() );

        Physics::GenericSixDofConstraint* NewDragger = PhysMan->CreateGenericSixDofConstraint(Target,LocalTrans);
        NewDragger->SetLinearLimitLower(Vector3(0,0,0));
        NewDragger->SetLinearLimitUpper(Vector3(0,0,0));
        NewDragger->SetAngularLimitLower(Vector3(1,1,1));
        NewDragger->SetAngularLimitUpper(Vector3(-1,-1,-1));

        /*Physics::Point2PointConstraint* NewDragger = PhysMan->CreatePoint2PointConstraint(Target,LocalTrans.Location);
        NewDragger->SetTAU(0.001);
        NewDragger->SetParam(Physics::Con_Stop_CFM,0.8,-1);
        NewDragger->SetParam(Physics::Con_CFM,0.8,-1);
        NewDragger->SetParam(Physics::Con_Stop_ERP,0.1,-1);
        NewDragger->SetParam(Physics::Con_ERP,0.1,-1);//*/

        NewDragger->EnableConstraint(true);
        return NewDragger;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void PlaneDragger::SetDragPlane(const Plane& ThePlane)
        { this->DragPlane = ThePlane; }

    const Plane& PlaneDragger::GetDragPlane() const
        { return this->DragPlane; }

    Physics::GenericSixDofConstraint* PlaneDragger::GetDraggingConstraint() const
        { return this->Dragger; }

    RayQueryHit PlaneDragger::GetBestQueryHit(const RayQuery::ResultContainer& Targets) const
    {
        if( Targets.size() > 0 ) {
            return Targets[0];
        }
        return RayQueryHit();
    }

    RayQueryHit PlaneDragger::GetBestQueryHit(const RayQuery::ResultContainer& Targets, const FilterDelegate& Filter) const
    {
        for( const RayQueryHit& CurrResult : Targets )
        {
            if( Filter(CurrResult) ) {
                return CurrResult;
            }
        }
        return RayQueryHit();
    }

    EntityProxy* PlaneDragger::GetBestProxy(EntityProxy* Target) const
    {
        const ComponentType DesiredTypeFirst = Mezzanine::CT_Physics_RigidProxy;
        //const ComponentType DesiredTypeLast = Mezzanine::CT_Physics_SoftBody;
        if( Target->GetComponentType() == DesiredTypeFirst )
            return Target;

        Entity* Parent = Target->GetParentEntity();
        if( Parent != NULL ) {
            EntityProxy* NewTarget = static_cast<EntityProxy*>( Parent->GetComponent(DesiredTypeFirst,0) );
            return ( NewTarget != NULL ? NewTarget : Target );
        }
        return Target;
    }

    EntityProxy* PlaneDragger::GetCurrentTarget() const
        { return ( this->Dragger != NULL ? this->Dragger->GetProxyA() : NULL ); }

    EntityProxy* PlaneDragger::GetLastTarget() const
        { return this->LastTarget; }

    Boole PlaneDragger::IsDragging() const
        { return ( this->Dragger != NULL ); }

    Boole PlaneDragger::StartDragging(EntityProxy* Target, const Vector3& Offset, const Ray& MouseRay)
    {
        if( this->Dragger == NULL ) {
            Transform TargetTrans(Offset);
            if( Target->GetComponentType() == Mezzanine::CT_Physics_RigidProxy ) {
                Physics::RigidProxy* CastedTarget = static_cast<Physics::RigidProxy*>(Target);
                this->SavedActivationState = CastedTarget->GetActivationState();
                CastedTarget->SetActivationState(Physics::AS_DisableDeactivation);
                this->Dragger = this->CreateDragger(CastedTarget,TargetTrans);
                return true;
            }/*else if( Target->GetComponentType() == Mezzanine::CT_Physics_SoftProxy ) {

            }*/
        }
        return false;
    }

    void PlaneDragger::ContinueDragging(const Ray& MouseRay, Input::Mouse* Cursor)
    {
        if( this->Dragger != NULL ) {
            MathTools::PlaneTestResult Result = this->DragPlane.Intersects(MouseRay);
            if( Result.first != MathTools::PS_Neither ) {
                this->Dragger->SetPivotALocation( Result.second );
                //this->Dragger->SetPivotB( Result.second );
            }
        }
    }

    void PlaneDragger::StopDragging()
    {
        if( this->Dragger != NULL ) {
            this->LastTarget = static_cast<Physics::CollidableProxy*>( this->GetCurrentTarget() );
            this->LastTarget->SetActivationState( this->SavedActivationState );
            Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( this->LastTarget->GetCreator() );
            PhysMan->DestroyConstraint(this->Dragger);
            this->Dragger = NULL;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // DistanceDragger Methods

    DistanceDragger::DistanceDragger(const Real DistIncrement) :
        DragDistance(0.0),
        DragIncrement(DistIncrement),
        SavedActivationState(Physics::AS_Active),
        Dragger(NULL),
        LastTarget(NULL)
        {  }

    Physics::GenericSixDofConstraint* DistanceDragger::CreateDragger(Physics::RigidProxy* Target, const Transform& LocalTrans)
    {
        Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( Target->GetCreator() );

        Physics::GenericSixDofConstraint* NewDragger = PhysMan->CreateGenericSixDofConstraint(Target,LocalTrans);
        NewDragger->SetLinearLimitLower(Vector3(0,0,0));
        NewDragger->SetLinearLimitUpper(Vector3(0,0,0));
        NewDragger->SetAngularLimitLower(Vector3(1,1,1));
        NewDragger->SetAngularLimitUpper(Vector3(-1,-1,-1));

        /*Physics::Point2PointConstraint* NewDragger = PhysMan->CreatePoint2PointConstraint(Target,LocalTrans.Location);
        NewDragger->SetTAU(0.001);
        NewDragger->SetParam(Physics::Con_Stop_CFM,0.8,-1);
        NewDragger->SetParam(Physics::Con_CFM,0.8,-1);
        NewDragger->SetParam(Physics::Con_Stop_ERP,0.1,-1);
        NewDragger->SetParam(Physics::Con_ERP,0.1,-1);//*/

        NewDragger->EnableConstraint(true);
        return NewDragger;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void DistanceDragger::SetDragIncrement(const Real DistIncrement)
        { this->DragIncrement = DistIncrement; }

    Real DistanceDragger::GetDragIncrement() const
        { return this->DragIncrement; }

    void DistanceDragger::IncrementDistance()
        { this->DragDistance += this->DragIncrement; }

    void DistanceDragger::DecrementDistance()
        { this->DragDistance -= this->DragIncrement; }

    Physics::GenericSixDofConstraint* DistanceDragger::GetDraggingConstraint() const
        { return this->Dragger; }

    RayQueryHit DistanceDragger::GetBestQueryHit(const RayQuery::ResultContainer& Targets) const
    {
        if( Targets.size() > 0 ) {
            return Targets[0];
        }
        return RayQueryHit();
    }

    RayQueryHit DistanceDragger::GetBestQueryHit(const RayQuery::ResultContainer& Targets, const FilterDelegate& Filter) const
    {
        for( const RayQueryHit& CurrResult : Targets )
        {
            if( Filter(CurrResult) ) {
                return CurrResult;
            }
        }
        return RayQueryHit();
    }

    EntityProxy* DistanceDragger::GetBestProxy(EntityProxy* Target) const
    {
        const ComponentType DesiredTypeFirst = Mezzanine::CT_Physics_RigidProxy;
        //const ComponentType DesiredTypeLast = Mezzanine::CT_Physics_SoftBody;
        if( Target->GetComponentType() == DesiredTypeFirst )
            return Target;

        Entity* Parent = Target->GetParentEntity();
        if( Parent != NULL ) {
            EntityProxy* NewTarget = static_cast<EntityProxy*>( Parent->GetComponent(DesiredTypeFirst,0) );
            return ( NewTarget != NULL ? NewTarget : Target );
        }
        return Target;
    }

    EntityProxy* DistanceDragger::GetCurrentTarget() const
        { return ( this->Dragger != NULL ? this->Dragger->GetProxyA() : NULL ); }

    EntityProxy* DistanceDragger::GetLastTarget() const
        { return this->LastTarget; }

    Boole DistanceDragger::IsDragging() const
        { return ( this->Dragger != NULL ); }

    Boole DistanceDragger::StartDragging(EntityProxy* Target, const Vector3& Offset, const Ray& MouseRay)
    {
        if( this->Dragger == NULL ) {
            Transform TargetTrans(Offset);
            if( Target->GetComponentType() == Mezzanine::CT_Physics_RigidProxy ) {
                Physics::RigidProxy* CastedTarget = static_cast<Physics::RigidProxy*>(Target);
                this->SavedActivationState = CastedTarget->GetActivationState();
                CastedTarget->SetActivationState(Physics::AS_DisableDeactivation);
                this->Dragger = this->CreateDragger(CastedTarget,TargetTrans);
                this->DragDistance = MouseRay.Origin.Distance(Target->ConvertLocalToGlobal(Offset));
                return true;
            }/*else if( Target->GetComponentType() == Mezzanine::CT_Physics_SoftProxy ) {

            }*/
        }
        return false;
    }

    void DistanceDragger::ContinueDragging(const Ray& MouseRay, Input::Mouse* Cursor)
    {
        if( this->Dragger != NULL ) {
            Input::DirectionalMotionState WheelState = Cursor->GetVerticalWheelState();
            if( WheelState == Input::DIRECTIONALMOTION_UPLEFT ) {
                this->IncrementDistance();
            }else if( WheelState == Input::DIRECTIONALMOTION_DOWNRIGHT ) {
                this->DecrementDistance();
            }

            this->Dragger->SetPivotALocation( MouseRay.GetPointAtDistance( this->DragDistance ) );
            //this->Dragger->SetPivotB( MouseRay.GetPointAtDistance( this->DragDistance ) );
        }
    }

    void DistanceDragger::StopDragging()
    {
        if( this->Dragger != NULL ) {
            this->LastTarget = static_cast<Physics::CollidableProxy*>( this->GetCurrentTarget() );
            this->LastTarget->SetActivationState( this->SavedActivationState );
            Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( this->LastTarget->GetCreator() );
            PhysMan->DestroyConstraint(this->Dragger);
            this->Dragger = NULL;
            this->DragDistance = 0.0;
        }
    }
}//Mezzanine

#endif
