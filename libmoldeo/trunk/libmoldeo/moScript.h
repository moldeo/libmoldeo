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
  Andr�s Colubri

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
 * pueden ser llamados m�todos de la clase (siempre y cuando sean registrados en la m�quina virtual).
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
     * Inicializa el script con la m�quina virtual .
     */
    void InitScript();
    /**
     * Finaliza el script.
     */
    void FinishScript();

    /**
     * Compila el archivo strFilename.
	 * @param strFilename nombre del archivo de script.
	 * @return el resultado de la operaci�n: true o false.
     */
    bool CompileFile(const char *strFilename);
    /**
     * Compila el b�fer pbBuffer.
	 * @param pbBuffer b�fer a compilar.
	 * @param szLen tama�o del b�fer.
	 * @return el resultado de la operaci�n: true o false.
     */
    bool CompileBuffer(unsigned char *pbBuffer, size_t szLen);

    /**
     * Registra una funci�n (m�todo) de la clase en la m�quina virtual de LUA.
	 * @param strFuncName nombre de la funci�n.
	 * @return regresa el �ndice de la funci�n den la tabla de LUA, -1 si la operaci�n no es exitosa.
     */
    int RegisterFunction(const char *strFuncName);

    /**
     * Selecciona una funci�n del script LUA para ejecutar con RunSelectedFunction.
	 * @param strFuncName nombre de la funci�n a seleccionar.
	 * @return el resultado de la operaci�n: true o false.
	 * @see RunSelectedFunction
     */
    bool SelectScriptFunction(const char *strFuncName);
    /**
     * Agrega un par�metro entero a la lista de par�metros para pasar a la funci�n del script seleccionada actualmente.
	 * @param iInt valor del par�metro entero.
     */
    void AddFunctionParam(int iInt);
    /**
     * Agrega un par�metro flotante a la lista de par�metros para pasar a la funci�n del script seleccionada actualmente.
	 * @param fFloat valor del par�metro flotante.
     */
    void AddFunctionParam(float fFloat);
    /**
     * Agrega un par�metro de texto a la lista de par�metros para pasar a la funci�n del script seleccionada actualmente.
	 * @param string valor del par�metro de texto.
     */
    void AddFunctionParam(char *string);

    /**
     * Ejecuta la funci�n del script LUA seleccionada con SelectScriptFunction.
	 * @param nReturns n�mero esperado de valores a devolver por la funci�n.
	 * @return el resultado de la operaci�n: true o false.
	 * @see SelectScriptFunction
     */
    bool RunSelectedFunction(int nReturns = 0);

    /**
     * Revisa que el script tenga la funci�n especificada.
	 * @param strFuncName nombre de la funci�n.
	 * @return true si la funci�n existe, false en caso contrario.
     */
   bool ScriptHasFunction(const char *strScriptName);

    /**
     * Devuelve cuantos m�todos de la clase han sido registrados en la m�quina virtual con RegisterFunction.
	 * @return n�mero de m�todos.
	 * @see RegisterFunction
     */
   int methods(void) { return m_nMethods; }

    /**
     * Esta funci�n es llamada cuando en el LUA script un m�todo registrado de la clase es ejecutado.
	 * @param vm referencia a la m�quina virtual.
	 * @param iFunctionNumber �ndice del m�todo siendo llamado.
	 * @return n�mero valores que el m�todo de la clase agrega a la pila de LUA. Estos valores son los que en
	 * el script son interpretados como los valores que retorna el m�todo. Pueden ser m�s de 1.
     */
   virtual int ScriptCalling(moLuaVirtualMachine& vm, int iFunctionNumber) = 0;

    /**
     * Esta funci�n es llamada cuando termina la ejecuci�n de una funci�n del script, de manera que
     * da la oportunidad de utilizar los valores devueltos por dicha funci�n.
	 * @param vm referencia a la m�quina virtual.
	 * @param strFunc nombre de la funci�n en el script.
     */
   virtual void HandleReturns(moLuaVirtualMachine& vm, const char *strFunc) = 0;

    /**
     * Devuelve una referencia a la m�quina virtual utilizada por este script.
	 * @return referencia a la m�quina virtual.
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
