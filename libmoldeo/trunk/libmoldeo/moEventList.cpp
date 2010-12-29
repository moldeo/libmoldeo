/*******************************************************************************

                              moEventList.cpp

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

#include "moEventList.h"

#include "moArray.cpp"
moDefineDynamicArray( moEventPacketArray );

//MESSAGE
moMessage::moMessage() {
}

moMessage::moMessage( MOint p_MoldeoIdDest, MOint p_MoldeoIdSrc, const moData& data ) {

	deviceid = p_MoldeoIdDest;
	devicecode = 0;
	reservedvalue3 = MO_MESSAGE;

	m_MoldeoIdDest = p_MoldeoIdDest;
	m_MoldeoIdSrc = p_MoldeoIdSrc;

	m_Data = data;

}

moMessage::moMessage( MOint p_MoldeoIdDest, MOint p_InletIdDest, MOint p_MoldeoIdSrc, const moData& data ) {

	deviceid = p_MoldeoIdDest;
	devicecode = 0;
	reservedvalue3 = MO_MESSAGE;

	m_MoldeoIdDest = p_MoldeoIdDest;
	m_InletIdDest = p_InletIdDest;
	m_MoldeoIdSrc = p_MoldeoIdSrc;

	m_Data = data;

}

moMessage::moMessage( MOint p_MoldeoIdDest, MOint p_InletIdDest, moMoldeoObjectType p_TypeDest, MOint p_MoldeoIdSrc, moMoldeoObjectType p_TypeSrc, const moData& p_data ) {
	deviceid = p_MoldeoIdDest;
	devicecode = 0;
	reservedvalue3 = MO_MESSAGE;

	m_MoldeoIdDest = p_MoldeoIdDest;
	m_InletIdDest = p_InletIdDest;
	m_TypeDest = p_TypeDest;

	m_MoldeoIdSrc = p_MoldeoIdSrc;
	m_TypeSrc = p_TypeSrc;

	m_Data = p_data;
}

moMessage::moMessage( MOint p_MoldeoIdDest, MOint p_InletIdDest, moMoldeoObjectType p_TypeDest, moText	p_NameDest, MOint p_MoldeoIdSrc, moMoldeoObjectType p_TypeSrc, moText p_NameSrc, const moData& p_data ) {
	deviceid = p_MoldeoIdDest;
	devicecode = 0;
	reservedvalue3 = MO_MESSAGE;

	m_MoldeoIdDest = p_MoldeoIdDest;
	m_TypeDest = p_TypeDest;
	m_InletIdDest = p_InletIdDest;
	m_NameDest = p_NameDest;

	m_MoldeoIdSrc = p_MoldeoIdSrc;
	m_TypeSrc = p_TypeSrc;
	m_NameSrc = p_NameSrc;

	m_Data = p_data;
}

moMessage::~moMessage() {
//skip
}

//EVENTO

moEvent::moEvent() {
	previous = NULL;
	next = NULL;
	deviceid = -1;//indefinido
	devicecode = 0;
	reservedvalue0 = 0;
	reservedvalue1 = 0;
	reservedvalue2 = 0;
	reservedvalue3 = 0;
}

moEvent::moEvent(MOint did, MOint cod, MOint val0, MOint val1, MOint val2, MOint val3, MOpointer ptr) {
		deviceid = did;
		devicecode = cod;
		reservedvalue0 = val0;
		reservedvalue1 = val1;
		reservedvalue2 = val2;
		reservedvalue3 = val3;
		pointer = ptr;

		next = NULL;
		previous = NULL;
}

moEvent::moEvent(MOint did, MOint cod, MOint val0, MOpointer ptr ) {
		deviceid = did;
		devicecode = cod;
		reservedvalue0 = val0;
		reservedvalue1 = reservedvalue2 = reservedvalue3 = 0;
		pointer = ptr;
}

moEvent::~moEvent() {
//skip
}

//EVENTOS

moEventList::moEventList() {
	First = NULL;
	Last = NULL;
}

moEventList::~moEventList() {
	Finish();
}

/*
void First() {
	Current = First;
}

void Last() {
	Current = Last;
}

void Next() {
	if(Current!=NULL) Current = Current->next;
}

void Anterior() {
	if(Current!=NULL) Current = Current->previous;
}

moEvent* Current() {
	return  Current;
}

moEvent* DeleteCurrent() {
	if(First)
}
*/
void moEventList::Add( moMessage* p_Message ) {

	m_lock.Lock();
	moEvent* p_Event = (moEvent*)p_Message;
	if(First==NULL) {//lista vacia
		First = p_Event;
		Last = First;
		First->previous = NULL;
		First->next = NULL;
	} else {//no vacia, lo ponemos al final
		Last->next = p_Event;
		if(Last->next != NULL) {
			Last->next->previous	= Last;
			Last->next->next = NULL;
			Last = Last->next;
		}
	}
	m_lock.Unlock();

}


