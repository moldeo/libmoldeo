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


*******************************************************************************/

#include "moIODeviceManager.h"

#include "moArray.h"
moDefineDynamicArray(moIODevicesArray)

moIODevice::moIODevice() {
	SetType(MO_OBJECT_IODEVICE);
}

moIODevice::~moIODevice() {

}

void moIODevice::SetValue( MOdevcode cd, MOint vl ) {
  MODebug2->Message(  " moIODevice::SetValue [NI] > cd: " + IntToStr(cd)
                    + " vl: " + IntToStr(vl) );
}

void moIODevice::SetValue( MOdevcode cd, MOfloat vl ) {
  MODebug2->Message(  " moIODevice::SetValue [NI] > cd: " + IntToStr(cd)
                    + " vl: " + FloatToStr(vl) );
}

void moIODevice::SetValue( MOdevcode cd , moData data ) {
  MODebug2->Message(  " moIODevice::SetValue [NI] > cd: " + IntToStr(cd)
                    + " data: " + data.ToText() );
}

void moIODevice::SetValue( MOdevcode cd, MOlong s, MOpointer pt ) {
  MODebug2->Message(  " moIODevice::SetValue [NI] > cd: " + IntToStr(cd)
                    + " s: " + IntToStr(s)
                    + " pointer: " + IntToStr((long)pt) );
}

moData
moIODevice::GetValue( MOdevcode cd, moDataType p_type ) {
  MODebug2->Message(  " moIODevice::GetValue [NI] > cd: " + IntToStr(cd)
                    + " p_type: " + IntToStr((int)p_type) );
	return moData();

}

MOint moIODevice::GetValue(MOdevcode devcode, MOint i) {
    MODebug2->Message(  " moIODevice::GetValue [NI] > devcode: " + IntToStr(devcode)
                    + " i: " + IntToStr((int)i) );
    return 0;
}

MOint moIODevice::GetNValues(MOdevcode devcode) {
    MODebug2->Message(  " moIODevice::GetNValues [NI] > devcode: " + IntToStr(devcode));
    return 0;
}

MOpointer moIODevice::GetPointer( MOdevcode devcode) {
  MODebug2->Message(  " moIODevice::GetPointer [NI] > devcode: " + IntToStr(devcode));
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
moIODeviceManager::NewIODevice( const moText& p_devname,  const moText& p_configname, const moText& p_labelname, const moText& p_keyname, moMoldeoObjectType type, int paramindex, int valueindex, bool p_activate  ) {

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
      moMobDefinition MDef = pdevice->GetMobDefinition();

	    MDef.SetConsoleParamIndex(paramindex);
	    MDef.SetConsoleValueIndex(valueindex);
      MDef.SetConfigName( p_configname );
      MDef.SetLabelName( p_labelname );
      MDef.SetKeyName( p_keyname );
      MDef.SetActivate(p_activate);

      pdevice->SetMobDefinition(MDef);
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

    pdevice = m_IODevices.GetRef(p_ID);
    if (pdevice) {
        moDeleteIODevice( pdevice, m_Plugins );
        m_IODevices.Remove(p_ID);
        return true;
    }

	return false;
}


void
moIODeviceManager::Update() {

  ///delete all events before poll new ones...
  ///that means Devices dont need to delete their events anymore

	PollEvents();

	for(MOuint i = 0; i < m_IODevices.Count(); i++) {
		moIODevice* piodevice = m_IODevices.GetRef(i);
		if(piodevice!=NULL) {
      if (piodevice->Activated())
        piodevice->Update(Events);
    }
	}


}

/**This function must be rewritten for direct access to OS events for each platform*/
void
moIODeviceManager::PollEvents() {

/*
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
*/
}

/**
ERASE ANY EVENT THAT IS NOT A MESSAGE

An event has no destination (BROADCAST): IODeviceManager must purge this events so they do not propagate indefinitly
A message has a destination (UNICAST / MULTICAST): IODeviceManager doesnt purge them,
*/
void
moIODeviceManager::PurgeEvents() {

	moEvent *actual=NULL,*tmp;
	///moMessage *pmessage;

	if (Events) actual = Events->First;

	///Procesamos los eventos recibidos de los MoldeoObject Outlets
	while(actual!=NULL) {
		tmp = actual->next;
		///procesamos aquellos Outlet q estan dirigidos a este objeto
		if (actual->reservedvalue3 != MO_MESSAGE
        && actual->reservedvalue3 != MO_DATAMESSAGE) {
						Events->Delete(actual);

		}
		actual = tmp;
	}
}

moEventList*
moIODeviceManager::GetEvents() {
	return Events;
}

MOdevcode
moIODeviceManager::GetCode(char *n) {
  MODebug2->Message( "moIODeviceManager::GetCode > n: " + moText(n) );
	return 0;
}

MOswitch
moIODeviceManager::GetStatus(MOdevcode a) {
  MODebug2->Message( "moIODeviceManager::GetStatus > MOdevcode a: " + IntToStr(a) );
	return 0;
}

MOswitch
moIODeviceManager::SetStatus(MOdevcode a,MOswitch b) {
  MODebug2->Message( "moIODeviceManager::GetStatus > MOdevcode a: " + IntToStr(a)
                    +" MOswitch b: " + IntToStr(b) );
	return 0;
}


MOboolean
moIODeviceManager::Finish() {
	if(Events!=NULL) delete Events;
	Events = NULL;
	return true;
}

