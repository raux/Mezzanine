//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _plane_cpp
#define _plane_cpp

#include <iostream>
#include <sstream>
#include <memory>

#include "exception.h"
#include "plane.h"
#include "world.h"
#include "xml.h"

#include "Ogre.h"

namespace phys
{

    ///////////////////////////////////////////////////////////////////////////////
    // Constructors
    Plane::Plane() : Normal(0,0,0), Distance(0)
        {}

    Plane::Plane(Vector3 Normal_, Real Distance_) : Normal(Normal_),Distance(Distance_)
        {}

    Plane::Plane(Ogre::Plane Plane_)
        { this->ExtractOgrePlane(Plane_); }

    Plane::Plane(const Vector3& rkPoint0, const Vector3& rkPoint1, const Vector3& rkPoint2)
    {
        Vector3 kEdge1 = rkPoint1 - rkPoint0;
		Vector3 kEdge2 = rkPoint2 - rkPoint0;
		this->Normal = kEdge1.CrossProduct(kEdge2);
		this->Normal.Normalize();
		this->Distance = -this->Normal.dotProduct(rkPoint0);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Conversions and adjustments
    Ogre::Plane Plane::GetOgrePlane() const
        { return Ogre::Plane( Normal.GetOgreVector3(), Distance); }

    void Plane::ExtractOgrePlane(const Ogre::Plane& Plane2)
    {
        this->Normal=Plane2.normal;
        this->Distance=Plane2.d;
    }

    void Plane::operator=(const Ogre::Plane& Plane2)
        { this->ExtractOgrePlane(Plane2); }

}
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::Plane& x)
{
    stream << "<Plane Version=\"1\" Distance=\"" << x.Distance << "\" >" << x.Normal << "</Plane>";
    return stream;
}


std::istream& PHYS_LIB operator >> (std::istream& stream, phys::Plane& x)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "Plane", OneTag) );

    Doc->GetFirstChild() >> x;

    return stream;
}

phys::xml::Node& PHYS_LIB operator >> (const phys::xml::Node& OneNode, phys::Plane& x)
{
    if(OneNode.GetAttribute("Version").AsInt() == 1)
    {
        x.Distance=OneNode.GetAttribute("Distance").AsReal();
        if(OneNode.GetFirstChild())
        {
            OneNode.GetFirstChild() >> x.Normal;
        }else{
            throw(phys::Exception("Normal not found while parsing phys::Plane"));
        }
    }else{
        throw( phys::Exception("Incompatible XML Version for Vector3: Not Version 1"));
    }
}

#endif // \phys_xml

#endif
