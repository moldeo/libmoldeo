/*******************************************************************************

								moScript.h

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
  Base class for Lua-scriptable classes. Based in the Lua article and
  VC project by Richard Shephard, available online at:
  http://www.codeproject.com/cpp/luaincpp.asp?msg=1741798

*******************************************************************************/

#ifndef __MO_SCRIPT_H__
#define __MO_SCRIPT_H__

#include <moLuaBase.h>

/**
 * Esta clase implementa la funcionalidad necesaria para crear clases scripteables con LUA.
 * Una clase que descienda de moScript puede ser asociada a un script de LUA, y desde el mismo
 * pueden ser llamados mtodos de la clase (siempre y cuando sean registrados en la mquina virtual).
 */
class LIBMOLDEO_API moScript
{
	public:
		/**
		 * El constructor por defecto.
		 */
		moScript ();

		/**
		 * El destructor por defecto.
		 */
		virtual ~ moScript (void);

		/**
		 * Inicializa el script con la mquina virtual .
		 */
		void InitScript ();

		/**
		 * Finaliza el script.
		 */
		void FinishScript ();
		bool IsInitialized ()
		{
			return m_initialized;
		}

		/**
		 * Compila el archivo strFilename.
		 * @param strFilename nombre del archivo de script.
		 * @return el resultado de la operacin: true o false.
		 */
		bool CompileFile (const char *strFilename);

		/**
		 * Compila el bfer pbBuffer.
		 * @param pbBuffer bfer a compilar.
		 * @param szLen tamao del bfer.
		 * @return el resultado de la operacin: true o false.
		 */
		bool CompileBuffer (unsigned char *pbBuffer, size_t szLen);

		/**
		 * Registra una funcin (mtodo) de la clase en la mquina virtual de LUA.
		 * @param strFuncName nombre de la funcin.
		 * @return regresa el ndice de la funcin den la tabla de LUA, -1 si la operacin no es exitosa.
		 */
		int RegisterFunction (const char *strFuncName);

		/**
		 * Registra la primera funcin (mtodo) de la clase en la mquina virtual de LUA.
		 * Se debe llamar primero esta funcin para registrar los mtodos dentro e la instancia de una clase
		 * Adems esta funcin guarda un registro de la cantidad de veces que se llamo a RegisterFunctions
		 * para heredar los mtodos de las clases de las que se deriva el objeto
		 * @param strFuncName nombre de la funcin.
		 * @return regresa el ndice de la funcin den la tabla de LUA, -1 si la operacin no es exitosa.
		 */
		int RegisterBaseFunction (const char *strFuncName);

		/**
		 * Selecciona una funcin del script LUA para ejecutar con RunSelectedFunction.
		 * @param strFuncName nombre de la funcin a seleccionar.
		 * @return el resultado de la operacin: true o false.
		 * @see RunSelectedFunction
		 */
		bool SelectScriptFunction (const char *strFuncName);

		/**
		 * Agrega un parmetro entero a la lista de parmetros para pasar a la funcin del script seleccionada actualmente.
		 * @param iInt valor del parmetro entero.
		 */
		void AddFunctionParam (int iInt);

		/**
		 * Agrega un parmetro flotante a la lista de parmetros para pasar a la funcin del script seleccionada actualmente.
		 * @param fFloat valor del parmetro flotante.
		 */
		void AddFunctionParam (float fFloat);

		/**
		 * Agrega un parmetro de texto a la lista de parmetros para pasar a la funcin del script seleccionada actualmente.
		 * @param string valor del parmetro de texto.
		 */
		void AddFunctionParam (char *string);

		/**
		 * Ejecuta la funcin del script LUA seleccionada con SelectScriptFunction.
		 * @param nReturns nmero esperado de valores a devolver por la funcin.
		 * @return el resultado de la operacin: true o false.
		 * @see SelectScriptFunction
		 */
		bool RunSelectedFunction (int nReturns = 0);

		/**
		 * Revisa que el script tenga la funcin especificada.
		 * @param strFuncName nombre de la funcin.
		 * @return true si la funcin existe, false en caso contrario.
		 */
		bool ScriptHasFunction (const char *strScriptName);

		/**
		 * Devuelve cuantos mtodos de la clase han sido registrados en la mquina virtual con RegisterFunction.
		 * @return nmero de mtodos.
		 * @see RegisterFunction
		 */
		int methods (void)
		{
			return m_nMethods;
		}

		/**
		 * Esta funcin es llamada cuando en el LUA script un mtodo registrado de la clase es ejecutado.
		 * @param vm referencia a la mquina virtual.
		 * @param iFunctionNumber ndice del mtodo siendo llamado.
		 * @return nmero valores que el mtodo de la clase agrega a la pila de LUA. Estos valores son los que en
		 * el script son interpretados como los valores que retorna el mtodo. Pueden ser ms de 1.
		 */
		virtual int ScriptCalling (moLuaVirtualMachine & vm, int iFunctionNumber) = 0;

		/**
		 * Esta funcin es llamada cuando se quiere reiniciar el indice iterativo de mtodos una vez encontrada la funcin
		 * para que de esta manera la siguiente bsqueda
		 * @param vm referencia a la mquina virtual.
		 * @param iFunctionNumber ndice del mtodo siendo llamado.
		 * @return nmero valores que el mtodo de la clase agrega a la pila de LUA. Estos valores son los que en
		 * el script son interpretados como los valores que retorna el mtodo. Pueden ser ms de 1.
		 */
		virtual int ResetScriptCalling ()
		{
			m_iMethodBaseIterator = m_iMethodBaseAncestors - 1;
			m_iMethodBase = m_MethodBases[m_iMethodBaseIterator--];
		} virtual int NextScriptCalling ()
		{
			if (m_iMethodBaseIterator >= 0)
				m_iMethodBase = m_MethodBases[m_iMethodBaseIterator--];
			return m_iMethodBase;
		}

		/**
		 * Esta funcin es llamada cuando termina la ejecucin de una funcin del script, de manera que
		 * da la oportunidad de utilizar los valores devueltos por dicha funcin.
		 * @param vm referencia a la mquina virtual.
		 * @param strFunc nombre de la funcin en el script.
		 */
		virtual void HandleReturns (moLuaVirtualMachine & vm, const char *strFunc) =
			0;

		/**
		 * Devuelve una referencia a la mquina virtual utilizada por este script.
		 * @return referencia a la mquina virtual.
		 */
		moLuaVirtualMachine & vm (void)
		{
			return m_vm;
		}
	protected:bool m_initialized;
	int m_nMethods;
	static moLuaVirtualMachine m_vm;
	static moLuaDebugger m_dbg;
	int m_iThisRef;
	int m_nArgs;
	const char *m_strFunctionName;

	// For indexing scripted methods.
	int m_iMethodBase;			 ///actual class index method base
	int m_iMethodBaseIterator;	 ///actual class iterator index method base
	//and manage inheritance indexed methods
	int m_iMethodBaseAncestors;	 ///increment on each call to RegisterFunctions
	int m_MethodBases[256];		 /// record the iMethodBase indivually for each ancestor on RegisterFunctions
};
#endif							 /*  */
