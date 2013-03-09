/*******************************************************************************

                              moEffect.cpp

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

// Clase padre de todos los efectos //

#include "moEffect.h"
#include <moArray.h>
#include <moDataManager.h>
#include <moFileManager.h>


moDefineDynamicArray(moEffectsArray)



moEffect::moEffect() {
	SetType( MO_OBJECT_EFFECT );
	devicecode = NULL;
	ncodes = 0;
}

moEffect::~moEffect() {

}


const
moEffectState& moEffect::GetEffectState() {
    return m_EffectState;
}

bool moEffect::SetEffectState(  const moEffectState& p_state ) {

    //TODO: check valid states!
    m_MobState = (moMobState&) p_state;
    m_EffectState = p_state;
    return true;

}

const
moMobState& moEffect::GetState() {
    return (moMobState&)m_EffectState;
}

bool
moEffect::SetState( const moMobState& p_MobState ) {

    //TODO: check things before commit changes
    m_MobState = p_MobState;
    (moMobState&)(m_EffectState) = p_MobState;
    return true;
}


// Esta funcion debe ser llamada al comienzo en cada implementacion
// de la funcion virtual Init().  Contiene el codigo obligatorio.
// 1) Levanta el config de disco
// 2) el parametro del Syncro
// 3) Inicializa el efecto en la primera preconfiguración.
MOboolean
moEffect::PreInit() {

	moText debug;

	devicecode = NULL;
	m_EffectState.Init();

    /** Crea INLETS INTERNOS, es decir que no tienen un parametro asociado... (especificamente para su uso generico*/
    moInlet* Inlet = new moInlet();
    if (Inlet) {
      //Inlet->Init( "tempo", m_Inlets.Count(), param.GetPtr() );
      //param.SetExternData( Inlet->GetData() );
      ((moConnector*)Inlet)->Init( moText("time"), m_Inlets.Count(), MO_DATA_NUMBER_DOUBLE );
      m_Inlets.Add(Inlet);
    }

    Inlet = new moInlet();
    if (Inlet) {
      //Inlet->Init( "tempo", m_Inlets.Count(), param.GetPtr() );
      //param.SetExternData( Inlet->GetData() );
      ((moConnector*)Inlet)->Init( moText("t"), m_Inlets.Count(), MO_DATA_NUMBER_DOUBLE );
      m_Inlets.Add(Inlet);
    }

    Inlet = new moInlet();
    if (Inlet) {
      //Inlet->Init( "tempo", m_Inlets.Count(), param.GetPtr() );
      //param.SetExternData( Inlet->GetData() );
      ((moConnector*)Inlet)->Init( moText("tempo"), m_Inlets.Count(), MO_DATA_NUMBER_DOUBLE );
      m_Inlets.Add(Inlet);
    }

	if (!m_pResourceManager) return false;

    ///Carga el config (definicion y archivo, corrige, etc)
    ///Asigna al config y resuelve variables como texturas, sonidos, etc...
    if (moMoldeoObject::Init()) {
        ///Al fin luego de levantar todas las configuraciones, creamos los conectores (Inlets <NO INTERNOS> y Outlets)
        moMoldeoObject::CreateConnectors();
    } else return false;

    isyncro = m_Config.GetParamIndex("syncro");
	iphase = m_Config.GetParamIndex("phase");
	if(isyncro==MO_PARAM_NOT_FOUND) MODebug2->Error(moText("syncro parameter missing."));
	if(iphase==MO_PARAM_NOT_FOUND) MODebug2->Error(moText("phase parameter missing."));


	if(m_EffectState.fulldebug==MO_ACTIVATED) MODebug2->Push(moText("Initializing m_EffectState"));//debug
	if(m_EffectState.fulldebug==MO_ACTIVATED) MODebug2->Push(moText("Setting preconfigs..."));//debug

	//devicecode es llenado por la moConsole(por defecto)
	//si en el nombrefecto.cfg encontramos el parametro ":acciones" entonces tomamos las acciones
	//definidas arbitrariamente allí....
	//se tratará de mantener valuees como: MOACCIONES_ALPHA... para poder hacer un FADE
	//la idea es que sea completamente reconfigurable la interfaz del teclado

  MODebug2->Message("moEffect::PreInit > OK! for object: " + GetName()+ " config: " + GetConfigName() + " label: " + GetLabelName() );

	return true;
}

