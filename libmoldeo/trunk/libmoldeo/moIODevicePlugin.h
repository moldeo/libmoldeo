/*******************************************************************************

                              moIODevicePlugin.h

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
#include "moTypes.h"
#include "moArray.h"
#include "moBasePlugin.h"
#include "moIODeviceManager.h"


#ifndef __MO_IODEVICE_PLUGIN_H__
#define __MO_IODEVICE_PLUGIN_H__

/// clase base para una fábrica de plugins de dipositivos de E/S
/**
 * Comentar.
 */
class LIBMOLDEO_API moIODeviceFactory
{
public:
    moIODeviceFactory() {}
    virtual ~moIODeviceFactory();
    virtual moIODevice* Create(void) = 0;
    virtual void Destroy(moIODevice* fx) = 0;
};

typedef moIODeviceFactory*(MO_PLG_ENTRY *CreateIODeviceFactoryFunction)();
typedef void(MO_PLG_ENTRY *DestroyIODeviceFactoryFunction)();


/// clase base para un plugin de dipositivos de E/S
/**
 * Comentar.
 */
class LIBMOLDEO_API moIODevicePlugin
{
public:
    moIODevice **array;
    int n;

    CreateIODeviceFactoryFunction CreateIODeviceFactory;
    DestroyIODeviceFactoryFunction DestroyIODeviceFactory;

    moIODeviceFactory* m_factory;

    moIODevicePlugin() { handle = NULL; n=0; array = NULL; m_factory = NULL; }
    moIODevicePlugin(moText plugin_file) { m_factory = NULL; handle = NULL; n = 0; array = NULL; Load(plugin_file); }
    virtual ~moIODevicePlugin() { if(handle != NULL) Unload(); }

    void Load(moText plugin_file);
    void Unload();
    moIODevice* Create();
    bool Destroy(moIODevice *iodevice);
    moText GetName() { return name; }

private:
    moText name;
    MOpluginHandle handle;

};
moDeclareExportedDynamicArray( moIODevicePlugin*, moIODevicePluginsArray );

LIBMOLDEO_API moIODevice* moNewIODevice(moText iodevice_name, moIODevicePluginsArray &plugins);
LIBMOLDEO_API bool moDeleteIODevice(moIODevice* iodevice, moIODevicePluginsArray &plugins);

#endif
