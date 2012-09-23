/*******************************************************************************

                                moTimer.h

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

#include "moTypes.h"

#ifndef __MO_TIMER_H__
#define __MO_TIMER_H__


///Devuelve en milisegundos el valor del reloj de Moldeo
/**
*   Funci�n global que devuelve en milisegundos el valor del reloj de Moldeo
*   Esta funci�n devuelve un valor relativo al inicio de la l�nea de tiempo de Moldeo
*   Si se detiene este reloj, el valor devuelto ser� 0
*   Si se pausa el reloj el valor siempre ser� el mismo
*   El valor siempre es en milisegundos
*/
LIBMOLDEO_API MOulong moGetTicks();

///Devuelve en milisegundos el valor del reloj de Moldeo
/**
*   Funci�n global que devuelve en milisegundos el valor del reloj absoluto
*   Esta funci�n devuelve un valor absoluto del reloj de la m�quina
*   Este valor depender� de la implementaci�n seg�n el sistema operativo y la librer�a utilizada,
*   y afectar� el comportamiento de los otros temporizadores
*/
LIBMOLDEO_API MOulong moGetTicksAbsolute();




/// Estado del temporizador
/**
*   Estado del temporizador
*
*/
enum moTimerState {
  MO_TIMERSTATE_STOPPED,/// Parado, Detenido
  MO_TIMERSTATE_PLAYING,/// Corriendo
  MO_TIMERSTATE_PAUSED/// Pausado
};

/// Inicia el temporizador global
/**
*   Inicia el temporizador global
*   El temporizador moTimeManager::MoldeoTimer , es un objeto est�tico que sirve de referencia para la l�nea de tiempo de Moldeo
*   Se inicia a trav�s de esta funci�n
*   @see moTimer
*/
LIBMOLDEO_API void moStartTimer();

/// Fija el valor del reloj del temporizador global
/**
*   Fija el valor del reloj del temporizador global
*   @see moTimer
* @param p_timecode duraci�n del temporizador global, en milisegundos
*/
LIBMOLDEO_API void moSetDuration( MOulong p_timecode );

/// Devuelve el valor del reloj del temporizador global
/**
*   Devuelve el valor del reloj del temporizador global
*   @see moTimer
* @return duraci�n del temporizador global, en milisegundos
*/
LIBMOLDEO_API MOulong moGetDuration();

/// Continua luego de una pausa el temporizador global
/**
*   Continua luego de una pausa el temporizador global
*   @see moTimer
*/
LIBMOLDEO_API void moContinueTimer();

/// Pausa el temporizador global
/**
*   Pausa el temporizador global
*   @see moTimer
*/
LIBMOLDEO_API void moPauseTimer();

/// Detiene el temporizador global
/**
*   Detiene el temporizador global
*   @see moTimer
*/
LIBMOLDEO_API void moStopTimer();


/// Devuelve el estado del temporizador global
/**
*   Devuelve el estado del temporizador global
*   @see moTimer
*   @see moTimerState
*/
LIBMOLDEO_API moTimerState moGetTimerState();


/// Devuelve verdadero si el temporizador global est� en pausa
/**
*   Devuelve verdadero si el temporizador global est� en pausa
*   @see moTimer
*   @see moTimerState
*   @return verdadero si pausado, falso en otro caso
*/
LIBMOLDEO_API bool moIsTimerPaused();


/// Devuelve verdadero si el temporizador global est� detenido
/**
*   Devuelve verdadero si el temporizador global est� detenido
*   @see moTimer
*   @see moTimerState
*   @return verdadero si detenido, falso en otro caso
*/
LIBMOLDEO_API bool moIsTimerStopped();

/// Devuelve verdadero si el temporizador global se encuentra corriendo
/**
*   Devuelve verdadero si el temporizador global se encuentra corriendo
*   @see moTimerState
*   @return verdadero si corre, falso en otro caso
*/
LIBMOLDEO_API bool moIsTimerPlaying();

/// Devuelve verdadero si el temporizador global se encuentra iniciado
/**
*   Devuelve verdadero si el temporizador global se encuentra iniciado
*   @see moTimerState
*   @return verdadero si iniciado, falso en otro caso
*/
LIBMOLDEO_API bool moIsTimerStarted();



/// Clase para el control de un temporizador (absoluto)
/**
*   Temporizador absoluto
*   Crea un temporizador que puede servir como cron�metro, puede ser pausado o reiniciado
*   La funci�n virtual Duration() es la encargada de fijar la duraci�n en relaci�n al reloj de la m�quina
*   SetDuration permite fijar el tiempo de manera arbitraria
*   En el caso del moTimerAbsolute, Duration() utiliza la funci�n global: moGetTicksAbsolute() , que devuelve el tiempo actual del reloj de la m�quina.
*/
class LIBMOLDEO_API moTimerAbsolute {
  public:
    /// Constructor
    /**
    *   Constructor gen�rico
    */
    moTimerAbsolute();
    moTimerAbsolute( const moTimerAbsolute &src);
    virtual ~moTimerAbsolute();

	moTimerAbsolute &operator = (const moTimerAbsolute &src);



    /// Devuelve el �ltimo valor del reloj
    /**
    *   Devuelve el �ltimo valor del reloj
    * @return duraci�n del temporizador
    */
    int LastDuration() const {
        return duration;
    }


