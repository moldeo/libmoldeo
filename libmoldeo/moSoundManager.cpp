/*******************************************************************************

                              moSoundManager.cpp

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
#undef generic
#include "moSoundManager.h"

#include "moArray.h"
moDefineDynamicArray( moSoundArray )
moDefineDynamicArray( moSoundEffectArray )
moDefineDynamicArray( moSoundBufferArray )

#ifdef MO_WIN32
/*
    #include <AL/alut.h>
*/
#endif

#ifdef MO_LINUX
    #include <AL/al.h>
    #include <AL/alut.h>
    #include <AL/alext.h>
#endif

#ifdef MO_MACOSX
    //#include <al.h>
    //#include <alut.h>
    //#include <alext.h>
#endif


moSound::moSound() {

	SetName("");
	SetFileName("");
	m_pAudioGraph = NULL;
	m_bIsPlaying = false;
	m_pAudioGraph = new moGsGraph();
	m_BufferSize = 0;
	m_ActualSample = 0;
	m_OldSample = 0;
}

moSound::~moSound() {
	Finish();
}


MOboolean moSound::Finish() {
  if (m_pAudioGraph) {
    delete m_pAudioGraph;
    m_pAudioGraph = NULL;
  }
  moAbstract::m_bInitialized=false;
  return (m_pAudioGraph == NULL);
}

MOboolean moSound::Init() {
  if (m_pAudioGraph) {
    moAbstract::m_bInitialized = m_pAudioGraph->InitGraph();
  }
  return m_bInitialized;
}

moText moSound::GetName() {
  return m_SoundName;
}

void moSound::SetName( moText name ) {
  m_SoundName = name;
}

moText moSound::GetFileName() {
  return m_FileName;
}

void moSound::SetFileName( moText filename ) {
    m_FileName = filename;
}

moSoundType moSound::GetType() {
  return m_SoundType;
}

MOboolean moSound::LoadFromFile( moText filename ) {
  if (m_pAudioGraph) {
    if (m_pAudioGraph->Initialized()) {
      if (m_pAudioGraph->BuildLiveSound( filename )) {
        SetFileName(filename);
        return true;
      }
    } else {
        MODebug2->Error("moSound::LoadFromFile >  Error: AudioGraph not initialized.");
    }
  }
  return false;
}

MOboolean moSound::SupportedFile(moText p_filename) {
  moFile soundFile(p_filename);
  if ( soundFile.GetExtension()==".mp3"
      || soundFile.GetExtension()==".wav"
      || soundFile.GetExtension()==".m4a"
      || soundFile.GetExtension()==".ogg" ) {
        return true;
  }
  return true;
}

void moSound::Play() {
  if (m_pAudioGraph) {
    m_pAudioGraph->Play();
  }
}

void moSound::Pause() {
  if (m_pAudioGraph)
    m_pAudioGraph->Pause();
}

void moSound::Stop() {
  if (m_pAudioGraph) {
    m_pAudioGraph->Stop();
  }
}


void moSound::Rewind()  {
  if (m_pAudioGraph)
    m_pAudioGraph->Stop();
}

moStreamState moSound::State()  {

  moStreamState stream_state = MO_STREAMSTATE_UNKNOWN;

  if (m_pAudioGraph) {

    stream_state = m_pAudioGraph->GetState();

    switch(stream_state) {
      case MO_STREAMSTATE_PLAYING:
        m_bIsPlaying = true;
        break;
      default:
        m_bIsPlaying = false;
        break;
    }

  }

  return stream_state;
}

bool moSound::IsPlaying() {

  if (m_pAudioGraph) {
    m_pAudioGraph->GetState();
  }
  return m_bIsPlaying;
}


void moSound::Update()  {
 // if (m_pAudioGraph)
 //   m_pAudioGraph->Stop();
}

MOulong moSound::GetPosition() {
  if (m_pAudioGraph)
    return m_pAudioGraph->GetPositionMS();
  return 0;
}

MOulong moSound::GetSampleCount() {
  if (m_pAudioGraph)
    return m_pAudioGraph->GetSamplesLength();
  return 0;
}


MOulong moSound::GetDuration() {
  if (m_pAudioGraph)
    return m_pAudioGraph->GetDuration();
  return 0;
}
void  moSound::Seek( int position, float rate ) {
  if (m_pAudioGraph)
    m_pAudioGraph->Seek( position, rate );
}

bool  moSound::IsEOS() {
  if (m_pAudioGraph)
    return m_pAudioGraph->IsEOS();
  return false;
}