void moEffect::ScriptExeDraw() {
    if (moScript::IsInitialized()) {
        if (ScriptHasFunction("Draw")) {
            SelectScriptFunction("Draw");
            RunSelectedFunction();
        }
    }
}

void moEffect::ScriptExeInteraction() {
    if (moScript::IsInitialized()) {
        if (ScriptHasFunction("Interaction")) {
            SelectScriptFunction("Interaction");
            RunSelectedFunction();
        }
    }
}


// Esta funcion debe ser llamada al comienzo en cada implementacion
// de la funcion virtual Draw().  Contiene el codigo obligatorio.
// 1) toma el nuevo Syncro del config
// 2) fija el value del tempo dependiendo si esta o no syncronizado con el master
void moEffect::PreDraw( moTempo *tempogral,moEffectState* parentstate) {

  BeginDraw( tempogral, parentstate );

}

void moEffect::BeginDraw( moTempo *tempogral,moEffectState* parentstate) {

	MOdouble syncrotmp;

	if(isyncro != MO_PARAM_NOT_FOUND) {
		moData *sync = m_Config.GetParam(isyncro).GetData();
		if (sync) {
		  moMathFunction* pFun = sync->Fun();
      if (sync->Type()==MO_DATA_FUNCTION && pFun) {
        //m_EffectState.tempo.syncro = pFun->Eval(m_EffectState.tempo.ang);
        m_EffectState.tempo.syncro = pFun->Eval();
      }
      else m_EffectState.tempo.syncro = sync->Double();
		}

		/**código alternativo*/
		//m_EffectState.tempo.syncro = m_Config.Fun(isyncro).Eval( m_EffectState.tempo.ang );
	}

    if(m_EffectState.synchronized==MO_DEACTIVATED)
    {
        //m_EffectState.tempo.ticks = moGetTicks();
        ///Clock independiente
        m_EffectState.tempo.Duration();
        m_EffectState.tempo.getTempo();
    }
    else
    {
        syncrotmp = m_EffectState.tempo.syncro;
        m_EffectState.tempo = *tempogral;
        m_EffectState.tempo.syncro = syncrotmp;
        m_EffectState.tempo.getTempo();
        //if(m_EffectState.fulldebug==MO_ACTIVATED) MODebug2->Push("SYNCRO: " + FloatToStr(m_EffectState.tempo.syncro,3));
    }

	if(iphase != MO_PARAM_NOT_FOUND) {
		moData *phase = m_Config.GetParam(iphase).GetData();
		if (phase) {
		  moMathFunction* pFun = phase->Fun();
          if (phase->Type()==MO_DATA_FUNCTION && pFun) {
            //m_EffectState.tempo.ang+= pFun->Eval(m_EffectState.tempo.ang);
            m_EffectState.tempo.ang+= pFun->Eval();
          }
          else m_EffectState.tempo.ang+= phase->Double();
        }
	}

	if(parentstate!=NULL) {
		//asginar parametros del state del padre al state del hijo
		m_EffectState = *parentstate;
	}

	if (m_Inlets.Count()>2) {
	  moInlet* InletTime = m_Inlets[0];
	  moInlet* InletT = m_Inlets[1];
	  moInlet* InletTempo = m_Inlets[2];
	  if (InletTime) {
            if (InletTime->GetData()) InletTime->GetData()->SetDouble( m_EffectState.tempo.ang );
    }
	  if (InletT) {
        if (InletT->GetData()) InletT->GetData()->SetDouble( m_EffectState.tempo.ang );
    }
	  if (InletTempo) {
        if (InletTempo->GetData()) InletTempo->GetData()->SetDouble( moMathd::FMod( m_EffectState.tempo.ang , moMathd::TWO_PI ) );
    }
  }

  ScriptExeRun();

}

