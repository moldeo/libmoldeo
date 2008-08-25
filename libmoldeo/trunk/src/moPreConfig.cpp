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
  Andrés Colubri

*******************************************************************************/

#include "moPreConfig.h"

#include "moArray.cpp"

moDefineDynamicArray( moPreConfigs )

moPreConfig::moPreConfig() {
}

moPreConfig::~moPreConfig()
{
	Finish();
}

MOboolean
moPreConfig::Init() {

	return true;
}


MOboolean
moPreConfig::Finish() {
	m_ValueIndexes.Empty();
	return true;
}

moPreConfig&
moPreConfig::operator = ( const moPreConfig& preconf) {

	m_ValueIndexes = preconf.m_ValueIndexes;
	return (*this);
}

moValueIndex
moPreConfig::operator [] ( const MOint paramindex) {
	moValueIndex vindex;
	vindex.m_ParamIndex = -1;
	vindex.m_ValueIndex = -1;
	if ( 0<=paramindex && paramindex<(MOint)m_ValueIndexes.Count()) {
		return m_ValueIndexes[paramindex];
	} else return vindex;
}

/*
void  moPreConfig::Init( moConfig* conf)
{
    MOint preconfig;
    int i, j;

    preconfig = conf->GetParamIndex("preconfig");
    if(preconfig != MO_PARAM_NOT_FOUND)
    {
        paramnum = conf->GetParamsCount();
        cantpreconfs = conf->GetValuesCount( preconfig);
        if(cantpreconfs>0)
        {
            preconf = new MOint*[cantpreconfs];
            for( i=0; i<cantpreconfs; i++)
                preconf[i] = new MOint[paramnum];
            for( i=0; i<cantpreconfs; i++)
                for( j=0; j<paramnum; j++)
                    preconf[i][j] = conf->GetParam(preconfig).GetValue(i).GetSubValue(j).Int();
            preconfactual = 0;
            SetCurrentPreConf( preconfactual, conf);
        }
    }
}

void
moPreConfig::Finish()
{
    if(preconf != NULL)
    {
        for( int i=0; i<cantpreconfs; i++)
            if(preconf[i]!=NULL)
				delete[] preconf[i];
        delete[] preconf;
    }
	preconf = NULL;
}

MOint moPreConfig::GetPreConfNum()
{
    return cantpreconfs;
}


MOint moPreConfig::GetCurrentPreConf()
{
    return preconfactual;
}


void  moPreConfig::SetCurrentPreConf( int p, moConfig *conf)
{
    if(0<=p && p<cantpreconfs)
    {
        for( int i=0; i<paramnum; i++)
            conf->SetCurrentValueIndex( i, preconf[p][i]);
        preconfactual = p;
    }
}


void  moPreConfig::PreConfFirst( moConfig* conf)
{
    if(cantpreconfs>0)
        SetCurrentPreConf( 0, conf);
}


void  moPreConfig::PreConfNext( moConfig* conf)
{
    if(preconfactual!=-1 && preconfactual<cantpreconfs-1)
        SetCurrentPreConf( ++preconfactual, conf);
}


void  moPreConfig::PreConfPrev( moConfig* conf)
{
    if(preconfactual!=-1 && preconfactual>0)
        SetCurrentPreConf( --preconfactual, conf);
}

*/
