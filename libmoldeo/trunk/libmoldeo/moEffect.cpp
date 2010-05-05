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
  Andrés Colubri

*******************************************************************************/

// Clase padre de todos los efectos //

#include <moEffect.h>
#include <moArray.h>
#include <moDataManager.h>
#include <moFileManager.h>

moDefineDynamicArray( moEffectsArray)



moEffect::moEffect() {
	SetType( MO_OBJECT_EFFECT );
	devicecode = NULL;
	ncodes = 0;
}

moEffect::~moEffect() {

}

// Esta funcion debe ser llamada al comienzo en cada implementacion
// de la funcion virtual Init().  Contiene el codigo obligatorio.
// 1) Levanta el config de disco
// 2) el parametro del Syncro
// 3) Inicializa el efecto en la primera preconfiguración.
MOboolean
moEffect::PreInit() {
    moText texto;

	moText confignamecompleto;
	moText debug;

	devicecode = NULL;
	state.Init();

	if (!m_pResourceManager) return false;

	confignamecompleto = m_pResourceManager->GetDataMan()->GetDataPath();
	confignamecompleto +=  moSlash + moText( GetConfigName() );
    confignamecompleto +=  moText(".cfg");

	//MODebug2->Push("*****Inicializando efecto*****");//debug
        texto = moText("*****Initializing  ");
        texto += GetName();
        texto += moText("******");

	MODebug2->Push(texto);//debug
	if(state.fulldebug==MO_ACTIVATED) {
            texto = moText("Loading config:");
            MODebug2->Push(texto);//debug
        }
	if(state.fulldebug==MO_ACTIVATED) MODebug2->Push(confignamecompleto);//debug

	if(m_Config.LoadConfig(confignamecompleto) != MO_CONFIG_OK ) {
        MODebug2->Push(moText("Config file not found."));
		MODebug2->Error(moText("Config file invalid or not found: ")+ (moText)confignamecompleto);
		return false;//bad
	}

    isyncro = m_Config.GetParamIndex("syncro");
	iphase = m_Config.GetParamIndex("phase");
	if(isyncro==MO_PARAM_NOT_FOUND) MODebug2->Error(moText("syncro parameter missing."));
	if(iphase==MO_PARAM_NOT_FOUND) MODebug2->Error(moText("phase parameter missing."));

	if(state.fulldebug==MO_ACTIVATED) MODebug2->Push(moText("Initializing state"));//debug

	if(state.fulldebug==MO_ACTIVATED) MODebug2->Push(moText("Setting preconfigs..."));//debug

	//devicecode es llenado por la moConsole(por defecto)
	//si en el nombrefecto.cfg encontramos el parametro ":acciones" entonces tomamos las acciones
	//definidas arbitrariamente allí....
	//se tratará de mantener valuees como: MOACCIONES_ALPHA... para poder hacer un FADE
	//la idea es que sea completamente reconfigurable la interfaz del teclado

	moMoldeoObject::Init();

	return true;
}



// Esta funcion debe ser llamada al comienzo en cada implementacion
// de la funcion virtual Draw().  Contiene el codigo obligatorio.
// 1) toma el nuevo Syncro del config
// 2) fija el value del tempo dependiendo si esta o no syncronizado con el master
void moEffect::PreDraw( moTempo *tempogral,moEffectState* parentstate) {
	MOdouble syncrotmp;

	if(isyncro != MO_PARAM_NOT_FOUND) {
		moData *sync = m_Config.GetParam(isyncro).GetData();
		if (sync->Type()==MO_DATA_FUNCTION) state.tempo.syncro = sync->Fun()->Eval(state.tempo.ang);
		else state.tempo.syncro = sync->Double();
	}

    if(state.synchronized==MO_DEACTIVATED)
    {
        state.tempo.ticks = moGetTicks();
		state.tempo.getTempo();
    }
    else
    {
        syncrotmp = state.tempo.syncro;
		state.tempo = *tempogral;
		state.tempo.syncro = syncrotmp;
		state.tempo.getTempo();
		//if(state.fulldebug==MO_ACTIVATED) MODebug2->Push("SYNCRO: " + FloatToStr(state.tempo.syncro,3));
    }

	if(iphase != MO_PARAM_NOT_FOUND) {
		moData *phase = m_Config.GetParam(iphase).GetData();
		if (phase->Type()==MO_DATA_FUNCTION) state.tempo.ang+= phase->Fun()->Eval(state.tempo.ang);
		else state.tempo.ang+= phase->Double();
	}

	if(parentstate!=NULL) {
		//asginar parametros del state del padre al state del hijo
		state = *parentstate;
	}
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

	if(state.fulldebug==MO_ACTIVATED) MODebug2->Message(moText("Cargando codigos de dispositivo especificos..."));

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
				coddisp = consolaesarray->IODevices().Get(j)->GetCode(strcod);
				if(coddisp != -1) break; //lo encontramos "j" es el nro de disp y "coddisp" el id
			}

			if((accioncod>=0) &&(accioncod<ncodes)) {
				if(coddisp==-1) {
				    texto += moText("\n");
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

	if(state.fulldebug==MO_ACTIVATED) MODebug2->Push(moText("Codes loaded."));
}


void moEffect::Interaction(moIODeviceManager *consolaes) {

	Update(consolaes->GetEvents());

}

void moEffect::SetColor( moValue& color, moValue& alpha, moEffectState& pstate ) {
	glColor4f(  color[MO_RED].Fun()->Eval(pstate.tempo.ang) * pstate.tintr,
                color[MO_GREEN].Fun()->Eval(pstate.tempo.ang) * pstate.tintg,
                color[MO_BLUE].Fun()->Eval(pstate.tempo.ang) * pstate.tintb,
				color[MO_ALPHA].Fun()->Eval(pstate.tempo.ang) *
				alpha[0].GetData()->Fun()->Eval(pstate.tempo.ang) * pstate.alpha);

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
	switch(blending) {
		//ALPHA DEPENDENT
		case MO_BLENDING_TRANSPARENCY:
			//TRANSPARENCY [Rs * As] + [Rd *(1 -As)] = As*(Rs-Rd) + Rd
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			break;
		case MO_BLENDING_ADDITIVEALPHA:
			//ADDITIVE WITH TRANSPARENCY: Rs*As + Rd*Ad
			glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
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
    this->state.on = MO_ON;
}

void moEffect::TurnOff() {
    this->state.on = MO_OFF;
}

void moEffect::Enable() {
    this->state.enabled = MO_ON;
}

void moEffect::Disable() {
    this->state.enabled = MO_OFF;
}

void moEffect::SwitchOn() {
    this->state.on*= -1;
}

void moEffect::SwitchEnabled() {
    this->state.enabled*= -1;
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
