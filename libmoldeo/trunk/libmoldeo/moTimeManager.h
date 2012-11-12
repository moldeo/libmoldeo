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


*******************************************************************************/

#ifndef __MO_TIMEMANAGER_H__
#define __MO_TIMEMANAGER_H__

#include "moTimer.h"

#include "moText.h"
#include "moConfig.h"
#include "moLock.h"
#include "moResourceManager.h"



/// Clase para el manejo de los temporizadores en Moldeo
/**
*   Administrador del tiempo
*   Este objeto es un recurso que centraliza el manejo de los temporizadores, tanto globales como relativos
*   Los temporizadores cumplen una función importante dentro de Moldeo, ya que la mayoría de los procesos deben ser sincronizados
*   de manera absoluta o relativa con el paso del tiempo.
*   Las funciones de movimiento aplicadas a distintos parámetros para ser independientes de la velocidad de reproducción (cuadros por segundo)
*   resultan más fluídos y consistentes si se sincronizan con un reloj
*   A su vez algunos procesos dependen del tiempo para su diseño, como por ejemplo la vida de un párticula, o la velocidad relativa de un objeto
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
class LIBMOLDEO_API moTimeManager : public moResource
{
	public:

        /// Constructor
        /**
        * Constructor
        */
		moTimeManager();

        /// Destructor
        /**
        * Destructor
        */
		virtual ~moTimeManager();

        /// Inicialización
        /**
        * Inicialización
        */
		virtual MOboolean Init();

        /// Finalización
        /**
        * Finalización
        */
		virtual MOboolean Finish();

        //static MOuint	StartTicks();
        /// Devuelve los milisegundos del reloj de la máquina
        /**
        * Devuelve los milisegundos del reloj de la máquina
        */
		static MOuint	GetTicks();

        /// Crea un nuevo temporizador (relativo)
        /**
        * Crea un nuevo temporizador (relativo)
        */
		moTimer*    NewTimer();

        /// Agrega un nuevo temporizador (relativo)
        /**
        * Agrega un nuevo temporizador (relativo)
        */
		void AddTimer( moTimer* pTimer ); ///agrega un temporizador para ser manipulado

        /// Fija los temporizadores relativos
        /**
        * Fija los temporizadores relativos
        */
		void    FixTimers(); /// modifica los temporizadores para adecuarse al cambio ocurrido en el temporizador absoluto...

        /// Fija el reloj del temporizador global
        /**
        * Fija el reloj del temporizador global
        * @param p_timecode valor del reloj, en milisegundos
        */
        void    SetDuration( MOulong p_timecode );

        /// Elimina los temporizadores asociados a cierto identificador de un objeto
        /**
        * Elimina los temporizadores asociados a cierto identificador de un objeto
        * @param p_objectid valor del identificador del objeto
        */
        void ClearByObjectId( long p_objectid );

        /// Elimina los temporizadores por su identificador interno
        /**
        * Elimina los temporizadores por su identificador interno
        * @param p_timerid valor del identificador del temporizador
        */
        void ClearByTimerId( long p_timerid );

        /// Corrige los temporizadores por el identificador del objeto asociado
        /**
        * Corrige los temporizadores por el identificador del objeto asociado
        * @param p_objectid valor del identificador del objeto asociado
        */
        void FixByObjectId( long p_objectid );

        /// Devuelve el puntero al temporizador global
        /**
        * Devuelve el puntero al temporizador global
        * @return puntero al temporizador global
        */

	protected:

		//static  m_StartTicks;
		//moMathFunctionArray m_func_array;
        moTimers    m_Timers;/// Temporizadores relativos, identificados y con asociaciones
};

#endif

