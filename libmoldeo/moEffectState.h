/*******************************************************************************

                              moEffectState.h

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

#ifndef __MO_EFFECT_STATE_H__
#define __MO_EFFECT_STATE_H__

#include "moTypes.h"
#include "moAbstract.h"
#include "moTempo.h"
#include "moMoldeoObject.h"

/**
 * moEffectState
 * estado de un efecto
 */

class LIBMOLDEO_API moEffectState : public moMobState
{

	public:
		moEffectState();
		moEffectState(const moEffectState& src);
		virtual ~moEffectState();

		moEffectState& operator=(const moEffectState& src);

		MOboolean Init();
		MOboolean Init(MOswitch);

        /**
         * moTempo
         * registro del beat del efecto
         */
		moTempo  tempo;

        /**
         * on
         * efecto apagado o prendido
         * apagado: -1
         * prendido: 1
         */
		MOswitch on;

        /**
         * synchronized
         * efecto sincronizado con el estado y beat de la consola
         * en sincro: 1
         * sin sincro: -1
         */
		MOswitch synchronized;

        /**
         * enabled (OBSOLETO: moEffectState desciende de moMobState que tiene el miembro m_Activated )
         * si est� activo o no
         * activado: 1
         * desactivado: -1
         */
		MOswitch enabled;

		MOfloat  magnitude;
		MOfloat  amplitude;

        /**
         * alpha: nivel de opacidad del efecto [0..1]
         * opaco: 1
         * transparente: 0
         */
		MOfloat alpha;

        /**
         * tint: nivel de brillo [0..1]
         * luz: 1
         * oscuridad: 0
         */
        MOfloat tint;  //brillo 0..1

        /**
         * tinr: nivel de rojo de la tinta [0..1]
         * rojo puro: 1
         * sin rojo: 0
         */
        MOfloat tintr; //rojo 0..1

        /**
         * ting: nivel de verde de la tinta [0..1]
         * verde puro: 1
         * verde rojo: 0
         */
		MOfloat tintg; //verde 0..1

        /**
         * tinb: nivel de azul de la tinta [0..1]
         * azul puro: 1
         * azul rojo: 0
         */
		MOfloat tintb; //azul 0..1

        /**
         * tinc: crominancia [0..1]
         * 0: 0
         * 360: 1
         */
		MOfloat tintc;

        /**
         * tints: saturaci�n [0..1]
         * 0: 0
         * 100%: 1
         */
		MOfloat tints; ///saturaci�n

        /**
         * fulldebug: informaci�n adicional para correcci�n de errores
         * descativado: -1
         * activado: 1
         */
		MOswitch fulldebug;

		/**
         * 3d stereo capable: capacitado para diferenciar visi�n entre ojo izquierdo y derecho, profundidad
         * descativado: -1
         * activado: 1
         */
		MOswitch stereo;
		moStereoSides stereoside;

   MOfloat mousex;
   MOfloat mousey;
   MOfloat mousefactor;

		moText fullJSON;
		moText fullXML;

    int Set( const moText& p_XmlText );
    void SetColorRGB( MOfloat r, MOfloat g, MOfloat b);
    void SetColorCSV( MOfloat c, MOfloat s, MOfloat t);
		void CSV2RGB();

    virtual const moText& ToJSON();
    virtual const moText& ToXML();
};

#endif

