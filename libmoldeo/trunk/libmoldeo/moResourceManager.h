/*******************************************************************************

                                moResourceManager.h

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
  Andr�s Colubri

*******************************************************************************/


#ifndef __MORESOURCESMANAGER_H
#define __MORESOURCESMANAGER_H

#include "moTypes.h"
#include "moArray.h"
#include "moMoldeoObject.h"
#include "moParam.h"

#define MAX_RESOURCES	20
#define MO_CFG_RESOURCE 0
#define MO_CFG_RESOURCE_CONFIG 1
#define MO_CFG_RESOURCE_LABEL 2

/// Tipos de recursos que se pueden implementar
/**
 * cada tipo especifica el tipo de contenido tratado por el recurso, esta lista es bastante completa
 * pero podr�a ser modificada en el futuro
 * cada tipo aqu� especificado puede estar implementado por un recurso interno del core o un plugin
 */

enum moResourceType {
	/*UNDEFINED*/
	MO_RESOURCETYPE_UNDEFINED = 0,
	/*INTERNAL CORE*/
	MO_RESOURCETYPE_FILE,
	MO_RESOURCETYPE_VIDEO,
	MO_RESOURCETYPE_NET,
	MO_RESOURCETYPE_FILTER,
	MO_RESOURCETYPE_TIME,
	MO_RESOURCETYPE_DATA,
	MO_RESOURCETYPE_TEXTURE,
	MO_RESOURCETYPE_SOUND,
	MO_RESOURCETYPE_MODEL,
	MO_RESOURCETYPE_FONT,
	MO_RESOURCETYPE_GUI,
	MO_RESOURCETYPE_RENDER,
	MO_RESOURCETYPE_GL,
	MO_RESOURCETYPE_FB,
	MO_RESOURCETYPE_SHADER,
	MO_RESOURCETYPE_SCRIPT,
	MO_RESOURCETYPE_MATH,
	MO_RESOURCETYPE_DEBUG,
	/*EXTRAS*/
	MO_RESOURCETYPE_TEXT,
	MO_RESOURCETYPE_MEMORY,
	MO_RESOURCETYPE_XML,
	MO_RESOURCETYPE_DB,
	MO_RESOURCETYPE_DYNAMICENGINE,

	/*RESERVED*/
	MO_RESOURCETYPE_RESERVED1,
	MO_RESOURCETYPE_RESERVED2,
	MO_RESOURCETYPE_RESERVED3,
	MO_RESOURCETYPE_RESERVED4
};///< enum tipos de recursos

#define MO_CFG_RESOURCE 0
#define MO_CFG_RESOURCE_CONFIG 1
#define MO_CFG_RESOURCE_LABEL 2

/// Recurso ( objeto para cargar y manipular objetos f�sicos de datos de im�genes, audio, video, 3d, 2d, fuentes, shaders y de cualquier otro tipo extendible por un plugin )
/**
 *
 *  Los recursos son aquellos objetos que implementan funciones de tratamiento de datos, generalmente
 *  localizados en dispositivos f�sicos, locales o remotos,o que tienen por destino alg�n tipo de representaci�n.
 *
 *  Aquellos recursos predeterminados dentro de la librer�a de Moldeo conforman la columna vertebral de las funcionalidades
 *  del Core de Moldeo.
 *  Estos son: moRenderManager, moGLManager, moTextureManager, mo3dModelManager, moShaderManager,
 *  moVideoManager, moSoundManager, moScriptManager, moTimeManager, moFBManager, moMathManager
 *
 *  Un plugin derivado de moResourcePlugin es usado t�picamente para acceder a archivos de distintos formatos para luego
 *  administrar estos contenidos en un arreglo interno din�mico evitando redundancias de datos.
 *
 *  Un ejemplo ser�a el moTextureManager, que administrar la carga de im�genes en texturas de opengl.
 *
 *  Otro ejemplo ser�a el mo3dModelManager, que administrar la carga de objeto 3d, en este caso del formato .3ds. El mo3dModelManager a su vez
 *  depende del moTextureManager para cargar las texturas especificadas por los materiales de los modelos tridimensionales. Mantener esta coherencia
 *  de los datos en Moldeo es una de las caracter�sticas importantes de su arquitectura.
 * @see moResourceType
 * @see moMoldeoObject
 * @see moTextureManager
 * @see mo3dModelManager
 */
class LIBMOLDEO_API moResource : public moMoldeoObject {

	public:

        /**
         * constructor gen�rico de la clase.
         */
		moResource();

        /**
         * constructor gen�rico de la clase.
         */
		virtual	~moResource();

        /**
         * constructor gen�rico de la clase.
         */
		virtual MOboolean Init();

        /**
         * constructor gen�rico de la clase.
         */
		virtual MOboolean Finish();

		/**
         * devuelve el tipo de recurso
         * @see moResourceType
         */
		moResourceType			GetResourceType();

		/**
         * devuelve el nombre del recurso
         */
		moText					GetResourceName();

	protected:
		void					SetResourceType( moResourceType p_restype );

	private:
		moResourceType			m_ResourceType;
		moText					m_ResourceName;

};

typedef moResource* moResourcePtr;

moDeclareExportedDynamicArray(moResourcePtr, moResources)

