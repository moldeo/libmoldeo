/*******************************************************************************

                                moConsole.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                     *
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

//#define MO_DEBUG

#ifndef __MOCONSOLE_H
#define __MOCONSOLE_H


//TYPES GENERALES
#include "moTypes.h"
#include "moText.h"

//RESOURCES MANAGER
#include "moResourceManager.h"

//EFFECTS MANAGER
#include "moEffectManager.h"

//CONFIGURATIONS - STATES
#include "moConfig.h"
#include "moPresets.h"
#include "moConsoleState.h"

//IO MANAGER - PLUGINS
#include "moIODevicePlugin.h"
#include "moIODeviceManager.h"


#define MO_MAX_EFFECT   40
#define MO_MAX_PRESETS  9
#define MO_NO_EFFECT    -1

#define MO_CFG_EFFECT   0
#define MO_CFG_EFFECT_CONFIG 1
#define MO_CFG_EFFECT_LABEL	2
#define MO_CFG_EFFECT_PRE	3
#define MO_CFG_EFFECT_ON	4
#define MO_CFG_EFFECT_KEY	5

#define MO_CFG_ACCION_STR 0
#define MO_CFG_ACCION_COD 1
#define MO_CFG_ACCION_CODDISP 2


#define MAX_SELECCIONADOS 255

#define MO_DEF_SCREEN_WIDTH	800
#define MO_DEF_SCREEN_HEIGHT 600
#define MO_DEF_RENDER_WIDTH 800
#define MO_DEF_RENDER_HEIGHT 600

#define MO_SCREEN_WIDTH 800
#define MO_SCREEN_HEIGHT 600
#define MO_SCREEN_DEPTH	32


enum moConsoleParamIndex {
	CONSOLE_DEVICES,
	CONSOLE_PREEFFECT,
	CONSOLE_EFFECT,
	CONSOLE_POSTEFFECT,
	CONSOLE_MASTEREFFECT,
	CONSOLE_RESOURCES,
	CONSOLE_ON,
	CONSOLE_FULLDEBUG
};

/// Objeto consola
/**
 * <b>Consola Moldeo</b><br>
 * Este objeto implementa las funciones de inicialización necesarias para la comunicación
 * entre objetos de moldeo, dispositivos de entrada/salida, y recursos<br>
 * al ser a su vez un objeto moldeo, puede ser instanciada como tal<br>
 * @see moMoldeoObject
 * @see moPreEffect
 * @see moPostEffect
 * @see moMasterEffect
 * @see moIODevice
 * @see moResource
 * @see moMoldeoObjectType
 */

class LIBMOLDEO_API moConsole : public moMoldeoObject {

    public:

        /**
         * constructor genérico de la clase.
         */
        moConsole();


        /**
         * constructor genérico de la clase.
         */
        virtual ~moConsole();


        /// Inicializador predeterminado
        /**
         * Derivado de moAbstract, inicializa con parámetros predeterminados.
         */
        virtual MOboolean Init();


        /// Inicializador paramétrico.
        /**
         * Estos parámetros son especificados por la aplicación de línea de comando.
         * @param p_datapath directorio de acceso a los datos
         * @param p_consoleconfig nombre completo incluyendo directorio del archivo de configuración .mol
         * @param p_pIODeviceManager (opcional) puntero al administrador de dispositivos de entrada/salida
         * @param p_pResourceManager (opcional) puntero al administrador de recursos
         * @param p_render_to_texture_mode (opcional MO_RENDER_TO_TEXTURE_FBSCREEN ) modo de render a textura
         * @param p_screen_width (opcional MO_DEF_SCREEN_WIDTH) ancho de la pantalla de salida
         * @param p_screen_height (opcional MO_DEF_SCREEN_HEIGHT) alto de la pantalla de salida
         * @param p_render_width (opcional MO_DEF_RENDER_WIDTH) ancho de la textura de salida
         * @param p_render_height (opcional MO_DEF_RENDER_HEIGHT) alto de la textura de salida
         */

