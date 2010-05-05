/*******************************************************************************
							   moPort.h
  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                     *
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

 ******************************************************************************
 Use it for Linux compilation.
 Must use autotools to determine this compatibility
 ******************************************************************************/

#ifndef _MOPORT_H_
#define _MOPORT_H_

#include <stdio.h>
#include <cstdio>

inline int
getch (void)
{
	int ch, charactr;
	while ((ch = getchar ()) != '\n')
	{
		charactr = ch;
	}
}


inline char *
itoa (int value, char *str, int radix)
{
	char buffer[100];
	char *res;
								 // Memory-safe version of sprintf.
	snprintf (buffer, 100, "%i", value);
	res = buffer;
	return res;
}


/* Bug! Esto ya est definido en FreeImage.h */
/*
typedef uint8_t BYTE;

struct RGBTRIPLE
{
	BYTE   rgbtBlue;
	BYTE   rgbtGreen;
	BYTE   rgbtRed;
	RGBTRIPLE(BYTE r, BYTE g, BYTE b)
	  : rgbtBlue(b), rgbtGreen(g), rgbtRed(r) {}
};
*/

// Estas constantes fueron agregadas para solucionar errores de compilacion cuando se compara un ajTexto
// char*(by Andres)
const char *const LINEAL_STR = "L";
const char *const EXPONENCIAL_STR = "E";
const char *const INVERSO_STR = "I";
const char *const OTRA_STR = "O";

const char *const BORRADO_STR = "borrado";
const char *const MIRROR_STR = "mirror";
const char *const MIRRORG_STR = "mirrorg";
const char *const MIRRORMORPHING_STR = "mirrormorphing";

const char *const DEBUG_STR = "debug";
const char *const VIDEOWALL_STR = "videowall";
const char *const VIDEOWALLMORPHING_STR = "videowallmorphing";

const char *const imagen_STR = "imagen";
const char *const emergente1_STR = "emergente1";
const char *const emergente2_STR = "emergente2";
const char *const emergente3_STR = "emergente3";
const char *const emergente4_STR = "emergente4";
const char *const icono_STR = "icono";
const char *const luces_STR = "luces";
const char *const texto_STR = "texto";
const char *const dibujoanimado_STR = "dibujoanimado";
const char *const interferencia_STR = "interferencia";
const char *const tunel_STR = "tunel";
const char *const grilla_STR = "grilla";
const char *const pelicula_STR = "pelicula";
const char *const multicitas_STR = "multicitas";
const char *const percusionluces_STR = "percusionluces";
const char *const cubemap_STR = "cubemap";
const char *const solido_STR = "solido";
const char *const escena_STR = "escena";
const char *const escena3d_STR = "escena3d";
const char *const icono3d_STR = "icono3d";

#include <string>
using namespace std;

inline bool
stricmp (const char *str1, const char *str2)
{
	return strcmp (str1, str2);
}
#endif							 /* _MOPORT_H_ */