void moEventList::Add( moEvent* p_Event ) {

	m_lock.Lock();
	//MODebug2->Message( " Event List :: added event " );
	if(First==NULL) {//lista vacia
		First = p_Event;
		Last = First;
		First->previous = NULL;
		First->next = NULL;
	} else {//no vacia, lo ponemos al final
		Last->next = p_Event;
		if(Last->next != NULL) {
			Last->next->previous	= Last;
			Last->next->next = NULL;
			Last = Last->next;
		}
	}
	m_lock.Unlock();

}

//revisado
void
moEventList::Add(MOint did, MOint cod, MOint val0, MOint val1, MOint val2, MOint val3, MOpointer ptr) {
	m_lock.Lock();
	if(First==NULL) {//lista vacia
		First = new moEvent(did,cod,val0,val1,val2,val3,ptr);
		Last = First;
		First->previous = NULL;
		First->next = NULL;
	} else {//no vacia, lo ponemos al final
		Last->next = new moEvent(did,cod,val0,val1,val2,val3,ptr);
		if(Last->next != NULL) {
			Last->next->previous	= Last;
			Last->next->next = NULL;
			Last = Last->next;
		}
	}
	m_lock.Unlock();
}

void
moEventList::Add(MOint did, MOint cod, MOint val0, MOpointer ptr ) {
	m_lock.Lock();

	if(First==NULL) {//lista vacia
		First = new moEvent(did,cod,val0,ptr);
		Last = First;
		First->previous = NULL;
		First->next = NULL;
	} else {//no vacia, lo ponemos al final
		Last->next = new moEvent(did,cod,val0,ptr);
		if(Last->next != NULL) {
			Last->next->previous	= Last;
			Last->next->next = NULL;
			Last = Last->next;
		}
	}
	m_lock.Unlock();
}



//recontra revisado!!! anda
MOboolean
moEventList::Delete(moEvent *ev) {

	m_lock.Lock();

	if(ev==First) {
		if(ev==Last) {
			First = NULL;
			Last = NULL;
		} else {
			First->next->previous = NULL;
			First = First->next;
		}
	} else if(ev==Last) {
		//el caso un solo nodo ya fue visto arriba
		Last->previous->next = NULL;
		Last = Last->previous;
	} else {
		ev->previous->next = ev->next;
		ev->next->previous = ev->previous;

	}
	//finalmente, lo borro al maldito
	delete ev;
	ev = NULL;

	m_lock.Unlock();
	//por si las dudas podes chequear que este bien muerto;
	return(ev == NULL);
}

MOboolean
moEventList::Delete(moMessage *ev) {

	m_lock.Lock();

	if(ev==First) {
		if(ev==Last) {
			First = NULL;
			Last = NULL;
		} else {
			First->next->previous = NULL;
			First = First->next;
		}
	} else if(ev==Last) {
		//el caso un solo nodo ya fue visto arriba
		Last->previous->next = NULL;
		Last = Last->previous;
	} else {
		ev->previous->next = ev->next;
		ev->next->previous = ev->previous;

	}
	//finalmente, lo borro al maldito
	delete ev;
	ev = NULL;

	m_lock.Unlock();
	//por si las dudas podes chequear que este bien muerto;
	return(ev == NULL);
}

MOboolean
moEventList::Init() {
	m_lock.Lock();
	First = NULL;
	Last = NULL;
	m_lock.Unlock();
	return true;
}

MOboolean
moEventList::Finish() {
	m_lock.Lock();
	while(First!=NULL) Delete(First);
	m_lock.Unlock();
	return true;
}



// moEventPacket class **************************************************

moEventPacket::moEventPacket(float p_sendInterval, int p_maxEventNum)
{
	sendInterval = p_sendInterval;
	maxEventNum = p_maxEventNum;

	buffer_events = new moEventStruct [maxEventNum];
	pointer = (void*)buffer_events;
	memset((void*)&empty_event,0,sizeof(moEventStruct));
	num_events = 0;

	ClearPacket();
}

moEventPacket::~moEventPacket()
{
	if (buffer_events != NULL)
	{
        delete[] buffer_events;
	    buffer_events = NULL;
	}
}

void moEventPacket::ClearPacket()
{
	for(int i=0;i<maxEventNum;i++) buffer_events[i] = empty_event;

	num_events = 0;
	packet_full = false;

    time0 = moGetTicks() / 1000.0;
}

bool moEventPacket::AddEvent(moEvent *e)
{
	moEventStruct new_event;

	new_event.valid = 1;
	new_event.devicecode = e->devicecode;
	new_event.deviceid = e->deviceid;
	new_event.reservedvalue0 = e->reservedvalue0;
    new_event.reservedvalue1 = e->reservedvalue1;
	new_event.reservedvalue2 = e->reservedvalue2;
	new_event.reservedvalue3 = e->reservedvalue3;

    if ( num_events < maxEventNum )
	{
		buffer_events[num_events] = new_event;
		num_events++;
	}
	else
	{
		packet_full = true;
	}

	return !packet_full;
}

bool moEventPacket::ReadyToSend()
{
	float time1 = moGetTicks() / 1000.0;
	return (sendInterval <= time1 - time0) || packet_full;
}
