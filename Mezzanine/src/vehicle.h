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
#ifndef _vehicle_h
#define _vehicle_h

/// @file
/// @brief This file contains the declaration for the vehicle base class used to represent a solid self-propelling object.

#include "entity.h"
#include "entityfactory.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class from which all the vehicles inherit.
    /// @details The vehicle classes are responsible for solid objects capable of self propulsion and may have unique rules
    /// for their locomotion.
    ///////////////////////////////////////
    class MEZZ_LIB Vehicle : public Entity
    {
    protected:
    public:
        /// @brief Blank constructor.
        /// @param TheWorld A pointer to the world this Vehicle belongs to.
        Vehicle(World* TheWorld);
        /// @brief Class constructor.
        /// @param EntID The unique ID of the Vehicle.
        /// @param TheWorld A pointer to the world this Vehicle belongs to.
        Vehicle(const EntityID& EntID, World* TheWorld);
        /// @brief Class destructor.
        virtual ~Vehicle();
    };//Vehicle

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A base factory type for the creation of Vehicle objects.
    ///////////////////////////////////////
    class MEZZ_LIB VehicleFactory : public EntityFactory
    {
    public:
        /// @brief Class constructor.
        VehicleFactory() = default;
        /// @brief Class destructor.
        virtual ~VehicleFactory() = default;
    };//VehicleFactory
}//Mezzanine

#endif
