// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _graphicsproceduralcycloidmodifier_h
#define _graphicsproceduralcycloidmodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

#include "vector2.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that will draw a series of curved line segments.
            /// @details Wikipedia says this about Cycloids: "A cycloid is the curve traced by a point on the rim of a circular wheel as the wheel rolls along a
            /// line without slippage. It is an example of a roulette, a curve generated by a curve rolling on another curve." @n
            /// Currently this modifier supports 6 types of curves that can be drawn: @n
            ///    Hypocyloid - http://en.wikipedia.org/wiki/Hypocycloid @n
            ///    Hypotrochoid - http://en.wikipedia.org/wiki/Hypotrochoid @n
            ///    Epicycloid - http://en.wikipedia.org/wiki/Epicycloid @n
            ///    Epitrochoid - http://en.wikipedia.org/wiki/Epitrochoid @n
            ///    Rose Curve - http://en.wikipedia.org/wiki/Rose_(mathematics) @n
            ///    Lissajous Curve - http://en.wikipedia.org/wiki/Lissajous_curve @n @n
            /// Setting the type will also set sane defaults for that type of Cycloid, but those default parameters are not strictly required and can be tinkered
            /// with, although you may get odd results depending on which parameter you alter and by how much.  Consult the documentation on the types of Cycloids
            /// if you feel adventurous.  The default parameters are as follows: @n
            /// @par Default parameters
            /// <table><tr><th>Name</th><th>Parameter <em>R</em></th><th>Parameter <em>r</em></th><th>Parameter <em>d</em></th><th>Parameter <em>e</em></th><th colspan="2">Parameter <em>k</em></th></tr>
            /// <tr><td>Hypocycloid</td><td>3/6 * Size</td><td>1/6 * Size</td><td align="center"><em>unsused</em></td><td align="center" rowspan="5"><em>unsused</em><td rowspan="4">k = R / r</td><td>3</td></tr>
            /// <tr><td>Hypotrochoid</td><td>5/14 * Size</td><td>3/14 * Size</td><td>5/14 * Size</td><td>2</td></tr>
            /// <tr><td>Epicycloid</td><td>3/10 * Size</td><td>1/10 * Size</td><td align="center"><em>unsused</em><td>3</td></tr>
            /// <tr><td>Epitrochoid</td><td>3/10 * Size</td><td>1/10 * Size</td><td>1/20 * Size</td><td>3</td></tr>
            /// <tr><td>Rose curve</td><td>1/2 * Size</td><td>4</td><td>1</td><td rowspan="2">k = r / d</td><td>4</td></tr>
            /// <tr><td>Lissajous curve</td><td>1/2 * Size</td><td>5</td><td>4</td><td>&pi;/2</td><td>5/4</td></tr></table>
            ///////////////////////////////////////
            class MEZZ_LIB CycloidModifier : public TextureModifier
            {
            public:
                /// @enum CycloidType
                /// @brief The type of Cycloid to draw.
                enum CycloidType
                {
                    CT_Hypocycloid     = 1,  ///< Sets defaults for drawing a Hypocyloid.  See class detailed description for more information.
                    CT_Hypotrochoid    = 2,  ///< Sets defaults for drawing a Hypotrochoid.  See class detailed description for more information.
                    CT_Epicycloid      = 3,  ///< Sets defaults for drawing a Epicycloid.  See class detailed description for more information.
                    CT_Epitrochoid     = 4,  ///< Sets defaults for drawing a Epitrochoid.  See class detailed description for more information.
                    CT_RoseCurve       = 5,  ///< Sets defaults for drawing a Rose Curve.  See class detailed description for more information.
                    CT_LissajousCurve  = 6   ///< Sets defaults for drawing a Lissajous Curve.  See class detailed description for more information.
                };
                /// @enum CycloidParameter
                /// @brief An enum describing the parameters that can be altered for each type of Cycloid.
                enum CycloidParameter
                {
                    CP_Param_R      = 1,     ///< Generally this is altering the radius of the base circle that the drawing circle rotates around.
                    CP_Param_r      = 2,     ///< Generally this is altering the radius of the drawing circle.
                    CP_Param_d      = 3,     ///< Generally this is altering the distance of the drawing pen from the center of the drawing circle.
                    CP_Param_e      = 4,     ///< This is exclusively used by a Lissajous Curve and skews the angle at which the drawing perspective is done, usually making it look more compressed.
                    CP_Param_k      = 5,     ///< In a Rose Curve this will generate k petals if 2*k is even.  If k is odd then it will generate k petals.

                    CP_LargeCircleRadius = CP_Param_R,    ///< Generally this is altering the radius of the base circle that the drawing circle rotates around.
                    CP_SmallCircleRadius = CP_Param_r,    ///< Generally this is altering the radius of the drawing circle.
                    CP_DistanceFromCenter = CP_Param_d,   ///< Generally this is altering the distance of the drawing pen from the center of the drawing circle.
                    CP_RotationAngle = CP_Param_e,        ///< This is exclusively used by a Lissajous Curve and skews the angle at which the drawing perspective is done, usually making it look more compressed.
                    CP_BasePetals = CP_Param_k            ///< In a Rose Curve this will generate k petals if 2*k is even.  If k is odd then it will generate k petals.
                };
            protected:
                /// @internal
                /// @brief The colour of the curve line to draw.
                ColourValue CycloidColour;
                /// @internal
                /// @brief The center of the cycloid on the target texture.
                Vector2 CycloidCenter;
                /// @internal
                /// @brief The size of the curved line to draw.
                Whole CycloidPenSize;
                /// @internal
                /// @brief See CycloidParameter enum for details.
                Real Parameter_R;
                /// @internal
                /// @brief See CycloidParameter enum for details.
                Real Parameter_r;
                /// @internal
                /// @brief See CycloidParameter enum for details.
                Real Parameter_d;
                /// @internal
                /// @brief See CycloidParameter enum for details.
                Real Parameter_e;
                /// @internal
                /// @brief The type of Cycloid to draw.
                CycloidType Type;

                /// @internal
                /// @brief Draws a Hypocycloid to the texture.
                /// @param XPos The central position on the X axis of the shape to draw on the texture.
                /// @param YPos The central position on the Y axis of the shape to draw on the texture.
                /// @param Step The pixel distance at which to end the current drawing segment.
                /// @param Buffer The buffer being drawn to.
                void ProcessHypocycloid(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer);
                /// @internal
                /// @brief Draws a Hypotrochoid to the texture.
                /// @param XPos The central position on the X axis of the shape to draw on the texture.
                /// @param YPos The central position on the Y axis of the shape to draw on the texture.
                /// @param Step The pixel distance at which to end the current drawing segment.
                /// @param Buffer The buffer being drawn to.
                void ProcessHypotrochoid(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer);
                /// @internal
                /// @brief Draws a Epicycloid to the texture.
                /// @param XPos The central position on the X axis of the shape to draw on the texture.
                /// @param YPos The central position on the Y axis of the shape to draw on the texture.
                /// @param Step The pixel distance at which to end the current drawing segment.
                /// @param Buffer The buffer being drawn to.
                void ProcessEpicycloid(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer);
                /// @internal
                /// @brief Draws a Epitrochoid to the texture.
                /// @param XPos The central position on the X axis of the shape to draw on the texture.
                /// @param YPos The central position on the Y axis of the shape to draw on the texture.
                /// @param Step The pixel distance at which to end the current drawing segment.
                /// @param Buffer The buffer being drawn to.
                void ProcessEpitrochoid(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer);
                /// @internal
                /// @brief Draws a Rose Curve to the texture.
                /// @param XPos The central position on the X axis of the shape to draw on the texture.
                /// @param YPos The central position on the Y axis of the shape to draw on the texture.
                /// @param Step The pixel distance at which to end the current drawing segment.
                /// @param Buffer The buffer being drawn to.
                void ProcessRoseCurve(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer);
                /// @internal
                /// @brief Draws a Lissajous Curve to the texture.
                /// @param XPos The central position on the X axis of the shape to draw on the texture.
                /// @param YPos The central position on the Y axis of the shape to draw on the texture.
                /// @param Step The pixel distance at which to end the current drawing segment.
                /// @param Buffer The buffer being drawn to.
                void ProcessLissajousCurve(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer);
                /// @internal
                /// @brief Convenience method for the setting of the actual pixels being drawn.
                /// @param XPos The position on the X axis of the pixel to paint to the texture.
                /// @param YPos The position on the Y axis of the pixel to paint to the texture.
                /// @param Step The pixel distance at which to end the current drawing segment.
                /// @param Buffer The buffer being drawn to.
                void Paint(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer);
            public:
                /// @brief Blank constructor.
                CycloidModifier();
                /// @brief Class destructor.
                virtual ~CycloidModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the type of Cycloid that will be rendered.
                /// @param ToDraw The type of Cycloid to draw.  Initial Value: CT_Hypocycloid.
                /// @return Returns a reference to this.
                CycloidModifier& SetCycloidType(const CycloidType ToDraw);
                /// @brief Sets the default values for every parameter.
                /// @param SquareSize The pixel width and height of the texture to draw to.
                /// @return Returns a reference to this.
                CycloidModifier& SetDefaultParameters(const Whole SquareSize);
                /// @brief Sets the default values for every parameter.
                /// @param TextureWidth The pixel width of the texture to draw to.
                /// @param TextureHeight The pixel height of the texture to draw to.
                /// @return Returns a reference to this.
                CycloidModifier& SetDefaultParameters(const Whole TextureWidth, const Whole TextureHeight);
                /// @brief Sets the colour of the lines to be drawn for the Cycloid.
                /// @param Colour The colour of the Cycloid to draw.  Initial Value: (1.0,1.0,1.0,1.0).
                /// @return Returns a reference to this.
                CycloidModifier& SetColour(const ColourValue& Colour);
                /// @brief Sets the colour of the lines to be drawn for the Cycloid.
                /// @param Red The amount of red in the Cycloid that will be drawn.  Initial Value: 1.0.
                /// @param Green The amount of green in the Cycloid that will be drawn.  Initial Value: 1.0.
                /// @param Blue The amount of blue in the Cycloid that will be drawn.  Initial Value: 1.0.
                /// @param Alpha The amount of alpha in the Cycloid that will be drawn.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                CycloidModifier& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the center position of the Cycloid.
                /// @param Center The center position of the Cycloid on the texture in relative units.  Initial Value: (0.5,0.5).
                /// @return Returns a reference to this.
                CycloidModifier& SetCycloidCenter(const Vector2& Center);
                /// @brief Sets the center position of the Cycloid.
                /// @param CenterX The center position of the Cycloid on the texture on the X axis in relative units.  Initial Value: 0.5.
                /// @param CenterY The center position of the Cycloid on the texture on the Y axis in relative units.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                CycloidModifier& SetCycloidCenter(const Real CenterX, const Real CenterY);
                /// @brief Sets the center position of the Cycloid on the X axis.
                /// @param X The center position of the Cycloid on the texture on the X axis in relative units.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                CycloidModifier& SetCycloidCenterX(const Real X);
                /// @brief Sets the center position of the Cycloid on the Y axis.
                /// @param Y The center position of the Cycloid on the texture on the Y axis in relative units.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                CycloidModifier& SetCycloidCenterY(const Real Y);
                /// @brief Sets a Cycloid parameter.
                /// @param Param The parameter to be altered.  See the CycloidParameter enum for more details.
                /// @param Value The value to be assigned to the specified parameter.  See Default Parameters table in the class detailed description for more information.
                /// @return Returns a reference to this.
                CycloidModifier& SetCycloidParameter(const CycloidParameter Param, const Real Value);
                /// @brief Sets the size of the line to draw for the Cycloid.
                /// @param Size The pixel width of the lines drawn for the Cycloid.  Initial Value: 1.
                /// @return Returns a reference to this.
                CycloidModifier& SetCycloidPenSize(const Real Size);
            };//CycloidModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
