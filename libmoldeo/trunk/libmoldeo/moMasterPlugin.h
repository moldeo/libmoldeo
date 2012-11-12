/*******************************************************************************

                              moMasterPlugin.h

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
#include "moBasePlugin.h"
#include "moMasterEffect.h"

#ifndef __MO_MASTER_PLUGIN_H__
#define __MO_MASTER_PLUGIN_H__

class LIBMOLDEO_API moMasterEffectFactory
{
public:
    moMasterEffectFactory() {}
    virtual ~moMasterEffectFactory();
    virtual moMasterEffect* Create(void) = 0;
    virtual void Destroy(moMasterEffect* fx) = 0;
};

typedef moMasterEffectFactory*(MO_PLG_ENTRY *CreateMasterEffectFactoryFunction)();
typedef void(MO_PLG_ENTRY *DestroyMasterEffectFactoryFunction)();

class LIBMOLDEO_API moMasterPlugin
{
public:
    moMasterEffect **array;
    int n;

    CreateMasterEffectFactoryFunction CreateMasterEffectFactory;
    DestroyMasterEffectFactoryFunction DestroyMasterEffectFactory;

    moMasterEffectFactory* m_factory;

    moMasterPlugin() { handle = NULL; n=0; array = NULL; m_factory = NULL; }
    moMasterPlugin(moText plugin_file) { m_factory = NULL; handle = NULL; n = 0; array = NULL; Load(plugin_file); }
    virtual ~moMasterPlugin() { if(handle != NULL) Unload(); }

    void Load(moText plugin_file);
    void Unload();
    moMasterEffect* Create();
    bool Destroy(moMasterEffect *mastereffect);
    moText GetName() { return name; }
private:
    moText name;
    MOpluginHandle handle;
};

/*
template class LIBMOLDEO_API moDynamicArray<moMasterPlugin*>;
typedef moDynamicArray<moMasterPlugin*> moMasterPluginsArray;
*/
moDeclareExportedDynamicArray( moMasterPlugin*, moMasterPluginsArray );

LIBMOLDEO_API moMasterEffect* moNewMasterEffect(moText effect_name, moMasterPluginsArray &plugins);
LIBMOLDEO_API bool moDeleteMasterEffect(moMasterEffect* effect, moMasterPluginsArray &plugins);

#endif

