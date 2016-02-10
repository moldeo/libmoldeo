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

*******************************************************************************/

#ifndef _MO_SOUNDMANAGER
#define _MO_SOUNDMANAGER

#include <moTypes.h>
#include <moText.h>
#include <moConfig.h>
#include <moResourceManager.h>
#include <moFileManager.h>
#include <moDataManager.h>
#include <moGsGraph.h>

#define NUMBUFFERS              (4)
#define	SERVICE_UPDATE_PERIOD	(20)
#define MO_SOUNDERROR -1

/// Tipo de sonido
/**
* Tipo de sonido
*/
enum moSoundType {
	MO_SOUNDTYPE_UNDEFINED,
	MO_SOUNDTYPE_MP3,
	MO_SOUNDTYPE_WAV,
	MO_SOUNDTYPE_OGG,
	MO_SOUNDTYPE_M4A
};

enum moSoundState {
  MO_SOUND_PLAYING,
  MO_SOUND_SEEKING,
  MO_SOUND_STOPPED
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

		virtual MOboolean Init();
		virtual MOboolean Finish();

		virtual moText GetName();
		virtual void SetName( moText name );

		virtual moText GetFileName();
		virtual void SetFileName( moText filename );

		moSoundType GetType();

    MOboolean LoadFromFile( moText filename );

    MOboolean SupportedFile(moText p_filename);

		virtual void Play();
		virtual void Stop();
		virtual void Pause();
		virtual void Rewind();
		virtual moStreamState State();
		virtual bool  IsPlaying();
		virtual void Update();
		virtual MOulong GetPosition();
		virtual MOulong GetDuration();
		virtual MOulong GetSampleCount();
		virtual void Seek( int position, float rate = 1.0);
		virtual bool  IsEOS();

		virtual void SetVolume( float gain );
		virtual float GetVolume();
		virtual void SetPitch( float pitch );
		virtual float GetPitch();
		virtual void SetBalance( float balance );
		virtual float GetBalance();

    virtual void SetEchoDelay( float delay );
		virtual float GetEchoDelay();
    virtual void SetEchoIntensity( float intensity );
		virtual float GetEchoIntensity();
    virtual void SetEchoFeedback( float feedback );
		virtual float GetEchoFeedback();

		moAudioFormat&   GetAudioFormat() { return m_AudioFormat; }
    MOint	GetBufferSize() { return m_BufferSize; }
    virtual void PlaySample( MOint sampleid );
    MOint	GetActualSample() { return m_ActualSample; }

    protected:

      moText          m_SoundName;
      moSoundType     m_SoundType;
      moText          m_FileName;

      moSoundParam	  m_SoundParam;

      moGsGraph*      m_pAudioGraph;
      moAudioFormat   m_AudioFormat;

      float           m_Position;
      float           m_Volume;
      float           m_Pitch;
      float           m_Balance; ///panorama

      float           m_EchoDelay;
      float           m_EchoIntensity;
      float           m_EchoFeedback;

      bool            m_bIsPlaying;
      MOint	          m_BufferSize;
      MOint			m_ActualSample;
      MOint			m_OldSample;


};


/// Clase base de sonido
/**
* Clase base de sonido
*/
class LIBMOLDEO_API moSound3D : public  moSound {
    public:

      moSound3D();
      virtual ~moSound3D();

      virtual MOboolean  Init();
      virtual MOboolean  Finish();

  /**
    OVERRIDE moSound functions
    OpenAL es diferente a GStreamer
  */
      virtual void Play() = 0;
      virtual void Stop() = 0;
      virtual void Pause() = 0;
      virtual void Rewind() = 0;
      virtual moStreamState State() = 0;
      virtual void Update() = 0;
      virtual void SetVolume( float gain ) = 0;
      virtual float GetVolume() = 0;
      virtual void SetPitch( float pitch ) = 0;
      virtual float GetPitch() = 0;

  /**
    OpenAL Specific
  */

      virtual MOboolean BuildEmpty( MOuint p_size ) = 0;
      virtual MOboolean BuildFromBuffer( MOuint p_size, GLvoid* p_buffer ) = 0;
      virtual MOboolean BuildFromFile( const moText& p_filename ) = 0;

      MOint GetSourceId() { return m_SourceId; }
      MOuint GetBufferId() { return m_BufferId; }
      virtual void Final() = 0;
      virtual void Frame(int frame) = 0;
      virtual void Repeat(int repeat) = 0;

      void SetPosition( float x, float y, float z );
      void SetVelocity( float x, float y, float z );
      void SetDirection( float x, float y, float z );



	public:

      moFile*			m_pFile;
      moDataManager*	m_pDataMan;
      moFileManager*	m_pFileMan;

      MOuint		    m_SourceId;
      MOuint			  m_BufferId;


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

		moSound* GetSound( moText p_name, bool create=true );
		int GetSoundId( moText p_name, bool create=true );
		int AddSound( moSound* p_Sound );

		int GetSoundCount();
		moSound* GetSound( int id);

		moSoundType GetTypeForFile( moText p_name );
		moSoundEffectArray* GetEffects();

	protected:

		moSound* CreateSound( moText p_name );

		moSoundArray	m_sounds_array;

		moSoundEffectArray	m_effects_array;

};


#endif

