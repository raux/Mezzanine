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
// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio-ZLIBLicense.txt
#ifndef _audiowavdecoder_cpp
#define _audiowavdecoder_cpp

#ifdef ENABLE_WAV_ENCODE

#include "Audio/wavdecoder.h"

#include <cstring>

namespace Mezzanine
{
    namespace Audio
    {
        WavDecoder::WavDecoder(DataStreamPtr Stream) :
            WavStream(Stream),
            WavStreamSize(0),
            WavStreamPos(0),
            SampleRate(0),
            ByteRate(0),
            DataSize(0),
            DataOffset(0),
            Channels(0),
            BlockAlign(0),
            BitsPerSample(0),
            Valid(false)
        {
            this->WavStream->seekg(0,std::ios_base::end);
            this->WavStreamSize = this->WavStream->tellg();

            this->ReadWavMetaData(Stream);
            this->WavStream->seekg(0);
        }

        WavDecoder::~WavDecoder()
            {  }

        void WavDecoder::ReadWavMetaData(DataStreamPtr Stream)
        {
            const char* RIFFTAG = "RIFF";
            const char* WAVETAG = "WAVE";
            const char* FORMATTAG = "fmt ";
            const char* DATATAG = "data";

            char Ident[4];
            Int32 Temp32 = 0;
            Int16 Temp16 = 0;
            //Int8 Temp8 = 0;
            UInt32 StartOffset = 0;

            // Read the first 4 bytes
            this->WavStream->seekg(0);
            this->WavStream->read(Ident,4);
            // Check to see if it is a valid RIFF file
            if( strncmp(Ident,RIFFTAG,4) != 0 )
                return;

            this->WavStream->read((char*)&Temp32,4);
            // Check to see if the file is big enough to be valid (not completely accurate)
            if( Temp32 < 44 )
                return;

            this->WavStream->read(Ident,4);
            // Check that it is a wave file
            if( strncmp(Ident,WAVETAG,4) != 0 )
                return;

            // Save our position
            StartOffset = this->WavStream->tellg();
            // Scan for the first fmt chuck (not necessarily right after)
            do{
                this->WavStream->read(Ident,4);
            } while( ( strncmp(Ident,FORMATTAG,4) != 0 ) && ( this->WavStream->tellg() < this->WavStreamSize ) );
            //Did we find it?
            if( this->WavStream->tellg() < ( this->WavStreamSize - 16 ) ) {
                //Yes, read it in
                this->WavStream->read((char*)&Temp32,4);
                if( Temp32 < 16 )
                    return;

                // Check that it is in PCM format, we don't support compressed wavs
                this->WavStream->read((char*)&Temp16,2);
                this->Channels = Temp16;
                // We only support mono or stereo wavs
                if( this->Channels > 2 )
                    return;

                this->WavStream->read((char*)&Temp32,4);
                this->SampleRate = Temp32;
                this->WavStream->read((char*)&Temp32,4);
                this->ByteRate = Temp32;
                this->WavStream->read((char*)&Temp16,2);
                this->BlockAlign = Temp16;
                this->WavStream->read((char*)&Temp16,2);
                this->BitsPerSample = Temp16;

                // We only support 8 bit or 16 bit wavs
                if( this->BitsPerSample != 8 && this->BitsPerSample != 16 )
                    return;

                // Reset our pointer to start scanning for the data block
                this->WavStream->seekg(StartOffset);
                // Scan for the first data chuck (not necessarily right after)
                do{
                    this->WavStream->read(Ident,4);
                } while( ( strncmp(Ident,DATATAG,4) != 0 ) && ( this->WavStream->tellg() < this->WavStreamSize ) );

                // Did we find it?
                if( this->WavStream->tellg() < this->WavStreamSize ) {
                    // Get size of data block
                    this->WavStream->read((char*)&Temp32,4);
                    this->DataSize = Temp32;
                    this->DataOffset = this->WavStream->tellg();
                    this->Valid = true;
                }
            }
        }

