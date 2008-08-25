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
  Andrés Colubri

  Description:
  Base class for Lua-scriptable classes. Based in the Lua article and
  VC project by Richard Shephard, available online at:
  http://www.codeproject.com/cpp/luaincpp.asp?msg=1741798

*******************************************************************************/

#ifndef __MO_SCRIPT_H__
#define __MO_SCRIPT_H__

#include "moLuaBase.h"

/**
 * Esta clase implementa la funcionalidad necesaria para crear clases scripteables con LUA.
 * Una clase que descienda de moScript puede ser asociada a un script de LUA, y desde el mismo
 * pueden ser llamados métodos de la clase (siempre y cuando sean registrados en la máquina virtual).
 */

class LIBMOLDEO_API moScript
{
public:
    /**
     * El constructor por defecto.
     */
    moScript();
    /**
     * El destructor por defecto.
     */
    virtual ~moScript (void);

    /**
     * Inicializa el script con la máquina virtual .
     */
    void InitScript();
    /**
     * Finaliza el script.
     */
    void FinishScript();

    /**
     * Compila el archivo strFilename.
	 * @param strFilename nombre del archivo de script.
	 * @return el resultado de la operación: true o false.
     */
    bool CompileFile(const char *strFilename);
    /**
     * Compila el búfer pbBuffer.
	 * @param pbBuffer búfer a compilar.
	 * @param szLen tamaño del búfer.
	 * @return el resultado de la operación: true o false.
     */
    bool CompileBuffer(unsigned char *pbBuffer, size_t szLen);

    /**
     * Registra una función (método) de la clase en la máquina virtual de LUA.
	 * @param strFuncName nombre de la función.
	 * @return regresa el índice de la función den la tabla de LUA, -1 si la operación no es exitosa.
     */
    int RegisterFunction(const char *strFuncName);

    /**
     * Selecciona una función del script LUA para ejecutar con RunSelectedFunction.
	 * @param strFuncName nombre de la función a seleccionar.
	 * @return el resultado de la operación: true o false.
	 * @see RunSelectedFunction
     */
    bool SelectScriptFunction(const char *strFuncName);
    /**
     * Agrega un parámetro entero a la lista de parámetros para pasar a la función del script seleccionada actualmente.
	 * @param iInt valor del parámetro entero.
     */
    void AddFunctionParam(int iInt);
    /**
     * Agrega un parámetro flotante a la lista de parámetros para pasar a la función del script seleccionada actualmente.
	 * @param fFloat valor del parámetro flotante.
     */
    void AddFunctionParam(float fFloat);
    /**
     * Agrega un parámetro de texto a la lista de parámetros para pasar a la función del script seleccionada actualmente.
	 * @param string valor del parámetro de texto.
     */
    void AddFunctionParam(char *string);

    /**
     * Ejecuta la función del script LUA seleccionada con SelectScriptFunction.
	 * @param nReturns número esperado de valores a devolver por la función.
	 * @return el resultado de la operación: true o false.
	 * @see SelectScriptFunction
     */
    bool RunSelectedFunction(int nReturns = 0);

    /**
     * Revisa que el script tenga la función especificada.
	 * @param strFuncName nombre de la función.
	 * @return true si la función existe, false en caso contrario.
     */
   bool ScriptHasFunction(const char *strScriptName);

    /**
     * Devuelve cuantos métodos de la clase han sido registrados en la máquina virtual con RegisterFunction.
	 * @return número de métodos.
	 * @see RegisterFunction
     */
   int methods(void) { return m_nMethods; }

    /**
     * Esta función es llamada cuando en el LUA script un método registrado de la clase es ejecutado.
	 * @param vm referencia a la máquina virtual.
	 * @param iFunctionNumber índice del método siendo llamado.
	 * @return número valores que el método de la clase agrega a la pila de LUA. Estos valores son los que en
	 * el script son interpretados como los valores que retorna el método. Pueden ser más de 1.
     */
   virtual int ScriptCalling(moLuaVirtualMachine& vm, int iFunctionNumber) = 0;

    /**
     * Esta función es llamada cuando termina la ejecución de una función del script, de manera que
     * da la oportunidad de utilizar los valores devueltos por dicha función.
	 * @param vm referencia a la máquina virtual.
	 * @param strFunc nombre de la función en el script.
     */
   virtual void HandleReturns(moLuaVirtualMachine& vm, const char *strFunc) = 0;

    /**
     * Devuelve una referencia a la máquina virtual utilizada por este script.
	 * @return referencia a la máquina virtual.
     */
   moLuaVirtualMachine& vm (void) { return m_vm; }
protected:
   bool m_initalized;
   int m_nMethods;
   static moLuaVirtualMachine m_vm;
   static moLuaDebugger m_dbg;
   int m_iThisRef;
   int m_nArgs;
   const char *m_strFunctionName;
};

#endif
