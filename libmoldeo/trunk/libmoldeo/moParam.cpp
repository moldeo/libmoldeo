/*******************************************************************************

                              moParam.cpp

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

#include "moParam.h"

#include "moArray.h"

moDefineDynamicArray(moParamDefinitions)
moDefineDynamicArray(moParams)
moDefineDynamicArray( moParamIndexes )

//================================================================
//	moParamDefinition
//================================================================

moParamDefinition::moParamDefinition() {
    m_Name = moText("");
    m_Index = -1;
    m_Type = MO_PARAM_UNDEFINED;
}

moParamDefinition::moParamDefinition( const moParamDefinition &src) {
	*this = src;
}

moParamDefinition::moParamDefinition( const moText& p_name, moParamType p_type ) {
		m_Name = p_name;
		m_Type = p_type;
		m_Index = -1;
}

bool
moParamDefinition::IsValid() const {
    return (m_Type!=MO_PARAM_UNDEFINED);
}
/*
MO_PARAM_ALPHA,			//value type: NUM or FUNCTION
	MO_PARAM_COLOR,			//value type: NUM[4] or FUNCTION[4] or
	MO_PARAM_SYNC,			//value type: NUM or FUNCTION
	MO_PARAM_TEXT,			//value type: TXT or LNK
	MO_PARAM_TEXTURE,		//value type: TXT or LNK
	MO_PARAM_FONT,		//value type: TXT or LNK
	MO_PARAM_VIDEO,		//value type: TXT or LNK
	MO_PARAM_SOUND,		//value type: TXT or LNK
	MO_PARAM_FUNCTION,		//value type: NUM or FUNCTION
	MO_PARAM_SCRIPT,		//value type: TXT or LNK
	MO_PARAM_TRA,			//value type: NUM[3] or FUNCTION[3]
	MO_PARAM_ROT,			//value type: NUM[3] or FUNCTION[3]
	MO_PARAM_SCA			//value type: NUM[3] or FUNCTION[3]
	MO_PARAM_INLET,			//value type: TXT or LNK
	MO_PARAM_OUTLET			//value type: TXT or LNK
*/
moParamDefinition::moParamDefinition( const moText& p_name, const moText& p_type,  const moText& p_interpolation, const moText& p_duration) {
		m_Name = p_name;
		bool valid_interpolation = false;
		if ( p_type == moText("ALPHA") ) {
			m_Type = MO_PARAM_ALPHA;
			valid_interpolation = true;
		} else
		if ( p_type == moText("COLOR") ) {
			m_Type = MO_PARAM_COLOR;
		} else
		if ( p_type == moText("BLENDING") ) {
			m_Type = MO_PARAM_BLENDING;
		} else
		if ( p_type == moText("POLYGONMODE") ) {
			m_Type = MO_PARAM_POLYGONMODE;
		} else
		if ( p_type == moText("SYNC") ) {
			m_Type = MO_PARAM_SYNC;
			valid_interpolation = true;
		} else
		if ( p_type == moText("PHASE") ) {
			m_Type = MO_PARAM_PHASE;
			valid_interpolation = true;
		} else
		if ( p_type == moText("TEXT") ) {
			m_Type = MO_PARAM_TEXT;
		} else
		if ( p_type == moText("TEXTURE") ) {
			m_Type = MO_PARAM_TEXTURE;
		} else
		if ( p_type == moText("TEXTUREFOLDER") ) {
			m_Type = MO_PARAM_TEXTUREFOLDER;
		} else
		if ( p_type == moText("FONT") ) {
			m_Type = MO_PARAM_FONT;
		} else
		if ( p_type == moText("3DMODEL") ) {
			m_Type = MO_PARAM_3DMODEL;
		} else
		if ( p_type == moText("OBJECT") ) {
			m_Type = MO_PARAM_OBJECT;
		} else
		if ( p_type == moText("VIDEO") ) {
			m_Type = MO_PARAM_VIDEO;
		} else
        if ( p_type == moText("FILTER") ) {
			m_Type = MO_PARAM_FILTER;
		} else
		if ( p_type == moText("SOUND") ) {
			m_Type = MO_PARAM_SOUND;
		} else
		if ( p_type == moText("NUM") ) {
			m_Type = MO_PARAM_NUMERIC;
			valid_interpolation = true;
		} else
		if ( p_type == moText("FUNCTION") ) {
			m_Type = MO_PARAM_FUNCTION;
			valid_interpolation = true;
		} else
        if ( p_type == moText("TRANSLATEX") ) {
			m_Type = MO_PARAM_TRANSLATEX;
			valid_interpolation = true;
		} else
        if ( p_type == moText("TRANSLATEY") ) {
			m_Type = MO_PARAM_TRANSLATEY;
			valid_interpolation = true;
		} else
        if ( p_type == moText("TRANSLATEZ") ) {
			m_Type = MO_PARAM_TRANSLATEZ;
			valid_interpolation = true;
		} else
        if ( p_type == moText("SCALEX") ) {
			m_Type = MO_PARAM_SCALEX;
			valid_interpolation = true;
		} else
        if ( p_type == moText("SCALEY") ) {
			m_Type = MO_PARAM_SCALEY;
			valid_interpolation = true;
		} else
        if ( p_type == moText("SCALEZ") ) {
			m_Type = MO_PARAM_SCALEZ;
			valid_interpolation = true;
		} else
        if ( p_type == moText("ROTATEX") ) {
			m_Type = MO_PARAM_ROTATEX;
			valid_interpolation = true;
		} else
        if ( p_type == moText("ROTATEY") ) {
			m_Type = MO_PARAM_ROTATEY;
			valid_interpolation = true;
		} else
        if ( p_type == moText("ROTATEZ") ) {
			m_Type = MO_PARAM_ROTATEZ;
			valid_interpolation = true;
		} else
		if ( p_type == moText("SCRIPT") ) {
			m_Type = MO_PARAM_SCRIPT;
		} else
		if ( p_type == moText("COMPOSE") ) {
			m_Type = MO_PARAM_COMPOSE;
		} else
		if ( p_type == moText("VECTOR") ) {
			m_Type = MO_PARAM_VECTOR;
		} else
		if ( p_type == moText("INLET") ) {
			m_Type = MO_PARAM_INLET;
		} else
		if ( p_type == moText("OUTLET") ) {
			m_Type = MO_PARAM_OUTLET;
		} else
		if ( p_type == moText("UNDEFINED") ) {
			m_Type = MO_PARAM_UNDEFINED;
		}

		m_Index = -1;

    moParamInterpolation paraminter;
    if (valid_interpolation) {
            cout << " moParamDefinition(..): function: " << p_interpolation << " duration: " << p_duration << endl;
            if ( p_interpolation == "linear" ) {
                cout << " moParamDefinition(..): interpolation recognized: [" << p_interpolation << "]";
                paraminter.SetInterpolation( MO_INTERPOLATION_LINEAR, p_duration );
                cout << " saved as:" << paraminter.GetFunctionToText() << endl;
                m_Interpolation = paraminter;
            } else if (p_interpolation==moText("easeinoutquad") ) {
                paraminter.SetInterpolation( MO_INTERPOLATION_EASEINOUTQUAD, p_duration );
                m_Interpolation = paraminter;
            } else if (p_interpolation==moText("easeinoutsin") ) {
                paraminter.SetInterpolation( MO_INTERPOLATION_EASEINOUTSIN, p_duration );
                m_Interpolation = paraminter;
            } else {
              cout << " moParamDefinition(..): interpolation unrecognized: [" << p_interpolation << "]" << endl;
              paraminter.SetInterpolation( MO_INTERPOLATION_NONE, "0" );
              m_Interpolation = paraminter;
              cout << "NO INTERPOLATION PARAMETERS: [" << p_interpolation << "] [" << p_duration << "]" << endl;
            }
    } else cout << "INVALID type of parameter: " << p_type << endl;
}


