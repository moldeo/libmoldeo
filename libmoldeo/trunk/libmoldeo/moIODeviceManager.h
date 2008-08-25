/*******************************************************************************

                                moIODeviceManager.h

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

#ifndef __MO_IODEVICE_H
#define __MO_IODEVICE_H

#include "moTypes.h"
#include "moText.h"
#include "moMoldeoObject.h"
#include "moDeviceCode.h"
#include "moEventList.h"
#include "moRenderManager.h"



/**
 * dispositivo de entrada salida (Input Output Device)
 * ejemplos típicos de estos objetos son el teclado (moKeyboard), el mouse (moMouse)
 *
 */

class LIBMOLDEO_API moIODevice : public moMoldeoObject {

	public:

        /**
         * constructor genérico de la clase.
         */
		moIODevice();

        /**
         * constructor genérico de la clase.
         */
		virtual ~moIODevice();

        /**
         * constructor genérico de la clase.
         */
        virtual void Update(moEventList*) = 0;

        /**
         * constructor genérico de la clase.
         */
        virtual MOboolean Init() = 0;

        /**
         * constructor genérico de la clase.
         */
        virtual MOswitch GetStatus( MOdevcode) = 0;//get the status of the specific devicecode

        /**
         * constructor genérico de la clase.
         */
        virtual void SetValue( MOdevcode, MOint );

        /**
         * constructor genérico de la clase.
         */
        virtual void SetValue( MOdevcode, MOfloat );

        /**
         * constructor genérico de la clase.
         */
        virtual void SetValue( MOdevcode, moData );

        /**
         * constructor genérico de la clase.
         */
        virtual void SetValue( MOdevcode, MOlong, MOpointer );

        /**
         * constructor genérico de la clase.
         */
        virtual MOpointer GetPointer( MOdevcode);//in case we need to get a buffer data pointer

        /**
         * constructor genérico de la clase.
         */
        virtual MOint GetValue( MOdevcode) = 0;//get the actual value of the specific devicecode

        /**
         * constructor genérico de la clase.
         */
        virtual moData GetValue( MOdevcode, moDataType );//get the actual value of the specific devicecode

        /**
         * constructor genérico de la clase.
         */
        virtual MOint GetValue( MOdevcode, MOint);//get the actual value of the specific devicecode

        /**
         * constructor genérico de la clase.
         */
        virtual MOint GetNValues( MOdevcode);//get the actual value of the specific devicecode

        /**
         * constructor genérico de la clase.
         */
        virtual MOswitch SetStatus( MOdevcode,MOswitch) = 0;

        /**
         * constructor genérico de la clase.
         */
        virtual MOdevcode GetCode( moText) = 0;//todos los codes estan definidos con un name en ascii por si las moscas

        /**
         * constructor genérico de la clase.
         */
        virtual MOboolean Finish() = 0;

        /**
         * constructor genérico de la clase.
         */
        MOint	GetId() { return m_DeviceId; }

        /**
         * constructor genérico de la clase.
         */
        void	SetId( MOint p_id ) { m_DeviceId = p_id; }

	protected:

		MOint		m_DeviceId;//device id relative

};

/*
template class LIBMOLDEO_API moDynamicArray<moIODevice*>;
typedef moDynamicArray<moIODevice*> moIODevicesArray;
*/
moDeclareExportedDynamicArray(moIODevice*, moIODevicesArray)

#include "moIODevicePlugin.h"




/**
 * administrador de dispositivos de entrada/salida y de eventos
 * a) crea nuevos dispositivos (moIODevice)
 * b) consulta los dispositivos por valores
 * c) administra la lista de eventos generados por el OS
*/

class LIBMOLDEO_API moIODeviceManager : public moAbstract {

	public:

        /**
         * constructor genérico de la clase.
         */
		moIODeviceManager();

        /**
         * constructor genérico de la clase.
         */
		virtual ~moIODeviceManager();

        /**
         * constructor genérico de la clase.
         */
		moIODevice*		NewIODevice( moText p_devname,  moText p_configname, moText p_labelname, moMoldeoObjectType p_type, int paramindex = -1, int valueindex = -1 );

        /**
         * constructor genérico de la clase.
         */
        MOboolean		RemoveIODevice( MOint p_ID );


        /**
         * constructor genérico de la clase.
         */
        moIODevicesArray&	IODevices();

        /**
         * constructor genérico de la clase.
         */
        void Update();

        /**
         * constructor genérico de la clase.
         */
        virtual MOboolean Init();

        /**
         * constructor genérico de la clase.
         */
        virtual MOboolean Finish();

        /**
         * constructor genérico de la clase.
         */
        MOswitch GetStatus(MOdevcode);

        /**
         * constructor genérico de la clase.
         */
        MOswitch SetStatus( MOdevcode,MOswitch);

        /**
         * constructor genérico de la clase.
         */
        MOdevcode GetCode( char*);

        /**
         * constructor genérico de la clase.
         */
        moEventList*	GetEvents();

	private:

		moEventList				*Events;
		moIODevicesArray		m_IODevices;
		moIODevicePluginsArray	m_Plugins;

		virtual void PollEvents();//aplicacion de la libreria de SDL(hay que hacer la propia)
};


#endif


