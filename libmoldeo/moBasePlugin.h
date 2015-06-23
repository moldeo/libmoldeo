/*******************************************************************************

                              moBasePlugin.h

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

#ifndef __MO_BASE_PLUGIN_H__
#define __MO_BASE_PLUGIN_H__

#include "moText.h"

#if defined(_WIN32)
  #include <windows.h>
  typedef HINSTANCE MOpluginHandle;
  #define MO_PLG_API __declspec(dllexport)
  #define MO_PLG_ENTRY __stdcall
#else
  #include <dlfcn.h>
  typedef void* MOpluginHandle;
  #define MO_PLG_API
  #define MO_PLG_ENTRY
  #ifdef MO_MACOSX
    #define moPluginExtension moText(".dylib")
  #endif
  #ifdef MO_LINUX
    #define moPluginExtension moText(".so")
  #endif
#endif

inline MOpluginHandle moLoadPlugin(moText fn)
{
    #if defined(_WIN32)
    return LoadLibraryA(fn);
    #else
    return dlopen(fn, RTLD_LAZY);
    #endif
}

inline void moUnloadPlugin(MOpluginHandle &handle)
{
    #if defined(_WIN32)
    FreeLibrary(handle);
    handle = NULL;
    #else
    dlclose(handle);
    handle = NULL;
    #endif
}

/// Definición de un plugin
/**
*   Se necesitan tres parámetros para reconocer un plugin. Su nombre para ser invocado, su tipo, y la posición del archivo.
*/
class LIBMOLDEO_API moPluginDefinition {

  public:

    moPluginDefinition() {
          m_Name = moText("");
          m_FullPath = moText("");
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

moDeclareExportedDynamicArray( moPluginDefinition, moPluginDefinitions );

#endif
