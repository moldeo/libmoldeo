/*******************************************************************************

                                moSoundManager.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                    *
  *                                                                          *
  *   This code is distributed in the hope that it will be useful, but       *
  *   WITHOUT ANY WARRANTY; without even the implied warranty of             *
  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
  *   General Public License for more details.                               *
  *                                                                          *
  *   A copy of the GNU General Public License is available on the World     *
  *   Wide Web at <http://www.gnu.org/copyleft/gpl.html>. You can also       *
  *   obtain it by writing to the Free Software Foundation,                  *
  *   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.         *
  *                                                                          *
  ****************************************************************************

  Copyright(C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa
  Andrés Colubri

*******************************************************************************/

#ifndef _MO_SOUNDMANAGER
#define _MO_SOUNDMANAGER

#include "moTypes.h"
#include "moText.h"
#include "moConfig.h"
#include "moResourceManager.h"

#ifdef MO_WIN32
    #include "Framework.h"
    #include "CWaves.h"
#endif

#ifdef MO_LINUX
    #include <AL/al.h>
    #include <AL/alut.h>
    #include <AL/alext.h>
#endif

#define NUMBUFFERS              (4)
#define	SERVICE_UPDATE_PERIOD	(20)
#define MO_SOUNDERROR 65535

enum moSoundType {
	MO_SOUNDTYPE_UNDEFINED,
	MO_SOUNDTYPE_MP3,
	MO_SOUNDTYPE_WAV,
	MO_SOUNDTYPE_OGG
};

struct LIBMOLDEO_API moSoundParam {
		WORD    wFormatTag;
		WORD    nChannels;
		DWORD   nSamplesPerSec;
		DWORD   nAvgBytesPerSec;
		WORD    nBlockAlign;
		WORD    wBitsPerSample;
		WORD    cbSize;
};

const moSoundParam MODefSoundParams =
{
    0,
    0,
	0,
	0,
	0,
	0,
	0
};

const moSoundParam MOUndefinedSound =
{
    MO_UNDEFINED,
    MO_UNDEFINED,
    MO_UNDEFINED,
    MO_UNDEFINED,
    MO_UNDEFINED,
    MO_UNDEFINED,
    MO_UNDEFINED
};

struct LIBMOLDEO_API moSoundEffect : public moAbstract {

	public:
		moSoundEffect() { m_EffectId=0; }
		virtual ~moSoundEffect() {}

		ALuint m_EffectId;

		void SetParameterF( ALenum param, ALfloat flValue );
		void GetParameterF( ALenum param, ALfloat* pflValue );

};

/*
template class LIBMOLDEO_API moDynamicArray<moSoundEffect*>;
typedef moDynamicArray<moSoundEffect*> moSoundEffectArray;
*/
moDeclareExportedDynamicArray( moSoundEffect*, moSoundEffectArray)

class LIBMOLDEO_API moSoundBuffer : public moAbstract {

	public:
		moSoundBuffer() {  }
		virtual ~moSoundBuffer() {}


};
/*
template class LIBMOLDEO_API moDynamicArray<moSoundBuffer*>;
typedef moDynamicArray<moSoundBuffer*> moSoundBufferArray;
*/
moDeclareExportedDynamicArray( moSoundBuffer*, moSoundBufferArray)


class LIBMOLDEO_API moSound : public  moAbstract {
    public:
		moSound();
		virtual ~moSound();

		virtual MOboolean  Init(moText p_name, MOuint p_moid, moSoundType p_type, moResourceManager* p_res, moSoundParam p_param = MODefSoundParams);
		virtual MOboolean  Finish();

		MOboolean BuildEmpty( MOuint p_size );
		MOboolean BuildFromBuffer( MOuint p_size, GLvoid* p_buffer );
		MOboolean BuildFromFile( moText p_filename );

		MOboolean SupportedFile(moText p_filename);
		MOboolean Load( moParam* p_param );

		moText GetName() { return m_name; }
		void SetName(moText p_name) { m_name = p_name; }
		MOint GetSourceId() { return m_SourceId; }
		MOuint GetBufferId() { return m_BufferId; }
		moSoundType GetType() { return m_SoundType; }

		MOint	GetBufferSize() { return m_BufferSize; }
		MOint	GetActualSample() { return m_ActualSample; }

		void Play();
		void PlaySample( MOint sampleid );
		void Stop();
		void Pause();
		void Rewind();
		void Final();
		void Frame(int frame);
		void Repeat(int repeat);
		MOint State();
		void Update();

		void SetPosition( float x, float y, float z );
		void SetVelocity( float x, float y, float z );
		void SetDirection( float x, float y, float z );
		void SetVolume( float gain );
		float GetVolume();
	protected:

		MOint			m_ActualSample;
		MOint			m_OldSample;

		moFile*			m_pFile;
		moDataManager*	m_pDataMan;
		moFileManager*	m_pFileMan;

		ALuint		    m_Buffers[NUMBUFFERS];
		ALuint		    m_SourceId;
		ALuint			m_BufferId;
		ALint			m_State;



		#ifdef MO_WIN32
		CWaves *		pWaveLoader;
		WAVEID			WaveID;
		ALint			iLoop;
		ALint			iBuffersProcessed, iTotalBuffersProcessed, iQueuedBuffers;
		WAVEFORMATEX	wfex;
		#endif

		unsigned long	ulDataSize;
		unsigned long	ulFrequency;
		unsigned long	ulFormat;
		unsigned long	ulBufferSize;
		unsigned long	ulBytesWritten;
		void *			pData;
		ALenum			eBufferFormat;

		MOint			m_BufferSize;

		moSoundParam	m_SoundParam;
		moSoundType		m_SoundType;
		moText m_name;

		float			m_Gain;
/*AL_GAIN
AL_MIN_GAIN
AL_MAX_GAIN
AL_MAX_DISTANCE
AL_ROLLOFF_FACTOR
AL_CONE_OUTER_GAIN
AL_CONE_INNER_ANGLE
AL_CONE_OUTER_ANGLE
AL_REFERENCE_DISTANCE*/

};

/*
template class LIBMOLDEO_API moDynamicArray<moSound*>;
typedef moDynamicArray<moSound*> moSoundArray;
*/
moDeclareExportedDynamicArray( moSound*, moSoundArray)

class LIBMOLDEO_API moSoundManager : public moResource {
	public:

		moSoundManager();
		virtual ~moSoundManager();

		MOboolean Init();
		MOboolean Finish();

		moSound* GetSound( moText p_name );
		MOuint GetSoundId( moText p_name );
		moSound* GetSound( MOuint id);
		moSoundType GetTypeForFile( moText p_name );
		moSoundEffectArray* GetEffects();
	protected:
		moSound* CreateSound( moText p_name, moSoundType p_type, moSoundParam p_param );
		moSoundArray	m_sounds_array;
		moSoundEffectArray	m_effects_array;

};


#endif
