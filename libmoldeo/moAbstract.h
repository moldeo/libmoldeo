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


*******************************************************************************/

#ifndef __MO_ABSTRACT_H__
#define __MO_ABSTRACT_H__

#include "moTypes.h"
#include "moText.h"
#include "moLock.h"

/// Lista de mensajes para la depuraci�n de errores y anuncio de errores y mensajes
/** Clase que implementa una lista de mensajes de texto para poder visualizar y depurar m�s sencillamente errores
*
*	A este objeto pueden acceder todas las funciones desde cualquier objeto que haya sido derivado de moAbstract.
*   Esta lista es de tipo FIFO, First In, First Out, Primero que entra, primero que sale.
*   @see moAbstract
*/

class LIBMOLDEO_API moDebug
{
public:

    moDebug();

    virtual ~moDebug();


    /// Fija el archivo de salida del flujo estandar de c y c++ (stdout y cout)
    /**
    *   Permite especificar el archivo donde se escribe la salida estandar de la consola.
    *   Si se quiere volver al sistema estandar se llama a esta funci�n con el par�metro vac�o.
    *   @param filename nombre del archivo para la salida
    */
    void SetStdout( const moText& filename = moText("") );


    /// Paraliza el acceso a las funciones de escritura
    /**
    *   Utiliza un moLock para proteger los datos para el acceso asincr�nico
    *   @see moLock
    *   @return verdadero si fue exitoso o falso si no
    */
		bool Lock() { return m_Lock.Lock(); }

    /// Libera el acceso a las funciones de escritura
    /**
    *   Utiliza un moLock para proteger los datos para el acceso asincr�nico.
    *   @see moLock
    *   @return verdadero si fue exitoso o falso si no
    */
		bool Unlock() { return m_Lock.Unlock(); }

    /// Anuncia y registra un error
    /**
    *   Anuncia un error apil�ndolo en la lista de mensajes y escribi�ndolo
    *   al archivo de mensajes de errores y al de mensajes registrados...
    */
    void Error( moText p_text );

    /// Anuncia y registra un llamado de atencion
    /**
     *   Anuncia un llamado de atencion apilandolo en la lista de mensajes y escribiendolo
     *   al archivo de mensajes de errores y al de mensajes registrados...
     */
    void Warning( moText p_text );

    /// Anuncia un mensaje al usuario adem�s de guardarlo en el log de texto
    /**
    * Pone el mensaje en la pila de mensajes, y escribe este al log de texto
    *
    */
    void Message( moText p_text );
    /*
    void Message( moData p_data );
    void Message( moDatas p_datas );
    void Message( moDataMessage p_message );
    void Message( moDataMessages p_messages );*/

    /// Escribe un mensaje en el archivo de registro (log)
    /**
    *   El archivo log se llama comunmente moldeolog.txt y se encuentra en el ra�z del archivo ejecutable
    *   de Moldeo
    *
    */
    void Log( moText p_text );

    /// Apila el mensaje dentro de la pila de mensajes
    /**
    *   Apila el mensaje dentro de la pila de mensajes
    *   @param p_text el mensaje
    */
    void Push( moText p_text ) {
      Lock();
      m_Debug.Push( p_text );
      Unlock();
    }

    /// Saca y devuelve el primer texto ingresado a la lista
    /**
    *   Saca y devuelve el primer texto ingresado a la lista
    *   @return un mensaje de texto
    */
    moText Pop() {
      moText poped;
      Lock();
      poped = m_Debug.Pop();
      Unlock();
      return poped;
    }


    /// Devuelve la cantidad de mensajes almacenados
    MOint       Count() {
        return m_Debug.Count();
    }

    /// Devuelve una referencia a la pila de mensajes
    /**
    * El uso correcto para evitar errores en caso de acceso asyncronico
    * es hacer una llamada primero a la funci�n de Lock() y luego al terminar de operar
    * con la referencia de la pila llamar a Unlock() para liberarla.
    * @return moTextHeap referencia a la pila de mensajes
    * @see moTextHeap
    * @see moText
    */
    moTextHeap&     GetMessages() {
        return m_Debug;
    }

private:

    /**
    * para depurar
    */
    moTextHeap m_Debug;///< Pila de mensajes

    /**
    * la funci�n Error dirige sus mensajes a esta salida
    */
    ofstream moErr;///< Cadena de salida de los mensajes de error

    /**
    * la funci�n Message y Log dirigen sus mensajes a esta salida
    */
    ofstream moLog;///< Cadena de salida de los mensajes registrados

    /**
    * cuando hacemos un printf o un cout << redirigimos la salida a este stream
    */
    ofstream moStdOut;///< Cadena de salida cout y stdout

    streambuf *psbuf, *backup; ///< Cadena de backup


    moLock	m_Lock;///< sem�foro para el acceso asincr�nico

};

/// Clase base abstracta de donde deben derivar los objetos [virtual pura]
/** Clase base abstracta, inicializaci�n y finalizaci�n de recursos internos y debug.
*   Todos los objetos al ser derivados de esta clase deber�n implementar las siguientes funciones.
*
*   MOboolean Init() : <b>Inicializaci�n:</b> crear recursos internos de la clase
*   MOboolean Finish() : <b>Finalizaci�n:</b> liberar los recursos creados en Init()
*
*   @see moDebug
*/
class LIBMOLDEO_API moAbstract
{
public:

    /**
    *   constructor gen�rico
    */
	moAbstract();
	virtual ~moAbstract();

    /**
    *   Funci�n de Inicializaci�n.
    *   Todos los recursos manejados exclusivamente por este objeto son generados aqu�.
    *   @return verdadero si fue exitosa o falso en otro caso
    */
	virtual MOboolean Init();///< Inicializa el objeto

    /**
    *  Funci�n de finalizaci�n.
    *  Libera los recursos generados en la funci�n de inicializaci�n.
    *   @return verdadero si fue exitosa o falso en otro caso
    */
	virtual MOboolean Finish();///< Finaliza el objeto, libera recursos

    /**
    *  Ccomprueba si el objeto ha sido inicializado
    *  Devuelve true si est� inicializado el objeto o false en caso contrario.
    *   @return verdadero si fue exitosa o falso en otro caso
    */
	MOboolean		Initialized();///< Pregunta si est� inicializado


	MOboolean		m_bInitialized;///< Valor de inicializaci�n

    static moDebug *MODebug2;///< Clase de impresi�n de errores para depuraci�n
	static moTextHeap *MODebug;///< Lista de textos
};



#endif
