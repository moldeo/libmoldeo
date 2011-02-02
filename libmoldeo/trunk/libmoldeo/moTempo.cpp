/*******************************************************************************

                              moTempo.cpp

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

#include "moTempo.h"

moTempo::moTempo() {
	ticks = 0;
	ticks_0 = 0;
	ticks_1 = 10000;

	syncro = 1.0;
	delta = 1.0;
	deltaprevious = 1.0;
	factor = 1.0;

	beatpulsecount = 0;
}

MOboolean moTempo::Init() {
	ticks = 0;
	ticks_0 = 0;
	ticks_1 = 10000;

	syncro = 1.0;
	delta = 1.0;
	deltaprevious = 1.0;
	factor = 1.0;

	beatpulsecount = 0;
	return true;
}

MOboolean moTempo::Init(MOdouble sync) {
	ticks = 0;
	ticks_0 = 0;
	ticks_1 = 10000;

	syncro = sync;
	delta = 1.0;
	deltaprevious = 1.0;
	factor = 1.0;

	beatpulsecount = 0;
	return true;
}

MOdouble moTempo::getPrincipalAngle() {

	MOdouble dx,dy;
	MOulong intv,modv,resv;

	MOdouble t0;
/*

*/

	//correcion para continuidad
	if(deltaprevious!=delta) {
		intv = ticks - ticks_0;
		modv = ticks_1 - ticks_0;
		modv =( modv == 0 ? 1 : modv );
		dy  =(double) 2.0 * moMathf::PI *(factor + deltaprevious - 1.0);
		dx  =(double) modv;
		ang =(double) intv * dy / dx;

		t0 = ticks - ang * modv /(2.0 * moMathf::PI *(factor + delta - 1.0) );

		//corregimos el ticks_0(desfazaje que fuerza el desplazamiento de la funcion para su continuidad)
		// Agregado conversion a int para evitar compiler warnings(by Andres)
		ticks_0 =(int) t0;
		ticks_1 =(int) t0 + modv;
		deltaprevious = delta;
	}


    intv = ticks - ticks_0;
	modv = ticks_1 - ticks_0;
	resv = intv % modv;
	dx =(double) resv /(double) modv;
	dy =(double) 2.0 * moMathf::PI *(factor + delta - 1.0);
	ang =(dx!=0.0 ? dy*dx : 0.0);
	ang*=syncro;
/*
    intv = ticks - ticks_0;
	modv = ticks_1 - ticks_0;
    modv =( modv == 0 ? 1 : modv );
	dy  =(double) 2.0 * pi *(factor + delta - 1.0);
    dx  =(double) modv;
    ang =(double) intv * dy / dx;
	ang *= syncro;
*/
	return (ang);
}

MOdouble moTempo::getTempo() {

	MOdouble dx,dy;
	MOulong intv,modv;

	MOdouble t0;
/*
    intv = ticks - ticks_0;
	modv = ticks_1 - ticks_0 +(MOulong) delta;
	resv = intv % modv;
	dx =(double) resv /(double) modv;
	dy =(double) 2.0 * pi;
	ang =(dx!=0.0 ? factor*dy*dx : 0.0);
	ang*=syncro;
*/

	//correcion para continuidad
	if(deltaprevious!=delta) {
		intv = ticks - ticks_0;
		modv = ticks_1 - ticks_0;
		modv =( modv == 0 ? 1 : modv );
		dy  =(double) 2.0 * moMathf::PI *(factor + deltaprevious - 1.0);
		dx  =(double) modv;
		ang =(double) intv * dy / dx;

		t0 = ticks - ang * modv /(2.0 * moMathf::PI *(factor + delta - 1.0) );

		//corregimos el ticks_0(desfazaje que fuerza el desplazamiento de la funcion para su continuidad)
		// Agregado conversion a int para evitar compiler warnings(by Andres)
		ticks_0 =(int) t0;
		ticks_1 =(int) t0 + modv;
		deltaprevious = delta;
	}

    intv = ticks - ticks_0;
	modv = ticks_1 - ticks_0;
    modv =( modv == 0 ? 1 : modv );
	dy  =(double) 2.0 * moMathf::PI *(factor + delta - 1.0);
    dx  =(double) modv;
    ang =(double) intv * dy / dx;
	ang *= syncro;
	return (ang);
}

void moTempo::BeatPulse(MOulong tik) {
	//beatpulsecount0-> contando tiempo
	if(beatpulsecount==0 || tik-beat_0 > 10000) {      // si pasaron mas de 10' lo toma como el primer beat.
		//entra el primero
		beat_0 = tik;//vamos todavia
		beatpulsecount = 1;//pasamos a 1
	}
	else
	if(beatpulsecount==1) {
		beat_1 = tik;
		ticks_0 = beat_0;
		ticks_1 = beat_1;
		beatpulsecount = 0;//ya esta, lo tenemos, volvemos a cero
		delta = 1.0;//reiniciamos el delta, asi podemos resetearlo
	}
}

moTempo* moTempo::getFromTempo() {
	return m_pFromTempo;
}

void moTempo::SetFromTempo( moTempo *pfromtempo ) {
	m_pFromTempo = pfromtempo;
}

void moTempo::getInterpolatedTempo( MOfloat inter_value ) {//ivalue entre 0.0 y 1.0
	//aca se hace el calculointerpolado
}

