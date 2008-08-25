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
  Andrés Colubri

*******************************************************************************/

#ifndef __MO_ABSTRACT_H__
#define __MO_ABSTRACT_H__

#include "moTypes.h"
#include "moText.h"

/// Lista de mensajes para la depuración de errores y anuncio de errores y mensajes
/** Clase que implementa una lista de mensajes de texto para poder visualizar y depurar más sencillamente errores
*
*	A este objeto pueden acceder todas las funciones desde cualquier objeto que haya sido derivado de moAbstract.
*   Esta lista es de tipo FIFO, First In, First Out, Primero que entra, primero que sale.
*   @see moAbstract
*/

class LIBMOLDEO_API moDebug
{
public:

	moDebug() {
	    moErr.open ("moldeoerrors.txt");
	    moLog.open ("moldeolog.txt");
	}

	virtual ~moDebug() {
	    moErr.close();
	    moLog.close();
	}


    /// Anuncia un error
    /**
    *   Anuncia un error apilandolo en la lista de mensajes y escribiéndolo
    *   al archivo de mensajes de errores y al de mensajes registrados...
    */
    void Error( moText p_text ) {
        cout << moText("MOL ERROR: ") << p_text << endl;
        moErr << moText("MOL ERROR: ") << p_text << endl;
        moLog << moText("MOL ERROR: ") << p_text << endl;
        m_Debug.Push( moText("MOL ERROR: ") + (moText)p_text );
    }

    /// Anuncia un mensaje al usuario además de guardarlo en el log de texto
    /**
    * Pone el mensaje en la pila de mensajes, y escribe este al log de texto
    *
    */
    void Message( moText p_text ) {
        cout << p_text << endl;
        moLog << p_text << endl;
        m_Debug.Push( p_text );
    }

    /// Escribe un mensaje en el archivo de registro (log)
    /**
    *   El archivo log se llama comunmente moldeolog.txt y se encuentra en el raíz del archivo ejecutable
    *   de Moldeo
    *
    */
    void Log( moText p_text ) {
        moLog << p_text << endl;
    }

    /**
    *   Apila el mensaje dentro de la pila de mensajes
    *   @param p_text el mensaje
    */
	void Push( moText p_text ) {
		m_Debug.Push( p_text );
	}

    /**
    *   Saca y devuelve el primer texto ingresado a la lista
    *   @return un mensaje de texto
    */
	moText Pop() {
		return m_Debug.Pop();
	}

    MOint       Count() {
        return m_Debug.count;
    }
private:

    /// Lista de mensajes
    /**
    * para depurar
    */
    moTextHeap m_Debug;

    /// Cadena de salida de los mensajes de error
    /**
    * la función Error dirige sus mensajes a esta salida
    */
    ofstream moErr;

    /// Cadena de salida de los mensajes registrados
    /**
    * la función Message y Log dirigen sus mensajes a esta salida
    */
    ofstream moLog;

};

/// Clase base abstracta de donde deben derivar los objetos [virtual pura]
/** Clase base abstracta, inicialización y finalización de recursos internos y debug.
*   Todos los objetos al ser derivados de esta clase deberán implementar las siguientes funciones.
*
*   MOboolean Init() : <b>Inicialización:</b> crear recursos internos de la clase
*   MOboolean Finish() : <b>Finalización:</b> liberar los recursos creados en Init()
*
*   @see moDebug
*/
class LIBMOLDEO_API moAbstract
{
public:

    /**
    *   constructor genérico
    */
	moAbstract();
	virtual ~moAbstract();

    /**
    *   Función de Inicialización.
    *   Todos los recursos manejados exclusivamente por este objeto son generados aquí.
    *   @return verdadero si fue exitosa o falso en otro caso
    */
	virtual MOboolean Init();/*!< Inicializa el objeto */

    /**
    *  Función de finalización.
    *  Libera los recursos generados en la función de inicialización.
    *   @return verdadero si fue exitosa o falso en otro caso
    */
	virtual MOboolean Finish();/*!< Finaliza el objeto, libera recursos */

    /**
    *  Ccomprueba si el objeto ha sido inicializado
    *  Devuelve true si está inicializado el objeto o false en caso contrario.
    *   @return verdadero si fue exitosa o falso en otro caso
    */
	MOboolean		Initialized();/*!< Pregunta si está inicializado */

protected:
	MOboolean		m_bInitialized;/**< Valor de inicialización*/

    static moDebug *MODebug2;/**< Clase de impresión de errores para depuración*/
	static moTextHeap *MODebug;/**< Lista de textos*/
};



#endif
