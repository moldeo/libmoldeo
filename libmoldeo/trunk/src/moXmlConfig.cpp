/*******************************************************************************

                               moXmlConfig.cpp

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
  Andr� Colubri

*******************************************************************************/

#include "moXmlConfig.h"


//====================================================================================================================
//
//
//			moXmlParam
//
//
//====================================================================================================================

/*
moXmlParam::moXmlParam()
{
  //subcount = 0;
}

moXmlParam::moXmlParam( char *nvalue)
{
    moXmlParam::SetValue( nvalue);
}

moXmlParam::~moXmlParam()
{
}

void moXmlParam::SetValue( char *nvalue)
{
    moText tmp( nvalue);

    value = nvalue;
    subcount = 0;
    tmp.Trim();
    while( tmp!=moText("") && subcount<MO_MAX_SUBVALOR)
    {
        //subvalue[subcount] = tmp.Scan(" \t");
		subvalue[subcount] = tmp.ScanEx(" \t");
        subcount++;

}

moText moXmlParam::GetValue()
{
    return value;
}

moText moXmlParam::GetValueStr( int i)
{
    if(0<=i && i<subcount)
        return subvalue[i];
    else
        return moText("");
}


char moXmlParam::GetValueChr( int i)
{
    if(0<=i && i<subcount)
        return subvalue[i][0];
    else
        return '\0';
}


long moXmlParam::GetValueInt( int i)
{
    long tmp;
    if(0<=i && i<subcount)
    {
        sscanf( subvalue[i], "%d", &tmp);
        return tmp;
    }
    else
        return 0;
}


double moXmlParam::GetValueFlt( int i)
{
    //double tmp;
	float tmp2;
	
    if(0<=i && i<subcount)
    {
		sscanf( subvalue[i], "%f", &tmp2);
        //return tmp;
		return tmp2;
    }
    else
        return 0.0;
}

//====================================================================================================================
//
//
//			moXmlConfig
//
//
//====================================================================================================================


moXmlConfig::moXmlConfig()
{
	m_ConfigLoaded = false;	
    
	param = NULL;
    paramcount = 0;
    currentparam = MO_PARAM_NOT_SEL;

    emptyparam.SetValue("0");
}

moXmlConfig::~moXmlConfig()
{
    DeleteConfig();
	
}

MOboolean
moXmlConfig::IsConfigLoaded() {

	return m_ConfigLoaded;

}

int moXmlConfig::LoadConfig( char *fname)
{
	m_ConfigLoaded = true;

    return MO_CONFIG_OK;
}

int moXmlConfig::SaveConfig( char* fname)
{

    return MO_CONFIG_OK;
}

void moXmlConfig::DeleteConfig()
{
	paramcount = 0;
    currentparam = MO_PARAM_NOT_SEL;

    for( i=0; i<MO_MAX_PARAM; i++)
        nameparam[i] = "";

    memset( valuecount, 0, sizeof(valuecount));
    memset( currentvalue, MO_PARAM_NOT_SEL, sizeof(currentvalue));

	m_ConfigLoaded = false;
}


void moXmlConfig::AddParam( char* name)
{
    if(paramcount == 0)
    {
        param = new moParam*[MO_MAX_PARAM];
        if(!param)
        {
            printf("Insufficient memory for the configuration.\n");
            exit(1);
        }
        else
            for( int i=0; i<MO_MAX_PARAM; i++)
                param[i] = NULL;
    }
    currentparam = paramcount;
    paramcount++;
    nameparam[currentparam] = name;
    valuecount[currentparam] = 0;
	
}

moParam& moXmlConfig::GetParam( int nroparam, int nroopcion)
{
  if(0<=nroparam  && nroparam<paramcount && 
      0<=nroopcion && nroopcion<valuecount[nroparam])
    return param[nroparam][nroopcion];
  else
  {
    printf("Parameter out of range.\n");
    return emptyparam;
  }
	return emptyparam;
}

moParam& moXmlConfig::GetParam( int nroparam)
{
  if(0<=nroparam  && nroparam<paramcount &&
      0<=currentvalue[nroparam] && currentvalue[nroparam]<valuecount[nroparam])
    return param[nroparam][currentvalue[nroparam]];
  else
  {
    printf("Parameter out of range.\n");
    return emptyparam;
  }
  return emptyparam;
}

moParam& moXmlConfig::GetParam()
{
  if(0<=currentparam  && currentparam<paramcount &&
      0<=currentvalue[currentparam] && currentvalue[currentparam]<valuecount[currentparam])
    return param[currentparam][currentvalue[currentparam]];
  else
  {
    printf("Parameter out of range.\n");
    return emptyparam;
  }
  return emptyparam;
}

void moXmlConfig::SetParam( int nroparam, int nroopcion, moParam& nparam)
{
    if(0<=nroparam && nroparam<paramcount  && 0<=nroopcion && nroopcion<valuecount[nroparam])
        param[nroparam][nroopcion] = nparam;
		
}

int moXmlConfig::GetParamCount()
{
  return m_Params.Count();
}

int moXmlConfig::GetCurrentParam()
{
  return m_ParamIndex;
}

void moXmlConfig::SetCurrentParam( int nactual)
{
  if(0<=nactual && nactual<m_Params.Count())
    m_ParamIndex = nactual;
}

void moXmlConfig::FirstParam()
{
  if(m_ParamIndex!=MO_PARAM_NOT_SEL && m_Params.Count()>0)
    m_ParamIndex=0;
}

void moXmlConfig::NextParam()
{
  if(m_ParamIndex!=MO_PARAM_NOT_SEL && m_ParamIndex<m_Params.Count()-1)
    m_ParamIndex++;
}

void moXmlConfig::PrevParam()
{
  if(m_ParamIndex!=MO_PARAM_NOT_SEL && m_ParamIndex>0)
    m_ParamIndex--;
}

int moXmlConfig::GetParamIndex( char* name)
{
  for( int i=0; i<paramcount; i++)
  {
      if( nameparam[i]== name)
          return i;
  }

  return MO_PARAM_NOT_FOUND;
}

moText moXmlConfig::GetParamName( int nroparam)
{
  if(0<=nroparam && nroparam<paramcount)
    return nameparam[nroparam];
  else
  {
    printf("Par�etro fuera de rango.\n");
    return moText("");
  }
  
	return moText("");
}


void moXmlConfig::AddValue( int nroparam, char* valueparam)
{
    if(valuecount[nroparam] == 0)
    {
        param[nroparam] = new moParam[MO_MAX_VALOR];
        if(!param[nroparam])
        {
            printf("Insuficiente memoria para la configuracion.\n");
            exit(1);
        }
    }
    currentvalue[nroparam] = valuecount[nroparam];
    valuecount[nroparam]++;
    param[nroparam][currentvalue[nroparam]].SetValue( valueparam);
	
}

int moXmlConfig::GetValueCount( int nroparam)
{
	
  if(0<=nroparam && nroparam<paramcount)
    return valuecount[nroparam];
  else
    return 0;
}

int moXmlConfig::GetCurrentValue( int nroparam)
{
  if(0<=nroparam && nroparam<paramcount)
    return currentvalue[nroparam];
  else
    return MO_PARAM_NOT_FOUND;
}

void moXmlConfig::SetCurrentValue( int nroparam, int nroopcion)
{
  if(0<=nroparam  && nroparam<paramcount && 0<=nroopcion && nroopcion<valuecount[nroparam])
    currentvalue[nroparam]=nroopcion;
	
}

void moXmlConfig::FirstValue()
{
  if(currentvalue[currentparam] != MO_PARAM_NOT_SEL && valuecount[currentparam] > 0)
    currentvalue[currentparam]=0;
	
}

void moXmlConfig::NextValue()
{
  if(currentvalue[currentparam] != MO_PARAM_NOT_SEL && currentvalue[currentparam] < valuecount[currentparam]-1)
    currentvalue[currentparam]++;
	
}

void moXmlConfig::PreviousValue()
{
  if(currentvalue[currentparam] != MO_PARAM_NOT_SEL && currentvalue[currentparam] > 0)
    currentvalue[currentparam]--;
	
}

*/
