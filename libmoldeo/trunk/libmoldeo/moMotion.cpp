/*******************************************************************************

                              moMotion.cpp

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

// moMotion.cpp: implementation of the moMotion class.
//
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <math.h>
#include "moMotion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

moMotion::moMotion()
{

}

moMotion::~moMotion()
{

}


//////////////////////////////////////////////////////////////////////
// Def. de las 3 funciones basicas de movimiento(movx, movy, movz)
//////////////////////////////////////////////////////////////////////

MOfloat moMotion::movx( moParam& param, moTempo& tempo)
{
  MOdouble ang, mov;

  ang = tempo.getTempo();
  mov = movx( param, ang);

  return mov;
}

MOfloat moMotion::movx( moParam& param, MOdouble ang)
{
  MOdouble mov = 0.0f;
  MOdouble a, v, d;
  MOchar t;
  MOint  cantval, cantfun;

  cantval = param.GetValue().GetSubValueCount();
  cantfun = 0;

  if(cantval >= 1)                 // Valor constante.
  {
	  mov = param.GetValue().GetSubValue(0).Float();
  }

  while(cantfun*4+1 < cantval)
  {
	  t = param.GetValue().GetSubValue(cantfun*4+1).Char();
    a = param.GetValue().GetSubValue(cantfun*4+2).Float();
    v = param.GetValue().GetSubValue(cantfun*4+3).Float();
    d = param.GetValue().GetSubValue(cantfun*4+4).Float()*torad;
    switch( toupper(t))
    {
    case 'E': // Esferico.
              mov += movEsf('x',cantfun, ang, a, v, d);
              break;
    case 'L': // Lineal.
              mov += movLin('x',cantfun, ang, a, v, d);
              break;
    case 'S': // Semiesferico.
              mov += movSEs('x',cantfun, ang, a, v, d);
              break;
    case 'C': // Cubico.
              mov += movCub('x',cantfun, ang, a, v, d);
              break;
    case 'F': // Fijo.
              mov += movFij('x',cantfun, ang, a, v, d);
              break;
    case 'B': // Bucle.
              mov += movBuc('x',cantfun, ang, a, v, d);
              break;
    case 'R': // Random.
              mov += movRan('x',cantfun, ang, a, v, d);
              break;
			  /*
    case 'P': // Pulse.
              mov += movPul('x',cantfun, ang, a, v, d);
              break;
			  */
    }
    cantfun++;
  }

  return mov;
}

MOfloat moMotion::movy( moParam& param, moTempo& tempo)
{
  MOdouble ang, mov;

  ang = tempo.getTempo();
  mov = movy( param, ang);

  return mov;
}

MOfloat moMotion::movy( moParam& param, MOdouble ang)
{
  MOdouble mov = 0.0f;
  MOdouble a, v, d;
  MOchar t;
  MOint  cantval, cantfun;

  cantval = param.GetValue().GetSubValueCount();
  cantfun = 0;

  if(cantval >= 1)                 // Valor constante.
  {
	  mov = param.GetValue().GetSubValue(0).Float();
  }

  while(cantfun*4+1 < cantval)
  {
    t = param.GetValue().GetSubValue(cantfun*4+1).Char();
	a = param.GetValue().GetSubValue(cantfun*4+2).Float();
    v = param.GetValue().GetSubValue(cantfun*4+3).Float();
    d = param.GetValue().GetSubValue(cantfun*4+4).Float()*torad;
    switch( toupper(t))
    {
    case 'E': // Esferico.
              mov += movEsf('y',cantfun, ang, a, v, d);
              break;
    case 'L': // Lineal.
              mov += movLin('y',cantfun, ang, a, v, d);
              break;
    case 'S': // Semiesferico.
              mov += movSEs('y',cantfun, ang, a, v, d);
              break;
    case 'C': // Cubico.
              mov += movCub('y',cantfun, ang, a, v, d);
              break;
    case 'F': // Fijo.
              mov += movFij('y',cantfun, ang, a, v, d);
              break;
    case 'B': // Bucle.
              mov += movBuc('y',cantfun, ang, a, v, d);
              break;
    case 'R': // Random.
              mov += movRan('y',cantfun, ang, a, v, d);
              break;
    case 'G':
              mov += movGan('y',cantfun, ang, a, v, d);
              break;
    case 'P':
              mov += movPul('x',cantfun, ang, a, v, d);
              break;
    }
    cantfun++;
  }

  return mov;
}

MOfloat moMotion::movz( moParam& param, moTempo& tempo)
{
  MOdouble ang, mov;

  ang = tempo.getTempo();
  mov = movz( param, ang);

  return mov;
}

