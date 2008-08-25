/*******************************************************************************

                              moPreConfig.h

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

#ifndef __MO_PRE_CONFIG_H__
#define __MO_PRE_CONFIG_H__

#include "moTypes.h"
#include "moValue.h"


class LIBMOLDEO_API moPreConfig {

	public:
	    moPreConfig();
	    virtual ~moPreConfig();
		virtual MOboolean Init();
		virtual MOboolean Finish();

		moValueIndex operator [] ( const MOint paramindex); //devuelve el indice del valor para el parametro elegido
		moPreConfig& operator = ( const moPreConfig& preconf);

		moValueIndexes	m_ValueIndexes;

};

moDeclareExportedDynamicArray( moPreConfig, moPreConfigs )

#endif
