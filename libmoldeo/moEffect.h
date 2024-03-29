/*******************************************************************************

                               moEffect.h

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

#ifndef __MO_EFFECT_H__
#define __MO_EFFECT_H__

class moEffect;

#include "moAbstract.h"
#include "moTempo.h"
#include "moArray.h"
#include "moMathFunctionIndex.h"
#include "moIODeviceManager.h"
#include "moConfig.h"
#include "moEffectState.h"
#include "moPreConfig.h"
#include "moPresets.h"
#include "moMotion.h"
#include "moText.h"
#include "moMoldeoObject.h"
#include "moResourceManager.h"
//resources
#include "moRenderManager.h"
#include "moMathManager.h"
#include "moTextureManager.h"
#include "moFontManager.h"
#include "moSoundManager.h"
#include "moScriptManager.h"
#include "mo3dModelManager.h"
#include "moRenderManager.h"
#include "moGUIManager.h"

#include "moArcBall.h"


/// clase base para objetos dibujables
/** \if spanish
 * clase base para definir Efectos.
 * un efecto es el objeto dibujable en la consola (simil de una capa)
 * en funci�n del orden en el que ha sido cargado en la consola de efectos
 * Es importante implementar las funciones de Init, Draw, Update e Interaction que son
 * necesarias para las operaciones de Inicializaci�n, Dibujado, Actualizaci�n e Interacci�n.
 * \endif
 * \if english
 * Base class to derive an effect object
 * the objects derived from this class are drawable objects
 * each plugin that is necessarely drawable by the principal console
 * instantiate one of this
 * \endif
 * @see moMoldeoObject
 * @see moPreEffect
 * @see moPostEffect
 * @see moMasterEffect
 * @see moIODevice
 * @see moResource
 * @see moMoldeoObjectType
 */
class LIBMOLDEO_API moEffect : public moMoldeoObject
{
public:

        /**
         * constructor gen�rico de la clase.
         */
        moEffect();

        /**
         * destructor gen�rico de la clase.
         */
        virtual ~moEffect();

		/**
		* Inicializador gen�rico derivado de moAbstract.
		* @see moAbstract
		*/
        virtual MOboolean Init() = 0;

		/**
		* Funci�n de dibujado del efecto
		*/
		virtual void Draw( moTempo*,moEffectState* parentstate=NULL) = 0;

		/**
		* Finalizador gen�rico derivado de moAbstract. Es importante que esta funci�n libere los recursos que han sido creados en la funci�n Init().
		* @see moAbstract
		*/
        virtual MOboolean Finish() = 0;

		/**
		* Este inicializador se ejecuta en primer lugar dentro de la funci�n Init, y efectua las inicializaciones esp�c�ficas de cualquier derivado de moEffect�.
		* a) carga del archivo de configuraci�n
		* b) verifica que existan par�metros necesarios espec�ficos de los efectos como ser SYNCRO y PHASE
		* c) llamar a MoldeoObject::Init() para la inclusi�n y parseo de todos los par�metros...
		*/
        virtual MOboolean PreInit();

		/**
		* PreDraw debe ejecutarse en la funci�n Draw, antes de cualquier operaci�n de dibujo.
		* a) toma el nuevo Syncro establecido del config o
		* b) Sincroniza el beat
		*/
        virtual void PreDraw( moTempo*,moEffectState* parentstate=NULL);
        virtual void BeginDraw( moTempo*,moEffectState* parentstate=NULL);
        virtual void EndDraw();

    virtual MOboolean PreFinish();

    virtual void Interaction( moIODeviceManager* );
    virtual void LoadCodes( moIODeviceManager* );
	virtual moConfigDefinition * GetDefinition( moConfigDefinition *p_configdefinition = NULL );//retreive the parameters definition of the Moldeo Object

    /**
    * Devuelve el estado del objeto
    */
    virtual const moEffectState& GetEffectState();
    virtual bool SetEffectState(  const moEffectState& p_state );
    virtual const moMobState& GetState();
    virtual bool SetState( const moMobState& p_MobState );


    //virtual void SetNombreConfig(moText texto);

    void SetColor( const moColorRGBA& color_rgba, double alpha, moEffectState& pstate );
    void SetColor( moValue& color, moValue& alpha, moEffectState& pstate  );
    void SetColor( moParam& color, moParam& alpha, moEffectState& pstate );
    void SetBlending( moBlendingModes blending );
    void SetPolygonMode( moPolygonModes polygonmode );

