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
  Andrs Colubri

*******************************************************************************/

#ifndef __MO_EFFECT_H__
#define __MO_EFFECT_H__

#include "moAbstract.h"
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

/// clase base para objetos dibujables
/** \if spanish
 * clase base para definir Efectos.
 * un efecto es el objeto dibujable en la consola (simil de una capa)
 * en funcin del orden en el que ha sido cargado en la consola de efectos
 * Es importante implementar las funciones de Init, Draw, Update e Interaction que son
 * necesarias para las operaciones de Inicializacin, Dibujado, Actualizacin e Interaccin.
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
class LIBMOLDEO_API moEffect:public moMoldeoObject
{
	public:

		/**
		 * constructor genrico de la clase.
		 */
		moEffect ();

		/**
		 * destructor genrico de la clase.
		 */
		virtual ~ moEffect ();

		/**
		 * Inicializador genrico derivado de moAbstract.
		 * @see moAbstract
		 */
		virtual MOboolean Init () = 0;

		/**
		 * Funcin de dibujado del efecto
		 */
		virtual void Draw (moTempo *, moEffectState * parentstate = NULL) = 0;

		/**
		 * Finalizador genrico derivado de moAbstract. Es importante que esta funcin libere los recursos que han sido creados en la funcin Init().
		 * @see moAbstract
		 */
		virtual MOboolean Finish () = 0;

		/**
		 * Este inicializador se ejecuta en primer lugar dentro de la funcin Init, y efectua las inicializaciones espcficas de cualquier derivado de moEffect.
		 * a) carga del archivo de configuracin
		 * b) verifica que existan parmetros necesarios especficos de los efectos como ser SYNCRO y PHASE
		 * c) llamar a MoldeoObject::Init() para la inclusin y parseo de todos los parmetros...
		 */
		virtual MOboolean PreInit ();

		/**
		 * PreDraw debe ejecutarse en la funcin Draw, antes de cualquier operacin de dibujo.
		 * a) toma el nuevo Syncro establecido del config o
		 * b) Sincroniza el beat
		 */
		virtual void PreDraw (moTempo *, moEffectState * parentstate = NULL);
		virtual MOboolean PreFinish ();

		virtual void Interaction (moIODeviceManager *);
		virtual void LoadCodes (moIODeviceManager *);
								 //retreive the parameters definition of the Moldeo Object
		virtual moConfigDefinition *GetDefinition (moConfigDefinition * p_configdefinition = NULL);

		//virtual void SetNombreConfig(moText texto);
		static void SetColor (moValue & color, moValue & alpha,
			moEffectState & pstate);
		static void SetBlending (moBlendingModes blending);
		static void SetPolygonMode (moPolygonModes polygonmode);
		void TurnOn ();
		void TurnOff ();
		void Enable ();
		void Disable ();
		void SwitchOn ();
		void SwitchEnabled ();

	public:

		moEffectState state;
		moPresets presets;
		moMotion mov;
								 //accion vs moDeviceCode, personalizable
		moDeviceCodeList *devicecode;
		MOint ncodes;

		MOint isyncro;
		MOint iphase;

		// Agregado para poder asignar efectos a teclas arbitrarias de las 4 filas del teclado.
		// Luego hay que discutirlo/implementarlo mejor...
		MOint keyidx;
};

/*
template class LIBMOLDEO_API moDynamicArray<moEffect*>;
typedef moDynamicArray<moEffect*> moEffectsArray;
*/
moDeclareExportedDynamicArray (moEffect *, moEffectsArray)
#endif
