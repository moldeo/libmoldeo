/*******************************************************************************

                               moXmlConfig.h

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



#ifndef __MO_XMLCONFIG_H__
#define __MO_XMLCONFIG_H__

#include <stdlib.h>
#include "moParam.h"
#include "moText.h"
#include "moArray.h"
#include "moConfig.h"
#include "tinyxml.h"

/*
NOTES:

	config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(  );

	config.GetParam( paramindex ).GetValue( valueindex ).GetSubValue(  );

	moXmlConfig
		+moXmlParam
			+moXmlValue
				+moXmlSubValue
				+moXmlSubValue
				+moXmlSubValue
			+moXmlValue
				+moXmlSubValue
				+moXmlSubValue
		+moXmlParam
			+moXmlValue
				+moXmlSubValue
		+moXmlParam		
			+moXmlValue
				+moXmlSubValue
				+moXmlSubValue
			+moXmlValue
				+moXmlSubValue

*/


/*

class moValueData {

}


class moValueType {

}

class LIBMOLDEO_API moValue {

	public:
		
		moText		Get();                 // Devuelve el parametro como fue guardado.
		moText		GetStr( int i=0) ;        // Los distintos sub-valuees dentro del value...
		char		GetChr( int i=0);        //   se pueden obtener convertidos a distintos...
		long		GetInt( int i=0);        //   tipos.
		double		GetFlt( int i=0);

		int			GetSubCount()    {   return subcount; }

	private:

		moValueType		m_Type;	
		moValueData		m_Data;

};

*/
/*
class LIBMOLDEO_API moXmlParam : public moParam {
	
public:
    moXmlParam();
    moXmlParam( char*);
    virtual ~moXmlParam();

    //Version param extendida para interpolaciones
	
    MOint		GetType();//tipo del parametro(principio de implementacion del XMLConfig //
    moParam*	GetFromParam();//puntero al parametro origen para calcular value interpolado
    void		SetFromParam(moParam *Prev);//setea el parametro origen para calcular el next
    //los calculos de valuees interpolados, se hacen aqui, o bien, por ejemplo en la funcion movimiento

	//ESTO CORRESPONDE EN REALIDAD AL moXmlValue!!!!, aquí puede tener aplicacion asignandoselo al extyendido
    void		SetValue( char*);           // Guarda el value en el parametro.
    moText		GetValue();                 // Devuelve el parametro como fue guardado.
    moText		GetValueStr( int i=0);        // Los distintos sub-valuees dentro del value...
    char		GetValueChr( int i=0);        //   se pueden obtener convertidos a distintos...
    long		GetValueInt( int i=0);        //   tipos.
    double		GetValueFlt( int i=0);

    int			GetSubCount()    {   return subcount; }

private:
    moText		value;                          // el value de parametro entero.
    moText		subvalue[MO_MAX_SUBVALOR];      // los sub-valuees parseados.
    int			subcount;                        // la cantidad de sub-valuees.

	//moValues		m_Values;

};

template class LIBMOLDEO_API moDynamicArray<moXmlParam>;
typedef  moDynamicArray<moXmlParam> moXmlParams;

class moXmlConfig : public moConfig {

	public:

    moXmlConfig();
    virtual ~moXmlConfig();

	MOboolean	  IsConfigLoaded();

    int       LoadConfig( char*);             // lee la configuracion de un archivo.
    int       SaveConfig( char*);            // guarda la configuracion en un archivo.
    void      DeleteConfig();                   // borra la configuracion que tiene armada.

    void      AddParam( char*);             // agrega un parametro a la config actual.
    moParam&  GetParam( int, int);              // devuelve el contenido del parametro indicado y el value indicado.
    moParam&  GetParam( int);                   // devuelve el contenido del parametro indicado y el value actual del parametro.
    moParam&  GetParam();                       // devuelve el contenido del parametro actual y el value actual del parametro.
    void      SetParam( int, int, moParam&);    // cambia el contenido del value indicado para el parametro indicado.
    int       GetParamCount();                   // devuelve la cantidad de parametros en la config.
    int       GetCurrentParam();                 // devuelve el indice del parametro actual.
    void      SetCurrentParam( int);             // mueve el indice del parametro actual al value indicado.
    void      FirstParam();                       // mueve el indice del parametro actual al primer parametro.
    void      NextParam();                       // mueve el indice del parametro actual al next parametro.
    void      PrevParam();                       // mueve el indice del parametro actual al previous parametro.
    int       GetParamIndex( char*);              // busca un parametro por el nombre y devuelve su numero de indice.
    moText    GetParamName( int);                // devuelve el nombre del parametro indicado.
    
    void      AddValue( int, char*);        // agrega un value al parametro indicado,
    int       GetValueCount( int);               // devuelve la cantidad de valuees que tiene el parametro indicado.
    int       GetCurrentValue( int);             // devuelve el indice de value actual para el parametro indicado.
    void      SetCurrentValue( int, int);        // mueve el indice de value actual para el parametro indicado a la posicion indicada.
    void      FirstValue();                       // mueve el indice de value actual a la primera posicion para el parametro actual.
    void      NextValue();                       // mueve el indice de value actual a la next posicion para el parametro actual.
    void      PreviousValue();                       // mueve el indice de value actual a la previous posicion para el parametro actual.


	private:

		TiXmlDocument	m_XMLDocument;
		moXmlParams		m_Params;
		MOboolean		m_ConfigLoaded;

		moText			*m_ParamNameIndex;   // los nombres de los parametros.		

		int				m_ParamIndex;		// el indice que indica cual es el parametro actual.
		int*			m_ValuesIndex;		//los indices que indican cual es el value actual para cada parametro.

		moParam emptyparam;                             // un parametro vacio para devolver en algunos casos de operaciones invalidas.

};
*/
#endif