void moEffect::EndDraw() {

  ScriptExeDraw();

}


// Esta funcion debe ser llamada al comienzo en cada implementacion
// de la funcion virtual Finish().  Contiene el codigo obligatorio.
MOboolean moEffect::PreFinish() {
	//vaciamos las listas de codigos de acciones
	if(devicecode!=NULL)
		delete[] devicecode;
	devicecode = NULL;
	return true;
}

void
moEffect::LoadCodes(moIODeviceManager *consolaesarray) {

	moText texto;
	MOint nroparam;
	MOuint i,j,k;
	MOint coddisp,accioncod;
	moText strcod;

	if(m_EffectState.fulldebug==MO_ACTIVATED) MODebug2->Message(moText("Cargando codigos de dispositivo especificos..."));

	nroparam = m_Config.GetParamIndex("codes");
	if(nroparam==MO_PARAM_NOT_FOUND) return;//se va, no hay codigos
	m_Config.SetCurrentParamIndex(nroparam);
	m_Config.FirstValue();
	ncodes = (MOint)m_Config.GetValuesCount(nroparam);
	devicecode = new moDeviceCodeList [ncodes];

#ifdef MO_DEBUG
        texto = moText("\nCargando codigos efecto ");
        texto += GetName();
        texto += moText(" ncodes: ");
        texto += IntToStr(ncodes);
	//printf(texto);
	MODebug2->Log( texto );
#endif
	if(ncodes>0)
	for( i = 0; i < (MOuint)ncodes; i++) {
		m_Config.SetCurrentValueIndex(nroparam,i);
		for( k = 1; k < m_Config.GetParam().GetValue().GetSubValueCount(); k++) {
			accioncod = i;
			coddisp = -1;
			strcod = m_Config.GetParam().GetValue().GetSubValue(k).Text();
			for( j=0 ; j < consolaesarray->IODevices().Count(); j++) {
			  moIODevice* pIODevice;
			  pIODevice = consolaesarray->IODevices().GetRef(j);
			  if (pIODevice) {
			    if (strcod.Trim().Length()>0) {
            coddisp = pIODevice->GetCode(strcod);
			    } else {
			      MODebug2->Error("string code is empty: [" + moText(strcod) +"].");
          }

			  } else {

			    MODebug2->Error("Device id:" + IntToStr(j) +" is null.");

        }
				if(coddisp != -1) break; //lo encontramos "j" es el nro de disp y "coddisp" el id
			}

			if((accioncod>=0) &&(accioncod<ncodes)) {
				if(coddisp==-1) {
				    texto = moText("\n");
            texto += GetConfigName();
            texto += moText(".cfg: no se encontró en ningun dispositivo el codigo de dispositivo correspondiente a: ");
            texto += strcod;
					MODebug2->Error(texto);
				} else {
					devicecode[accioncod].Add(j,coddisp); //agregar un cod disp a la lista
#ifdef MO_DEBUG
					texto = moText("\naccioncod: ");
					texto += IntToStr(accioncod);
					texto += moText(" strcod: ");
					texto += strcod;
					texto += moText(" dispositivo: ");
					texto += IntToStr(j);
					texto += moText(" codisp: ");
					texto += IntToStr(coddisp);
					MODebug2->Log(texto);
#endif
				}
			} else {
				MODebug2->Error("error: codigo de la accion fuera de rango");
			}

		}
	}

	if(m_EffectState.fulldebug==MO_ACTIVATED) MODebug2->Push(moText("Codes loaded."));
}


void moEffect::Interaction(moIODeviceManager *consolaes) {

	Update(consolaes->GetEvents());

}

