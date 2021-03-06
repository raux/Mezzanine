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
/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural

 For the latest info, see http://code.google.com/p/ogre-procedural/

 Copyright (c) 2010-2013 Michael Broutin

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
#ifndef _graphicsprocedurallathe_h
#define _graphicsprocedurallathe_h

#include "Graphics/Procedural/Mesh/meshgenerator.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            class Shape;
            class MultiShape;
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A generator that produces a Mesh by rotating a 2D shape around the Y axis.
            /// @details Shapes that are Lathe'd do not need to be closed, and in general are assumed to be defined in the ( X >= 0 ) half-plane.
            /// In other words, the Y axis in local space for the generated Mesh will line up with "0" on the X axis in the shape, and be rotated
            /// to produce a mesh.
            ///////////////////////////////////////
            class MEZZ_LIB Lathe : public MeshGenerator<Lathe>
            {
            protected:
                /// @internal
                /// @brief A pointer to the Shape that will be used to generate a Mesh.
                Shape* ShapeToLathe;
                /// @internal
                /// @brief A pointer to the MultiShape that will be used to generate a Mesh.
                MultiShape* MultiShapeToLathe;
                /// @internal
                /// @brief The number of segments to have when rotating the shape to produce the Mesh.  Large number of segments means smoother shape.
                Whole NumSegments;
                /// @internal
                /// @brief The angle at which the rotation will start.
                Real AngleBegin;
                /// @internal
                /// @brief The angle at which the rotation will end.
                Real AngleEnd;
                /// @internal
                /// @brief Whether or not the Lathe is configured to rotate a full 360 degrees.
                Boole Closed;
                /// @internal
                /// @brief Whether or not the ends should be capped if the shape isn't rotated a full 360 degrees.
                Boole Capped;

                /// @internal
                /// @brief Creates the caps at the ends of the rotated shape.
                /// @param Buffer The buffer to append this generators vertices and indices to.
                void GenerateCaps(TriangleBuffer& Buffer) const;
                /// @internal
                /// @brief Creates the main body of the lathe'd shape.
                /// @param Buffer The buffer to append this generators vertices and indices to.
                /// @param ToLathe The shape to be rotated to produce a Mesh.
                void GenerateBody(TriangleBuffer& Buffer, const Shape* ToLathe) const;
            public:
                /// @brief Class constructor.
                Lathe();
                /// @brief Shape constructor.
                /// @param ToLathe A pointer to the Shape to be Lathed.
                /// @param Segments The number of segments to have when rotating the shape to produce the Mesh.  Large number of segments means smoother shape.  Will be clamped to be at least 3.
                Lathe(Shape* ToLathe, const Whole Segments);
                /// @brief MultiShape constructor.
                /// @param ToLathe A pointer to the MultiShape to be Lathed.
                /// @param Segments The number of segments to have when rotating the shape to produce the Mesh.  Large number of segments means smoother shape.  Will be clamped to be at least 3.
                Lathe(MultiShape* ToLathe, const Whole Segments);
                /// @brief Class destructor.
                virtual ~Lathe();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc MeshGenerator::AddToTriangleBuffer(TriangleBuffer&) const
                /// @exception If neither the shape or multishape are defined an INVALID_STATE_EXCEPTION will be thrown.
                virtual void AddToTriangleBuffer(TriangleBuffer& Buffer) const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the Shape to to be Lathe'd.
                /// @remarks This will remove any MultiShape that is set.
                /// @param ToLathe A pointer to the Shape that will be used to generate a Mesh.
                /// @return Returns a reference to this.
                Lathe& SetShapeToLathe(Shape* ToLathe);
                /// @brief Sets the MultiShape to to be Lathe'd.
                /// @remarks This will remove any Shape that is set.
                /// @param ToLathe A pointer to the MultiShape that will be used to generate a Mesh.
                /// @return Returns a reference to this.
                Lathe& SetMultiShapeToLathe(MultiShape* ToLathe);
                /// @brief Sets the number of segments in the rotation to generate.
                /// @exception If the number of segments is being set to a number less than 3, an INVALID_PARAMETERS exception will be thrown.
                /// @param Segments The number of segments to have when rotating the shape to produce the Mesh.  Large number of segments means smoother shape.
                /// @return Returns a reference to this.
                Lathe& SetNumSegments(const Whole Segments);
                /// @brief Sets the starting angle for the rotation.
                /// @param Begin The angle at which the rotation will start.  Initial Value: 0.0.
                /// @return Returns a reference to this.
                Lathe& SetAngleBegin(const Real Begin);
                /// @brief Sets the endng angle for the rotation.
                /// @param End The angle at which the rotation will end.  Initial Value: 2Pi.
                /// @return Returns a reference to this.
                Lathe& SetAngleEnd(const Real End);
                /// @brief Sets whether or not the start and end segments should be connected.
                /// @param ShapeClosed Whether or not the Lathe is configured to rotate a full 360 degrees.
                /// @return Returns a reference to this.
                Lathe& SetClosed(const Boole ShapeClosed);
                /// @brief Sets whether or not the outside ends of the rotated shape should be enclosed.
                /// @param ShapeCapped Whether or not the ends should be capped if the shape isn't rotated a full 360 degrees.
                /// @return Returns a reference to this.
                Lathe& SetCapped(const Boole ShapeCapped);
            };//Lathe
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
