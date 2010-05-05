/*******************************************************************************

								moDebugManager.h

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
  Andrs Colubri

*******************************************************************************/

#include "moTypes.h"
#include "moAbstract.h"
#include "moText.h"
#include "moParam.h"
#include "moResourceManager.h"

#ifndef __MODEBUGMANAGER_H
#define __MODEBUGMANAGER_H
class moDebugManager:public moResource
{
	public:moDebugManager ();
	virtual ~ moDebugManager ();
	virtual MOboolean Init ();
	virtual MOboolean Finish ();

	/// Anuncia un error
	/**
	 *   Anuncia un error apilandolo en la lista de mensajes y escribindolo
	 *   al archivo de mensajes de errores y al de mensajes registrados...
	 */
	static void Error (moText p_text);

	/// Anuncia un mensaje al usuario adems de guardarlo en el log de texto
	/**
	 * Pone el mensaje en la pila de mensajes, y escribe este al log de texto
	 *
	 */
	static void Message (moText p_text);

	/// Escribe un mensaje en el archivo de registro (log)
	/**
	 *   El archivo log se llama comunmente moldeolog.txt y se encuentra en el raz del archivo ejecutable
	 *   de Moldeo
	 *
	 */
	static void Log (moText p_text);

	/**
	 *   Apila el mensaje dentro de la pila de mensajes
	 *   @param p_text el mensaje
	 */
	static void Push (moText p_text);

	/**
	 *   Saca y devuelve el primer texto ingresado a la lista
	 *   @return un mensaje de texto
	 */
	static moText Pop ();
	static MOint Count ();
};
#endif							 /*  */
