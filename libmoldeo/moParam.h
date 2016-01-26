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
	MO_PARAM_ALPHA,			///value type: NUM or FUNCTION
	MO_PARAM_COLOR,			///value type: NUM[4] or FUNCTION[4] or
	MO_PARAM_BLENDING,		///value type: NUM or TEXT (to script or shader for chroma???)
	MO_PARAM_POLYGONMODE,	///value type: NUM or TEXT ( 0:FILL 1:LINE 2:POINT)
	MO_PARAM_SYNC,			///value type: NUM or FUNCTION
	MO_PARAM_PHASE,			///value type: NUM or FUNCTION
	MO_PARAM_TEXT,			///value type: TXT or LNK
	MO_PARAM_TEXTURE,		///value type: TXT or LNK
	MO_PARAM_TEXTUREFOLDER,	///value type: TXT or LNK
	MO_PARAM_FONT,          ///value type: TXT or LNK
	MO_PARAM_3DMODEL,		///value type: TXT or LNK
	MO_PARAM_FILE,		    ///value type: TXT or LNK
	MO_PARAM_MOLDEO_OBJECT,		///value type: TXT or LNK, or XML
	MO_PARAM_OBJECT,		///value type: TXT or LNK, or XML
  MO_PARAM_VIDEO,			///value type: TXT or LNK
	MO_PARAM_SOUND,			///value type: TXT or LNK
	MO_PARAM_NUMERIC,		///value type: NUM
	MO_PARAM_FUNCTION,		///value type: NUM or FUNCTION
	MO_PARAM_TRANSLATEX,    ///value type: NUM or FUNCTION
	MO_PARAM_TRANSLATEY,    ///value type: NUM or FUNCTION
	MO_PARAM_TRANSLATEZ,    ///value type: NUM or FUNCTION
	MO_PARAM_SCALEX,		///value type: NUM or FUNCTION
	MO_PARAM_SCALEY,		///value type: NUM or FUNCTION
	MO_PARAM_SCALEZ,		///value type: NUM or FUNCTION
	MO_PARAM_ROTATEX,		///value type: NUM or FUNCTION
	MO_PARAM_ROTATEY,		///value type: NUM or FUNCTION
	MO_PARAM_ROTATEZ,		///value type: NUM or FUNCTION
	MO_PARAM_SCRIPT,		///value type: TXT or LNK
	MO_PARAM_FILTER,        ///value type: TXT or LNK
	MO_PARAM_COMPOSE,        ///any composition of types
	MO_PARAM_VECTOR,    ///vector of one type
	MO_PARAM_INLET,			///value type: TXT or LNK
	MO_PARAM_OUTLET,			///value type: TXT or LNK
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

//check CSS3
/**
http://www.w3.org/TR/css3-transitions/#transition-property

ease
    The ease function is equivalent to cubic-bezier(0.25, 0.1, 0.25, 1).
linear
    The linear function is equivalent to cubic-bezier(0, 0, 1, 1).
ease-in
    The ease-in function is equivalent to cubic-bezier(0.42, 0, 1, 1).
ease-out
    The ease-out function is equivalent to cubic-bezier(0, 0, 0.58, 1).
ease-in-out
    The ease-in-out function is equivalent to cubic-bezier(0.42, 0, 0.58, 1)
step-start
    The step-start function is equivalent to steps(1, start).
step-end
    The step-end function is equivalent to steps(1, end).
steps(<integer>[, [ start | end ] ]?)
    Specifies a stepping function, described above, taking two parameters. The first parameter specifies the number of intervals in the function. It must be a positive integer (greater than 0). The second parameter, which is optional, is either the value ‘start’ or ‘end’, and specifies the point at which the change of values occur within the interval. If the second parameter is omitted, it is given the value ‘end’.
cubic-bezier(<number>, <number>, <number>, <number>)
    Specifies a cubic-bezier curve. The four values specify points P1 and P2 of the curve as (x1, y1, x2, y2). Both x values must be in the range [0, 1] or the definition is invalid. The y values can exceed this range.
*/
enum moParamInterpolationFunction {
    MO_INTERPOLATION_NONE=0,
    MO_INTERPOLATION_LINEAR,
    MO_INTERPOLATION_EASE,
    MO_INTERPOLATION_EASEIN,
    MO_INTERPOLATION_EASEOUT,
    MO_INTERPOLATION_EASEINOUT,
    MO_INTERPOLATION_STEPSTART,
    MO_INTERPOLATION_STEPEND,
    MO_INTERPOLATION_CUBICBEZIER,
    MO_INTERPOLATION_EXPRESSION,
    MO_INTERPOLATION_EASEINOUTQUAD,
    MO_INTERPOLATION_EASEINOUTSIN,
    MO_INTERPOLATION_EASEINOUTCUBIC
};


class LIBMOLDEO_API moParamInterpolation {

    public:

        moParamInterpolation();
        moParamInterpolation(const moParamInterpolation &src);
        virtual ~moParamInterpolation();

        moParamInterpolation &operator = (const moParamInterpolation &src);

        void StartInterpolation( const moData& p_data_in, const moData& p_data_out );
        void StartInterpolation( const moValue& p_value_in, const moValue& p_value_out );
        void StopInterpolation();
        const moTimer& GetTimer() const;
        bool IsOn() const;
        void Activate();