moParamDefinition::~moParamDefinition() {
}

moParamDefinition &moParamDefinition::operator = (const moParamDefinition &src)
{
		m_Name = src.m_Name;
		m_Type = src.m_Type;
		m_Index = src.m_Index;
		m_DefaultValue = src.m_DefaultValue;
		m_Interpolation = src.m_Interpolation;
		return *this;
}


moText
moParamDefinition::GetTypeStr() const {
    switch(m_Type) {
        case MO_PARAM_UNDEFINED:
            return moText("UNDEFINED");
            break;
        case MO_PARAM_ALPHA:
            return moText("ALPHA");
            break;
        case MO_PARAM_COLOR:
            return moText("COLOR");
            break;
        case MO_PARAM_BLENDING:
            return moText("BLENDING");
            break;
        case MO_PARAM_POLYGONMODE:
            return moText("POLYGONMODE");
            break;
        case MO_PARAM_SYNC:
            return moText("SYNC");
            break;
        case MO_PARAM_PHASE:
            return moText("PHASE");
            break;
        case MO_PARAM_TEXT:
            return moText("TEXT");
            break;
        case MO_PARAM_TEXTURE:
            return moText("TEXTURE");
            break;
        case MO_PARAM_TEXTUREFOLDER:
            return moText("TEXTUREFOLDER");
            break;
        case MO_PARAM_FONT:
            return moText("FONT");
            break;
        case MO_PARAM_3DMODEL:
            return moText("3DMODEL");
            break;
        case MO_PARAM_OBJECT:
            return moText("OBJECT");
            break;
        case MO_PARAM_FILTER:
            return moText("FILTER");
            break;
        case MO_PARAM_NUMERIC:
            return moText("NUM");
            break;
        case MO_PARAM_FUNCTION:
            return moText("FUNCTION");
            break;
        case MO_PARAM_TRANSLATEX:
            return moText("TRANSLATEX");
            break;
        case MO_PARAM_TRANSLATEY:
            return moText("TRANSLATEY");
            break;
        case MO_PARAM_TRANSLATEZ:
            return moText("TRANSLATEZ");
            break;
        case MO_PARAM_SCALEX:
            return moText("SCALEX");
            break;
        case MO_PARAM_SCALEY:
            return moText("SCALEY");
            break;
        case MO_PARAM_SCALEZ:
            return moText("SCALEZ");
            break;
        case MO_PARAM_ROTATEX:
            return moText("ROTATEX");
            break;
        case MO_PARAM_ROTATEY:
            return moText("ROTATEY");
            break;
        case MO_PARAM_ROTATEZ:
            return moText("ROTATEZ");
            break;
        case MO_PARAM_SCRIPT:
            return moText("SCRIPT");
            break;
        case MO_PARAM_SOUND:
            return moText("SOUND");
            break;
        case MO_PARAM_VIDEO:
            return moText("VIDEO");
            break;
        case MO_PARAM_COMPOSE:
            return moText("COMPOSE");
            break;
        case MO_PARAM_VECTOR:
            return moText("VECTOR");
            break;
        case MO_PARAM_INLET:
            return moText("INLET");
            break;
        case MO_PARAM_OUTLET:
            return moText("OUTLET");
            break;
    }
    return moText("UNDEFINED");
}


