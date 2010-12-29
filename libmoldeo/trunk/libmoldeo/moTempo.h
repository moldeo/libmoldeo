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

#include "moTypes.h"
#include "moMath.h"

#ifndef __MO_TEMPO_H
#define __MO_TEMPO_H

/// Tempo, beat, ritmo.
/**
*   Esta clase implementa las funciones de control de tempo
*   @see moTimeManager
*   @see moConsole
*   @see moEffect
*/
class LIBMOLDEO_API moTempo {

private:
	moTempo* m_pFromTempo;

public:
	MOulong ticks;      //getticks() milisegundos del clock
	MOulong ticks_0;    //primer beat
	MOulong ticks_1;    //segundo beat

	MOdouble delta;     //ticks
	MOdouble deltaprevious;
	MOdouble factor;    //grados(radianes)

	MOdouble syncro;    //factor de sincronizacion

	MOdouble ang;  //angulo en radianes entre 0 y 2*pi

	MOulong beat_0,beat_1, beatpulsecount;

	moTempo();

	MOboolean Init();

	MOboolean Init(MOdouble sync);

	MOdouble getPrincipalAngle();

	MOdouble getTempo();

	void BeatPulse(MOulong tik);

	moTempo* getFromTempo();

	void SetFromTempo( moTempo *pfromtempo );

	void getInterpolatedTempo( MOfloat inter_value );
};

#endif
