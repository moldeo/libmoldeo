/*******************************************************************************

								moTimeManager.h

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

#ifndef __MO_TIMEMANAGER_H__
#define __MO_TIMEMANAGER_H__

#include "moTypes.h"
#include "moText.h"
#include "moConfig.h"
#include "moLock.h"
#include "moResourceManager.h"

/// Estado del temporizador
/**
 *   Estado del temporizador
 *
 */
enum moTimerState
{								 /// Parado, Detenido
	MO_TIMERSTATE_STOPPED,
	MO_TIMERSTATE_PLAYING,		 /// Corriendo
	MO_TIMERSTATE_PAUSED		 /// Pausado
};

/// Inicia el temporizador global
/**
 *   Inicia el temporizador global
 *   El temporizador moTimeManager::MoldeoTimer , es un objeto esttico que sirve de referencia para la lnea de tiempo de Moldeo
 *   Se inicia a travs de esta funcin
 */
LIBMOLDEO_API void moStartTimer ();

/// Fija el valor del reloj del temporizador global
/**
 *   Fija el valor del reloj del temporizador global
 * @param p_timecode duracin del temporizador global, en milisegundos
 */
LIBMOLDEO_API void moSetDuration (MOulong p_timecode);

/// Devuelve el valor del reloj del temporizador global
/**
 *   Devuelve el valor del reloj del temporizador global
 * @return duracin del temporizador global, en milisegundos
 */
LIBMOLDEO_API MOulong moGetDuration ();

/// Continua luego de una pausa el temporizador global
/**
 *   Continua luego de una pausa el temporizador global
 */
LIBMOLDEO_API void moContinueTimer ();

/// Pausa el temporizador global
/**
 *   Pausa el temporizador global
 */
LIBMOLDEO_API void moPauseTimer ();

/// Detiene el temporizador global
/**
 *   Detiene el temporizador global
 */
LIBMOLDEO_API void moStopTimer ();

/// Clase para el control de un temporizador (absoluto)
/**
 *   Temporizador absoluto
 *   Crea un temporizador que puede servir como cronmetro, puede ser pausado o reiniciado
 *   La funcin virtual Duration() es la encargada de fijar la duracin en relacin al reloj de la mquina
 *   SetDuration permite fijar el tiempo de manera arbitraria
 *   En el caso del moTimerAbsolute, Duration() utiliza la funcin global: moGetTicksAbsolute() , que devuelve el tiempo actual del reloj de la mquina.
 */
class LIBMOLDEO_API moTimerAbsolute
{
	public:
		/// Constructor
		/**
		 *   Constructor genrico
		 */
		moTimerAbsolute ()
		{
			on = false;
			pause_on = false;
			start_tick = 0;
			start_last = 0;
			duration = 0;
		}
		/// Devuelve el ltimo valor del reloj
		/**
		 *   Devuelve el ltimo valor del reloj
		 * @return duracin del temporizador
		 */
		int LastDuration ()
		{
			return duration;
		}

		/// Inicia el temporizador
		/**
		 *   Inicia el temporizador
		 */
		virtual void Start ();

		/// Detiene el temporizador
		/**
		 *   Detiene el temporizador
		 */
		void Stop ()
		{
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
		void Pause ()
		{
			pause_on = true;
		}
		/// Prosigue el temporizador
		/**
		 *   Prosigue el temporizador
		 */
		void Continue ()
		{
			pause_on = false;
		}
		/// Devuelve el estado del temporizador
		/**
		 *   Devuelve el estado del temporizador
		 * @return verdadero si fue iniciado, falso si no
		 */
		bool Started ()
		{
			return on;
		}

		/// Devuelve el estado en pausa del cronmetro
		/**
		 *   Devuelve el estado en pausa del cronmetro
		 * @return verdadero si fue pausado, falso si no
		 */
		bool Paused ()
		{
			return pause_on;
		}

		/// Fija el valor del reloj del temporizador
		/**
		 *   Fija el valor del reloj del temporizador
		 * @param p_timecode duracin del temporizador, en milisegundos
		 */
		virtual void SetDuration (MOulong p_timecode);

		/// Devuelve el valor del reloj del temporizador
		/**
		 *   Devuelve el valor del reloj del temporizador
		 * @return duracin del temporizador, en milisegundos
		 */
		virtual long Duration ();
	protected:bool on;
	bool pause_on;
	long start_tick;
	long start_last;
	long duration;
	long last_duration;
};

/// Clase para el control de un temporizador (relativo) con identificacin
/**
 *   Temporizador relativo, con identificacin
 *   Posee tanto un identificador interno, como uno relativo a un objeto
 *   La funcin Duration esta vez llama a la funcin moGetTicks(), para sincronizar el temporizador
 *   esta funcin sincronizar con el temporizador global creado por nica vez por moTimerManager
 *   moTimeManager::MoldeoTimer, al depender del temporizador global, todos los temporizadores relativos debern
 *   corregirse en caso de que este se detenga o prosiga su conteo.
 *
 */
class LIBMOLDEO_API moTimer:public moTimerAbsolute
{
	public:
		/// Constructor
		/**
		 *   Constructor del temporizador relativo, fija en -1 los identificadores
		 *   -1 = indeterminado
		 */
		moTimer ():moTimerAbsolute ()
		{
			m_TimerId = -1;
			m_ObjectId = -1;
		}
		/// Inicia el temporizador
		/**
		 *   Inicia el temporizador
		 */
		virtual void Start ();

		/// Fija el valor del reloj del temporizador
		/**
		 *   Fija el valor del reloj del temporizador
		 * @param p_timecode duracin del temporizador, en milisegundos
		 */
		virtual void SetDuration (MOulong p_timecode);

