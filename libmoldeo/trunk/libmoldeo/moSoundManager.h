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

#include <moTypes.h>
#include <moText.h>
#include <moConfig.h>
#include <moResourceManager.h>
#include <moFileManager.h>
#include <moDataManager.h>


#define NUMBUFFERS              (4)
#define	SERVICE_UPDATE_PERIOD	(20)
#define MO_SOUNDERROR 65535

/// Tipo de sonido
/**
* Tipo de sonido
*/
enum moSoundType {
	MO_SOUNDTYPE_UNDEFINED,
	MO_SOUNDTYPE_MP3,
	MO_SOUNDTYPE_WAV,
	MO_SOUNDTYPE_OGG
};

/// Parámetros de sonido
/**
* Parámetros de sonido
*/
struct LIBMOLDEO_API moSoundParam {
		moWord    wFormatTag;
		moWord    nChannels;
		moDWord   nSamplesPerSec;
		moDWord   nAvgBytesPerSec;
		moWord    nBlockAlign;
		moWord    wBitsPerSample;
		moWord    cbSize;
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

/// Efecto de sonido
/**
* Efecto de sonido
*/
struct LIBMOLDEO_API moSoundEffect : public moAbstract {

	public:
		moSoundEffect() { m_EffectId=0; }
		virtual ~moSoundEffect() {}

		MOuint m_EffectId;

		void SetParameterF( MOint param, MOfloat flValue );
		void GetParameterF( MOint param, MOfloat* pflValue );

};


moDeclareExportedDynamicArray( moSoundEffect*, moSoundEffectArray);

/// Buffer de sonido
/**
* Buffer de sonido
*/
class LIBMOLDEO_API moSoundBuffer : public moAbstract {

	public:
		moSoundBuffer() {  }
		virtual ~moSoundBuffer() {}


};

moDeclareExportedDynamicArray( moSoundBuffer*, moSoundBufferArray);

/// Clase base de sonido
/**
* Clase base de sonido
*/
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

		MOuint		    m_Buffers[NUMBUFFERS];
		MOuint		    m_SourceId;
		MOuint			m_BufferId;
		MOint			m_State;



		#ifdef MO_WIN32
		/*
		CWaves *		pWaveLoader;
		WAVEID			WaveID;
		ALint			iLoop;
		ALint			iBuffersProcessed, iTotalBuffersProcessed, iQueuedBuffers;
		WAVEFORMATEX	wfex;
		*/
		#endif

		MOint			eBufferFormat;


		unsigned long	ulDataSize;
		unsigned long	ulFrequency;
		unsigned long	ulFormat;
		unsigned long	ulBufferSize;
		unsigned long	ulBytesWritten;
		void *			pData;



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


moDeclareExportedDynamicArray( moSound*, moSoundArray);


/// Administrador de sonidos
/**
* Administrador de sonidos
*/
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