//================================================================
//	moParamInterpolation
//================================================================


moParamInterpolation::moParamInterpolation() :
    m_bIsOn(false), m_Duration(0), m_Function(MO_INTERPOLATION_NONE)
{
}

moParamInterpolation::moParamInterpolation(const moParamInterpolation &src) {
    *this = src;
}

moParamInterpolation::~moParamInterpolation() {

}

moParamInterpolation &
moParamInterpolation::operator=(const moParamInterpolation &src) {
    m_bIsOn = src.m_bIsOn;
    m_DataIn = src.m_DataIn;
    m_DataOut = src.m_DataOut;
    m_DataInterpolated = src.m_DataInterpolated;

    m_Timer = (moTimer&)src.m_Timer;
    m_Duration = src.m_Duration;
    m_Function = src.m_Function;

    return(*this);
}

void
moParamInterpolation::StartInterpolation( const moData& p_data_in, const moData& p_data_out ) {
    m_DataIn = p_data_in;
    m_DataOut = p_data_out;
    m_DataIn.Eval();
    m_DataOut.Eval();
    m_DataInterpolated = p_data_in;
    m_Timer.Start();
}

void
moParamInterpolation::SetDuration(  MOlong p_fun_duration ) {
    m_Duration = p_fun_duration;
}

MOlong
moParamInterpolation::GetDuration() {
    return m_Duration;
}

void
moParamInterpolation::StopInterpolation() {
    m_Timer.Stop();
}

