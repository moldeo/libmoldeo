/*******************************************************************************

                              moPreConfig.cpp

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

#include "moPreConfig.h"

#include "moArray.h"
moDefineDynamicArray( moPreConfigs )
moDefineDynamicArray( moPreconfigIndexes )

/*
 moPreconfigParamIndex::moPreconfigParamIndex() {
    m_ParamName = "";
    m_ParamIndex = -1;
    m_ValueIndex = -1;
}
*/
/*
moPreconfigParamIndex::~moPreconfigParamIndex() {
  //nothing
  m_ParamName = "";
  m_ParamIndex = -1;
  m_ValueIndex = -1;
}
*/

/**
  PRECONFIG
*/

moPreConfig::moPreConfig() {
}

moPreConfig::~moPreConfig()
{
	Finish();
}



moPreConfig&
moPreConfig::operator = ( const moPreConfig& preconf) {

	//m_ValueIndexes = preconf.m_ValueIndexes;
	m_PreconfIndexes = preconf.m_PreconfIndexes;
	m_Name = preconf.m_Name;
	return (*this);
}

moPreConfig::moPreConfig( moPreconfigIndexes& preconfindexes ) {
    m_PreconfIndexes = preconfindexes;
    /*
    m_ValueIndexes.Empty();
    for( MOuint i=0; i<m_PreconfIndexes.Count(); i++ ) {

        moValueIndex PIndex;
        PIndex.m_ParamIndex = m_PreconfIndexes[i].m_ParamIndex;
        PIndex.m_ValueIndex = m_PreconfIndexes[i].m_ValueIndex;

        m_ValueIndexes.Add( PIndex );
    }
    */
}
/*
moPreConfig::moPreConfig( moValueIndexes& valueindexes ) {
    m_ValueIndexes = valueindexes;
    m_PreconfIndexes.Empty();
    for( MOuint i=0; i<m_PreconfIndexes.Count(); i++ ) {

        moPreconfigParamIndex PIndex;
        PIndex.m_ParamIndex = m_ValueIndexes[i].m_ParamIndex;
        PIndex.m_ValueIndex = m_ValueIndexes[i].m_ValueIndex;

        m_PreconfIndexes.Add( PIndex );
    }
}
*/

MOboolean
moPreConfig::Init() {

	return true;
}


MOboolean
moPreConfig::Finish() {
	//m_ValueIndexes.Empty();
	m_PreconfIndexes.Empty();
	return true;
}


moPreconfigParamIndex
moPreConfig::operator [] ( MOint paramindex) {
	moPreconfigParamIndex vindex;
	if ( 0<=paramindex && paramindex< (MOint) this->m_PreconfIndexes.Count() ) {
		vindex = this->m_PreconfIndexes[paramindex];
	}
	return vindex;
}

const moText&
moPreConfig::ToJSON() {

  moText fieldseparation = ",";
  m_FullJSON = "{ 'name': '" + m_Name +"'";
  m_FullJSON+= fieldseparation
               + "'valueindexes': {";
  fieldseparation  = "";
  for( int vi = 0; vi< (int)m_PreconfIndexes.Count(); vi++ ) {

    moPreconfigParamIndex vindex = m_PreconfIndexes[vi];

    m_FullJSON+= fieldseparation
          + "'" + vindex.m_ParamName + "': {";
    fieldseparation = ",";

    m_FullJSON+= "'paramindex': " + IntToStr(vindex.m_ParamIndex);
    m_FullJSON+= fieldseparation + "'valueindex': " + IntToStr(vindex.m_ValueIndex);
    m_FullJSON+=  "}";
  }
  m_FullJSON+=  "}";///fin valueindexes
  m_FullJSON+=  "}";///fin Preconfig
  return  m_FullJSON;
}