        virtual MOboolean Init( moText p_datapath,
						moText p_consoleconfig,
						moIODeviceManager* p_pIODeviceManager = NULL,
						moResourceManager *p_pResourceManager = NULL,
		               MOint p_render_to_texture_mode = MO_RENDER_TO_TEXTURE_FBSCREEN,
					   MOint p_screen_width = MO_DEF_SCREEN_WIDTH, MOint p_screen_height = MO_DEF_SCREEN_HEIGHT,
					   MOint p_render_width = MO_DEF_RENDER_WIDTH, MOint p_render_height = MO_DEF_RENDER_HEIGHT,
					   MO_HANDLE p_OpWindowHandle  = 0 );


        /// Procesa los eventos de los dispositivos de entrada/salida
        /**
         * Este función procesa los eventos regidos por el administrador de dispositivos
         * @see moIODeviceManager
         * @see moIODevice
         */
        virtual int  Interaction();


        /// Actualiza el estado de los recursos
        /**
         * Cada objeto moResource implementa su propia función de actualización de recursos.
         */
         virtual void Update();


        /// Dibuja
        /**
         * Cada objeto derivado de moEffect es dibujado según la jerarquía que los define.
         */
        virtual void Draw();


        /// Finalizador
        /**
         * finaliza liberando todos los recursos
         */
        virtual MOboolean Finish();


        /// inicializa el administrador de recursos
        /**
         * Crea un administrador nuevo o inicializa el predefinido.
         * @param pResourceManager
         * @param p_datapath
         * @param p_consoleconfig
         * @param p_render_to_texture_mode
         * @param p_screen_width
         * @param p_screen_height
         * @param p_render_width
         * @param p_render_height
         * @see moResourceManager
         * @see moResource
         * @see moConfig
         */

         virtual void InitResources(moResourceManager *pResourceManager,
						moText p_datapath,
						moConfig& p_consoleconfig,
		               MOint p_render_to_texture_mode = MO_RENDER_TO_TEXTURE_FBSCREEN,
					   MOint p_screen_width = MO_DEF_SCREEN_WIDTH, MOint p_screen_height = MO_DEF_SCREEN_HEIGHT,
					   MOint p_render_width = MO_DEF_RENDER_WIDTH, MOint p_render_height = MO_DEF_RENDER_HEIGHT,
                       MO_HANDLE p_OpWindowHandle = 0);


        /// asignación del administrador de dispositivos
        /**
         * Crea el administrador de dispositivos o asigna el predeterminado.
         * @param p_IODeviceManager
         * @see moIODeviceManager
         */

        void SetIODeviceManager( moIODeviceManager*	p_IODeviceManager );


        /// asignación de la definición del archivo de configuración de la consola
        /**
         * Fija las definiciones de los parámetros de este MOB. Si no se especifica parámetro
         * lo hará dentro del archivo de configuración propio.
         * @param p_configdefinition (opcional) puntero al objeto que será poblado con las definiciones de los parámetros de este MOB
         * @see moConfigDefinition
         * @see moMoldeoObject
         * @see moConfig
         */

        moConfigDefinition* GetDefinition( moConfigDefinition *p_configdefinition = NULL );

    protected:

        virtual MOulong GetTicks();
        virtual void GLSwapBuffers();

        moConsoleState			state;

        MOboolean				m_bExternalResources;

        MOboolean				m_bIODeviceManagerDefault;
        moIODeviceManager*		m_pIODeviceManager;
        moMoldeoObjects			m_MoldeoObjects;
        moEffectManager			m_EffectManager;

        int idebug,iligia,iborrado;

        MOfloat fps_current;
        MOfloat fps_mean;
        MOint fps_count;

        void LoadConnections();

        void LoadResources();
        void UnloadResources();
        void LoadIODevices();
        void UnloadIODevices();
        void LoadMasterEffects();
        void UnloadMasterEffects();
        void LoadPreEffects();
        void UnloadPreEffects();
        void LoadEffects();
        void UnloadEffects();
        void LoadPostEffects();
        void UnloadPostEffects();

        void InitializeAllEffects();
        void FinalizeAllEffects();

        void StartMasterEffects();
        void StopMasterEffects();

        void InitPrevTexture();

        void SaveStatus(int);
        void LoadStatus(int);

        MOint ConvertKeyNameToIdx(moText& name);

};

#endif
