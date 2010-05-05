/*******************************************************************************

							   moLuaBase.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *   (at your option) any later version.                                    *
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

  Copyright (C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa
  Andrs Colubri

  Description:
  Classes for Lua scripting. Based in the Lua article and VC project by Richard
  Shephard, available online at:
  http://www.codeproject.com/cpp/luaincpp.asp?msg=1741798

*******************************************************************************/

#ifndef __MO_LUA_BASE_H__
#define __MO_LUA_BASE_H__

#include <moTypes.h>
#include <moText.h>

// LUA includes.
extern "C"
{

	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
} enum
{
	DBG_MASK_CALL = LUA_MASKCALL, DBG_MASK_RET = LUA_MASKRET, DBG_MASK_LINE =
	LUA_MASKLINE, DBG_MASK_COUNT = LUA_MASKCOUNT
};
class moLuaDebugger;

/**
 * Clase que encapsula una implementacin de la mquina virtual de LUA.
 */
class LIBMOLDEO_API moLuaVirtualMachine
{
	public:
		/**
		 * El constructor por defecto.
		 */
		moLuaVirtualMachine (void);

		/**
		 * El destructor por defecto.
		 */
		virtual ~ moLuaVirtualMachine (void);

		/**
		 * Inicializador: carga las libreras de LUA.
		 * @return el resultado de la operacin: true o false.
		 */
		bool InitialiseVM (void);

		/**
		 * Finalizador: limpia el estado de LUA.
		 * @return el resultado de la operacin: true o false.
		 */
		bool FinaliseVM (void);

		/**
		 * Compila y ejectua un archivo de LUA script.
		 * @param strFilename nombre del archivo de script.
		 * @return el resultado de la operacin: true o false.
		 */
		bool RunFile (const char *strFilename);

		/**
		 * Compila y ejecuta un bfer de datos precompilados.
		 * @param pbBuffer puntero al bfer de datos.
		 * @param szLen tamao del bfer.
		 * @param strName nombre del bfer.
		 * @return el resultado de la operacin: true o false.
		 */
		bool RunBuffer (const unsigned char *pbBuffer, size_t szLen,
			const char *strName = NULL);

		/**
		 * Llama a una funcin ya existente en la pila (stack).
		 * @param nArgs nmero de argumentos (ya existentes en la pila).
		 * @param nReturns nmero esperado de valores a devolver por la funcin (sern agregados a la pila).
		 * @return el resultado de la operacin: true o false.
		 */
		bool CallFunction (int nArgs, int nReturns = 0);

		/**
		 * Devuelve el estado de la pila de LUA.
		 * @return puntero al objecto de estado.
		 */
		operator   lua_State * (void)
		{
			return m_pState;
		}

		/**
		 * Es llamada cuando ocurre un error.
		 * @param lua puntero al estado de LUA.
		 */
		static void Panic (lua_State * lua);

		/**
		 * Revisa que el estado mquina virtual sea correcto y pueda seguir siendo utilizada.
		 * @return true si el estado de la mquina virtual es correcto, false en caso contrario.
		 */
		virtual bool Ok (void)
		{
			return m_fIsOk;
		}

		/**
		 * Asocia un debugger a la mquina virtual. El debugger maneja los mensajes de error de LUA.
		 * @param dbg puntero al debugger.
		 */
		void AttachDebugger (moLuaDebugger * dbg)
		{
			m_pDbg = dbg;
		} protected:lua_State * m_pState;
		bool m_fIsOk;
		moLuaDebugger * m_pDbg;
};

/**
 * Clase que encapsula una implementacin de un debugger para la mquina virtual de LUA.
 */
class LIBMOLDEO_API moLuaDebugger
{
	public:
		/**
		 * Constructor del debugger.
		 * @param vm referencia a la mquina virtual.
		 */
		moLuaDebugger (moLuaVirtualMachine & vm);

		/**
		 * Destructor por defecto.
		 */
		virtual ~ moLuaDebugger (void);

		/**
		 * Inicializador.
		 * @param moTextHeap heap de texto para almacenar los mensajes de error/debug.
		 */
		void InitaliseDBG (moTextHeap * pDebug);

		/**
		 * Finalizador.
		 */
		void FinaliseDBG (void);

		/**
		 * Establece los hooks.
		 * @param iMask mscara.
		 */
		void SetHooksFlag (int iMask);

		/**
		 * Establece el nmero de mscaras.
		 * @param iCount nmero de mscaras.
		 */
		void SetCount (int iCount)
		{
			m_iCountMask = iCount;
		}
		/**
		 * Agrega un nuevo mensaje de error al heap.
		 * @param iErrorCode cdigo del error.
		 */
		void ErrorRun (int iErrorCode);
	protected:int m_iCountMask;
	moLuaVirtualMachine & m_vm;
	moTextHeap * MODebug;
};

/**
 * Clase que restaura la pila de LUA a su estado original.
 */

/// restaurador del stack de LUA
/**
 * Comentar.
 */
class LIBMOLDEO_API moLuaRestoreStack
{
	public:
		/**
		 * Constructor.
		 * @param vm referencia a la mquina virtual.
		 */
		moLuaRestoreStack (moLuaVirtualMachine & vm):m_pState (NULL)
		{
			m_pState = (lua_State *) vm;
			if (vm.Ok ())

			{
				m_iTop = lua_gettop (m_pState);
			}
		}

		/**
		 * Destructor por defecto.
		 */
		virtual ~ moLuaRestoreStack (void)
		{
			lua_settop (m_pState, m_iTop);
		} protected:lua_State * m_pState;
		int m_iTop;
};

/**
 * Clase que establece la tabla global "this" utilizada por los scripts.
 */
class LIBMOLDEO_API moLuaThis
{
	public:
		/**
		 * Constructor.
		 * @param vm referencia a la mquina virtual.
		 * @param iRef nmero de referencia.
		 */
		moLuaThis (moLuaVirtualMachine & vm, int iRef):m_iOldRef (0), m_vm (vm)
		{
			lua_State * state = (lua_State *) vm;
			if (vm.Ok ())

			{

				// Save the old "this" table
				lua_getglobal (state, "this");
				m_iOldRef = luaL_ref (state, LUA_REGISTRYINDEX);

				// replace it with our new one
				lua_rawgeti (state, LUA_REGISTRYINDEX, iRef);
				lua_setglobal (state, "this");
			}
		}

		/**
		 * Destructor por defecto.
		 */
		virtual ~ moLuaThis (void)
		{
			lua_State * state = (lua_State *) m_vm;
			if (m_iOldRef > 0 && m_vm.Ok ())

			{

				// Replace the old "this" table
				lua_rawgeti (state, LUA_REGISTRYINDEX, m_iOldRef);
				lua_setglobal (state, "this");
				luaL_unref (state, LUA_REGISTRYINDEX, m_iOldRef);
			}
		}
	protected:int m_iOldRef;
	moLuaVirtualMachine & m_vm;
};
#endif							 /*  */
