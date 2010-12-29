/*******************************************************************************

                              moPrePlugin.h

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

#ifndef __MO_PRE_PLUGIN_H__
#define __MO_PRE_PLUGIN_H__

#include "moBasePlugin.h"
#include "moPreEffect.h"

class LIBMOLDEO_API moPreEffectFactory
{
public:
    moPreEffectFactory() {}
    virtual ~moPreEffectFactory();
    virtual moPreEffect* Create(void) = 0;
    virtual void Destroy(moPreEffect* fx) = 0;
};

typedef moPreEffectFactory*(MO_PLG_ENTRY *CreatePreEffectFactoryFunction)();
typedef void(MO_PLG_ENTRY *DestroyPreEffectFactoryFunction)();

class LIBMOLDEO_API moPrePlugin
{
public:
    moPreEffect **array;
    int n;

    CreatePreEffectFactoryFunction CreatePreEffectFactory;
    DestroyPreEffectFactoryFunction DestroyPreEffectFactory;

    moPreEffectFactory* m_factory;

    moPrePlugin() { handle = NULL; n = 0; array = NULL; m_factory = NULL;}
    moPrePlugin(moText plugin_file) { handle = NULL; n = 0; array = NULL; m_factory = NULL; Load(plugin_file); }
    virtual ~moPrePlugin() { if(handle != NULL) Unload(); }

    void Load(moText plugin_file);
    void Unload();
    moPreEffect* Create();
    bool Destroy(moPreEffect *preeffect);
    moText GetName() { return name; }
private:
    moText name;
    MOpluginHandle handle;
};

moDeclareExportedDynamicArray( moPrePlugin*, moPrePluginsArray );

LIBMOLDEO_API moPreEffect* moNewPreEffect(moText effect_name, moPrePluginsArray &plugins);
LIBMOLDEO_API bool moDeletePreEffect(moPreEffect* effect, moPrePluginsArray &plugins);

#endif