void moEffect::SetColor( moValue& color, moValue& alpha, moEffectState& pstate ) {
	glColor4f(  color[MO_RED].Eval() * pstate.tintr,
                color[MO_GREEN].Eval() * pstate.tintg,
                color[MO_BLUE].Eval() * pstate.tintb,
				color[MO_ALPHA].Eval() *
				alpha[0].Eval() * pstate.alpha);

}

void moEffect::SetColor( moParam& color, moParam& alpha, moEffectState& pstate ) {
      moVector4d vRGBA( color[MO_SELECTED][MO_RED].Eval(),
                        color[MO_SELECTED][MO_GREEN].Eval(),
                       color[MO_SELECTED][MO_BLUE].Eval(),
                       color[MO_SELECTED][MO_ALPHA].Eval() );

      MOfloat alphav = alpha.GetData()->Eval();

      glColor4f(  vRGBA.X()*pstate.tintr,
                  vRGBA.Y()*pstate.tintg,
                  vRGBA.Z()*pstate.tintb,
                  vRGBA.W()*pstate.alpha*alphav );
}

void moEffect::SetPolygonMode( moPolygonModes polygonmode ) {

    switch( polygonmode ) {

        case MO_POLYGONMODE_FILL:
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            break;

        case MO_POLYGONMODE_LINE:
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            glBindTexture(GL_TEXTURE_2D, 0);
            break;

        case MO_POLYGONMODE_POINT:

            break;
        default:
            break;
    }

}


void moEffect::SetBlending( moBlendingModes blending ) {
  glEnable (GL_BLEND);
	switch(blending) {
		//ALPHA DEPENDENT
		case MO_BLENDING_TRANSPARENCY:
			//TRANSPARENCY [Rs * As] + [Rd *(1 -As)] = As*(Rs-Rd) + Rd
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			break;
		case MO_BLENDING_ADDITIVEALPHA:
			//ADDITIVE WITH TRANSPARENCY: Rs*As + Rd*Ad
			/** DOESNT WORK NICELY BECAUSE DST_ALPHA DOESNT AFFECT FINAL FRAMEBUFFER */
			glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

            /** THIS WORKS LIKE A CHARM*/
			//ADDITIVE WITH SRC TRANSPARENCY: Rs*As + Rd
			//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			break;

		//NON ALPHA
		case MO_BLENDING_MIXING:
			//MIXING [Rs *( 1 - Rd )] + [ Rd * 1] = Rs + Rd - Rs*Rd
			//additive without saturation
			glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE );
			break;
		case MO_BLENDING_MULTIPLY:
			//MULTIPLY: [Rs * Rd] + [Rd * 0] = Rs * Rd
			glBlendFunc( GL_DST_COLOR, GL_ZERO );
			break;
		case MO_BLENDING_EXCLUSION:
			//EXCLUSION: [Rs *(1 - Rd)] + [Rd *(1 - Rs)] = Rs + Rd - 2*Rs*Rd
			glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);//
			break;
		case MO_BLENDING_ADDITIVE:
			//ADDITIVE Rs+Rd
			glBlendFunc( GL_ONE, GL_ONE );
			break;
		case MO_BLENDING_OVERLAY:
			//OVERLAY: 2*Rs*Rd
			glBlendFunc( GL_DST_COLOR, GL_SRC_COLOR );
			break;
		case MO_BLENDING_SUBSTRACTIVE:
			//SUBSTRACTIVE [Rs *( 1 - Rd )] + [ Rd * 0] = Rs - Rs*Rd
			//substractive
			glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ZERO );
			break;
		case MO_BLENDING_SATURATE:
			// [Rs * min(As,1-Ad) ] + [ Rd * Ad]
			//
			glBlendFunc( GL_SRC_ALPHA_SATURATE,  GL_DST_ALPHA);
			break;
			//Multiply mode:(a*b)
			//Average mode:(a+b)/2
			//Screen mode:  f(a,b) = 1 -(1-a) *(1-b)
			//Difference mode:  f(a,b) = |a - b|
			//Negation mode:  f(a,b) = 1 - |1 - a - b|
			//Exclusion mode f(a,b) = a + b - 2ab or f(a,b) = average(difference(a,b),negation(a,b))
			//Overlay mode f(a,b) =   	2ab(for a < ) 1 - 2 *(1 - a) *(1 - b)(else)
			//Color dodge mode:  f(a,b) = a /(1 - b)
			//Color burn mode:  f(a,b) = 1 -(1 - a) / b
			//Inverse color dodge mode:  f(a,b) = b /(1 - a)
			//Inverse color burn mode:  f(a,b) = 1 -(1 - b) / a
		default: //alpha transparent
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			break;
	}

}

