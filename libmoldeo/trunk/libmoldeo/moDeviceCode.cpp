/*******************************************************************************

                              moDeviceCode.cpp

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

#include "moDeviceCode.h"


//CODIGO DISPOSITIVO

moDeviceCode::moDeviceCode() {
	device = -1;
	devicecode = -1;
	value = 0;
	next = NULL;
	previous = NULL;
}

moDeviceCode::moDeviceCode(MOint did,MOint cod,MOint val) {
	device = did;
	devicecode = cod;
	value = val;
	next = NULL;
	previous = NULL;
}


///CODIGO DISPOSITIVO LISTA

moDeviceCodeList::moDeviceCodeList() {
	First = NULL;
	Last = NULL;
}

moDeviceCodeList::~moDeviceCodeList() {
	Finish();
}

void
moDeviceCodeList::Add(MOint did, MOint cod, MOint value) {
	if(First==NULL) {//lista vacia
		First = new moDeviceCode(did,cod,value);
		Last = First;
		First->previous = NULL;
		First->next = NULL;
	} else {//no vacia, lo ponemos al final
		Last->next = new moDeviceCode(did,cod,value);
		if(Last->next != NULL) {
			Last->next->previous	= Last;
			Last = Last->next;
		}
	}

}


//este code es el mismo que el de events
//fue revisado
MOboolean
moDeviceCodeList::Delete(moDeviceCode *ev) {
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
	//por si las dudas podes chequear que este bien muerto;
	if(ev == NULL) return true;
	return false;

}

MOboolean
moDeviceCodeList::Init() {
	First = NULL;
	Last = NULL;
	return true;
}

MOboolean
moDeviceCodeList::Finish() {

	while(First!=NULL) Delete(First);
	return true;
}

void
moDeviceCodeList::Copy(moDeviceCodeList *copy) {

	moDeviceCode *act;

	//borramos nuestro contenido, igualmente
	while(First!=NULL) Delete(First);

	act = copy->First;
	while(act!=NULL) {
		Add(act->device,act->devicecode,act->value);
		act = act->next;
	}

}