		/// Devuelve el valor del reloj del temporizador
		/**
		 *   Devuelve el valor del reloj del temporizador
		 * @return duracin del temporizador, en milisegundos
		 */
		virtual long Duration ();

		/// Corrige el reloj
		/**
		 *   Corrige el reloj
		 *   internamente el conteo del reloj es relativo al inicio del temporizador
		 *   la marca de inicio vuelve a ser reseteada con esta funcin, esto puede acarrear problemas si no se manipula correctamente
		 *   ya que la cuenta relativa puede ser negativa
		 */
		void Fix ();

		/// Fija el valor del identificador interno del temporizador
		/**
		 *   Fija el valor del identificador interno del temporizador
		 * @param p_timerid entero que representa el identificador interno
		 */
		void SetTimerId (long p_timerid)
		{
			m_TimerId = p_timerid;
		}
		/// Devuelve el valor del identificador interno del temporizador
		/**
		 *   Devuelve el valor del identificador interno del temporizador
		 * @return entero que representa el identificador interno
		 */
		long GetTimerId ()
		{
			return m_TimerId;
		}

		/// Fija el valor del identificador interno del objeto asociado
		/**
		 *   Fija el valor del identificador interno del objeto asociado
		 * @param p_objectid entero que representa el identificador del objeto relacionado
		 */
		void SetObjectId (long p_objectid)
		{
			m_ObjectId = p_objectid;
		}
		/// Devuelve el valor del identificador interno del objeto asociado
		/**
		 *   Devuelve el valor del identificador interno del objeto asociado
		 * @return entero que representa el identificador del objeto relacionado
		 */
		long GetObjectId ()
		{
			return m_ObjectId;
		}
	private:long m_TimerId;		 /// identificador del temporizador
	long m_ObjectId;			 /// identificador del objeto asociado
};

moDeclareExportedDynamicArray (moTimer *, moTimers);

/// Clase para el manejo de los temporizadores en Moldeo
/**
 *   Administrador del tiempo
 *   Este objeto es un recurso que centraliza el manejo de los temporizadores, tanto globales como relativos
 *   Los temporizadores cumplen una funcin importante dentro de Moldeo, ya que la mayora de los procesos deben ser sincronizados
 *   de manera absoluta o relativa con el paso del tiempo.
 *   Las funciones de movimiento aplicadas a distintos parmetros para ser independientes de la velocidad de reproduccin (cuadros por segundo)
 *   resultan ms fludos y consistentes si se sincronizan con un reloj
 *   A su vez algunos procesos dependen del tiempo para su diseo, como por ejemplo la vida de un prticula, o la velocidad relativa de un objeto
 *   @see moTimerAbsolute
 *   @see moTimer
 *   @see moTimerState
 *
 *   @see moStartTimer
 *   @see moStopTimer
 *   @see moPauseTimer
 *   @see moContinueTimer
 *   @see moSetDuration
 *   @see moGetDuration
 */
class LIBMOLDEO_API moTimeManager:public moResource
{
	public:
		/// Constructor
		/**
		 * Constructor
		 */
		moTimeManager ();

		/// Destructor
		/**
		 * Destructor
		 */
		virtual ~ moTimeManager ();

		/// Inicializacin
		/**
		 * Inicializacin
		 */
		virtual MOboolean Init ();

		/// Finalizacin
		/**
		 * Finalizacin
		 */
		virtual MOboolean Finish ();

		//static MOuint StartTicks();
		/// Devuelve los milisegundos del reloj de la mquina
		/**
		 * Devuelve los milisegundos del reloj de la mquina
		 */
		static MOuint GetTicks ();

		/// Crea un nuevo temporizador (relativo)
		/**
		 * Crea un nuevo temporizador (relativo)
		 */
		moTimer * NewTimer ();

		/// Agrega un nuevo temporizador (relativo)
		/**
		 * Agrega un nuevo temporizador (relativo)
		 */
								 ///agrega un temporizador para ser manipulado
		void AddTimer (moTimer * pTimer);

		/// Fija los temporizadores relativos
		/**
		 * Fija los temporizadores relativos
		 */
		void FixTimers ();		 /// modifica los temporizadores para adecuarse al cambio ocurrido en el temporizador absoluto...

		/// Fija el reloj del temporizador global
		/**
		 * Fija el reloj del temporizador global
		 * @param p_timecode valor del reloj, en milisegundos
		 */
		void SetDuration (MOulong p_timecode);

		/// Elimina los temporizadores asociados a cierto identificador de un objeto
		/**
		 * Elimina los temporizadores asociados a cierto identificador de un objeto
		 * @param p_objectid valor del identificador del objeto
		 */
		void ClearByObjectId (long p_objectid);

		/// Elimina los temporizadores por su identificador interno
		/**
		 * Elimina los temporizadores por su identificador interno
		 * @param p_timerid valor del identificador del temporizador
		 */
		void ClearByTimerId (long p_timerid);

		/// Corrige los temporizadores por el identificador del objeto asociado
		/**
		 * Corrige los temporizadores por el identificador del objeto asociado
		 * @param p_objectid valor del identificador del objeto asociado
		 */
		void FixByObjectId (long p_objectid);

		/// Devuelve el puntero al temporizador global
		/**
		 * Devuelve el puntero al temporizador global
		 * @return puntero al temporizador global
		 */
		static moTimerAbsolute *MoldeoTimer;
	protected:
		//static  m_StartTicks;
		//moMathFunctionArray m_func_array;
		moTimers m_Timers;		 /// Temporizadores relativos, identificados y con asociaciones
};
#endif							 /*  */