void moSound::SetVolume( float gain )  {
  if (m_pAudioGraph)
    m_pAudioGraph->SetVolume(gain);

  m_Volume = gain;

}


float moSound::GetVolume()  {
    return m_Volume;

}


void moSound::SetPitch( float pitch )  {
  if (m_pAudioGraph) {
    m_pAudioGraph->SetPitch(pitch);
    m_Pitch = pitch;
  }

}


float moSound::GetPitch()  {
    return m_Pitch;
}


void moSound::SetBalance( float balance )  {
  if (m_pAudioGraph) {
    m_pAudioGraph->SetBalance( balance );
    m_Balance = balance;
  }
}


float moSound::GetBalance()  {
    return m_Balance;
}



void moSound::SetEchoDelay( float delay )  {

  if (m_pAudioGraph) {
    m_pAudioGraph->SetEchoDelay(delay);
    m_EchoDelay = delay;
  }
}


float moSound::GetEchoDelay()  {
    return m_EchoDelay;
}


void moSound::SetEchoIntensity( float intensity )  {
  if (m_pAudioGraph) {
    m_pAudioGraph->SetEchoIntensity(intensity);
    m_EchoIntensity = intensity;
  }
}


float moSound::GetEchoIntensity()  {
 return m_EchoIntensity;
}


void moSound::SetEchoFeedback( float feedback )  {
  if (m_pAudioGraph) {
    m_pAudioGraph->SetEchoFeedback(feedback);
    m_EchoFeedback = feedback;
  }
}


float moSound::GetEchoFeedback()  {
 return m_EchoFeedback;
}

void
moSound::PlaySample( MOint sampleid ) {
  MODebug2->Error("Not implemented in moSound, must use an effect as Sound3D to get a full moSound3D interface.");
}



/**-------------------------------------------------

  OpenAl Sound - 3D + FX

-------------------------------------------------*/

moSound3D::moSound3D() {
}

moSound3D::~moSound3D() {
	Finish();
}

MOboolean moSound3D::Finish() {
	// Clean up by deleting Source(s) and Buffer(s)
	return true;
}


MOboolean moSound3D::Init() {
	m_BufferSize = 0;
	m_ActualSample = 0;
	m_OldSample = 0;
	return true;
}



void moSound3D::SetPosition( float x, float y, float z ) {
/*
	alSource3f( m_SourceId, AL_POSITION, x, y, z );
*/
  x = 0;
  y = 0;
  z = 0;
}

void moSound3D::SetVelocity( float x, float y, float z ) {
/*
	alSource3f( m_SourceId, AL_VELOCITY, x, y, z );
*/
  x = 0;
  y = 0;
  z = 0;
}

void moSound3D::SetDirection( float x, float y, float z ) {
/*
	alSource3f( m_SourceId, AL_DIRECTION, x, y, z );
*/
  x = 0;
  y = 0;
  z = 0;
}



/*======================*/
/*		moSoundManager		*/
/*======================*/
void moSoundEffect::SetParameterF( MOint param, MOfloat flValue ) {
#ifdef MO_WIN32
	//alEffectf( m_EffectId, param, flValue );
#endif
  param = 0;
  flValue = 0;
}

void moSoundEffect::GetParameterF( MOint param, MOfloat *pflValue ) {
#ifdef MO_WIN32
	//alGetEffectf( m_EffectId, param,  pflValue );
#endif
  param = 0;
  pflValue = NULL;
}

moSoundManager::moSoundManager() {
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_SOUND );

	SetName("soundmanager");
	SetLabelName("soundmanager");
}


moSoundManager::~moSoundManager() {
	Finish();
}


MOboolean moSoundManager::Init() {

	if (!m_pResourceManager) return false;

  moResource::Init();

#ifdef MO_WIN32
    #ifdef MO_USING_ALFW
	ALenum			eBufferFormat = 0;

	ALFWInit();

	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
		return 0;
	}

	// For this example, we are going to play a 5.1 test file ...
	/*eBufferFormat = alGetEnumValue("AL_FORMAT_51CHN16");
	if (!eBufferFormat)
	{
		ALFWprintf("No support for 5.1 playback!\n");
		ALFWShutdownOpenAL();
		ALFWShutdown();
		return 0;
	}
*/

	if (ALFWIsXRAMSupported()) {
		//continue
	}
	if (ALFWIsEFXSupported()) {

		//esto deberia configurarse desde el config
		moSoundEffect* pneweffect = new moSoundEffect();
		/*
		ALuint genfxid;
		if (pneweffect) {
			alGenEffects(1, &genfxid);
			pneweffect->m_EffectId  = genfxid;
			alEffecti(pneweffect->m_EffectId, AL_EFFECT_TYPE, AL_EFFECT_COMPRESSOR);
			m_effects_array.Add(pneweffect);
		}

		pneweffect = new moSoundEffect();
		genfxid;
		if (pneweffect) {
			alGenEffects(1, &genfxid);
			pneweffect->m_EffectId  = genfxid;
			alEffecti(pneweffect->m_EffectId, AL_EFFECT_TYPE, AL_EFFECT_EQUALIZER);
			m_effects_array.Add(pneweffect);
		}*/
	}
  #endif
	#endif

	#ifdef MO_LINUX

	#endif

	m_sounds_array.Init( 0, NULL );

	m_bInitialized = true;

	return m_bInitialized;
}

