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
  Andr�s Colubri

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
    return LoadLibrary(fn);
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

#endif
