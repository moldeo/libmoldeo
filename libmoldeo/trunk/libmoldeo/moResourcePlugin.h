/*******************************************************************************

                              moResourcePlugin.h

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

#include "moBasePlugin.h"
#include "moResourceManager.h"

#ifndef __MO_RESOURCE_PLUGIN_H__
#define __MO_RESOURCE_PLUGIN_H__

class LIBMOLDEO_API moResourceFactory
{
public:
    moResourceFactory() {}
    virtual ~moResourceFactory();
    virtual moResource* Create(void) = 0;
    virtual void Destroy(moResource* fx) = 0;
};

typedef moResourceFactory*(MO_PLG_ENTRY *CreateResourceFactoryFunction)();
typedef void(MO_PLG_ENTRY *DestroyResourceFactoryFunction)();

class LIBMOLDEO_API moResourcePlugin
{
public:
    moResource **array;
    int n;

    CreateResourceFactoryFunction CreateResourceFactory;
    DestroyResourceFactoryFunction DestroyResourceFactory;

    moResourceFactory* m_factory;

    moResourcePlugin() { handle = NULL; n=0; array = NULL; m_factory = NULL; }
    moResourcePlugin(moText plugin_file) { m_factory = NULL; handle = NULL; n = 0; array = NULL; Load(plugin_file); }
    virtual ~moResourcePlugin() { if(handle != NULL) Unload(); }

    void Load(moText plugin_file);
    void Unload();
    moResource* Create();
    bool Destroy(moResource *Resource);
    moText GetName() { return name; }
private:
    moText name;
    MOpluginHandle handle;
};

moDeclareExportedDynamicArray(moResourcePlugin*, moResourcePluginsArray);

LIBMOLDEO_API moResource* moNewResource(moText resource_name, moResourcePluginsArray &plugins);
LIBMOLDEO_API bool moDeleteResource(moResource* Resource, moResourcePluginsArray &plugins);

#endif
