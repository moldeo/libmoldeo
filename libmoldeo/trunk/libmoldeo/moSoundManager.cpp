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
  Andrés Colubri

*******************************************************************************/

#include "moSoundManager.h"

#include <moArray.h>
moDefineDynamicArray(moSoundArray)
moDefineDynamicArray(moSoundEffectArray)
moDefineDynamicArray(moSoundBufferArray)

#ifdef MO_WIN32
    #include "Framework.h"
    #include "CWaves.h"
#endif

#ifdef MO_LINUX
    #include <AL/al.h>
    #include <AL/alut.h>
    #include <AL/alext.h>
#endif

#ifdef MO_MACOSX
    #include <al.h>
    #include <alut.h>
    //#include <alext.h>
#endif

moSound::moSound() {
	pData = NULL;
}

moSound::~moSound() {
	Finish();
}

MOboolean moSound::Finish() {
	// Clean up by deleting Source(s) and Buffer(s)
	alSourceStop(m_SourceId);
	alSourcei(m_SourceId, AL_BUFFER, 0);
    alDeleteSources(1, &m_SourceId);
	alDeleteBuffers(1, &m_BufferId);
	return true;
}


MOboolean moSound::Init(moText p_name, MOuint p_moid, moSoundType p_type, moResourceManager* p_res, moSoundParam p_param ) {
    m_name = p_name;
	m_BufferSize = 0;
	m_ActualSample = 0;
	m_OldSample = 0;
	m_SoundType = p_type;
	return true;
}


MOboolean
moSound::BuildEmpty( MOuint p_size ) {

	// Generate an AL Buffer
	alGenBuffers( 1, &m_BufferId );

	// Generate a Source to playback the Buffer
	alGenSources( 1, &m_SourceId );

	return true;
}

MOboolean
moSound::BuildFromBuffer( MOuint p_size, GLvoid* p_buffer ) {
	BuildEmpty(p_size);
	return true;
}

MOboolean
moSound::BuildFromFile( moText p_filename ) {

	BuildEmpty(0);

	switch(m_SoundType) {
		case MO_SOUNDTYPE_WAV:
			// Load Wave file into OpenAL Buffer
			#ifdef MO_WIN32
      #if defined(_MSC_VER)
			if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(p_filename), m_BufferId))
			{
				ALFWprintf("Failed to load %s\n", ALFWaddMediaPath(p_filename));
			} else {
				alGetBufferi( m_BufferId, AL_SIZE, &m_BufferSize );
			}
      #endif
			#endif
			break;
		case MO_SOUNDTYPE_MP3:
			break;
		case MO_SOUNDTYPE_OGG:
			break;
		case MO_SOUNDTYPE_UNDEFINED:
			break;
	}

	// Attach Source to Buffer
	alSourcei( m_SourceId, AL_BUFFER, m_BufferId );

	return true;

}

void moSound::Play() {
	// Start playing source
	alSourcePlay(m_SourceId);
}


void moSound::PlaySample( MOint sampleid ) {

	m_OldSample = m_ActualSample;

	alGetSourcei( m_SourceId , AL_BUFFER /*AL_BYTE_OFFSET*/, &m_ActualSample);

	alSourceStop(m_SourceId);

	if (m_ActualSample!=sampleid) {
		alSourcei( m_SourceId, AL_BUFFER /*AL_BYTE_OFFSET*/, sampleid );
		m_ActualSample = sampleid;
	}

	alGetError();
	alSourcePlay(m_SourceId);

}

void moSound::Pause() {
	alSourcePause(m_SourceId);
}

void moSound::Stop() {
	// Stop the Source and clear the Queue
	alSourceStop(m_SourceId);

}

void moSound::Rewind() {
	alSourceRewind(m_SourceId);
}


void moSound::Final() {

	Update();
}

void moSound::Frame(int frame) {
	Update();
}

void moSound::Repeat(int repeat) {

}

MOint moSound::State() {
	// Get Source State
	alGetSourcei( m_SourceId, AL_SOURCE_STATE, &m_State);
	return m_State;
}

void moSound::Update() {
	alGetSourcei( m_SourceId, AL_SOURCE_STATE, &m_State );
	alGetSourcei( m_SourceId, AL_BUFFER /*AL_BYTE_OFFSET*/, &m_ActualSample );
}

void moSound::SetPosition( float x, float y, float z ) {

	alSource3f( m_SourceId, AL_POSITION, x, y, z );

}

void moSound::SetVelocity( float x, float y, float z ) {

	alSource3f( m_SourceId, AL_VELOCITY, x, y, z );

}

void moSound::SetDirection( float x, float y, float z ) {

	alSource3f( m_SourceId, AL_DIRECTION, x, y, z );

}

void moSound::SetVolume( float gain ) {
	GetVolume();
	alSourcef( m_SourceId, AL_GAIN, (m_Gain+gain) );
}

