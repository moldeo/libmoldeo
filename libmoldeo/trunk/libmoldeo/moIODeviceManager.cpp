/*******************************************************************************

                              moIODeviceManager.cpp

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

#include "moIODeviceManager.h"

#ifdef MO_WIN32
  #include <SDL.h>
#endif

#ifdef MO_LINUX
  #include <SDL/SDL.h>
#endif

#ifdef MO_MACOSX
  #include <SDL/SDL.h>
#endif

#include "moArray.h"
moDefineDynamicArray(moIODevicesArray)

moIODevice::moIODevice() {
	SetType(MO_OBJECT_IODEVICE);
}

moIODevice::~moIODevice() {

}

void moIODevice::SetValue( MOdevcode cd, MOint vl ) {

}

void moIODevice::SetValue( MOdevcode cd, MOfloat vl ) {

}

void moIODevice::SetValue( MOdevcode cd , moData data ) {

}

void moIODevice::SetValue( MOdevcode cd, MOlong s, MOpointer pt ) {

}

moData
moIODevice::GetValue( MOdevcode cd, moDataType p_type ) {

	return moData();

}

MOint moIODevice::GetValue(MOdevcode devcode, MOint i) {
    return 0;
}

MOint moIODevice::GetNValues(MOdevcode devcode) {
    return 0;
}

MOpointer moIODevice::GetPointer( MOdevcode) {

	return NULL;
}

//===================================
//
//		CONSOLA E/S ARRAY
//
//===================================

moIODeviceManager::moIODeviceManager() {
	Events = NULL;
	m_IODevices.Init(0,NULL);
}

moIODeviceManager::~moIODeviceManager() {
	Finish();
}


MOboolean
moIODeviceManager::Init() {
	Events = new moEventList();
	m_Plugins.Init(0,NULL);
	return true;
}


moIODevice*
moIODeviceManager::NewIODevice( moText p_devname, moText p_cfname, moText p_labelname, moMoldeoObjectType type, int paramindex, int valueindex  ) {

	moIODevice* pdevice = NULL;

	switch (type) {
		case MO_OBJECT_IODEVICE:
			pdevice = moNewIODevice( p_devname, m_Plugins );
			if (pdevice) m_IODevices.Add( pdevice );
			break;
		default:
			break;
	}

	if (pdevice) {
	    pdevice->GetMobDefinition().GetMobIndex().SetParamIndex(paramindex);
	    pdevice->GetMobDefinition().GetMobIndex().SetValueIndex(valueindex);
		pdevice->SetConfigName( p_cfname );
		pdevice->SetLabelName( p_labelname );
	}

	return pdevice;
}

moIODevicesArray&
moIODeviceManager::IODevices() {

	return m_IODevices;
}

MOboolean
moIODeviceManager::RemoveIODevice( MOint p_ID ) {


	moIODevice* pdevice = NULL;

    pdevice = m_IODevices.Get(p_ID);
    if (pdevice) {
        moDeleteIODevice( pdevice, m_Plugins );
        m_IODevices.Remove(p_ID);
        return true;
    }

	return false;
}


void
moIODeviceManager::Update() {

	PollEvents();

	for(MOuint i = 0; i < m_IODevices.Count(); i++) {
		moIODevice* piodevice = m_IODevices.Get(i);
		if(piodevice!=NULL) piodevice->Update(Events);
	}
}

/*This function must be rewritten for direct access to OS events for each platform*/
void
moIODeviceManager::PollEvents() {


	SDL_Event event;

	   //SDL_KEYDOWN,			// Keys pressed
       //SDL_KEYUP,				// Keys released
       //SDL_MOUSEMOTION,		// Mouse moved
       //SDL_MOUSEBUTTONDOWN,	// Mouse button pressed
       //SDL_MOUSEBUTTONUP,		// Mouse button released

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
				case SDL_MOUSEMOTION:
					Events->Add(MO_IODEVICE_MOUSE,SDL_MOUSEMOTION, event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y);
					break;
				case SDL_MOUSEBUTTONDOWN:
					Events->Add(MO_IODEVICE_MOUSE,SDL_MOUSEBUTTONDOWN, event.button.button, event.button.x, event.button.y);
					break;
				case SDL_MOUSEBUTTONUP:
					Events->Add(MO_IODEVICE_MOUSE,SDL_MOUSEBUTTONUP, event.button.button, event.button.x, event.button.y);
					break;
				case SDL_KEYDOWN:
					// Ignore ALT-TAB for windows
					if((event.key.keysym.sym == SDLK_LALT) ||
						(event.key.keysym.sym == SDLK_RALT) ||
						(event.key.keysym.sym == SDLK_TAB)) {
						break;
					}
					Events->Add(MO_IODEVICE_KEYBOARD,SDL_KEYDOWN, event.key.keysym.sym, event.key.keysym.mod,0,0);
					break;

				case SDL_KEYUP:
					if((event.key.keysym.sym == SDLK_LALT) ||
						(event.key.keysym.sym == SDLK_RALT) ||
						(event.key.keysym.sym == SDLK_TAB)) {
						break;
					}
					Events->Add(MO_IODEVICE_KEYBOARD,SDL_KEYUP, event.key.keysym.sym, event.key.keysym.mod,0,0);
					break;
				default:
					break;
		}
	}

}

moEventList*
moIODeviceManager::GetEvents() {
	return Events;
}

MOdevcode
moIODeviceManager::GetCode(char *n) {
//as	asasd
	return 0;
}

MOswitch
moIODeviceManager::GetStatus(MOdevcode a) {
//asdasd
	return 0;
}

MOswitch
moIODeviceManager::SetStatus(MOdevcode a,MOswitch b) {
//sdsdadf
	return 0;
}


MOboolean
moIODeviceManager::Finish() {
	if(Events!=NULL) delete Events;
	Events = NULL;
	return true;
}
