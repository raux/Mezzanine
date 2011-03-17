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
#ifndef eventcollision_cpp
#define eventcollision_cpp

#include "actorcontainerbase.h"
#include "eventcollision.h"
#include "world.h"

#include <memory>

namespace phys {
    EventCollision::EventCollision(ActorBase* actora, ActorBase* actorb, Vector3 location, Real impulse)
    {
        ActorA=actora;
        ActorB=actorb;
        Location=location;
        Impulse=impulse;
    }

    EventCollision::EventCollision(const EventCollision& Other)
    {
        ActorA=Other.ActorA;
        ActorB=Other.ActorB;
        Location=Other.Location;
        Impulse=Other.Impulse;
    }

    EventCollision::~EventCollision()
    {
    }

    EventBase::EventType EventCollision::GetType() const
    {
        return EventBase::Collision;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::EventCollision& Ev)
{
    stream  << "<EventCollision Version=\"1\" Impulse=\"" << Ev.Impulse << "\" ActorA=\"" << Ev.ActorA->GetName() << "\" ActorB=\"" << Ev.ActorB->GetName() << "\" >"
            <<  Ev.Location
            << "</EventCollision>";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::EventCollision& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "EventCollision", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

void operator >> (const phys::xml::Node& OneNode, phys::EventCollision& Ev)
{
    if(OneNode.GetAttribute("Version").AsInt() == 1)
    {

        Ev.ActorA=phys::World::GetWorldPointer()->GetActorManager()->FindActor(OneNode.GetAttribute("ActorA").AsString());
        Ev.ActorB=phys::World::GetWorldPointer()->GetActorManager()->FindActor(OneNode.GetAttribute("ActorB").AsString());
        Ev.Impulse=OneNode.GetAttribute("Impulse").AsReal();

        if(OneNode.GetFirstChild())
        {
            OneNode.GetFirstChild() >> Ev.Location;
        }else{
            throw(phys::Exception("Normal not found while parsing phys::EventCollision"));
        }

    }else{
        throw( phys::Exception("Incompatible XML Version for EventCollision: Not Version 1"));
    }
}
#endif // \PHYSXML

#endif
