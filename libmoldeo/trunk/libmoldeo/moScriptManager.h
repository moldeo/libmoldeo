/*******************************************************************************

                                moScriptManager.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                     *
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

#ifndef __MO_SCRIPT_MANAGER_H__
#define __MO_SCRIPT_MANAGER_H__

#include "moTypes.h"
#include "moAbstract.h"
#include "moText.h"
#include "moParam.h"
#include "moLunaClasses.h"

#include "moResourceManager.h"

/// Administrador de scripts (LUA)
/**
 * Esta clase implementa el administrador de scripts LUA. Inicializa la máquina virtual de LUA y el debugger
 * y registra las clases de Moldeo que serán accessibles desde los scripts de LUA.
 */
class LIBMOLDEO_API moScriptManager : public moResource
{
public:
    /**
     * El constructor por defecto.
     */
	moScriptManager();
    /**
     * El destructor por defecto.
     */
	~moScriptManager();

    /**
     * Inicializador del administrador.
	 * @return el resultado de la operación: true o false.
     */
	virtual MOboolean Init();

    /**
     * Finalizador del administrador.
	 * @return el resultado de la operación: true o false.
     */
	virtual MOboolean Finish();

    /**
     * Devuelve una referencia a la máquina virtual.
	 * @return referencia a la máquina virtual.
     */
    static moLuaVirtualMachine& GetVM(void);
    static void InitVM();
    /**
     * Devuelve una referencia al debugger.
	 * @return referencia al debugger.
     */
    static moLuaDebugger& GetDBG(void);


    moLuaResourceManager* PushLuaResourceManager( moResourceManager* p_pResourceManager = NULL );

private:

    moLuaResourceManager*  m_pLuaResourceManager;

   void RegisterLunaClasses();
};

#endif