MOfloat moMotion::movz( moParam& param, MOdouble ang)
{
  MOdouble mov = 0.0f;
  MOdouble a, v, d;
  MOchar t;
  MOint  cantval, cantfun;

  cantval = param.GetValue().GetSubValueCount();
  cantfun = 0;

  if(cantval >= 1)                 // Valor constante.
  {
	  mov = param.GetValue().GetSubValue(0).Float();
  }

  while(cantfun*4+1 < cantval)
  {
    t = param.GetValue().GetSubValue(cantfun*4+1).Char();
    a = param.GetValue().GetSubValue(cantfun*4+2).Float();
    v = param.GetValue().GetSubValue(cantfun*4+3).Float();
    d = param.GetValue().GetSubValue(cantfun*4+4).Float()*torad;
    switch( toupper(t))
    {
    case 'E': // Esferico.
              mov += movEsf('z',cantfun, ang, a, v, d);
              break;
    case 'L': // Lineal.
              mov += movLin('z',cantfun, ang, a, v, d);
              break;
    case 'S': // Semiesferico.
              mov += movSEs('z',cantfun, ang, a, v, d);
              break;
    case 'C': // Cubico.
              mov += movCub('z',cantfun, ang, a, v, d);
              break;
    case 'F': // Fijo.
              mov += movFij('z',cantfun, ang, a, v, d);
              break;
    case 'B': // Bucle.
              mov += movBuc('z',cantfun, ang, a, v, d);
              break;
    case 'R': // Random.
              mov += movRan('z',cantfun, ang, a, v, d);
              break;
    case 'P':
              mov += movPul('x',cantfun, ang, a, v, d);
              break;
    }
    cantfun++;
  }

  return mov;
}



//////////////////////////////////////////////////////////////////////
// Def. de las funciones de movimiento especificas a una forma.
//////////////////////////////////////////////////////////////////////

MOdouble moMotion::movEsf( char e, int c, double ang, double a, double v, double d)
{
  MOdouble mov = 0.0f;
  c = c % 2 + 1;
  // Primera componente.
  if(c == 1)
    switch(tolower(e))
    {
    case 'x': mov = a*cos(v*(ang-d));   break;
    case 'y': mov = a*sin(v*(ang-d));   break;
    case 'z': mov = a*sin(v*(ang-d));   break;
    }
  else
  // Segunda componente.
  if(c == 2)
    switch(tolower(e))
    {
    case 'x': mov = a*sin(v*(ang-d));   break;
    case 'y': mov = a*cos(v*(ang-d));   break;
    case 'z': mov = a*sin(v*(ang-d));   break;
    }
  return mov;
}

MOdouble moMotion::movLin( char e, int c, double ang, double a, double v, double d)
{
  MOdouble mov = 0.0f;
  c = c % 2 + 1;
  // Primera componente.
  if(c == 1)
    switch(tolower(e))
    {
    case 'x': mov = a*sin(v*(ang-d));   break;
    case 'y': mov = a*sin(v*(ang-d));   break;
    case 'z': mov = a*sin(v*(ang-d));   break;
    }
  else
  // Segunda componente.
  if(c == 2)
    switch(tolower(e))
    {
    case 'x': mov = a*sin(v*(ang-d));   break;
    case 'y': mov = a*sin(v*(ang-d));   break;
    case 'z': mov = a*sin(v*(ang-d));   break;
    }
  return mov;
}

MOdouble moMotion::movSEs( char e, int c, double ang, double a, double v, double d)
{
  MOdouble mov = 0.0f;
  c = c % 2 + 1;
  // Primera componente.
  if(c == 1)
    switch(tolower(e))
    {
    case 'x': mov = a*fabs(cos(v*(ang-d)));   break;
    case 'y': mov = a*fabs(sin(v*(ang-d)));   break;
    case 'z': mov = a*fabs(sin(v*(ang-d)));   break;
    }
  else
  // Segunda componente.
  if(c == 2)
    switch(tolower(e))
    {
    case 'x': mov = a*fabs(sin(v*(ang-d)));   break;
    case 'y': mov = a*fabs(cos(v*(ang-d)));   break;
    case 'z': mov = a*fabs(sin(v*(ang-d)));   break;
    }
  return mov;
}

