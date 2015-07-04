/*******************************************************************************

                              moTypes.cpp

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


/**devuelve un valor con distribucion uniforme 0..1
value entre 0 y 1*/
LIBMOLDEO_API MOfloat morand() {
	//randomize();
	return((double)rand() /(double)(RAND_MAX+1));
}

LIBMOLDEO_API MOint moRand(MOint n) {
	//randomize();
	double nf;
	MOint ni;
	nf =(double)(morand() *(double)(n));
	ni =(int) nf;
	return(ni);
}

//entrada entre 0..255, salida entre 0..1
LIBMOLDEO_API void HSVtoRGB(short iInHue, short iInSaturation, short iInValue, float* piRed, float* piGreen, float* piBlue)
{
	if(iInSaturation == 0) {
		float fInValue;
		fInValue =(float)(iInValue) / 255.0;
		*piRed = fInValue;
		*piGreen = fInValue;
		*piBlue = fInValue;
	} else {
		float fHue, fValue, fSaturation;
		int i;
		float f;
		float p,q,t;

		if(iInHue == 360) iInHue = 0;

		fHue =(float)iInHue / 60;

		i = (int)fHue;
		f = fHue -(float)i;

		fValue =(float)iInValue / 255;
		fSaturation =(float)iInSaturation / 255;
		p = fValue *(1.0 - fSaturation);
		q = fValue *(1.0 -(fSaturation *f));
		t = fValue *(1.0 -(fSaturation *(1.0 - f)));

		switch(i) {
			case 0:
				*piRed = fValue ;//* 255;
				*piGreen = t ;//* 255;
				*piBlue = p ;//* 255;
				break;
			case 1:
				*piRed = q ;//* 255;
				*piGreen = fValue;// * 255;
				*piBlue = p ;//* 255;
				break;
			case 2:
				*piRed = p ;//* 255;
				*piGreen = fValue ;//* 255;
				*piBlue = t ;//* 255;
				break;
			case 3:
				*piRed = p ;//* 255;
				*piGreen = q ;//* 255;
				*piBlue = fValue ;//* 255;
				break;
			case 4:
				*piRed = t ;//* 255;
				*piGreen = p ;//* 255;
				*piBlue = fValue ;//* 255;
				break;
			case 5:
				*piRed = fValue ;//* 255;
				*piGreen = p ;//* 255;
				*piBlue = q ;//* 255;
				break;
		}
	}
}

/**
// RGB are from 0..1, H is from 0..360, SV from 0..1
double maxC = b;
if (maxC < g) maxC = g;
if (maxC < r) maxC = r;
double minC = b;
if (minC > g) minC = g;
if (minC > r) minC = r;

double delta = maxC - minC;

double V = maxC;
double S = 0;
double H = 0;

if (delta == 0)
{
	H = 0;
	S = 0;
}
else
{
	S = delta / maxC;
	double dR = 60*(maxC - r)/delta + 180;
	double dG = 60*(maxC - g)/delta + 180;
	double dB = 60*(maxC - b)/delta + 180;
	if (r == maxC)
		H = dB - dG;
	else if (g == maxC)
		H = 120 + dR - dB;
	else
		H = 240 + dG - dR;
}

if (H<0)
	H+=360;
if (H>=360)
	H-=360;

*/
#ifdef MO_WIN32
#define PACKAGE_VERSION "0.9.1.0"
#else
#include "config.h"
#endif

moText moGetVersionStr() {

    return moText(PACKAGE_VERSION);

}

