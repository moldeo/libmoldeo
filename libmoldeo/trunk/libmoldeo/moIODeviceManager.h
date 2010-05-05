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
  Andrs Colubri

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
 * ejemplos tpicos de estos objetos son el teclado (moKeyboard), el mouse (moMouse)
 *
 */
class LIBMOLDEO_API moIODevice:public moMoldeoObject
{
	public:
		/**
		 * constructor genrico de la clase.
		 */
		moIODevice ();

		/**
		 * constructor genrico de la clase.
		 */
		virtual ~ moIODevice ();

		/**
		 * constructor genrico de la clase.
		 */
		virtual void Update (moEventList *) = 0;

		/**
		 * constructor genrico de la clase.
		 */
		virtual MOboolean Init () = 0;

		/**
		 * constructor genrico de la clase.
		 */
								 //get the status of the specific devicecode
		virtual MOswitch GetStatus (MOdevcode) = 0;

		/**
		 * constructor genrico de la clase.
		 */
		virtual void SetValue (MOdevcode, MOint);

		/**
		 * constructor genrico de la clase.
		 */
		virtual void SetValue (MOdevcode, MOfloat);

		/**
		 * constructor genrico de la clase.
		 */
		virtual void SetValue (MOdevcode, moData);

		/**
		 * constructor genrico de la clase.
		 */
		virtual void SetValue (MOdevcode, MOlong, MOpointer);

		/**
		 * constructor genrico de la clase.
		 */
								 //in case we need to get a buffer data pointer
		virtual MOpointer GetPointer (MOdevcode);

		/**
		 * constructor genrico de la clase.
		 */
								 //get the actual value of the specific devicecode
		virtual MOint GetValue (MOdevcode) = 0;

		/**
		 * constructor genrico de la clase.
		 */
								 //get the actual value of the specific devicecode
		virtual moData GetValue (MOdevcode, moDataType);

		/**
		 * constructor genrico de la clase.
		 */
								 //get the actual value of the specific devicecode
		virtual MOint GetValue (MOdevcode, MOint);

		/**
		 * constructor genrico de la clase.
		 */
								 //get the actual value of the specific devicecode
		virtual MOint GetNValues (MOdevcode);

		/**
		 * constructor genrico de la clase.
		 */
		virtual MOswitch SetStatus (MOdevcode, MOswitch) = 0;

		/**
		 * constructor genrico de la clase.
		 */
								 //todos los codes estan definidos con un name en ascii por si las moscas
		virtual MOdevcode GetCode (moText) = 0;

		/**
		 * constructor genrico de la clase.
		 */
		virtual MOboolean Finish () = 0;

		/**
		 * constructor genrico de la clase.
		 */
		MOint GetId ()
		{
			return m_DeviceId;
		}

		/**
		 * constructor genrico de la clase.
		 */
		void SetId (MOint p_id)
		{
			m_DeviceId = p_id;
		} protected:MOint m_DeviceId;
		//device id relative
};

moDeclareExportedDynamicArray (moIODevice *, moIODevicesArray);

#include "moIODevicePlugin.h"

/**
 * administrador de dispositivos de entrada/salida y de eventos
 * a) crea nuevos dispositivos (moIODevice)
 * b) consulta los dispositivos por valores
 * c) administra la lista de eventos generados por el OS
 */
class LIBMOLDEO_API moIODeviceManager:public moAbstract
{
	public:
		/**
		 * constructor genrico de la clase.
		 */
		moIODeviceManager ();

		/**
		 * constructor genrico de la clase.
		 */
		virtual ~ moIODeviceManager ();

		/**
		 * constructor genrico de la clase.
		 */
		moIODevice * NewIODevice (moText p_devname, moText p_configname,
			moText p_labelname, moMoldeoObjectType p_type,
			int paramindex = -1, int valueindex = -1);

		/**
		 * constructor genrico de la clase.
		 */
		MOboolean RemoveIODevice (MOint p_ID);

		/**
		 * constructor genrico de la clase.
		 */
		moIODevicesArray & IODevices ();

		/**
		 * constructor genrico de la clase.
		 */
		void Update ();

		/**
		 * constructor genrico de la clase.
		 */
		virtual MOboolean Init ();

		/**
		 * constructor genrico de la clase.
		 */
		virtual MOboolean Finish ();

		/**
		 * constructor genrico de la clase.
		 */
		MOswitch GetStatus (MOdevcode);

		/**
		 * constructor genrico de la clase.
		 */
		MOswitch SetStatus (MOdevcode, MOswitch);

		/**
		 * constructor genrico de la clase.
		 */
		MOdevcode GetCode (char *);

		/**
		 * constructor genrico de la clase.
		 */
		moEventList * GetEvents ();
	private:moEventList * Events;
	moIODevicesArray m_IODevices;
	moIODevicePluginsArray m_Plugins;
	virtual void PollEvents ();	 //aplicacion de la libreria de SDL(hay que hacer la propia)
};
#endif							 /*  */