void moEffect::TurnOn() {
    m_EffectState.Activate();
    Activate();
}

void moEffect::TurnOff() {
    m_EffectState.Deactivate();
    Deactivate();
}

void moEffect::Enable() {
    m_EffectState.enabled = MO_ON;
}

void moEffect::Disable() {
    m_EffectState.enabled = MO_OFF;
}

void moEffect::SwitchOn() {
    ( m_EffectState.Activated() ) ? m_EffectState.Deactivate() : m_EffectState.Activate();
}

void moEffect::SwitchEnabled() {
    this->m_EffectState.enabled*= -1;
}


void moEffect::Synchronize() {
    this->m_EffectState.synchronized = MO_ACTIVATED;
}


void moEffect::Unsynchronize() {
    this->m_EffectState.synchronized = MO_DEACTIVATED;
}

bool moEffect::Synchronized() {
    return (this->m_EffectState.synchronized == MO_ACTIVATED);
}

void moEffect::BeatPulse() {
    this->m_EffectState.synchronized = MO_DEACTIVATED;
    this->m_EffectState.tempo.BeatPulse(moGetTicksAbsolute());
}


double
moEffect::TempoDelta( double p_delta ) {

    if (p_delta!=0.0) this->Unsynchronize();

    m_EffectState.tempo.delta+=p_delta;
    if(m_EffectState.tempo.delta>2.0)
        m_EffectState.tempo.delta = 2.0;
    else
    if( m_EffectState.tempo.delta < 0.005 )
        m_EffectState.tempo.delta = 0.0;

    return m_EffectState.tempo.delta;
}

double
moEffect::GetTempoDelta() const {
    return m_EffectState.tempo.delta;
}


double
moEffect::TempoFactor( double p_factor ) {
    m_EffectState.tempo.factor+=p_factor;
    if(m_EffectState.tempo.factor>50.0)
        m_EffectState.tempo.factor = 50.0;
    else
    if(m_EffectState.tempo.factor<1.0)
        m_EffectState.tempo.factor = 1.0;
    return m_EffectState.tempo.factor;
}

double
moEffect::GetTempoFactor() const {
    return m_EffectState.tempo.factor;
}


double moEffect::Alpha( double alpha ) {
    m_EffectState.alpha+= alpha;
    if(m_EffectState.alpha>=1.0)
        m_EffectState.alpha=1.0;
    else
    if(m_EffectState.alpha<=0.0)
        m_EffectState.alpha=0.0;
    return m_EffectState.alpha;
}

double moEffect::GetAlpha() const {
    return m_EffectState.alpha;
}

double
moEffect::Amplitude( double amplitude ) {
    m_EffectState.amplitude = amplitude;
    return m_EffectState.amplitude;
}

double
moEffect::GetAmplitude() const {
    return m_EffectState.amplitude;
}

double
moEffect::Magnitude( double magnitude ) {
    m_EffectState.magnitude = magnitude;
    return m_EffectState.magnitude;
}

double
moEffect::GetMagnitude() const {
    return m_EffectState.magnitude;
}

