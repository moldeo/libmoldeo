/*******************************************************************************

                              moDataManager.cpp

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

#include "moDataManager.h"

//===========================================
//
//				moDataSessionConfig
//
//===========================================

moDataSessionConfig::moDataSessionConfig() {
	m_DataPath = "";
	m_ConsoleConfigName = "";
}

moDataSessionConfig::moDataSessionConfig( moText p_datapath, moText p_consoleconfig ) {
	m_DataPath = p_datapath;
	m_ConsoleConfigName = p_consoleconfig;
}

moDataSessionConfig::~moDataSessionConfig() {
}

moText
moDataSessionConfig::GetDataPath() {
	return m_DataPath;
}

moText
moDataSessionConfig::GetConsoleConfigName() {
	return m_ConsoleConfigName;
}

//===========================================
//
//				moDataManager
//
//===========================================


moDataManager::moDataManager() {
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_DATA );

	SetName("Data Manager");

	m_pDataSession = NULL;
	m_DataSessionIndex = -1;
}

moDataManager::~moDataManager() {

}

MOboolean moDataManager::Init() {
	m_pDataSession = new moDataSessionConfig( moText("data"), moText("data/console.mol") );
	return true;
}

MOboolean moDataManager::Init( moText p_datapath, moText p_consoleconfig ) {

	m_pDataSession = new moDataSessionConfig( p_datapath, p_consoleconfig );
	m_DataSessionIndex = 0;
	return true;
}

moText
moDataManager::GetDataPath() {
	//m_DataSessionIndex
	if (m_pDataSession)
		return m_pDataSession->GetDataPath();
	return moText("");
}


moText
moDataManager::GetConsoleConfigName() {
	//m_DataSessionIndex
	if (m_pDataSession)
		return m_pDataSession->GetConsoleConfigName();
	return moText("");
}


MOboolean moDataManager::Finish() {
	return true;
}

