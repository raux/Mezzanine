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
#ifndef _graphicsproceduraldilatemodifier_cpp
#define _graphicsproceduraldilatemodifier_cpp

#include "Graphics/Procedural/Texture/dilatemodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            DilateModifier::DilateModifier() :
                NumIterations(10)
                {  }

            DilateModifier::~DilateModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void DilateModifier::Modify(TextureBuffer& Buffer)
            {
                Integer Width = static_cast<Integer>( Buffer.GetWidth() );
                Integer Height = static_cast<Integer>( Buffer.GetHeight() );
                TextureBuffer IntBuffer( Buffer );
                TextureBuffer DestBuffer( Buffer );

                TextureBuffer* pSrc = NULL;
                TextureBuffer* pDest = NULL;

                for( Integer CurrIteration = 0 ; CurrIteration < static_cast<Integer>( this->NumIterations ) ; ++CurrIteration )
                {
                    if( CurrIteration == 0 ) {
                        pSrc = &Buffer;
                    }else{
                        pSrc = ( (CurrIteration % 2) != (this->NumIterations % 2) ) ? &DestBuffer : &IntBuffer;
                    }
                    pDest = ( (CurrIteration % 2) == (this->NumIterations % 2) ) ? &DestBuffer : &IntBuffer;

                    for( Integer Y = 0 ; Y < Height ; ++Y )
                    {
                        for( Integer X = 0 ; X < Width ; ++X )
                        {
                            Integer Sum = -1;
                            pDest->SetPixel(X,Y,pSrc->GetPixel(X,Y));

                            for( Integer V = -1 ; V < 2 ; ++V )
                            {
                                for( Integer U = -1 ; U < 2 ; ++U )
                                {
                                    ColourValue Pixel = pSrc->GetPixel( ( X + Width + U ) % Width, ( Y + Height + V ) % Height );
                                    if( ( Pixel.RedChannel + Pixel.GreenChannel + Pixel.BlueChannel ) * 255.0 > Sum ) {
                                        Sum = static_cast<Integer>( ( Pixel.RedChannel + Pixel.GreenChannel + Pixel.BlueChannel ) * 255.0 );
                                        pDest->SetPixel(X,Y,Pixel);
                                    }
                                }
                            }
                        }
                    }
                }

                Buffer.SetData(DestBuffer);
            }

            String DilateModifier::GetName() const
                { return "DilateModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            DilateModifier& DilateModifier::SetNumIterations(const UInt8 Iterations)
                { this->NumIterations = Iterations;  return *this; }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