        void WavDecoder::ClearStreamErrors()
        {
            if( this->WavStream->eof() ) {
                this->WavStream->clear( this->WavStream->rdstate() ^ ( std::ios::eofbit | std::ios::failbit ) );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Boole WavDecoder::IsValid()
        {
            return this->Valid;
        }

        Audio::Encoding WavDecoder::GetEncoding() const
        {
            return Audio::Enc_WAV;
        }

        Boole WavDecoder::IsSeekingSupported()
        {
            return true;
        }

        Audio::BitConfig WavDecoder::GetBitConfiguration() const
        {
            if(this->Channels == 1 && this->BitsPerSample == 8)
                return Audio::BC_8Bit_Mono;
            else if(this->Channels == 1 && this->BitsPerSample == 16)
                return Audio::BC_16Bit_Mono;
            else if(this->Channels == 2 && this->BitsPerSample == 8)
                return Audio::BC_8Bit_Stereo;
            else if(this->Channels == 2 && this->BitsPerSample == 16)
                return Audio::BC_16Bit_Stereo;
            else
                return Audio::BC_8Bit_Mono;//fallback return
        }

        UInt32 WavDecoder::GetFrequency() const
        {
            return this->SampleRate;
        }

        DataStreamPtr WavDecoder::GetStream() const
        {
            return this->WavStream;
        }

        Boole WavDecoder::IsEndOfStream() const
        {
            return ( this->WavStream->eof() || this->WavStream->tellg() >= this->WavStreamSize );
        }

        Boole WavDecoder::SetPosition(Int32 Position, const Boole Relative)
        {
            Int32 CurrPos = this->WavStream->tellg();
            Int32 StartPos = this->DataOffset;
            Int32 EndPos = this->DataOffset + this->DataSize;

            if( Relative ) {
                if( CurrPos + Position < StartPos ) Position = StartPos;
                else if( CurrPos + Position > EndPos ) Position = EndPos;
            }else{
                if( Position < StartPos ) Position = StartPos;
                else if( Position > EndPos ) Position = EndPos;
            }

            this->WavStream->seekg(Position);
            this->WavStreamPos = Position;
            return true;
        }

        Int32 WavDecoder::GetPosition() const
        {
            return this->WavStreamPos;
        }

        Boole WavDecoder::Seek(const Real Seconds, const Boole Relative)
        {
            Int32 SeekInBytes = ( Seconds * static_cast<Real>(this->SampleRate) * static_cast<Real>(this->Channels) * (static_cast<Real>(this->BitsPerSample) / 8.0) );
            return this->SetPosition(SeekInBytes,Relative);
        }

        UInt32 WavDecoder::ReadAudioData(void* Output, UInt32 Amount)
        {
            Int32 CurrPos = this->WavStream->tellg();
            Int32 StartPos = this->DataOffset;
            Int32 EndPos = this->DataOffset + this->DataSize;
            Int32 ReadClamped = Amount;

            if( CurrPos > EndPos ) {
                return 0;
            }

            if( CurrPos < StartPos ) {
                this->WavStream->seekg(StartPos);
                CurrPos = this->WavStream->tellg();
            }

            if( CurrPos + ReadClamped > EndPos )
                ReadClamped = EndPos - CurrPos;

            if( ReadClamped < 0 )
                return 0;

            this->WavStream->read(static_cast<char*>(Output),ReadClamped);
            return this->WavStream->gcount();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Stats

        Real WavDecoder::GetTotalTime() const
        {
            Real Second = ( static_cast<Real>(this->SampleRate) * static_cast<Real>(this->Channels) * (static_cast<Real>(this->BitsPerSample) / 8.0) );
            return static_cast<Real>(this->WavStreamSize) / Second;
        }

        Real WavDecoder::GetCurrentTime() const
        {
            Real Second = ( static_cast<Real>(this->SampleRate) * static_cast<Real>(this->Channels) * (static_cast<Real>(this->BitsPerSample) / 8.0) );
            return static_cast<Real>(this->WavStream->tellg()) / Second;
        }

        UInt32 WavDecoder::GetTotalSize() const
        {
            return this->WavStreamSize;
        }

        UInt32 WavDecoder::GetCompressedSize() const
        {
            return this->WavStreamSize;
        }

        UInt32 WavDecoder::GetCurrentPosition() const
        {
            return this->WavStream->tellg();
        }

        UInt32 WavDecoder::GetCurrentCompressedPosition() const
        {
            return this->WavStream->tellg();
        }
    }//Audio
}//Mezzanine

#endif //ENABLE_WAV_ENCODE

#endif