void
moParamInterpolation::SetInterpolation( moParamInterpolationFunction p_interpol_fun,
                                moText p_fun_duration,
                                moText p_fun_expression) {
    moParserFunction* mp_dur = new moParserFunction();
    //moParserFunction* mp_exp = new moParserFunction();
    mp_dur->Init( p_fun_duration );

    //setting
    m_Function = p_interpol_fun;
    m_Duration = mp_dur->Eval();

    cout << "SetInterpolation: settting duration: " << p_fun_duration << " is finally m_Duration:" << m_Duration << endl;
    Activate();
    cout << "SetInterpolation: Activated: " << IsOn() << endl;
}

bool
moParamInterpolation::IsOn() const {
    return m_bIsOn;
}

void
moParamInterpolation::Activate() {
    m_bIsOn = true;
}


const moTimer&
moParamInterpolation::GetTimer() const {
    return m_Timer;
}

moText
moParamInterpolation::GetFunctionToText() {
    switch(m_Function) {
        case MO_INTERPOLATION_NONE:
            return moText("none");
            break;
        case MO_INTERPOLATION_LINEAR:
            return moText("linear");
            break;
        case MO_INTERPOLATION_EASEINOUTQUAD:
            return moText("easeinoutquad");
            break;
        case MO_INTERPOLATION_ATSPEED:
            return moText("atspeed");
            break;
        case MO_INTERPOLATION_EXPRESSION:
            return moText("expression");
            break;
        case MO_INTERPOLATION_EASEINOUTSIN:
            return moText("easeinoutsin");
            break;
        default:
            return moText("");
            break;
    }
    return moText("");
}

/*

Easing functions in c++

http://www.gizma.com/easing/#l

*/

double bezier_x( double t, double Ax, double Bx, double Cx) {
   return t * (Cx + t * (Bx + t * Ax));
}

double bezier_y( double t, double Ay, double By, double Cy) {
  return t * (Cy + t * (By + t * Ay));
}

double bezier_x_der( double t, double Ax, double Bx, double Cx) {
  return Cx + t * (2*Bx + 3*Ax * t);
}

double find_x_for( double t, double Ax, double Bx, double Cx ) {

  double x=t, i=0, z;

  while (i < 5) { // making 5 iterations max
    z = bezier_x(x, Ax, Bx, Cx) - t;

    if ( fabs(z) < 0.001) break; // if already got close enough

    x = x - z/bezier_x_der(x, Ax, Bx, Cx);
    i++;
  }

  return x;
}

double cubic_bezier( double t, double p1, double p2, double p3, double p4 ) {

  double Cx = 3 * p1;
  double Bx = 3 * (p3 - p1) - Cx;
  double Ax = 1 - Cx - Bx;

  double Cy = 3 * p2;
  double By = 3 * (p4 - p2) - Cy;
  double Ay = 1 - Cy - By;

  return bezier_y( find_x_for( t, Ax, Bx, Cx ), Ay, By, Cy );
}

