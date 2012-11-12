/*******************************************************************************

                              moPostPlugin.h

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

#ifndef __MO_POST_PLUGIN_H__
#define __MO_POST_PLUGIN_H__

#include "moBasePlugin.h"
#include "moPostEffect.h"

class LIBMOLDEO_API moPostEffectFactory
{
public:
    moPostEffectFactory() {}
    virtual ~moPostEffectFactory();
    virtual moPostEffect* Create(void) = 0;
    virtual void Destroy(moPostEffect* fx) = 0;
};

typedef moPostEffectFactory*(MO_PLG_ENTRY *CreatePostEffectFactoryFunction)();
typedef void(MO_PLG_ENTRY *DestroyPostEffectFactoryFunction)();

class LIBMOLDEO_API moPostPlugin
{
public:
    moPostEffect **array;
    int n;

    CreatePostEffectFactoryFunction CreatePostEffectFactory;
    DestroyPostEffectFactoryFunction DestroyPostEffectFactory;

    moPostEffectFactory* m_factory;

    moPostPlugin() { handle = NULL; n = 0; array = NULL; m_factory = NULL;}
    moPostPlugin(moText plugin_file) { handle = NULL; n = 0; array = NULL; m_factory = NULL; Load(plugin_file); }
    ~moPostPlugin() { if(handle != NULL) Unload(); }

    void Load(moText plugin_file);
    void Unload();
    moPostEffect* Create();
    bool Destroy(moPostEffect *posteffect);
    moText GetName() { return name; }
private:
    moText name;
    MOpluginHandle handle;
};

moDeclareExportedDynamicArray( moPostPlugin*, moPostPluginsArray );

LIBMOLDEO_API moPostEffect* moNewPostEffect(moText effect_name, moPostPluginsArray &plugins);
LIBMOLDEO_API bool moDeletePostEffect(moPostEffect* effect, moPostPluginsArray &plugins);

#endif


