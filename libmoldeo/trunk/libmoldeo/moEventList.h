/*******************************************************************************

                                moEventList.h

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

#include "moTypes.h"
#include "moValue.h"
#include "moAbstract.h"
#include "moLock.h"

#ifndef __MOEVENTO_H
#define __MOEVENTO_H

struct LIBMOLDEO_API moEventStruct
{
	MOint valid;
	MOint deviceid;
	MOint devicecode;
	MOint reservedvalue0;
	MOint reservedvalue1;
	MOint reservedvalue2;
	MOint reservedvalue3;
};


class LIBMOLDEO_API moEvent {
public:

	moEvent *previous;
	moEvent *next;

	MOint deviceid; //esto vuela
	MOint devicecode;//esto puede q quede
	MOint reservedvalue0;
	MOint reservedvalue1;
	MOint reservedvalue2;
	MOint reservedvalue3;
	MOpointer pointer;

	moEvent();
	moEvent(MOint did, MOint cod, MOint val0 /**/= 0, MOint val1 /**/= 0, MOint val2 /**/= 0, MOint val3 /**/= 0, MOpointer ptr/**/=0);
	moEvent(MOint did, MOint cod, MOint val0, MOpointer ptr );
	virtual ~moEvent();

};

#define MO_MESSAGE 0xFABC05

class LIBMOLDEO_API moMessage : public moEvent {

	public:
        moMessage();
        moMessage(const moMessage&);
		moMessage( MOint m_MoldeoIdDest, MOint m_MoldeoIdSrc, const moData& data );
		moMessage( MOint m_MoldeoIdDest, MOint m_InletIdDest, MOint m_MoldeoIdSrc, const moData& data );
		moMessage( MOint p_MoldeoIdDest, MOint m_InletIdDest, moMoldeoObjectType p_TypeDest, MOint p_MoldeoIdSrc, moMoldeoObjectType p_TypeSrc, const moData& p_data );
		moMessage( MOint p_MoldeoIdDest, MOint m_InletIdDest, moMoldeoObjectType p_TypeDest, moText	p_NameDest, MOint p_MoldeoIdSrc, moMoldeoObjectType p_TypeSrc, moText p_NameSrc, const moData& p_data );
		virtual ~moMessage();

		moData					m_Data;

		MOint					m_MoldeoIdDest;
		MOint					m_InletIdDest;
		moMoldeoObjectType		m_TypeDest;
		moText					m_NameDest;

		MOint					m_MoldeoIdSrc;
		MOint					m_OutletIdSrc;
		moMoldeoObjectType		m_TypeSrc;
		moText					m_NameSrc;

};

class LIBMOLDEO_API moEventList : public moAbstract {

public:

	moLock	m_lock;

	moEvent *First;
	moEvent *Last;

	moEventList();
	virtual ~moEventList();


	MOboolean Init();
	void Add( moMessage* p_Message );
	void Add( moEvent* p_Event );
	void Add(MOint did, MOint cod, MOint val0 /**/= 0, MOint val1 /**/= 0, MOint val2 /**/= 0, MOint val3 /**/= 0, MOpointer ptr/**/=0);
	void Add(MOint did, MOint cod, MOint val0, MOpointer ptr );
	MOboolean Delete(moEvent *ev);
	MOboolean Delete(moMessage *ev);
	MOboolean Finish();

};

// moEventPacket class **************************************************

class LIBMOLDEO_API moEventPacket : public moAbstract
{
public:
	moEventPacket(float p_sendInterval, int p_maxEventNum);
    ~moEventPacket();

	void ClearPacket();
	bool AddEvent(moEvent *e);
	bool ReadyToSend();

	int GetNumEvents() { return num_events; }
	int GetPacketSize() { return num_events * sizeof(moEventStruct); }
	moEventStruct* GetPacket() { return buffer_events; }
	moEventStruct GetEvent(int i) { return buffer_events[i]; }
private:
	float sendInterval;
	int maxEventNum;

    float time0;

    moEventStruct* buffer_events;
	void* pointer;
	moEventStruct empty_event;
	int num_events;
	bool packet_full;
};
/*
template class LIBMOLDEO_API moDynamicArray<moEventPacket*>;
typedef moDynamicArray<moEventPacket*> moEventPacketArray;
*/
moDeclareExportedDynamicArray( moEventPacket*, moEventPacketArray )
#endif