class	moDebugManager;
class	moNetManager;
class	moFileManager;
class	moVideoManager;
class	moFilterManager;
class	moTimeManager;
class	moDataManager;
class	moFBManager;
class	moGLManager;
class	moRenderManager;
class	moShaderManager;
class	moMathManager;
class	moFontManager;
class	moGUIManager;
class	moScriptManager;
class	moTextureManager;
class	moSoundManager;
class	mo3dModelManager;

#include "moResourcePlugin.h"

/// Administrador de recursos
/**
 * los recursos son aquellos objetos que implementan funciones de tratamiento de datos, generalmente de dispositivos f�sicos,
 * como son im�genes, videos, bases de dato
 * @see moResource
 */
class LIBMOLDEO_API moResourceManager : public moAbstract {

	public:

        /**
         * constructor gen�rico de la clase.
         */
		moResourceManager();
		virtual ~moResourceManager();

        /**
         * inicializaci�n
         * se especifican los par�metros generales para el acceso a los datos
         *
         */
		virtual MOboolean	Init(	const moText& p_datapath,
									moConfig& p_consoleconfig,
									MOint p_render_to_texture_mode,
									MOint p_screen_width,
									MOint p_screen_height,
									MOint p_render_width,
									MOint p_render_height,
									MO_HANDLE p_OpWindowHandle = 0 );
        /**
         * Finalizador
         */
         virtual MOboolean	Finish();

        /**
         * crea un nuevo recurso
         *
         *
         */
		MOboolean				NewResource( moText p_resname, int paramindex = -1, int valueindex = -1 );

        /**
         * crea un nuevo recurso
         *
         *
         */
		MOboolean				AddResource( moResource* m_pResource );

        /**
         * crea un nuevo recurso
         *
         *
         */
		MOboolean				RemoveResource( MOint p_ID );

        /**
         * crea un nuevo recurso
         *
         *
         */
		MOint					GetResourceId( moText p_resname );

        /**
         * crea un nuevo recurso
         *
         *
         */
		moText					GetResourceName( MOint p_ID );

        /**
         * crea un nuevo recurso
         *
         *
         */
		moResource*				GetResource( MOint p_ID );

        /**
         * crea un nuevo recurso
         *
         *
         */
		moResourceType			GetResourceType( MOint p_ID );

        /**
         * crea un nuevo recurso
         *
         *
         */
		moResource*				GetResourceByType( moResourceType p_type );

        /**
         * crea un nuevo recurso
         *
         *
         */
		moResources&			Resources();

		//predefined managers: 14
        /**
         * crea un nuevo recurso
         *
         *
         */
		moFileManager*			GetFileMan() { return MOFileMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moVideoManager*			GetVideoMan() { return MOVideoMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moFilterManager*		GetFilterMan() { return MOFilterMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moNetManager*			GetNetMan() { return MONetMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moDataManager*			GetDataMan() { return MODataMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moTimeManager*			GetTimeMan() { return MOTimeMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moFBManager*			GetFBMan() { return MOFBMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moGLManager*			GetGLMan() { return MOGLMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moRenderManager*		GetRenderMan() { return MORenderMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moMathManager*			GetMathMan() { return MOMathMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moShaderManager*		GetShaderMan() { return MOShaderMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moFontManager*			GetFontMan() { return MOFontMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moGUIManager*			GetGuiMan() { return MOGuiMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moScriptManager*		GetScriptMan() { return MOScriptMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moTextureManager*		GetTextureMan() { return MOTextureMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moSoundManager*			GetSoundMan() { return MOSoundMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		mo3dModelManager*		GetModelMan() { return MOModelMan; }

        /**
         * crea un nuevo recurso
         *
         *
         */
		moDebugManager*			GetDebugMan() { return MODebugMan; }

	private:

		//predefined managers
		moFileManager*			MOFileMan;
		moVideoManager*			MOVideoMan;
		moFilterManager*		MOFilterMan;
		moNetManager*			MONetMan;
		moTimeManager*			MOTimeMan;
		moDataManager*			MODataMan;
		moFBManager*			MOFBMan;
		moGLManager*			MOGLMan;
		moRenderManager*		MORenderMan;
		moShaderManager*		MOShaderMan;
		moMathManager*			MOMathMan;
		moFontManager*			MOFontMan;
		moGUIManager*			MOGuiMan;
		moScriptManager*		MOScriptMan;
		moTextureManager*		MOTextureMan;
		moSoundManager*			MOSoundMan;
		mo3dModelManager*		MOModelMan;
		moDebugManager*			MODebugMan;

		moResources				m_Resources;

		moResourcePluginsArray	m_Plugins;

};

#include "moDebugManager.h"
#include "moNetManager.h"
#include "moFileManager.h"
#include "moFontManager.h"
#include "moGUIManager.h"
#include "moScriptManager.h"
#include "mo3dModelManager.h"

#include "moShaderManager.h"
#include "moFilterManager.h"
#include "moTimeManager.h"
#include "moDataManager.h"
#include "moTextureManager.h"
#include "moVideoManager.h"
#include "moFBManager.h"
#include "moGLManager.h"
#include "moRenderManager.h"
#include "moMathManager.h"
#include "moSoundManager.h"

#endif
