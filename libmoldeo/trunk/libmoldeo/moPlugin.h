/*******************************************************************************

                              moPlugin.h

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

#ifndef __MO_PLUGIN_H__
#define __MO_PLUGIN_H__

#include "moBasePlugin.h"
#include "moEffect.h"


/// Definición de un plugin
/**
*   Se necesitan tres parámetros para reconocer un plugin. Su nombre para ser invocado, su tipo, y la posición del archivo.
*/
class LIBMOLDEO_API moPluginDefinition {

  public:

    moPluginDefinition() {
          m_Name = "";
          m_FullPath = "";
          m_MoldeoObjectType = MO_OBJECT_UNDEFINED;
    }

    moPluginDefinition( const moPluginDefinition &src ) {
        *this = src;
    }

    ///constructor
    /**
    * @param p_name el nombre del plugin tal cual se invocará luego
    * @param p_fullpath el nombre del archivo completo incluyendo la ruta
    * @param p_type el tipo de objeto que este plugin implementa
    */
    moPluginDefinition( const moText& p_name, const moText& p_fullpath, moMoldeoObjectType p_type ) {
          m_Name = p_name;
          m_FullPath = p_fullpath;
          m_MoldeoObjectType = p_type;
    }

    virtual ~moPluginDefinition() {}

    moPluginDefinition &operator = (const moPluginDefinition &src) {
       m_Name = src.m_Name;
       m_FullPath = src.m_FullPath;
       m_MoldeoObjectType = src.m_MoldeoObjectType;
       return *this;
    }

    /// Devuelve el nombre del plugin
    moText GetName() {
        return m_Name;
    }

    /// Devuelve la ruta y nombre completos del archivo del plugin
    moText GetFullPath() {
        return m_FullPath;
    }

    /// Devuelve el tipo de objeto que crea el plugin
    moMoldeoObjectType GetType() {
        return m_MoldeoObjectType;
    }

  protected:

    moText              m_Name;
    moText              m_FullPath;
    moMoldeoObjectType  m_MoldeoObjectType;


};

moDeclareExportedDynamicArray( moPluginDefinition, moPluginDefinitions )


/// Fábrica de efectos, clase abstracta pura a implementar para un plugin derivado de moEffect
/**
*   Define las funciones de creación y destrucción de un plugin.
*/
class LIBMOLDEO_API moEffectFactory
{
public:
    moEffectFactory() {}
    virtual ~moEffectFactory();
    virtual moEffect* Create(void) = 0;
    virtual void Destroy(moEffect* fx) = 0;
};

typedef moEffectFactory*(MO_PLG_ENTRY *CreateEffectFactoryFunction)();
typedef void(MO_PLG_ENTRY *DestroyEffectFactoryFunction)();


/// Plugin de efecto
/**
*   Este objeto se crea al invocar un plugin.
*   Crea y administra las instancias de objetos que este plugin implementa.
*/
class LIBMOLDEO_API moPlugin
{
    public:
        moEffect **array;
        int n;

        CreateEffectFactoryFunction CreateEffectFactory;
        DestroyEffectFactoryFunction DestroyEffectFactory;

        moEffectFactory* m_factory;

        moPlugin() { handle = NULL; n=0; array=NULL; m_factory = NULL;}
        moPlugin(moText plugin_file) { handle = NULL; n=0; array=NULL; m_factory = NULL; Load(plugin_file); }
        virtual ~moPlugin() { if(handle != NULL) Unload(); }

        void Load(moText plugin_file);
        void Unload();
        moEffect* Create();
        void Destroy(moEffect *effect);
        moText GetName() { return name; }

    private:
        moText name;
        MOpluginHandle handle;

};
/*
template class LIBMOLDEO_API moDynamicArray<moPlugin*>;
typedef moDynamicArray<moPlugin*> moPluginsArray;
*/
moDeclareExportedDynamicArray( moPlugin*, moPluginsArray )

LIBMOLDEO_API moEffect* moNewEffect(moText effect_name, moPluginsArray &plugins);
LIBMOLDEO_API void moDeleteEffect(moEffect* effect, moPluginsArray &plugins);

#endif
