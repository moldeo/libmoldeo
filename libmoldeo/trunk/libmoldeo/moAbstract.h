/*******************************************************************************

								moAbstract.h

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

#ifndef __MO_ABSTRACT_H__
#define __MO_ABSTRACT_H__

//#include <moTypes.h>
#include <moText.h>

/// Lista de mensajes para la depuracin de errores y anuncio de errores y mensajes
/** Clase que implementa una lista de mensajes de texto para poder visualizar y depurar ms sencillamente errores
 *
 *	A este objeto pueden acceder todas las funciones desde cualquier objeto que haya sido derivado de moAbstract.
 *   Esta lista es de tipo FIFO, First In, First Out, Primero que entra, primero que sale.
 *   @see moAbstract
 */
class LIBMOLDEO_API moDebug
{
	public:moDebug ()
	{
		moErr.open ("moldeoerrors.txt");
		moLog.open ("moldeolog.txt");
	} virtual ~ moDebug ()
	{
		moErr.close ();
		moLog.close ();
	}
	/// Anuncia un error
	/**
	 *   Anuncia un error apilandolo en la lista de mensajes y escribindolo
	 *   al archivo de mensajes de errores y al de mensajes registrados...
	 */
	void Error (moText p_text)
	{
		cout << moText ("MOL ERROR: ") << p_text << endl;
		moErr << moText ("MOL ERROR: ") << p_text << endl;
		moLog << moText ("MOL ERROR: ") << p_text << endl;
		m_Debug.Push (moText ("MOL ERROR: ") + (moText) p_text);
	}
	/// Anuncia un mensaje al usuario adems de guardarlo en el log de texto
	/**
	 * Pone el mensaje en la pila de mensajes, y escribe este al log de texto
	 *
	 */
	void Message (moText p_text)
	{
		cout << p_text << endl;
		moLog << p_text << endl;
		m_Debug.Push (p_text);
	}
	/// Escribe un mensaje en el archivo de registro (log)
	/**
	 *   El archivo log se llama comunmente moldeolog.txt y se encuentra en el raz del archivo ejecutable
	 *   de Moldeo
	 *
	 */
	void Log (moText p_text)
	{
		moLog << p_text << endl;
	}
	/**
	 *   Apila el mensaje dentro de la pila de mensajes
	 *   @param p_text el mensaje
	 */
	void Push (moText p_text)
	{
		m_Debug.Push (p_text);
	}
	/**
	 *   Saca y devuelve el primer texto ingresado a la lista
	 *   @return un mensaje de texto
	 */
	moText Pop ()
	{
		return m_Debug.Pop ();
	}
	MOint Count ()
	{
		return m_Debug.count;
	}
	moTextHeap & GetMessages ()
	{
		return m_Debug;
	}
	private:
		/// Lista de mensajes
		/**
		 * para depurar
		 */
		moTextHeap m_Debug;

		/// Cadena de salida de los mensajes de error
		/**
		 * la funcin Error dirige sus mensajes a esta salida
		 */
		ofstream moErr;

		/// Cadena de salida de los mensajes registrados
		/**
		 * la funcin Message y Log dirigen sus mensajes a esta salida
		 */
		ofstream moLog;
};

/// Clase base abstracta de donde deben derivar los objetos [virtual pura]
/** Clase base abstracta, inicializacin y finalizacin de recursos internos y debug.
 *   Todos los objetos al ser derivados de esta clase debern implementar las siguientes funciones.
 *
 *   MOboolean Init() : <b>Inicializacin:</b> crear recursos internos de la clase
 *   MOboolean Finish() : <b>Finalizacin:</b> liberar los recursos creados en Init()
 *
 *   @see moDebug
 */
class LIBMOLDEO_API moAbstract
{
	public:
		/**
		 *   constructor genrico
		 */
		moAbstract ();
		virtual ~ moAbstract ();

		/**
		 *   Funcin de Inicializacin.
		 *   Todos los recursos manejados exclusivamente por este objeto son generados aqu.
		 *   @return verdadero si fue exitosa o falso en otro caso
		 */
								 /*!< Inicializa el objeto */
		virtual MOboolean Init ();

		/**
		 *  Funcin de finalizacin.
		 *  Libera los recursos generados en la funcin de inicializacin.
		 *   @return verdadero si fue exitosa o falso en otro caso
		 */
								 /*!< Finaliza el objeto, libera recursos */
		virtual MOboolean Finish ();

		/**
		 *  Ccomprueba si el objeto ha sido inicializado
		 *  Devuelve true si est inicializado el objeto o false en caso contrario.
		 *   @return verdadero si fue exitosa o falso en otro caso
		 */
		MOboolean Initialized ();/*!< Pregunta si est inicializado */
		MOboolean m_bInitialized;/**< Valor de inicializacin*/
		static moDebug *MODebug2;/**< Clase de impresin de errores para depuracin*/
								 /**< Lista de textos*/
		static moTextHeap *MODebug;
};
#endif							 /*  */
