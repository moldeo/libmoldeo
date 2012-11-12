/*******************************************************************************

                                moTempo.h

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
  Andres Colubri

*******************************************************************************/

#include "moTimer.h"

#ifndef __MO_TEMPO_H
#define __MO_TEMPO_H

/// Tempo, beat, ritmo.
/**
*   Esta clase implementa las funciones de control de tempo
*   @see moTimeManager
*   @see moConsole
*   @see moEffect
*/
class LIBMOLDEO_API moTempo : public moTimer {

public:
    moTempo();
    moTempo( const moTempo &src);

    virtual ~moTempo();
	moTempo &operator = (const moTempo &src);

    virtual long Duration();

private:
	moTempo* m_pFromTempo;

public:
    ///tiempo en milisegundos (obsoleto) se usa moTimer ahora.
	MOulong ticks;      //getticks() milisegundos del clock

	///Primer tick del beat
	MOulong ticks_0;    //primer beat

	///Segundo tick del beat
	MOulong ticks_1;    //segundo beat

    ///variacion de frecuencia entre 0X y 2X, delta es real
	MOdouble delta;
	MOdouble deltaprevious;

	///Multiplicador a partir de 2X hasta KX, donde K en principio es entero
	MOdouble factor;

    ///Syncronización o factor fraccionario, 1.0 predeterminado, sino KX donde K puede ser fraccional (un número real)
	MOdouble syncro;

	MOdouble ang;  //angulo en radianes entre 0 y 2*pi

    ///Temporales del beat
	MOulong beat_0,beat_1, beatpulsecount;

	MOboolean Init();

	MOboolean Init(MOdouble sync);

	MOdouble getPrincipalAngle();


    /**
    *   getTempo
    *   función que corrige y devuelve
    *   @return valor entre 0 y 2PI del ciclo del reloj
    */
	MOdouble getTempo();

	void BeatPulse(MOulong tik);

	moTempo* getFromTempo();

	void SetFromTempo( moTempo *pfromtempo );

	void getInterpolatedTempo( MOfloat inter_value );
};

#endif

