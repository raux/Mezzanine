// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2017 BlackTopp Studios Inc.
/* This file is part of The DAGFrameScheduler.

    The DAGFrameScheduler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The DAGFrameScheduler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The DAGFrameScheduler.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'doc' folder. See 'gpl.txt'
*/
/* We welcome the use of the DAGFrameScheduler to anyone, including companies who wish to
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
#ifndef _mutex_h
#define _mutex_h

/// @file
/// @brief This file declares and defines a mutex that is a partial implementation

// Parts of this library use the TinyThread++ libary and those parts are also covered by the following license
/*
Copyright (c) 2010-2012 Marcus Geelnard

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#if !defined(SWIG) || defined(SWIG_THREADING) // Do not read when in swig and not in the threading module
#include "atomicoperations.h"
#endif

/// @file
/// @brief Declares a Mutex, Mutex tools

/// @brief Forward declaration
#if defined(_MEZZ_THREAD_WIN32_)
#if defined(__MINGW64_VERSION_MAJOR)
struct _RTL_CRITICAL_SECTION;
typedef _RTL_CRITICAL_SECTION CRITICAL_SECTION;
#else//__MINGW64_VERSION_MAJOR
class _CRITICAL_SECTION;
typedef _CRITICAL_SECTION CRITICAL_SECTION;
#endif//__MINGW64_VERSION_MAJOR
#endif//_MEZZ_THREAD_WIN32_

namespace Mezzanine
{
    namespace Threading
    {
        /// @brief A cross-platform abstraction of the OS's mutex.
        /// @details Use this for potentially long delays, like making multiple system calls. This is likely slower
        /// than a spinlock, but it informs the OS of delay and could allow another thread to be scheduled, making use
        /// of the time the current thread would otherwise spin.
        class MEZZ_LIB Mutex
        {
            private:

                /// @brief Deleted assignment operator
                #ifdef _MEZZ_CPP11_PARTIAL_
                    Mutex& operator=(const Mutex&) = delete;
                #else
                    Mutex& operator=(const Mutex&);
                #endif

                /// @brief Deleted assignment operator copy constructor
                #ifdef _MEZZ_CPP11_PARTIAL_
                    Mutex(const Mutex&) = delete;
                #else
                    Mutex(const Mutex&);
                #endif

                /// @var mHandle
                /// @brief A native handle to the OS specific synchronization object.
                #if defined(_MEZZ_THREAD_WIN32_)
                    CRITICAL_SECTION* mHandle;
                    /// @brief Is this critical section already locked?
                    bool mAlreadyLocked;
                #else
                    pthread_mutex_t mHandle;
                #endif


            public:
                ///@brief Constructor, creates an unlocked mutex
                Mutex();

                ///@brief Destructor.
                ~Mutex();

                /// @brief Lock the mutex.
                /// @details The method will block the calling thread until a lock on the mutex can
                /// be obtained. The mutex remains locked until @c unlock() is called.
                /// @see lock_guard
                void Lock();
                /// @brief Try to lock the mutex.
                /// @details The method will try to lock the mutex. If it fails, the function will
                /// return immediately (non-blocking).
                /// @return @c true if the lock was acquired, or @c false if the lock could
                /// not be acquired.
                bool TryLock();
                /// @brief Unlock the mutex.
                /// @details If any threads are waiting for the lock on this mutex, one of them will
                /// be unblocked.
                void Unlock();

                /// @brief Simply calls Lock() for compatibility with lock_guard and other more standard mutexes
                void lock()
                    { Lock(); }
                /// @brief Simply calls Unlock() for compatibility with lock_guard and other more standard mutexes
                void unlock()
                    { Unlock(); }
        };//Mutex
    }//Threading
}//Mezzanine

#endif