	virtual void TurnOn();
    virtual void TurnOff();
    virtual void Enable();
    virtual void Disable();
    virtual void SwitchOn();
    virtual void SwitchEnabled();
    virtual void Synchronize();
    virtual void Unsynchronize();
    virtual bool Synchronized();
    virtual void BeatPulse();
    virtual double TempoDelta( double p_delta );
    virtual double GetTempoDelta() const;
    double SetTempoDelta( double p_delta );
    virtual double TempoFactor( double p_factor );
    virtual double GetTempoFactor() const;

    virtual double Alpha( double alpha );
    virtual double GetAlpha() const;
    virtual double Amplitude( double amplitude );
    virtual double GetAmplitude() const;
    virtual double Magnitude( double magnitude );
    virtual double GetMagnitude() const;

    virtual void TintCSV( double tintc, double tints, double tint );
    virtual moVector3f GetTintCSV() const;
    virtual moVector3f GetTintRGB() const;


    virtual void Play();
    virtual void Stop();
    virtual void Pause();
    virtual void PlayPause();
    virtual void Continue();

    virtual moTimerState State();

    virtual const moText& ToJSON();

	public:

		moPresets			presets;
		moMotion			mov;
		moDeviceCodeList*	devicecode; //accion vs moDeviceCode, personalizable
		MOint				ncodes;

		MOint				isyncro;
		MOint				iphase;

		// Agregado para poder asignar efectos a teclas arbitrarias de las 4 filas del teclado.
		// Luego hay que discutirlo/implementarlo mejor...
		MOint				keyidx;
    ArcBallT   ArcBall;
    //ArcBall_t


  protected:

        moEffectState		m_EffectState;
        moObject3D          m_Effect3D;

        virtual int ScriptCalling(moLuaVirtualMachine& vm, int iFunctionNumber);
        void RegisterFunctions();


        int luaPlay( moLuaVirtualMachine& vm );
        int luaPause( moLuaVirtualMachine& vm );
        int luaStop( moLuaVirtualMachine& vm );
        int luaState( moLuaVirtualMachine& vm );
        int luaPlayPause( moLuaVirtualMachine& vm );

        int luaGetTicks(moLuaVirtualMachine& vm);
        int luaSetTicks(moLuaVirtualMachine& vm);

        ///habilita deshabilita
        int luaDisable(moLuaVirtualMachine& vm);
        int luaEnable(moLuaVirtualMachine& vm);
        ///cambia el estado de un efecto (sus valores)
        ///tinta, velocidad, alpha, etc...
        int luaSetEffectState(moLuaVirtualMachine& vm);
        int luaGetEffectState(moLuaVirtualMachine& vm);

        moInlet*        InletTime;
        moInlet*        InletTimems;
        moInlet*        InletMilliseconds;
        moInlet*        InletTimes;
        moInlet*        InletSeconds;
        moInlet*        InletT;
        moInlet*        InletTempo;
        moInlet*        InletMouseX;
        moInlet*        InletMouseY;
        moInlet*        InletMouseFactor;

        moInlet*        InletMouseButtonLeft;
        moInlet*        InletMouseButtonRight;
        moInlet*        InletMouseButtonMiddle;

        moInlet*        InletMouseXButtonLeft;
        moInlet*        InletMouseXButtonRight;
        moInlet*        InletMouseXButtonMiddle;

        moInlet*        InletMouseYButtonLeft;
        moInlet*        InletMouseYButtonRight;
        moInlet*        InletMouseYButtonMiddle;

        moInlet*        InletMouseXWheel;
        moInlet*        InletMouseYWheel;

        //moInlet*        InletSpecialShift,InletSpecialAlt,InletSpecialCtrl;
        MOdouble        mousex,mousey;
        MOdouble        mousefactor,mousebuttonleft,mousebuttonright,mousebuttonmiddle;
        MOdouble        mousexbuttonleft,mouseybuttonleft;
        MOdouble        mousexbuttonright,mouseybuttonright;
        MOdouble        mousexbuttonmiddle,mouseybuttonmiddle;
        MOdouble        mousexwheel,mouseywheel;

        virtual void ScriptExeDraw();
        virtual void ScriptExeInteraction();

};

moDeclareExportedDynamicArray( moEffect*, moEffectsArray);

#endif
