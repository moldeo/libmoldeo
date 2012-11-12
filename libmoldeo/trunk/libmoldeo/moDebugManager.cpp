/*******************************************************************************

                              moDebugManager.cpp

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

#include "moDebugManager.h"

//===========================================
//
//				moGUIManager
//
//===========================================


moDebugManager::moDebugManager() {

	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_DEBUG );

	SetName("debugmanager");
	SetLabelName("debugmanager");

}


moDebugManager::~moDebugManager() {


}

MOboolean
moDebugManager::Init() {
	return true;
}

MOboolean
moDebugManager::Finish() {
	return true;
}


void moDebugManager::Error( moText p_text ) {
    MODebug2->Error(p_text);
}

void moDebugManager::Message( moText p_text ) {

    MODebug2->Message( p_text );
}

void moDebugManager::Log( moText p_text ) {
    MODebug2->Log( p_text );
}

void moDebugManager::Push( moText p_text ) {
    MODebug2->Push( p_text );
}

/**
*   Saca y devuelve el primer texto ingresado a la lista
*   @return un mensaje de texto
*/
moText moDebugManager::Pop() {
    return MODebug2->Pop();
}

MOint  moDebugManager::Count() {
    return MODebug2->Count();
}