MOdouble moMotion::movCub( char e, int c, double ang, double a, double v, double d)
{
  MOdouble mov = 0.0f;
  MOdouble gang = long(v*(ang-d)*togra) % 360;
  c = c % 2 + 1;
  // Primera componente.
  if(c == 1)
    switch(tolower(e))
    {
    case 'x':
          if(315.0<=gang || gang< 45.0)    mov = a;
          if( 45.0<=gang && gang<135.0)    mov = a*cos(v*(ang-d))/sqrt2;
          if(135.0<=gang && gang<225.0)    mov = -a;
          if(225.0<=gang && gang<315.0)    mov = a*cos(v*(ang-d))/sqrt2;
          break;
    case 'y':
          if(315.0<=gang || gang< 45.0)    mov = a*sin(v*(ang-d))/sqrt2;
          if( 45.0<=gang && gang<135.0)    mov = a;
          if(135.0<=gang && gang<225.0)    mov = a*sin(v*(ang-d))/sqrt2;
          if(225.0<=gang && gang<315.0)    mov = -a;
          break;
    case 'z':
          if(315.0<=gang || gang< 45.0)    mov = a*sin(v*(ang-d))/sqrt2;
          if( 45.0<=gang && gang<135.0)    mov = a;
          if(135.0<=gang && gang<225.0)    mov = a*sin(v*(ang-d))/sqrt2;
          if(225.0<=gang && gang<315.0)    mov = -a;
    }
  else
  // Segunda componente.
  if(c == 2)
    switch(tolower(e))
    {
    case 'x':
          if(315.0<=gang || gang< 45.0)    mov = -a;
          if( 45.0<=gang && gang<135.0)    mov = a*sin(v*(ang-d))/sqrt2;
          if(135.0<=gang && gang<225.0)    mov = a;
          if(225.0<=gang && gang<315.0)    mov = a*sin(v*(ang-d))/sqrt2;
          break;
    case 'y':
          if(315.0<=gang || gang< 45.0)    mov = a*cos(v*(ang-d))/sqrt2;
          if( 45.0<=gang && gang<135.0)    mov = a;
          if(135.0<=gang && gang<225.0)    mov = a*cos(v*(ang-d))/sqrt2;
          if(225.0<=gang && gang<315.0)    mov = -a;
          break;
    case 'z':
          if(315.0<=gang || gang< 45.0)    mov = -a;
          if( 45.0<=gang && gang<135.0)    mov = a*sin(v*(ang-d))/sqrt2;
          if(135.0<=gang && gang<225.0)    mov = a;
          if(225.0<=gang && gang<315.0)    mov = a*sin(v*(ang-d))/sqrt2;
    }
  return mov;
}

MOdouble moMotion::movFij( char e, int c, double ang, double a, double v, double d)
{
  return v*(ang-d) + a*sin(v*(ang-d));
}

MOdouble moMotion::movBuc( char e, int c, double ang, double a, double v, double d)
{
  MOdouble mov = 0.0f;
  c = c % 2 + 1;
  // Primera componente.
  if(c == 1)
    switch(tolower(e))
    {
    case 'x': mov = a*cos(v*(ang-d))*sin(ang-d);   break;
    case 'y': mov = a*sin(v*(ang-d))*cos(ang-d);   break;
    case 'z': mov = a*sin(v*(ang-d))*cos(ang-d);   break;
    }
  else
  // Segunda componente.
  if(c == 2)
    switch(tolower(e))
    {
    case 'x': mov = a*sin(v*(ang-d))*cos(ang-d);   break;
    case 'y': mov = a*cos(v*(ang-d))*sin(ang-d);   break;
    case 'z': mov = a*sin(v*(ang-d))*cos(ang-d);   break;
    }
  return mov;
}

MOdouble moMotion::movRan( char e, int c, double ang, double a, double v, double d)
{
  MOdouble mov = 0.0f;
  MOdouble randval, pos;

  ang = ang*togra - d;                              // convierto el angulo a grados.
  pos = v *(MOint)(ang/v + 0.5);                   // posicion donde debe responder.
  if(fabs(ang-pos) < 1.0)
  {
    randval = rand()/double(RAND_MAX);              // value aleatorio entre 0 y 1.
    mov = 2*a*randval-a;                            // el value final esta entre -a y a.
  }

  return mov;
}

MOdouble moMotion::movGan( char e, int c, double ang, double a, double v, double d)
{
	MOdouble mov = 0.0f;
	MOdouble gang = long(v*(ang-d)*togra) % 360;
	
	mov =(gang/360.0f)	* a;

    return mov;
}

MOdouble moMotion::movPul( char e, int c, double ang, double a, double v, double d)
{
  MOdouble mov = 0.0f;
  /*
  MOdouble x =(ang - PI) / PI; // Using x alone, the pulse is centered at PI and has a width of 2PI.
  mov = a * pulse(v *(x - d));
  */
  return mov;
}
