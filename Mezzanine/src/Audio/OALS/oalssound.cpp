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
// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio-ZLIBLicense.txt
#ifndef _audiooalssound_cpp
#define _audiooalssound_cpp

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#define OALS_STRUCTS_DECLARED

#include "Audio/OALS/oalssound.h"
#include "Audio/OALS/oalsfilter.h"
#include "Audio/OALS/oalseffect.h"
#include "Audio/OALS/oalsdefines.h"
#include "exception.h"

#include "Audio/audiomanager.h"
#include "Audio/decoderfactory.h"

#include <algorithm>
#include <cstring>
#include <memory>

namespace
{
    /// @internal
    /// @brief Converts the BitConfig value to the internal type.
    /// @param Config The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertBitConfigEnum(Mezzanine::Audio::BitConfig Config)
    {
        switch(Config)
		{
            case Mezzanine::Audio::BC_8Bit_Mono:     return AL_FORMAT_MONO8;      break;
            case Mezzanine::Audio::BC_16Bit_Mono:    return AL_FORMAT_MONO16;     break;
            case Mezzanine::Audio::BC_8Bit_Stereo:   return AL_FORMAT_STEREO8;    break;
            case Mezzanine::Audio::BC_16Bit_Stereo:  return AL_FORMAT_STEREO16;   break;
            default:                                 return AL_FORMAT_MONO8;      break;
		}
    }
}

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            /// @brief This enum describes a set of Boole options common for objects playing back audio.
            enum PlaybackState
            {
                PS_Playing = 1,
                PS_Paused  = 2,
                PS_Stopped = 4,
                PS_Looping = 8
            };//PlaybackState

            ///////////////////////////////////////////////////////////////////////////////
            // Sound Methods

            Sound::Sound(const UInt16 Type, iDecoder* Decode, ALCcontext* PlayContext) :
                Context(PlayContext),
                SoundFilter(NULL),
                SoundDecoder(Decode),
                SType(Type),
                State(OALS::PS_Stopped),
                SoundPitch(1.0),
                BaseVolume(1.0),
                MinVolume(0.0),
                MaxVolume(1.0),
                InternalSource(0)
            {
                // Set the context, so we can safely do all our business with the sources
                this->MakeCurrent();
                // Create the storage space for our buffer handles
                this->Buffers.resize(OALS_SOURCE_NUM_BUFFERS,0);
                // Create the storage space for our effect slots
                this->Effects.resize(OALS_SOURCE_MAX_EFFECT_SLOTS,NULL);

                // Create and place our buffers
                alGenBuffers(OALS_SOURCE_NUM_BUFFERS,&Buffers[0]);
                // Create our source
                alGenSources(1,&InternalSource);
                // Tell our source it'll be playing in 2D mode
                alSourcei(InternalSource,AL_SOURCE_RELATIVE,true);
            }

            Sound::~Sound()
            {
                // Set the context, so we can safely do all our business with the sources
                this->MakeCurrent();
                // Stop the source
                alSourceStop(InternalSource);
                // Eradicate the source
                alDeleteSources(1,&InternalSource);
                // Eradicate the buffers
                alDeleteBuffers(OALS_SOURCE_NUM_BUFFERS,&this->Buffers[0]);
                // Clean out our decoder
                delete this->SoundDecoder;
            }

            void Sound::MakeCurrent()
            {
                ALCcontext* CurrContext = alcGetCurrentContext();
                if( CurrContext != this->Context ) {
                    alcMakeContextCurrent(this->Context);
                }
            }

            Boole Sound::StreamToBuffer(const UInt32 BufferID)
            {
                if( this->SoundDecoder ) {
                    UInt32 TotalRead = 0;
                    UInt8 ErrorCount = 0;
                    Char8 ALBuffer[OALS_SOURCE_BUFFER_SIZE];
                    while( TotalRead < OALS_SOURCE_BUFFER_SIZE )
                    {
                        Char8 DecodeBuffer[OALS_SOURCE_BUFFER_SIZE];
                        UInt32 AmountDecoded = this->SoundDecoder->ReadAudioData(DecodeBuffer,OALS_SOURCE_BUFFER_SIZE - TotalRead);
                        if( AmountDecoded > 0 ) {
                            memcpy(ALBuffer+TotalRead,DecodeBuffer,AmountDecoded);
                            TotalRead += AmountDecoded;
                        }else if( AmountDecoded < 0 ) {
                            ++ErrorCount;
                            if( ErrorCount >= 3 ) {
                                this->Stop();
                                return false;
                                //break;
                            }
                        }else if( AmountDecoded == 0 ) {
                            if( this->IsLooping() ) this->SoundDecoder->SetPosition(0,false);
                            else break;
                        }
                    }

                    if( TotalRead == 0 ) {
                        return false;
                    }
                    alBufferData(BufferID,ConvertBitConfigEnum(this->SoundDecoder->GetBitConfiguration()),ALBuffer,TotalRead,this->SoundDecoder->GetFrequency());
                    return true;
                }
                return false;
            }

            void Sound::RefreshFilter()
            {
                if( this->SoundFilter != NULL && this->SoundFilter->_IsDirty() ) {
                    ALuint FilterID = this->SoundFilter->_GetInternalFilter();
                    alSourcei(this->InternalSource,AL_DIRECT_FILTER,FilterID);
                }
            }

            void Sound::RefreshEffects()
            {
                for( UInt32 Index = 0 ; Index < this->Effects.size() ; ++Index )
                {
                    OALS::Effect* CurrEffect = this->Effects.at(Index);
                    if( CurrEffect != NULL && CurrEffect->_IsDirty() ) {
                        //ALuint EffectID = CurrEffect->_GetInternalEffect();
                        ALuint EffectSlotID = CurrEffect->_GetInternalEffectSlot();
                        ALuint FilterID = ( CurrEffect->GetFilter() != NULL ? static_cast<OALS::Filter*>(CurrEffect->GetFilter())->_GetInternalFilter() : AL_FILTER_NULL );
                        alSource3i(this->InternalSource,AL_AUXILIARY_SEND_FILTER,EffectSlotID,Index,FilterID);
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            Boole Sound::IsValid() const
            {
                return ( this->SoundDecoder && this->SoundDecoder->GetStream() && this->InternalSource );
            }

            UInt16 Sound::GetType() const
            {
                return this->SType;
            }

            iDecoder* Sound::GetDecoder() const
            {
                return this->SoundDecoder;
            }

            void Sound::SetPitch(const Real Pitch)
            {
                if( this->SoundPitch != Pitch ) {
                    this->MakeCurrent();
                    alSourcef(this->InternalSource,AL_PITCH,Pitch);
                    this->SoundPitch = Pitch;
                }
            }

            Real Sound::GetPitch() const
            {
                return this->SoundPitch;
            }

            void Sound::SetStream(DataStreamPtr Stream, const Audio::Encoding Encode)
            {
                iDecoderFactory* Factory = AudioManager::GetSingletonPtr()->GetDecoderFactory(Encode);
                if( Factory != NULL ) {
                    if( this->SoundDecoder ) {
                        delete this->SoundDecoder;
                        this->SoundDecoder = NULL;
                    }
                    this->SoundDecoder = Factory->CreateDecoder(Stream);
                }
            }

            void Sound::SetStream(const UInt16 Type, DataStreamPtr Stream, const Audio::Encoding Encode)
            {
                this->SType = Type;
                this->SetStream(Stream,Encode);
            }

            void Sound::SetStream(iDecoder* Decode)
            {
                if( this->SoundDecoder ) {
                    delete this->SoundDecoder;
                    this->SoundDecoder = NULL;
                }
                this->SoundDecoder = Decode;
            }

            void Sound::SetStream(const UInt16 Type, iDecoder* Decode)
            {
                this->SType = Type;
                this->SetStream(Decode);
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Playback

            Boole Sound::Play()
            {
                if( !this->IsPlaying() ) {
                    if( !this->IsPaused() ) {
                        UInt32 QueueSize = 0;
                        for( UInt32 BuffIndex = 0 ; BuffIndex < this->Buffers.size() ; ++BuffIndex )
                        {
                            if( this->StreamToBuffer(Buffers[BuffIndex]) ) ++QueueSize;
                            else return false;
                        }
                        alSourceQueueBuffers(this->InternalSource,QueueSize,&this->Buffers[0]);
                    }

                    alSourcePlay(this->InternalSource);
                    this->State = ( this->IsLooping() ? OALS::PS_Playing | OALS::PS_Looping : OALS::PS_Playing );
                }
                return true;
            }

            Boole Sound::IsPlaying() const
            {
                return (this->State & OALS::PS_Playing);
            }

            void Sound::Pause()
            {
                // Pause the source
                alSourcePause(this->InternalSource);
                this->State = ( this->IsLooping() ? OALS::PS_Paused | OALS::PS_Looping : OALS::PS_Paused );
            }

            Boole Sound::IsPaused() const
            {
                return (this->State & OALS::PS_Paused);
            }

            void Sound::Stop()
            {
                // Stop the source
                alSourceStop(this->InternalSource);
                // Unbind the buffers
                Int32 BufferCount;
                ALuint TempBuffer;
                alGetSourcei(this->InternalSource,AL_BUFFERS_PROCESSED,&BufferCount);
                for( Int32 BuffIndex = 0 ; BuffIndex < BufferCount ; ++BuffIndex )
                    { alSourceUnqueueBuffers(this->InternalSource,1,&TempBuffer); }
                // Inform the decoder
                this->SoundDecoder->SetPosition(0,false);
                this->State = ( this->IsLooping() ? OALS::PS_Stopped | OALS::PS_Looping : OALS::PS_Stopped );
            }

            Boole Sound::IsStopped() const
            {
                return (this->State & OALS::PS_Stopped);
            }

            void Sound::Loop(Boole ToLoop)
            {
                if(ToLoop) this->State = ( this->State | OALS::PS_Looping );
                else this->State = ( this->State & ~OALS::PS_Looping );
            }

            Boole Sound::IsLooping() const
            {
                return (this->State & OALS::PS_Looping);
            }

            Boole Sound::Seek(const Real Seconds, Boole Relative)
            {
                Boole Ret = false;
                if( this->SoundDecoder->IsSeekingSupported() ) {
                    Ret = this->SoundDecoder->Seek(Seconds,Relative);
                }
                return Ret;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Volume Control

            Real Sound::GetVolume() const
            {
                AudioManager* AudioMan = AudioManager::GetSingletonPtr();
                return ( AudioMan->IsMuted() ? 0 : this->BaseVolume * AudioMan->GetTypeVolume(this->SType) );
            }

            void Sound::SetBaseVolume(const Real Base)
            {
                this->BaseVolume = Base;
            }

            Real Sound::GetBaseVolume() const
            {
                return this->BaseVolume;
            }

            void Sound::SetMinVolume(const Real MinVol)
            {
                if( this->MinVolume != MinVol ) {
                    this->MakeCurrent();
                    alSourcef(this->InternalSource,AL_MIN_GAIN,MinVol);
                    this->MinVolume = MinVol;
                }
            }

            Real Sound::GetMinVolume() const
            {
                return this->MinVolume;
            }

            void Sound::SetMaxVolume(const Real MaxVol)
            {
                if( this->MaxVolume != MaxVol ) {
                    this->MakeCurrent();
                    alSourcef(this->InternalSource,AL_MAX_GAIN,MaxVol);
                    this->MaxVolume = MaxVol;
                }
            }

            Real Sound::GetMaxVolume() const
            {
                return this->MaxVolume;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Effects Methods

            Boole Sound::AttachEffect(const UInt32 Slot, iEffect* Eff)
            {
                if( Eff && Eff->IsValid() == false ) {
                    return false;
                }

                if( Slot < this->Effects.size() ) {
                    OALS::Effect* BeingAdded = static_cast<OALS::Effect*>(Eff);
                    if( BeingAdded ) {
                        this->Effects[Slot] = BeingAdded;

                        ALuint EffectSlotID = BeingAdded->_GetInternalEffectSlot();
                        ALuint FilterID = ( BeingAdded->GetFilter() != NULL ? static_cast<OALS::Filter*>(BeingAdded->GetFilter())->_GetInternalFilter() : AL_FILTER_NULL );
                        alSource3i(this->InternalSource,AL_AUXILIARY_SEND_FILTER,EffectSlotID,Slot,FilterID);
                        return true;
                    }
                }
                return false;
            }

            iEffect* Sound::GetEffect(const UInt32 Slot) const
            {
                if( Slot < this->Effects.size() ) {
                    return Effects.at(Slot);
                }else{
                    return NULL;
                }
            }

            UInt32 Sound::GetMaxEffectSlots() const
            {
                return this->Effects.size();
            }

            UInt32 Sound::GetNumEffectSlotsAvailable() const
            {
                UInt32 Available = 0;
                for( ConstEffectIterator EffIt = this->Effects.begin() ; EffIt != this->Effects.end() ; ++EffIt )
                {
                    if( (*EffIt) != NULL )
                        ++Available;
                }
                return Available;
            }

            void Sound::RemoveEffect(const UInt32 Slot)
            {
                if( Slot < this->Effects.size() ) {
                    this->Effects[Slot] = NULL;
                    alSource3i(this->InternalSource,AL_AUXILIARY_SEND_FILTER,AL_EFFECTSLOT_NULL,Slot,AL_FILTER_NULL);
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Filter Methods

            Boole Sound::AttachFilter(iFilter* Fil)
            {
                if( Fil && Fil->IsValid() == false )
                    return false;

                this->SoundFilter = static_cast<OALS::Filter*>(Fil);
                if( this->SoundFilter != NULL ) {
                    ALuint FilterID = this->SoundFilter->_GetInternalFilter();
                    alSourcei(this->InternalSource,AL_DIRECT_FILTER,FilterID);
                }
                return true;
            }

            iFilter* Sound::GetFilter() const
            {
                return this->SoundFilter;
            }

            void Sound::RemoveFilter()
            {
                if( this->SoundFilter != NULL ) {
                    this->SoundFilter = NULL;
                    alSourcei(this->InternalSource,AL_DIRECT_FILTER,AL_FILTER_NULL);
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            void Sound::_Update()
            {
                this->MakeCurrent();
                // Update our volume
                alSourcef(this->InternalSource,AL_GAIN,this->GetVolume());

                this->RefreshFilter();
                this->RefreshEffects();

                // Update our streaming buffers
                this->_UpdateBuffers();
            }

            Boole Sound::_UpdateBuffers()
            {
                Int32 Processed = 0;
                Boole Active = true;
                if( this->IsPlaying() ) {
                    alGetSourcei(this->InternalSource,AL_BUFFERS_PROCESSED,&Processed);

                    while( Processed-- )
                    {
                        UInt32 Buffer = 0;
                        alSourceUnqueueBuffers(this->InternalSource,1,&Buffer);
                        Active = this->StreamToBuffer(Buffer);

                        if( Active ) {
                            alSourceQueueBuffers(this->InternalSource,1,&Buffer);
                        }
                    }
                }

                ALenum OALSState;
                alGetSourcei(this->InternalSource,AL_SOURCE_STATE,&OALSState);
                if( OALSState == AL_STOPPED && !this->IsStopped() ) {
                    this->SoundDecoder->SetPosition(0,false);
                    this->State = ( this->IsLooping() ? OALS::PS_Stopped | OALS::PS_Looping : OALS::PS_Stopped );
                }
                return Active;
            }
        }//OALS
    }//Audio
}//Mezzanine

#endif
