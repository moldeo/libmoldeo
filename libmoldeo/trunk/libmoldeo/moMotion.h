/*******************************************************************************

                              moMotion.h

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

#ifndef __MO_MOTION_H__
#define __MO_MOTION_H__

#include "moParam.h"
#include "moTypes.h"

#define torad  0.0174532925199432957692369076848861f    // PI/180
#define togra  57.2957795130823208767981548141052f      // 180/PI
#define sqrt2  0.707106781186547524400844362104849f     // sqr(2)/2

class LIBMOLDEO_API moMotion
{
public:
    moMotion();
    virtual ~moMotion();

    MOfloat movx( moParam& param, moTempo& tempo);
    MOfloat movy( moParam& param, moTempo& tempo);
    MOfloat movz( moParam& param, moTempo& tempo);

    MOfloat movx( moParam& param, MOdouble ang);
    MOfloat movy( moParam& param, MOdouble ang);
    MOfloat movz( moParam& param, MOdouble ang);

private:
    //moPulse pulse;

    MOdouble movEsf( char, int, double, double, double, double);
    MOdouble movLin( char, int, double, double, double, double);
    MOdouble movCub( char, int, double, double, double, double);
    MOdouble movSEs( char, int, double, double, double, double);
    MOdouble movFij( char, int, double, double, double, double);
    MOdouble movBuc( char, int, double, double, double, double);
    MOdouble movRan( char, int, double, double, double, double);
    MOdouble movGan( char, int, double, double, double, double);
    MOdouble movPul( char, int, double, double, double, double);
};

#endif

// :-D

// Falta crear una clase ajFuncMov que tenga la estructura para contener las N funciones 
// de movimiento y que permita acceder a cualquier value en tiempo de ejecucion, para 
// poder hacer por ejemplo  fm.amplitude *= amp;  y que ademas se pueda convertir ida y
// vuelta en un moParam.    