float moSound::GetVolume() {

	alGetSourcef( m_SourceId, AL_GAIN, &m_Gain );
	return m_Gain;

}


/*======================*/
/*		moSoundManager		*/
/*======================*/
void moSoundEffect::SetParameterF( ALenum param, ALfloat flValue ) {
#ifdef MO_WIN32
	alEffectf( m_EffectId, param, flValue );
#endif

}

void moSoundEffect::GetParameterF( ALenum param, ALfloat *pflValue ) {
#ifdef MO_WIN32
	alGetEffectf( m_EffectId, param,  pflValue );
#endif

}

moSoundManager::moSoundManager() {
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_SOUND );
	SetName("soundmanager");
}


moSoundManager::~moSoundManager() {
	Finish();
}


MOboolean moSoundManager::Init() {

	moText confignamecompleto;

	if (!m_pResourceManager) return false;

	confignamecompleto = m_pResourceManager->GetDataMan()->GetDataPath();
	confignamecompleto +=  moText("/") + (const moText)GetConfigName();
    confignamecompleto +=  moText(".cfg");
/*
	if(m_Config.LoadConfig(confignamecompleto) != MO_CONFIG_OK ) {
    MODebug->Push(moText("ERROR: Config file not found."));
		cout << "Error moSoundManager :"<< confignamecompleto << " invalid" << endl;
		return false;//bad
	}
	*/
#ifdef MO_WIN32
    #ifdef MO_USING_VC
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

	m_bInitialized = true;

	return m_bInitialized;
}

moSoundEffectArray*
moSoundManager::GetEffects() {

	return &m_effects_array;

}

moSound* moSoundManager::GetSound( MOuint id) {

	if (id>0 && id<m_sounds_array.Count()) {
		return m_sounds_array[id];
	}
	return NULL;

}

moSound* moSoundManager::GetSound( moText p_name) {
	int id;

        moText text;

	id = GetSoundId( p_name);

	if(id == MO_SOUNDERROR) {
                text = moText("\nERROR DE SONIDO, no se encontro un id para ");
                text += p_name;
                text +=  moText("\n");
		MODebug2->Error(text);
		return(NULL);
	}  //error
	else {
		return(m_sounds_array[id]);
	}
}

MOuint
moSoundManager::GetSoundId( moText p_name ) {

	MOuint i,idnuevo;

	for( i=0; i<m_sounds_array.Count(); i++ ) {
		if(!stricmp( m_sounds_array[i]->GetName(), p_name )) {
			return(i);
			break;
		}
	}

	moSoundType stype = GetTypeForFile(p_name);
	if (!m_pResourceManager) return MO_SOUNDERROR;

	moText namefull = m_pResourceManager->GetDataMan()->GetDataPath();
	namefull +=  moText("/") + (const moText)p_name;
	moSoundParam sparam = MODefSoundParams ;
	moSound* psound = CreateSound( namefull, stype, sparam);
	if (psound!=NULL) {
		psound->BuildFromFile(namefull);
		m_sounds_array.Add(psound);
		idnuevo = m_sounds_array.Count() - 1;
	}
	//si es superior a 65535 es un error
	return(idnuevo);
}

moSoundType
moSoundManager::GetTypeForFile( moText p_name ) {

		moText extension = p_name;
		extension.Right(3);

		if(!stricmp(extension,"mp3") || !stricmp(extension,"mpg") || !stricmp(extension,"peg")) {
			return MO_SOUNDTYPE_MP3;
		} else if(!stricmp(extension,"wav")) {
			return MO_SOUNDTYPE_WAV;
		} else if(!stricmp(extension,"ogg")) {
			return MO_SOUNDTYPE_OGG;
		} else return MO_SOUNDTYPE_UNDEFINED;
}

moSound* moSoundManager::CreateSound( moText p_name, moSoundType p_type, moSoundParam p_param ) {

	MOuint idnuevo;
	moText namefull; //con path relativo al raiz
	moText extension;
	moSound* psound;

	psound = new moSound();
	if (psound) {
		idnuevo = m_sounds_array.Count();
		psound->Init( p_name, idnuevo, p_type, m_pResourceManager, p_param);
		return psound;
	} else return NULL;

}


MOboolean moSoundManager::Finish() {
	for(MOuint i=0; i<m_sounds_array.Count(); i++) {
		moSound* psound = m_sounds_array[i];
		if (psound) delete psound;
		psound = NULL;
	}
	m_sounds_array.Empty();

	for(MOuint i=0; i<m_effects_array.Count(); i++) {
		moSoundEffect* psoundeffect = m_effects_array[i];
		if (psoundeffect) delete psoundeffect;
		psoundeffect = NULL;
	}
	m_effects_array.Empty();

	#ifdef MO_WIN32
    #ifdef MO_USING_VC
    if (m_bInitialized) {
          ALFWShutdownOpenAL();
          ALFWShutdown();
    }
    #endif
	#endif
	return true;
}
