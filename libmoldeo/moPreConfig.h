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


*******************************************************************************/

#ifndef __MO_PRE_CONFIG_H__
#define __MO_PRE_CONFIG_H__

#include "moTypes.h"
#include "moValue.h"

class LIBMOLDEO_API moPreconfigParamIndex {

    public:

      moPreconfigParamIndex() {
          m_ParamName = "";
          m_ParamIndex = -1;
          m_ValueIndex = -1;
      }

      moPreconfigParamIndex( const moPreconfigParamIndex& src ) {
        (*this) = src;
      }

      ~moPreconfigParamIndex() {
        //
      }


      moPreconfigParamIndex& operator = ( const moPreconfigParamIndex& src) {
        m_ParamName = src.m_ParamName;
        m_ParamIndex = src.m_ParamIndex;
        m_ValueIndex = src.m_ValueIndex;
        return (*this);
      }


      moText  m_ParamName;
      MOint	m_ParamIndex;
      MOint	m_ValueIndex;
};

moDeclareExportedDynamicArray( moPreconfigParamIndex, moPreconfigIndexes );

class LIBMOLDEO_API moPreConfig {

	public:
	    moPreConfig();
	    moPreConfig( const moPreConfig& src ) {
	      (*this) = src;
	    }
	    moPreConfig( moPreconfigIndexes& preconfindexes );
	    //moPreConfig( moValueIndexes& valueindexes );
	    virtual ~moPreConfig();
		virtual MOboolean Init();
		virtual MOboolean Finish();

		moPreconfigParamIndex operator [] ( MOint paramindex); //devuelve el indice del valor para el parametro elegido
		moPreConfig& operator = ( const moPreConfig& preconf);

    const moText& ToJSON();

    moText              m_Name;
		//moValueIndexes	    m_ValueIndexes;
		moPreconfigIndexes  m_PreconfIndexes;

		private:
      moText              m_FullJSON;
      moPreconfigParamIndex m_pindex;
};

moDeclareExportedDynamicArray( moPreConfig, moPreConfigs );

#endif

