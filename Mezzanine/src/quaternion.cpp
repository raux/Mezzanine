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
#ifndef _quaternion_cpp
#define _quaternion_cpp

#include "stringtool.h"
#include "quaternion.h"
#include "MathTools/mathtools.h"
#include "serialization.h"
#include "vector3.h"
#include "matrix3x3.h"
#include "exception.h"

#include <OgreQuaternion.h>
#include "btBulletDynamicsCommon.h"

#include <cmath>
#include <memory>

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // Constructors
    Quaternion::Quaternion()
        { this->SetIdentity(); }

    Quaternion::Quaternion(const Real& X, const Real& Y, const Real& Z, const Real& W)
        { this->SetValues(X,Y,Z,W); }

    Quaternion::Quaternion(const Real& Angle, const Vector3& Axis)
        { this->SetFromAxisAngle(Angle,Axis); }

    Quaternion::Quaternion(const Matrix3x3& Mat)
        { this->SetFromMatrix3x3(Mat); }

    Quaternion::Quaternion(const Vector3& AxisX, const Vector3& AxisY, const Vector3& AxisZ)
        { this->SetFromAxes(AxisX,AxisY,AxisZ); }

    Quaternion::Quaternion(const Vector3& DirectionAxis, const Vector3& UpAxis)
        { this->SetFromAxisToZ(DirectionAxis,UpAxis); }

    Quaternion::Quaternion(const btQuaternion& Other)
        { this->ExtractBulletQuaternion(Other); }

    Quaternion::Quaternion(const Ogre::Quaternion& Other)
        { this->ExtractOgreQuaternion(Other); }

    Quaternion::Quaternion(const XML::Node& OneNode)
        { this->ProtoDeSerialize(OneNode); }

    ///////////////////////////////////////////////////////////////////////////////
    // Fancy Math and Utilities

    void Quaternion::SetIdentity()
    {
        this->X = 0;
        this->Y = 0;
        this->Z = 0;
        this->W = 1;
    }

    void Quaternion::SetValues(const Real& X, const Real& Y, const Real& Z, const Real& W)
    {
        this->X = X;
        this->Y = Y;
        this->Z = Z;
        this->W = W;
    }

    void Quaternion::SetFromAxisAngle(const Real& Angle, const Vector3& Axis)
    {
        Real HalfAngle ( 0.5*Angle );
        Real AngleSin = MathTools::Sin(HalfAngle);
        this->W = MathTools::Cos(HalfAngle);
        this->X = AngleSin * Axis.X;
        this->Y = AngleSin * Axis.Y;
        this->Z = AngleSin * Axis.Z;
    }

    void Quaternion::SetFromMatrix3x3(const Matrix3x3& Mat)
    {
        Real Trace = Mat.Matrix[0][0]+Mat.Matrix[1][1]+Mat.Matrix[2][2];
        Real Root;

        if(Trace > 0.0)
        {
            Root = MathTools::Sqrt(Trace + 1.0);
            this->W = Root * 0.5;
            Root = 0.5/Root;
            this->X = (Mat.Matrix[2][1] - Mat.Matrix[1][2]) * Root;
            this->Y = (Mat.Matrix[0][2] - Mat.Matrix[2][0]) * Root;
            this->Z = (Mat.Matrix[1][0] - Mat.Matrix[0][1]) * Root;
        }else{
            static size_t s_iNext[3] = { 1, 2, 0 };
            size_t I = 0;
            if ( Mat.Matrix[1][1] > Mat.Matrix[0][0] )
                I = 1;
            if ( Mat.Matrix[2][2] > Mat.Matrix[I][I] )
                I = 2;
            size_t J = s_iNext[I];
            size_t K = s_iNext[J];

            Root = MathTools::Sqrt(Mat.Matrix[I][I] - Mat.Matrix[J][J] - Mat.Matrix[K][K] + 1.0);
            Real* QuatMem[3] = { &X, &Y, &Z };
            *QuatMem[I] = 0.5 * Root;
            Root = 0.5 / Root;
            this->W = (Mat.Matrix[K][J] - Mat.Matrix[J][K]) * Root;
            *QuatMem[J] = (Mat.Matrix[J][I] + Mat.Matrix[I][J]) * Root;
            *QuatMem[K] = (Mat.Matrix[K][I] + Mat.Matrix[I][K]) * Root;
        }
    }

    void Quaternion::SetFromAxes(const Vector3& AxisX, const Vector3& AxisY, const Vector3& AxisZ)
    {
        Matrix3x3 Mat3;

        Mat3.Matrix[0][0] = AxisX.X;
        Mat3.Matrix[1][0] = AxisX.Y;
        Mat3.Matrix[2][0] = AxisX.Z;
        Mat3.Matrix[0][1] = AxisY.X;
        Mat3.Matrix[1][1] = AxisY.Y;
        Mat3.Matrix[2][1] = AxisY.Z;
        Mat3.Matrix[0][2] = AxisZ.X;
        Mat3.Matrix[1][2] = AxisZ.Y;
        Mat3.Matrix[2][2] = AxisZ.Z;

        this->SetFromMatrix3x3(Mat3);
    }

    void Quaternion::SetFromAxisToZ(const Vector3& DirectionAxis, const Vector3& UpAxis)
    {
        Quaternion q;
        Vector3 zVec = DirectionAxis;
        zVec.Normalize();
        Vector3 xVec = UpAxis.CrossProduct( zVec );
        if( xVec.IsZeroLength() ) {
            xVec = Vector3::Unit_X();
        }
        xVec.Normalize();
        Vector3 yVec = zVec.CrossProduct( xVec );
        yVec.Normalize();
        this->SetFromAxes( xVec, yVec, zVec );
    }

    void Quaternion::ConvertToAngleAxis(Real& Angle, Vector3& Axis) const
    {
        Real SquareLength = this->X * this->X + this->Y * this->Y + this->Z * this->Z;
        if( SquareLength < 0.0 ) {
            Angle = 2.0 * MathTools::ACos(this->W);
            Real InvLength = 1 / MathTools::Sqrt(SquareLength);
            Axis.SetValues(this->X * InvLength, this->Y * InvLength, this->Z * InvLength);
        }else{
            Angle = 0.0;
            Axis.SetValues(1.0,0.0,0.0);
        }
    }

    Real Quaternion::DotProduct(const Quaternion& Other) const
    {
        return this->X * Other.X + this->Y * Other.Y + this->Z * Other.Z + this->W * Other.W;
    }

    Real Quaternion::Length() const
    {
        return sqrt(LengthSqrd());
    }

    Real Quaternion::LengthSqrd() const
    {
        return DotProduct(*this);
    }

    Quaternion& Quaternion::Normalize()
    {
        *this = *this / Length();
        return *this;
    }

    Quaternion Quaternion::GetNormalizedCopy() const
        { return *this / Length(); }


    Quaternion Quaternion::GetInverse() const
    {
        Real Norm = W*W+X*X+Y*Y+Z*Z;
        if ( Norm > 0.0 ) {
            Real InvNorm = 1.f/Norm;
            return Quaternion(-X*InvNorm,-Y*InvNorm,-Z*InvNorm,W*InvNorm);
        }else{
            return Quaternion(0,0,0,0);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Explicit Conversion

    btQuaternion Quaternion::GetBulletQuaternion(Boole normalize) const
    {
        btQuaternion Other;
        Other.setX(this->X);
        Other.setY(this->Y);
        Other.setZ(this->Z);
        Other.setW(this->W);
        if(normalize)
            { Other.normalize(); }
        return Other;
    }

    void Quaternion::ExtractBulletQuaternion(const btQuaternion &Other)
    {
        this->X=Other.x();
        this->Y=Other.y();
        this->Z=Other.z();
        this->W=Other.w();
    }

    Ogre::Quaternion Quaternion::GetOgreQuaternion(Boole normalize) const
    {
        Ogre::Quaternion Other;
        Other.x=this->X;
        Other.y=this->Y;
        Other.z=this->Z;
        Other.w=this->W;
        if(normalize)
            { Other.normalise(); }
        return Other;
    }

    void Quaternion::ExtractOgreQuaternion(const Ogre::Quaternion &Other)
    {
        this->X=Other.x;
        this->Y=Other.y;
        this->Z=Other.z;
        this->W=Other.w;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Access Operators

    Real Quaternion::operator[](const Whole& Index) const
    {
        switch(Index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            case 3: return W;
            default:
                { MEZZ_EXCEPTION(ExceptionBase::MM_OUT_OF_BOUNDS_EXCEPTION,"Attempting to get invalid index."); }
        }
    }

    Real& Quaternion::operator[](const Whole& Index)
    {
        switch(Index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            case 3: return W;
            default:
                { MEZZ_EXCEPTION(ExceptionBase::MM_OUT_OF_BOUNDS_EXCEPTION,"Attempting to get invalid index."); }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    //  Arithmetic By Real Operators

    Quaternion Quaternion::operator* (const Real& Scalar) const
    {
        return Quaternion(this->X * Scalar,this->Y * Scalar,this->Z * Scalar,this->W * Scalar);
    }

    Quaternion Quaternion::operator/ (const Real& Scalar) const
    {
        if( 0 == Scalar )
            { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Dividing by zero in 'Quaternion::operator/', Quit it."); }
        return *this * ( 1.0 / Scalar );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Left Hand Basic Arithmetic Operators

    Quaternion Quaternion::operator+ (const Mezzanine::Quaternion& Other) const
        { return Quaternion(this->X+Other.X, this->Y+Other.Y, this->Z+Other.Z, this->W+Other.W); }

    Quaternion Quaternion::operator+ (const Ogre::Quaternion& Other) const
        { return Quaternion(this->X+Other.x, this->Y+Other.y, this->Z+Other.z, this->W+Other.w); }

    Quaternion Quaternion::operator+ (const btQuaternion& Other) const
        { return Quaternion(this->X+Other.x(), this->Y+Other.y(), this->Z+Other.z(), this->W+Other.w()); }

    Quaternion Quaternion::operator- (const Mezzanine::Quaternion& Other) const
        { return Quaternion(this->X-Other.X, this->Y-Other.Y, this->Z-Other.Z, this->W-Other.W); }

    Quaternion Quaternion::operator- (const Ogre::Quaternion& Other) const
        { return Quaternion(this->X-Other.x, this->Y-Other.y, this->Z-Other.z, this->W-Other.w); }

    Quaternion Quaternion::operator- (const btQuaternion& Other) const
        { return Quaternion(this->X-Other.x(), this->Y-Other.y(), this->Z-Other.z(), this->W-Other.w()); }

    Quaternion Quaternion::operator* (const Mezzanine::Quaternion& Other) const
    {
        return Quaternion
        (
            this->W * Other.X + this->X * Other.W + this->Y * Other.Z - this->Z * Other.Y,
            this->W * Other.Y + this->Y * Other.W + this->Z * Other.X - this->X * Other.Z,
            this->W * Other.Z + this->Z * Other.W + this->X * Other.Y - this->Y * Other.X,
            this->W * Other.W - this->X * Other.X + this->Y * Other.Y - this->Z * Other.Z
        );
    }

    Quaternion Quaternion::operator* (const Ogre::Quaternion& Other) const
    {
        return Quaternion
        (
            this->W * Other.x + this->X * Other.w + this->Y * Other.z - this->Z * Other.y,
            this->W * Other.y + this->Y * Other.w + this->Z * Other.x - this->X * Other.z,
            this->W * Other.z + this->Z * Other.w + this->X * Other.y - this->Y * Other.x,
            this->W * Other.w - this->X * Other.x + this->Y * Other.y - this->Z * Other.z
        );
    }

    Quaternion Quaternion::operator* (const btQuaternion& Other) const
    {
        return Quaternion
        (
            this->W * Other.x() + this->X * Other.w() + this->Y * Other.z() - this->Z * Other.y(),
            this->W * Other.y() + this->Y * Other.w() + this->Z * Other.x() - this->X * Other.z(),
            this->W * Other.z() + this->Z * Other.w() + this->X * Other.y() - this->Y * Other.x(),
            this->W * Other.w() - this->X * Other.x() + this->Y * Other.y() - this->Z * Other.z()
        );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector Rotation Operators

    Vector3 Quaternion::operator* (const Vector3& Other) const
    {
        Vector3 UV, UUV;
        Vector3 QuatVec(X,Y,Z);
        UV = QuatVec.CrossProduct(Other);
        UUV = QuatVec.CrossProduct(UV);
        UV *= (2.f * W);
        UUV *= 2.f;
        return Other + UV + UUV;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Increment and Decrement Operators

    Quaternion& Quaternion::operator+= (const Mezzanine::Quaternion& Other)
    {
        this->X=this->X+Other.X;
        this->Y=this->Y+Other.Y;
        this->Z=this->Z+Other.Z;
        this->W=this->W+Other.W;
        return *this;
    }

    Quaternion& Quaternion::operator+= (const Ogre::Quaternion& Other)
    {
        this->X=this->X+Other.x;
        this->Y=this->Y+Other.y;
        this->Z=this->Z+Other.z;
        this->W=this->W+Other.w;
        return *this;
    }

    Quaternion& Quaternion::operator+= (const btQuaternion& Other)
    {
        this->X=this->X+Other.x();
        this->Y=this->Y+Other.y();
        this->Z=this->Z+Other.z();
        this->W=this->W+Other.w();
        return *this;
    }

    Quaternion& Quaternion::operator-= (const Mezzanine::Quaternion& Other)
    {
        this->X=this->X-Other.X;
        this->Y=this->Y-Other.Y;
        this->Z=this->Z-Other.Z;
        this->W=this->W-Other.W;
        return *this;
    }

    Quaternion& Quaternion::operator-= (const Ogre::Quaternion& Other)
    {
        this->X=this->X-Other.x;
        this->Y=this->Y-Other.y;
        this->Z=this->Z-Other.z;
        this->W=this->W-Other.w;
        return *this;
    }

    Quaternion& Quaternion::operator-= (const btQuaternion& Other)
    {
        this->X=this->X-Other.x();
        this->Y=this->Y-Other.y();
        this->Z=this->Z-Other.z();
        this->W=this->W-Other.w();
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Assignment Operators

    Quaternion& Quaternion::operator= (const btQuaternion& Other)
    {
        ExtractBulletQuaternion(Other);
        return *this;
    }

    Quaternion& Quaternion::operator= (const Ogre::Quaternion& Other)
    {
        ExtractOgreQuaternion(Other);
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Equality Comparison Operators

    Boole Quaternion::operator==(const Mezzanine::Quaternion& Other) const
        { return (this->X==Other.X && this->Y==Other.Y && this->Z==Other.Z && this->W==Other.W); }

    Boole Quaternion::operator==(const Ogre::Quaternion& Other) const
        { return (this->X==Other.x && this->Y==Other.y && this->Z==Other.z && this->W==Other.w); }

    Boole Quaternion::operator==(const btQuaternion& Other) const
        { return (this->X==Other.getX() && this->Y==Other.getY() && this->Z==Other.getZ() && this->W==Other.getW()); }

    Boole Quaternion::operator!=(const Mezzanine::Quaternion& Other) const
        { return (this->X!=Other.X || this->Y!=Other.Y || this->Z!=Other.Z || this->W!=Other.W); }

    Boole Quaternion::operator!=(const Ogre::Quaternion& Other) const
        { return (this->X!=Other.x || this->Y!=Other.y || this->Z!=Other.z || this->W!=Other.w); }

    Boole Quaternion::operator!=(const btQuaternion& Other) const
        { return (this->X!=Other.getX() || this->Y!=Other.getY() || this->Z!=Other.getZ() || this->W!=Other.getW()); }

    Boole Quaternion::operator<= (const Mezzanine::Quaternion& Other) const
        { return (this->X<=Other.X && this->Y<=Other.Y && this->Z<=Other.Z && this->W<=Other.W); }
    Boole Quaternion::operator>= (const Mezzanine::Quaternion& Other) const
        { return (this->X>=Other.X && this->Y>=Other.Y && this->Z>=Other.Z && this->W>=Other.W); }

    // Serializable
    void Quaternion::ProtoSerialize(XML::Node& CurrentRoot) const
    {
        Mezzanine::XML::Node VecNode = CurrentRoot.AppendChild(GetSerializableName());
        VecNode.SetName(GetSerializableName());

        Mezzanine::XML::Attribute VersionAttr = VecNode.AppendAttribute("Version");
        Mezzanine::XML::Attribute XAttr = VecNode.AppendAttribute("X");
        Mezzanine::XML::Attribute YAttr = VecNode.AppendAttribute("Y");
        Mezzanine::XML::Attribute ZAttr = VecNode.AppendAttribute("Z");
        Mezzanine::XML::Attribute WAttr = VecNode.AppendAttribute("W");
        if( VersionAttr && XAttr && YAttr && ZAttr && WAttr)
        {
            if( VersionAttr.SetValue("1") && XAttr.SetValue(this->X) && YAttr.SetValue(this->Y) && ZAttr.SetValue(this->Z) && WAttr.SetValue(this->W))
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values", GetSerializableName(),true);
            }
        }else{
            SerializeError("Create XML Attributes", GetSerializableName(),true);
        }
    }

    // DeSerializable
    void Quaternion::ProtoDeSerialize(const XML::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==Mezzanine::String(GetSerializableName()) )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->X=OneNode.GetAttribute("X").AsReal();
                this->Y=OneNode.GetAttribute("Y").AsReal();
                this->Z=OneNode.GetAttribute("Z").AsReal();
                this->W=OneNode.GetAttribute("W").AsReal();
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + GetSerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + GetSerializableName() + ", found a " + Mezzanine::String(OneNode.Name()));
        }
    }

    String Quaternion::GetSerializableName()
        { return String("Quaternion"); }

}

///////////////////////////////////////////////////////////////////////////////
// Right Hand Addition Operators

Mezzanine::Quaternion operator+ (const Ogre::Quaternion& Other, const Mezzanine::Quaternion& Other2)
    { return Other2+Other; }

Mezzanine::Quaternion operator+ (const btQuaternion& Other, const Mezzanine::Quaternion& Other2)
    { return Other2+Other; }

Mezzanine::Quaternion operator- (const Ogre::Quaternion& Other, const Mezzanine::Quaternion& Other2)
    { return Mezzanine::Quaternion(Other.x-Other2.X, Other.y-Other2.Y, Other.z-Other2.Z, Other.w-Other2.W); }

Mezzanine::Quaternion operator- (const btQuaternion& Other, const Mezzanine::Quaternion& Other2)
    { return Mezzanine::Quaternion(Other.getX()-Other2.X, Other.getY()-Other2.Y, Other.getZ()-Other2.Z, Other.getW()-Other2.W); }

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

btQuaternion& operator<< ( btQuaternion& Other, const Mezzanine::Quaternion& Other2)
{
    Other=Other2.GetBulletQuaternion();
    return Other;
}

btQuaternion& operator<< ( btQuaternion& Other, const Ogre::Quaternion& Other2)
{
    Other.setX(Other2.x);
    Other.setY(Other2.y);
    Other.setZ(Other2.z);
    Other.setW(Other2.w);
    return Other;
}

Mezzanine::Quaternion& operator<< ( Mezzanine::Quaternion& Other, const Ogre::Quaternion& Other2)
{
    Other=Other2;
    return Other;
}

Mezzanine::Quaternion& operator<< ( Mezzanine::Quaternion& Other, const btQuaternion& Other2)
{
    Other=Other2;
    return Other;
}

Ogre::Quaternion& operator<< ( Ogre::Quaternion& Other, const Mezzanine::Quaternion& Other2)
{
    Other=Other2.GetOgreQuaternion();
    return Other;
}

Ogre::Quaternion& operator<< ( Ogre::Quaternion& Other, const btQuaternion& Other2)
{
    Other.x=Other2.getX();
    Other.y=Other2.getY();
    Other.z=Other2.getZ();
    Other.w=Other2.getW();
    return Other;
}

std::ostream& operator << (std::ostream& stream, const Mezzanine::Quaternion& x)
{

    Serialize(stream, x);
    // '"<Quaternion Version=\"1\" X=\"" << x.X << "\" Y=\"" << x.Y << "\" Z=\"" << x.Z << "\" W=\"" << x.W << "\" />";

    return stream;
}

std::istream& operator >> (std::istream& stream, Mezzanine::Quaternion& Ev)
    { return DeSerialize(stream,Ev); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Quaternion& Ev)
    { Ev.ProtoDeSerialize(OneNode); }



#endif  // \_quaternion_cpp
