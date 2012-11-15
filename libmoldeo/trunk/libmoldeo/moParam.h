/*******************************************************************************

                              moParam.h

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

  Gustavo Lado?(donde mas?)

*******************************************************************************/

#ifndef __MO_PARAM_H__
#define __MO_PARAM_H__

#include "moTypes.h"
#include "moText.h"
#include "moValue.h"

enum moParamType {
	MO_PARAM_ALPHA,			//value type: NUM or FUNCTION
	MO_PARAM_COLOR,			//value type: NUM[4] or FUNCTION[4] or
	MO_PARAM_BLENDING,		//value type: NUM or TEXT (to script or shader for chroma???)
	MO_PARAM_POLYGONMODE,	//value type: NUM or TEXT ( 0:FILL 1:LINE 2:POINT)
	MO_PARAM_SYNC,			//value type: NUM or FUNCTION
	MO_PARAM_PHASE,			//value type: NUM or FUNCTION
	MO_PARAM_TEXT,			//value type: TXT or LNK
	MO_PARAM_TEXTURE,		//value type: TXT or LNK
	MO_PARAM_TEXTUREFOLDER,	//value type: TXT or LNK
	MO_PARAM_FONT,          //value type: TXT or LNK
	MO_PARAM_3DMODEL,		//value type: TXT or LNK
	MO_PARAM_OBJECT,		//value type: TXT or LNK
    MO_PARAM_VIDEO,			//value type: TXT or LNK
	MO_PARAM_SOUND,			//value type: TXT or LNK
	MO_PARAM_NUMERIC,		//value type: NUM
	MO_PARAM_FUNCTION,		//value type: NUM or FUNCTION
	MO_PARAM_TRANSLATEX,    //value type: NUM or FUNCTION
	MO_PARAM_TRANSLATEY,    //value type: NUM or FUNCTION
	MO_PARAM_TRANSLATEZ,    //value type: NUM or FUNCTION
	MO_PARAM_SCALEX,		//value type: NUM or FUNCTION
	MO_PARAM_SCALEY,		//value type: NUM or FUNCTION
	MO_PARAM_SCALEZ,		//value type: NUM or FUNCTION
	MO_PARAM_ROTATEX,		//value type: NUM or FUNCTION
	MO_PARAM_ROTATEY,		//value type: NUM or FUNCTION
	MO_PARAM_ROTATEZ,		//value type: NUM or FUNCTION
	MO_PARAM_SCRIPT,		//value type: TXT or LNK
	MO_PARAM_FILTER,        //value type: TXT or LNK
	MO_PARAM_COMPOSE,        //any composition of types
	MO_PARAM_VECTOR,    //vector of one type
	MO_PARAM_INLET,			//value type: TXT or LNK
	MO_PARAM_OUTLET,			//value type: TXT or LNK
	MO_PARAM_UNDEFINED = -1
};

class LIBMOLDEO_API moParamReference {
	public:
	moParamReference() {
		reference=-1;
	}

	moParamReference(MOint i) {
		reference= i;
	}
	moParamReference(MOuint i) {
		reference = (MOlong)i;
	}

	moParamReference( const moParamReference& src) {
	  (*this) = src;
	}
	moParamReference& operator=( const moParamReference& src) {
	  reference = src.reference;
	  return (*this);
	}

	MOlong reference;
};

class LIBMOLDEO_API moParamIndex {
	public:
	moParamIndex() {
		index=-1;
	}
	moParamIndex(MOint i) {
		index= i;
	}
	moParamIndex(MOuint i) {
		index = (MOlong)i;
	}

	moParamIndex( const moParamIndex& src) {
	  (*this) = src;
	}
	moParamIndex& operator=( const moParamIndex& src) {
	  index = src.index;
	  return (*this);
	}


	MOlong index;
};

typedef moParamReference moR;
typedef moParamIndex moP;

moDeclareExportedDynamicArray( moParamIndex, moParamIndexes );

//==========================================================
//				CONFIG DEFINITION
//==========================================================


class LIBMOLDEO_API moParamDefinition
{
	public:
		moParamDefinition();
		moParamDefinition( const moParamDefinition &src);
		moParamDefinition( const  moText& p_name, moParamType p_type );
		moParamDefinition( const  moText& p_name, const moText& p_type );
		virtual ~moParamDefinition();

		moParamDefinition &operator = (const moParamDefinition &src);

		bool   IsValid() const;

		const moText& GetName() const {
			return m_Name;
		}
		moParamType GetType() const {
			return m_Type;
		}
		moText GetTypeStr()  const;

		MOint GetIndex()  const {
			return m_Index;
		}
		void SetIndex( MOint p_index ) {
			m_Index = p_index;
		}

		void SetDefault( moValue& p_defaultvalue) {
            m_DefaultValue = p_defaultvalue;
        }

        moValue& GetDefaultValue() {
            return m_DefaultValue;
        }


        void SetOptions( const moTextArray& p_options ) {
            m_Options = p_options;
        }

        moTextArray&    GetOptions() {
                return m_Options;
        }

	private:
		moText			m_Name;
		moParamType		m_Type;//type of parameter ()
		MOint			m_Index;//index of this parameter on moConfig parameters array

		moValue         m_DefaultValue;
		moTextArray     m_Options;

};

moDeclareExportedDynamicArray( moParamDefinition, moParamDefinitions);

class LIBMOLDEO_API moParam
{
	public:
		moParam();
		moParam(const moParam &src);
		moParam( moParamDefinition &p_paramdefinition);
		virtual ~moParam();

		moParam &operator = (const moParam &src);

		moValue&	operator [] ( MOint i ) {
			return GetValue( i );
		}

		moValues& GetValues();
		MOuint GetValuesCount() const;

		void AddValue( moValue &value );
		void DeleteValue( MOint i );
		moValue& GetValue( MOint i = -1 );
		void SetDefaultValue();

		void SetParamDefinition( moParamDefinition &p_paramdefinition );
		moParamDefinition& GetParamDefinition();

		void SetIndexValue( int indexvalue );
		int GetIndexValue() const;

		void NextValue();
		void PrevValue();
		void FirstValue();

		moParam* GetPtr();
		moData* GetData();
		void Update();
		void SetExternData( moData* p_pExternData);

	private:
		moParamDefinition	m_ParamDefinition;
		moValues			m_Values;
		int					m_CurrentIndexValue;

		moData*			m_pExternData;//Definido cuando el valor del parámetro es intervenido por un Inlet o un Outlet
		MOboolean		m_bExternDataUpdated;//Marcado cuando un Inlet actualiza este parámetro

};

moDeclareExportedDynamicArray( moParam, moParams);


#endif

