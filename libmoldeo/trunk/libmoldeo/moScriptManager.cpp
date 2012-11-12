/*******************************************************************************

                              moScriptManager.cpp

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

#include "moScriptManager.h"

moScriptManager::moScriptManager()
{
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_SCRIPT );

	SetName("Script Manager");
	SetLabelName("Script Manager");
}

moScriptManager::~moScriptManager()
{
	Finish();
}

MOboolean moScriptManager::Init()
{

  moScriptManager::InitVM();
  RegisterLunaClasses();
	return true;
}

void moScriptManager::InitVM() {
    if (!m_vm.Ok()) {
      moScript::m_vm.InitialiseVM();
      moScript::m_dbg.InitaliseDBG();
    }
}


MOboolean moScriptManager::Finish()
{
	m_vm.FinaliseVM();
	m_dbg.FinaliseDBG();
	return true;
}

moLuaVirtualMachine& moScriptManager::GetVM(void)
{
	return moScript::m_vm;
}

moLuaDebugger& moScriptManager::GetDBG(void)
{
	return m_dbg;
}

void moScriptManager::RegisterLunaClasses()
{
	lua_State *state = (lua_State *) m_vm;

	REGISTER_CLASS(moLuaResourceManager, state);
	//REGISTER_CLASS(moLuaMath, state);
	//REGISTER_CLASS(moLuaParserFunction, state);
	REGISTER_CLASS(moLuaP5, state);

  m_pLuaResourceManager = new moLuaResourceManager( state );
  m_pLuaResourceManager->Set( GetResourceManager() );
}

moLuaResourceManager* moScriptManager::PushLuaResourceManager( moResourceManager* p_pResourceManager ) {

  lua_State *state = (lua_State *) m_vm;

  if (p_pResourceManager) {
    m_pLuaResourceManager->Set( p_pResourceManager );
  }

  if (m_pLuaResourceManager) {
    moLuna<moLuaResourceManager>::createFromExisting( state, m_pLuaResourceManager );
  }
  return m_pLuaResourceManager;
}

