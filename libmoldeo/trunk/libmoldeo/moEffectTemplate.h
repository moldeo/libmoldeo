/*******************************************************************************

                             moEffectTemplate.h

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

// Clase modelo de efecto //

#ifndef __MO_EFFECT_TEMPLATE_H__
#define __MO_EFFECT_TEMPLATE_H__

#include "moEffect.h"

#ifndef MO_RED
#define MO_RED      0
#define MO_GREEN    1
#define MO_BLUE     2
#define MO_ALPHA    3
#endif

class LIBMOLDEO_API moEffectTemplate : public moEffect
{
public:
    MOint color;
    MOint transx, transy, transz;
    MOint rotatx, rotaty, rotatz;
    MOint scalex, scaley, scalez;

    moEffectTemplate();
    ~moEffectTemplate();

    MOboolean Init();
    void Draw( moTempo*,moEffectState* parentstate=NULL);
    MOboolean Finish();
};

#endif