        void SetInterpolationFunction(  const moText& p_interpol_fun );
        void SetInterpolation(  const moText& p_interpol_fun,
                                const moText& p_fun_duration = moText(1000),
                                const moText& p_fun_expression = moText() );
        void SetInterpolation(  moParamInterpolationFunction p_interpol_fun,
                                            const moText& p_fun_duration = moText("1000"),
                                            const moText& p_fun_expression = moText("") );
        void SetDuration(  MOlong p_fun_duration );
        MOlong GetDuration();

        moText GetFunctionToText();

        moData* InterpolateData( moData& pParamData  );

        moValue* InterpolateValue( moValue& pParamValue  );

        const moText& ToJSON() {

          moText fieldseparation = ",";

          m_FullJSON = "{";
          m_FullJSON+= "'interpolation': '" + GetFunctionToText() + "'";
          m_FullJSON+= fieldseparation + "'duration': '" + IntToStr(GetDuration())+ "'";
          m_FullJSON+= fieldseparation + "'on': '" + IntToStr(IsOn())+ "'";
          m_FullJSON+= "}";

          return m_FullJSON;
        }

        double bezier_x( double t, double Ax, double Bx, double Cx);

        double bezier_y( double t, double Ay, double By, double Cy);

        double bezier_x_der( double t, double Ax, double Bx, double Cx);

        double find_x_for( double t, double Ax, double Bx, double Cx );

        double cubic_bezier( double t, double p1, double p2, double p3, double p4 );

    private:
        bool    m_bIsOn;
        moTimer m_Timer;
        MOlong  m_Duration; //in milliseconds
        moParamInterpolationFunction m_Function;

        moData  m_DataIn;
        moData  m_DataOut;
        moData  m_DataInterpolated;

        moValue m_ValueIn;
        moValue m_ValueOut;
        moValue m_ValueInterpolated;

        moText  m_FullJSON;
};


class LIBMOLDEO_API moParamDefinition
{

	public:

		moParamDefinition();
		moParamDefinition( const moParamDefinition &src);
		moParamDefinition( const  moText& p_name, moParamType p_type );
		moParamDefinition( const  moText& p_name, const moText& p_type, const moText& p_property=moText(""), const moText& p_group=moText(""), const moText& p_interpolation=moText(""), const moText& p_duration=moText(""), const moText& p_optionsstr=moText("") );
		moParamDefinition( const  moText& p_name, const moText& p_type, const moText& p_property, const moText& p_group, const moText& p_interpolation, const moText& p_duration, const moTextArray& p_Options );
		virtual ~moParamDefinition();

		moParamDefinition &operator = (const moParamDefinition &src);

		bool   IsValid() const;

		const moText& GetName() const {
			return m_Name;
		}

		void SetName( const moText& p_Name) {
        m_Name = p_Name;
		}


    static moParamType ParamTypeFromStr( const moText& p_type );

		moParamType GetType() const {
			return m_Type;
		}
		moText GetTypeStr()  const;
    void  SetType( moParamType p_paramType );


		MOint GetIndex()  const {
			return m_Index;
		}
		void SetIndex( MOint p_index ) {
			m_Index = p_index;
		}

    const moText& GetProperty() const {
			return m_Property;
		}

		void SetProperty( const moText& p_Property) {
        m_Property = p_Property;
		}

    const moText& GetGroup() const {
			return m_Group;
		}

		void SetGroup( const moText& p_Group ) {
        m_Group = p_Group;
		}

		void SetDefault( const moValue& p_defaultvalue);

    moValue& GetDefaultValue() {
        return m_DefaultValue;
    }


    void SetOptions( const moTextArray& p_options );
    void SetOptions( const moText& p_OptionsStr );

    moTextArray&    GetOptions() {
            return m_Options;
    }

    const moText&    GetOptionsStr() {
            return m_OptionsStr;
    }

    void SetInterpolation( moParamInterpolation& p_Interpolation ) {
        m_Interpolation = p_Interpolation;
    }

    moParamInterpolation& GetInterpolation() {
        return m_Interpolation;
    }

    int Set( const moText& p_XmlText );

    const moText& ToJSON();
    const moText& ToXML();

	private:

		moText			    m_Name;
		moParamType		  m_Type;//type of parameter ()
		MOint			      m_Index;//index of this parameter on moConfig parameters array

    moText          m_Property;//published or not
    moText			    m_Group;
		moValue         m_DefaultValue;
		moTextArray     m_Options;
		moText          m_OptionsStr;
		moParamInterpolation    m_Interpolation;

		moText          m_FullJSON;
		moText          m_FullXML;

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
		const moText& ToJSON();

    bool FixType( moParamType m_NewType = MO_PARAM_UNDEFINED );
    bool FixOptions( moTextArray& m_NewOptions );

	private:
		moParamDefinition	m_ParamDefinition;
		moValues			m_Values;
		int					m_CurrentIndexValue;

		moData*			m_pExternData;//Definido cuando el valor del parámetro es intervenido por un Inlet o un Outlet
		MOboolean		m_bExternDataUpdated;//Marcado cuando un Inlet actualiza este parámetro

		moText        m_fullJSON;

};

moDeclareExportedDynamicArray( moParam, moParams);
typedef std::map< std::string, moParam* > moParamsMap;


#endif

