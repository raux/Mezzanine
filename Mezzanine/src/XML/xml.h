//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
 * pugixml parser - version 1.2
 * --------------------------------------------------------
 * Copyright © 2006-2012, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 * Report bugs and download new versions at http://pugixml.org/
 *
 * This library is distributed under the MIT License. See notice at the end
 * of this file.
 *
 * This work is based on the pugxml parser, which is:
 * Copyright © 2003, by Kristen Wegner (kristen@tima.net)
 */
#ifndef _XML_H
#define _XML_H

#ifndef XML_VERSION
// Define version macro; evaluates to major * 100 + minor so that it's safe to use in less-than comparisons
/// @def XML_VERSION This correlates to the version of pugixml this uses
/// @brief
#	define XML_VERSION 100
#endif

#include "XML/objectrange.h"
#include "XML/attribute.h"
#include "XML/attributeiterator.h"
#include "XML/document.h"
#include "XML/memorymanagement.h"
#include "XML/nodeiterator.h"
#include "XML/nodetext.h"
#include "XML/node.h"
#include "XML/parseresult.h"
#include "XML/treewalker.h"
#include "XML/utf8conversion.h"
#include "XML/writer.h"
#include "XML/xmldoc.h"
#include "XML/xmlenumerations.h"
#include "XML/xpathnode.h"
#include "XML/xpathnodeset.h"
#include "XML/xpathparseresult.h"
#include "XML/xpathvariable.h"
#include "XML/xpathvariableset.h"
#include "XML/xpathquery.h"


namespace Mezzanine
{
    namespace XML
    {

    }

}

#endif

/*
 * Copyright © 2006-2012 Arseny Kapoulkine
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