void
moEffect::TintCSV( double tintc, double tints, double tint ) {
    m_EffectState.tint+= tint;
    m_EffectState.tintc+= tintc;
    m_EffectState.tints+= tints;

    //circular (0..1) > (0..1) > (0..1)
    if (m_EffectState.tintc>1.0) {
        m_EffectState.tintc = 0.0;
    } else if (m_EffectState.tintc<0.0) {
        m_EffectState.tintc = 1.0;
    }

    //lineal [0 .. 1.0]
    if (m_EffectState.tints>1.0) {
        m_EffectState.tints = 1.0;
    } else if (m_EffectState.tints<0.0) {
        m_EffectState.tints = 0.0;
    }

    //lineal 0..1.0
    if (m_EffectState.tint>1.0) {
        m_EffectState.tint = 1.0;
    } else if (m_EffectState.tint<0.0) {
        m_EffectState.tint = 0.0;
    }


    m_EffectState.CSV2RGB();
}


moVector3f
moEffect::GetTintCSV() const {
    return moVector3f( m_EffectState.tintc, m_EffectState.tints, m_EffectState.tint );
}

moVector3f
moEffect::GetTintRGB() const {
    return moVector3f( m_EffectState.tintr, m_EffectState.tintg, m_EffectState.tintb );
}



void moEffect::Play() {
    Unsynchronize();
    return this->m_EffectState.tempo.Start();
}

void moEffect::Stop(){
    Unsynchronize();
    return this->m_EffectState.tempo.Stop();
}

void moEffect::Pause(){
    Unsynchronize();
    return this->m_EffectState.tempo.Pause();
}

void moEffect::Continue(){
    Unsynchronize();
    return this->m_EffectState.tempo.Continue();
}

moTimerState moEffect::State() {
    return this->m_EffectState.tempo.State();
}

/*
MO_PARAM_ALPHA,			//value type: NUM or FUNCTION
MO_PARAM_COLOR,			//value type: NUM[4] or FUNCTION[4] or
MO_PARAM_SYNC,			//value type: NUM or FUNCTION
MO_PARAM_TEXT,			//value type: TXT or LNK
MO_PARAM_TEXTURE,		//value type: TXT or LNK
MO_PARAM_VIDEO,		//value type: TXT or LNK
MO_PARAM_SOUND,		//value type: TXT or LNK
MO_PARAM_FUNCTION,		//value type: NUM or FUNCTION
MO_PARAM_SCRIPT,		//value type: TXT or LNK
MO_PARAM_TRA,			//value type: NUM[3] or FUNCTION[3]
MO_PARAM_ROT,			//value type: NUM[3] or FUNCTION[3]
MO_PARAM_SCA			//value type: NUM[3] or FUNCTION[3]
*/
moConfigDefinition *
moEffect::GetDefinition( moConfigDefinition *p_configdefinition ) {

	p_configdefinition = moMoldeoObject::GetDefinition(p_configdefinition);
	p_configdefinition->Add( moText("alpha"), MO_PARAM_ALPHA );
	p_configdefinition->Add( moText("color"), MO_PARAM_COLOR );
	p_configdefinition->Add( moText("syncro"), MO_PARAM_SYNC );
	p_configdefinition->Add( moText("phase"), MO_PARAM_PHASE );
	return p_configdefinition;
}

void moEffect::RegisterFunctions()
{
    ///first inherit methods from MoldeoObjects
    moMoldeoObject::RegisterFunctions();

    ///register our own methods starting with RegisterBaseFunction
    RegisterBaseFunction("Play");//0
    RegisterFunction("Pause");//1
    RegisterFunction("Stop");//2
    RegisterFunction("State");//3
    RegisterFunction("Enable");//4
    RegisterFunction("Disable");//5

    RegisterFunction("SetTicks");//6
    RegisterFunction("GetTicks");//7

    RegisterFunction("GetEffectState");//8
    RegisterFunction("SetEffectState");//9

    ResetScriptCalling();

}