moData*
moParamInterpolation::InterpolateData( const moData& pParamData  ) {

    if (m_Function==MO_INTERPOLATION_NONE) return &pParamData;

    //FIRST treat the UNDEFINED... starting
    if (   m_DataIn.Type() == MO_DATA_UNDEFINED
        || m_DataOut.Type() == MO_DATA_UNDEFINED ) {
        cout << "InterpolateData > datain or out UNDEFINED : resetting " << endl;
        //no in or out are set...
        StopInterpolation();
        //set them...
        StartInterpolation( pParamData, pParamData );
    } else {

    //RUNNING; NEW DATA COMING!!

        //it target is different just set with it the new Out
        if ( m_DataOut.ToText() != pParamData.ToText() ) {
            StopInterpolation();
            StartInterpolation( m_DataInterpolated, pParamData );
            cout << "StartInterpolation: " << m_DataIn.ToText() << "  --> TO --> m_DataOut: " << m_DataOut.ToText() << endl;
        }

        m_DataIn.Eval();//so it records last val
        m_DataOut.Eval();//so it records his last val
        cout << "m_DataIn: " << m_DataIn.LastEval() << "  --> TO --> m_DataOut: " << m_DataOut.LastEval() << endl;

        double RDouble;
        double differential = 1.0f* m_Timer.Duration() / (1.0f*m_Duration);

        //process here with some function
        double c = (double) ( m_DataOut.LastEval() - m_DataIn.LastEval() );
        double t = (double) m_Timer.Duration();
        double d = (double) m_Duration;
        double b = (double) m_DataIn.LastEval();



        if (d>0.0f && t<=d && t>0.0f)
        switch(m_Function) {

            //cases based on http://www.gizma.com/easing/

            case MO_INTERPOLATION_LINEAR:
                RDouble = c * t/d + b;
                break;

            case MO_INTERPOLATION_EASEINOUTQUAD:
                RDouble = c* cubic_bezier( t/d, 0.42, 0.0, 0.58, 1.0 ) + b;
                break;

            case MO_INTERPOLATION_EASEINOUTSIN:
                RDouble = -c/2 * ( moMathd::Cos( moMathd::PI*t/d) - 1) + b;
                break;
            case MO_INTERPOLATION_EXPRESSION:
                if (differential<0.0f) differential = 0.0f;
                if (differential>1.0f) differential = 1.0f;
                RDouble = ( m_DataOut.LastEval() - m_DataIn.LastEval() ) * differential + m_DataIn.LastEval();
                break;
            case MO_INTERPOLATION_ATSPEED:
                if (differential<0.0f) differential = 0.0f;
                if (differential>1.0f) differential = 1.0f;
                RDouble = ( m_DataOut.LastEval() - m_DataIn.LastEval() ) * differential + m_DataIn.LastEval();
                break;
        }
        m_DataInterpolated.SetDouble(RDouble);

    }

    return &m_DataInterpolated;

}



//================================================================
//	moParam
//================================================================


moParam::moParam() {
	m_CurrentIndexValue = 0;
	m_bExternDataUpdated = false;
	m_pExternData = NULL;
}

moParam::moParam(const moParam &src) {
	*this = src;
}

moParam::moParam( moParamDefinition &p_paramdefinition) {
    m_CurrentIndexValue = 0;
	m_bExternDataUpdated = false;
	m_pExternData = NULL;
	m_ParamDefinition = p_paramdefinition;

}

moParam::~moParam() {

}

moParam &moParam::operator = (const moParam &src) {
		m_bExternDataUpdated = src.m_bExternDataUpdated;
		m_pExternData = src.m_pExternData;
		m_ParamDefinition = src.m_ParamDefinition;
		m_Values = src.m_Values;
		m_CurrentIndexValue = src.m_CurrentIndexValue;
		return *this;
}

moValues&
moParam::GetValues() {
	return m_Values;
}

MOuint
moParam::GetValuesCount() const {
	return m_Values.Count();
}


void
moParam::AddValue( moValue &value ) {
	m_Values.Add( value );
}


void
moParam::DeleteValue( MOint i ) {
	m_Values.Remove(i);
}