moSoundEffectArray*
moSoundManager::GetEffects() {

	return &m_effects_array;

}

int moSoundManager::GetSoundCount() {
  return m_sounds_array.Count();
}


moSound* moSoundManager::GetSound( int id) {

	if (id>=0 && id<(int)m_sounds_array.Count()) {
		return m_sounds_array[id];
	}
	return NULL;

}

moSound* moSoundManager::GetSound( moText p_name, bool create) {

	int id;

  if (p_name=="") {
      return NULL;
  }

	id = GetSoundId( p_name, create);

	if(id == MO_SOUNDERROR ) {
    if (create)
      MODebug2->Error("ERROR DE SONIDO, no se pudo cargar: " + p_name );
		return(NULL);
	}  //error
	else {
		return(m_sounds_array[id]);
	}
}

MOint
moSoundManager::GetSoundId( moText p_name, bool create ) {

	MOint i,idnuevo;

	for( i=0; i<(int)m_sounds_array.Count(); i++ ) {
		if(!stricmp( m_sounds_array[i]->GetName(), p_name )) {
			return(i);
			break;
		}
	}

  if (!create) return -1;

	moSoundType stype = GetTypeForFile(p_name);
	if (!m_pResourceManager) return MO_SOUNDERROR;

	moText namefull = m_pResourceManager->GetDataMan()->GetDataPath();
	namefull +=  moSlash + (const moText)p_name;

	moFile fileSound(namefull);
	namefull = fileSound.GetCompletePath();

	moSoundParam sparam = MODefSoundParams ;
	moSound* pSound = CreateSound( p_name );

	if (pSound) {

		pSound->LoadFromFile( namefull );

		m_sounds_array.Add( pSound );

		idnuevo = m_sounds_array.Count() - 1;
	} else {
	  idnuevo = -1;
	}
	//si es superior a 65535 es un error
	stype = MO_SOUNDTYPE_UNDEFINED;
	return(idnuevo);
}

int
moSoundManager::AddSound( moSound* p_Sound ) {

  if (p_Sound) {
    m_sounds_array.Add( p_Sound );
    return m_sounds_array.Count()-1;
  }
  return -1;

}

moSoundType
moSoundManager::GetTypeForFile( moText p_name ) {

		moText extension = p_name;
		extension.Right(3);

		if(!stricmp(extension,"mp3") || !stricmp(extension,"mpg") || !stricmp(extension,"peg")) {
			return MO_SOUNDTYPE_MP3;
        } else if(!stricmp(extension,"m4a")) {
			return MO_SOUNDTYPE_M4A;
		} else if(!stricmp(extension,"wav")) {
			return MO_SOUNDTYPE_WAV;
		} else if(!stricmp(extension,"ogg")) {
			return MO_SOUNDTYPE_OGG;
		} else return MO_SOUNDTYPE_UNDEFINED;
}

moSound* moSoundManager::CreateSound( moText p_name ) {

	moSound* pSound;

	pSound = new moSound();

	if (pSound) {

		pSound->SetName(p_name);
		pSound->Init();

		return pSound;
	} else return NULL;

}


MOboolean moSoundManager::Finish() {
	for(MOuint i=0; i<m_sounds_array.Count(); i++) {
		//moSound* psound = m_sounds_array[i];
		//if (psound) delete psound;
		//psound = NULL;
	}
	m_sounds_array.Empty();

	for(MOuint i=0; i<m_effects_array.Count(); i++) {
		//moSoundEffect* psoundeffect = m_effects_array[i];
		//if (psoundeffect) delete psoundeffect;
		//psoundeffect = NULL;
	}
	m_effects_array.Empty();

	#ifdef MO_WIN32
    #ifdef MO_USING_ALFW
    if (m_bInitialized) {
          ALFWShutdownOpenAL();
          ALFWShutdown();
    }
    #endif
	#endif
	return true;
}