int moEffect::ScriptCalling(moLuaVirtualMachine& vm, int iFunctionNumber)
{

    switch ( iFunctionNumber - m_iMethodBase )
    {
        case 0:
            ResetScriptCalling();
            return luaPlay(vm);
        case 1:
            ResetScriptCalling();
            return luaPause(vm);
        case 2:
            ResetScriptCalling();
            return luaStop(vm);
        case 3:
            ResetScriptCalling();
            return luaState(vm);

        case 4:
            ResetScriptCalling();
            return luaEnable(vm);
        case 5:
            ResetScriptCalling();
            return luaDisable(vm);



        case 6:
            ResetScriptCalling();
            return luaSetTicks(vm);
        case 7:
            ResetScriptCalling();
            return luaGetTicks(vm);

        ///for this Effect
        case 8:
            ResetScriptCalling();
            return luaGetEffectState(vm);
        case 9:
            ResetScriptCalling();
            return luaSetEffectState(vm);

        default:
            NextScriptCalling();
            return moMoldeoObject::ScriptCalling( vm, iFunctionNumber );
	}
}


int moEffect::luaPlay( moLuaVirtualMachine& vm ) {

    lua_State *luastate = (lua_State *) vm;

    Play();

    return 0;
}

int moEffect::luaPause( moLuaVirtualMachine& vm ) {

    lua_State *luastate = (lua_State *) vm;

    Pause();

    return 0;
}

int moEffect::luaStop( moLuaVirtualMachine& vm ) {

    lua_State *luastate = (lua_State *) vm;

    Stop();

    return 0;
}

int moEffect::luaState( moLuaVirtualMachine& vm ) {

    lua_State *luastate = (lua_State *) vm;

    moTimerState elstate = State();
    int retstate = (int) elstate;
    lua_pushnumber( luastate, (lua_Number) retstate);

    return 1;
}

int moEffect::luaSetTicks(moLuaVirtualMachine& vm)
{
    lua_State *luastate = (lua_State *) vm;

    MOint ticksint = (MOint) lua_tonumber (luastate, 1);

    //this->SetTicks(ticksint);

    return 0;
}

int moEffect::luaGetTicks(moLuaVirtualMachine& vm)
{
    lua_State *luastate = (lua_State *) vm;

    lua_pushnumber(luastate, (lua_Number) moGetTicks() );

    return 1;
}

int moEffect::luaEnable(moLuaVirtualMachine& vm)
{
    lua_State *luastate = (lua_State *) vm;

    TurnOn();

    return 0;
}

int moEffect::luaDisable(moLuaVirtualMachine& vm)
{
    lua_State *luastate = (lua_State *) vm;

    TurnOff();

    return 0;
}



int moEffect::luaSetEffectState(moLuaVirtualMachine& vm) {

    lua_State *luastate = (lua_State *) vm;


    m_EffectState.alpha = (MOfloat) lua_tonumber (luastate, 1);
    m_EffectState.tint = (MOfloat) lua_tonumber (luastate, 2);
    m_EffectState.tintr = (MOfloat) lua_tonumber (luastate, 3);
    m_EffectState.tintg = (MOfloat) lua_tonumber (luastate, 4);
    m_EffectState.tintb = (MOfloat) lua_tonumber (luastate, 5);
    m_EffectState.tempo.ang = (MOfloat) lua_tonumber (luastate, 7);

    return 0;

}

int moEffect::luaGetEffectState(moLuaVirtualMachine& vm) {
    lua_State *luastate = (lua_State *) vm;

    lua_pushnumber(luastate, (lua_Number) m_EffectState.alpha );
    lua_pushnumber(luastate, (lua_Number) m_EffectState.tint );
    lua_pushnumber(luastate, (lua_Number) m_EffectState.tintr );
    lua_pushnumber(luastate, (lua_Number) m_EffectState.tintg );
    lua_pushnumber(luastate, (lua_Number) m_EffectState.tintb );
    lua_pushnumber(luastate, (lua_Number) m_EffectState.tempo.ang );

    return 0;
}