void moParam::SetDefaultValue() {

    //atencion: siempre setear el tipo despues de pasar el valor del texto...
    if (this->GetValuesCount()==0) {
        moValue xvalue;
        moValueBase valuebase;

        switch( (int)m_ParamDefinition.GetType() ) {
            case MO_PARAM_COLOR:
                valuebase.SetText( "1.0" );
                valuebase.SetType( MO_VALUE_FUNCTION );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_BLENDING:
            case MO_PARAM_POLYGONMODE:
                valuebase.SetText( "0" );
                valuebase.SetInt(0);
                valuebase.SetType( MO_VALUE_NUM );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_ALPHA:
            case MO_PARAM_SYNC:
            case MO_PARAM_SCALEX:
            case MO_PARAM_SCALEY:
            case MO_PARAM_SCALEZ:
                valuebase.SetText( "1.0" );
                valuebase.SetType( MO_VALUE_FUNCTION );
                xvalue.AddSubValue( valuebase );
                break;

            case MO_PARAM_FUNCTION:
            case MO_PARAM_PHASE:
            case MO_PARAM_TRANSLATEX:
            case MO_PARAM_TRANSLATEY:
            case MO_PARAM_TRANSLATEZ:
            case MO_PARAM_ROTATEX:
            case MO_PARAM_ROTATEY:
            case MO_PARAM_ROTATEZ:
                valuebase.SetText( "0.0" );
                valuebase.SetType( MO_VALUE_FUNCTION );
                xvalue.AddSubValue( valuebase );
                break;

            case MO_PARAM_TEXTURE:
                valuebase.SetText( "default" );
                valuebase.SetType( MO_VALUE_TXT );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_3DMODEL:
            case MO_PARAM_OBJECT:
            case MO_PARAM_VIDEO:
            case MO_PARAM_TEXTUREFOLDER:
            case MO_PARAM_SOUND:
            case MO_PARAM_SCRIPT:
            case MO_PARAM_TEXT:
                valuebase.SetText( "" );
                valuebase.SetType( MO_VALUE_TXT );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_FONT:
                valuebase.SetText( "fonts/arial.ttf" );
                valuebase.SetType( MO_VALUE_TXT );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_FILTER:
                valuebase.SetText( "" );
                valuebase.SetType( MO_VALUE_TXT );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_INLET:
            case MO_PARAM_OUTLET:
                //valuebase.SetText( "" );
                //valuebase.SetType( MO_VALUE_TXT );
                //xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_NUMERIC:
                valuebase.SetText( "" );
                valuebase.SetType( MO_VALUE_NUM );
                valuebase.SetInt(0);
                break;
            case MO_PARAM_COMPOSE:
                valuebase.SetText( "" );
                valuebase.SetType( MO_VALUE_TXT );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                break;
            case MO_PARAM_VECTOR:
                valuebase.SetText( "0.0" );
                valuebase.SetType( MO_VALUE_NUM_FLOAT );
                xvalue.AddSubValue( valuebase );
                xvalue.AddSubValue( valuebase );
                break;

        }

        AddValue(xvalue);
    }
}

moValue&
moParam::GetValue( MOint i ) {

	if ( i == -1 )
		i = m_CurrentIndexValue;

	if (0<=i && i<(MOint)m_Values.Count() )
		return m_Values[i];
	else
		return m_Values[m_CurrentIndexValue];
}

void
moParam::SetIndexValue( int indexvalue ) {

	if (0<=indexvalue && indexvalue<(MOint)m_Values.Count() ) {
		m_CurrentIndexValue = indexvalue;
		m_bExternDataUpdated = false;
	}

}


int
moParam::GetIndexValue() const {

	return m_CurrentIndexValue;

}

void
moParam::NextValue() {
	if ( m_Values.Count() > 0 ) {
	    m_bExternDataUpdated = false;
		if ( m_CurrentIndexValue < ((MOint)m_Values.Count()-1) ) {
			m_CurrentIndexValue++;
		}
	} else m_CurrentIndexValue = -1;
}

void
moParam::PrevValue() {
	if ( m_Values.Count() > 0 ) {
	    m_bExternDataUpdated = false;
		if ( m_CurrentIndexValue > 0 ) {
			m_CurrentIndexValue--;
		}
	} else m_CurrentIndexValue = -1;
}

void
moParam::FirstValue() {
	if (m_Values.Count() > 0) {
	    m_bExternDataUpdated = false;
		m_CurrentIndexValue = 0;
	} else m_CurrentIndexValue = -1;
}

void
moParam::SetParamDefinition( moParamDefinition &p_paramdefinition ) {
	m_ParamDefinition = p_paramdefinition;
}


moParamDefinition&
moParam::GetParamDefinition() {

	return m_ParamDefinition;

}

moParam*
moParam::GetPtr() {
	return (this);
}

void
moParam::Update() {
	if (m_pExternData)
		m_bExternDataUpdated = true;
}

moData*
moParam::GetData() {

    moData* pReturnData  = NULL;

    ///dato modificado externamente
	if (m_pExternData && m_bExternDataUpdated) {
		pReturnData = m_pExternData;
	///dato original del config
	} else pReturnData = GetValue().GetSubValue().GetData();

    if ( m_ParamDefinition.GetInterpolation().IsOn() && pReturnData ) {
        pReturnData->Eval();
        pReturnData = m_ParamDefinition.GetInterpolation().InterpolateData( *pReturnData );
    }

	return pReturnData;
}

void
moParam::SetExternData( moData* p_pExternData) {
	m_pExternData = p_pExternData;
}