    /// Inicia el temporizador
    /**
    *   Inicia el temporizador
    */
    virtual void Start();

    /// Detiene el temporizador
    /**
    *   Detiene el temporizador
    */
    void Stop() {
        on = false;
        pause_on = false;
        start_tick = 0;
        start_last = 0;
        duration = 0;
    }

    /// Congela o pausa el temporizador
    /**
    *   Congela o pausa el temporizador
    */
    void Pause() {
        pause_on = true;
    }

    /// Prosigue el temporizador
    /**
    *   Prosigue el temporizador
    */
    void Continue() {
        pause_on = false;
    }

    /// Devuelve el estado del temporizador
    /**
    *   Devuelve el estado del temporizador
    * @return verdadero si fue iniciado, falso si no
    */
    bool Started() const {
        return on;
    }

    /// Devuelve el estado en pausa del cron�metro
    /**
    *   Devuelve el estado en pausa del cron�metro
    * @return verdadero si fue pausado, falso si no
    */
    bool Paused() const {
        return pause_on;
    }

    /// Fija el valor del reloj del temporizador
    /**
    *   Fija el valor del reloj del temporizador
    * @param p_timecode duraci�n del temporizador, en milisegundos
    */
    virtual void SetDuration( MOulong p_timecode );

    /// Devuelve el valor del reloj del temporizador
    /**
    *   Devuelve el valor del reloj del temporizador
    * @return duraci�n del temporizador, en milisegundos
    */
    virtual long Duration();


    virtual moTimerState State() const;

protected:
        bool on;
        bool pause_on;

        long start_tick;
        long start_last;

        long duration;

        long last_duration;

};


/// Clase para el control de un temporizador (relativo) con identificaci�n
/**
*   Temporizador relativo, con identificaci�n
*   Posee tanto un identificador interno, como uno relativo a un objeto
*   La funci�n Duration esta vez llama a la funci�n moGetTicks(), para sincronizar el temporizador
*   esta funci�n sincronizar con el temporizador global creado por �nica vez por moTimerManager
*   moTimeManager::MoldeoTimer, al depender del temporizador global, todos los temporizadores relativos deber�n
*   corregirse en caso de que este se detenga o prosiga su conteo.
*
*/
class LIBMOLDEO_API moTimer : public moTimerAbsolute {

    public:

        /// Constructor
        /**
        *   Constructor del temporizador relativo, fija en -1 los identificadores
        *   -1 = indeterminado
        */
        moTimer();

        moTimer( moTimer& src );
        virtual ~moTimer();
        moTimer& operator=(moTimer& src);



        /// Inicia el temporizador
        /**
        *   Inicia el temporizador
        */
        virtual void Start();

        /// Fija el valor del reloj del temporizador
        /**
        *   Fija el valor del reloj del temporizador
        * @param p_timecode duraci�n del temporizador, en milisegundos
        */
        virtual void SetDuration( MOulong p_timecode );

        /// Devuelve el valor del reloj del temporizador
        /**
        *   Devuelve el valor del reloj del temporizador
        * @return duraci�n del temporizador, en milisegundos
        */
        virtual long Duration();

        /// Corrige el reloj
        /**
        *   Corrige el reloj
        *   internamente el conteo del reloj es relativo al inicio del temporizador
        *   la marca de inicio vuelve a ser reseteada con esta funci�n, esto puede acarrear problemas si no se manipula correctamente
        *   ya que la cuenta relativa puede ser negativa
        */
        void Fix();

        /// Fija el valor del identificador interno del temporizador
        /**
        *   Fija el valor del identificador interno del temporizador
        * @param p_timerid entero que representa el identificador interno
        */
        void SetTimerId( long p_timerid ) {
                m_TimerId = p_timerid;
        }

        /// Devuelve el valor del identificador interno del temporizador
        /**
        *   Devuelve el valor del identificador interno del temporizador
        * @return entero que representa el identificador interno
        */
        long GetTimerId() {
                return m_TimerId;
        }

        /// Fija el valor del identificador interno del objeto asociado
        /**
        *   Fija el valor del identificador interno del objeto asociado
        * @param p_objectid entero que representa el identificador del objeto relacionado
        */
        void SetObjectId( long p_objectid ) {
                m_ObjectId = p_objectid;
        }

        /// Devuelve el valor del identificador interno del objeto asociado
        /**
        *   Devuelve el valor del identificador interno del objeto asociado
        * @return entero que representa el identificador del objeto relacionado
        */
        long GetObjectId() {
                return m_ObjectId;
        }

        /// Fija un temporizador relativo
        /**
        *   Fija un temporizador relativo
        * @param puntero al temporizador relacionado
        */
        void SetRelativeTimer( moTimerAbsolute* p_pRelativeTimer = NULL ) {
            m_pRelativeTimer = p_pRelativeTimer;
        }


        /// Devuelve el puntero al temporizador relativo
        /**
        * Devuelve el puntero al temporizador relativo
        * @return puntero al temporizador relacionado
        */
        moTimerAbsolute* GetRelativeTimer() {
            return m_pRelativeTimer;
        }

    private:
        long m_TimerId;/// identificador del temporizador
        long m_ObjectId;/// identificador del objeto asociado

        moTimerAbsolute*    m_pRelativeTimer;
};


moDeclareExportedDynamicArray( moTimer*, moTimers);

#endif
