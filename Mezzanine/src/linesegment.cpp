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
#ifndef _linesegment_cpp
#define _linesegment_cpp

#include "linesegment.h"
#include "MathTools/mathtools.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // LineSegment2D

    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    LineSegment2D::LineSegment2D(const Vector2& A, const Vector2& B) :
        PointA(A),
        PointB(B)
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    LineSegment2D::IntersectionTestResult LineSegment2D::Intersects(const LineSegment2D& Other) const
        { return MathTools::Intersects(*this,Other); }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    Boole LineSegment2D::operator==(const LineSegment2D& Other) const
        { return ( this->PointA == Other.PointA && this->PointB == Other.PointB ); }

    Boole LineSegment2D::operator!=(const LineSegment2D& Other) const
        { return ( this->PointA != Other.PointA || this->PointB != Other.PointB ); }

    ///////////////////////////////////////////////////////////////////////////////
    // LineSegment3D

    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    LineSegment3D::LineSegment3D(const Vector3& A, const Vector3& B) :
        PointA(A),
        PointB(B)
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Boole LineSegment3D::EpsilonEquivalent(const LineSegment3D& Other) const
    {
        return ( ( this->PointA.SquaredDistance( Other.PointA ) < 1e-8 && this->PointB.SquaredDistance( Other.PointB ) < 1e-8 ) ||
		         ( this->PointA.SquaredDistance( Other.PointB ) < 1e-8 && this->PointB.SquaredDistance( Other.PointA ) < 1e-8 ) );
    }

    LineSegment3D LineSegment3D::GetOrderedCopy() const
    {
        if( Vector3LengthCompare()(this->PointB,this->PointA) )
			return LineSegment3D(this->PointB,this->PointA);
		return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    Boole LineSegment3D::operator==(const LineSegment3D& Other) const
        { return ( this->PointA == Other.PointA && this->PointB == Other.PointB ); }

    Boole LineSegment3D::operator!=(const LineSegment3D& Other) const
        { return ( this->PointA != Other.PointA || this->PointB != Other.PointB ); }
}//Mezzanine

std::ostream& operator << (std::ostream& Stream, const Mezzanine::LineSegment2D& Data)
{
    Stream << "[" << Data.PointA << "," << Data.PointB << "]";
    return Stream;
}

std::ostream& operator << (std::ostream& Stream, const Mezzanine::LineSegment3D& Data)
{
    Stream << "[" << Data.PointA << "," << Data.PointB << "]";
    return Stream;
}

#endif
