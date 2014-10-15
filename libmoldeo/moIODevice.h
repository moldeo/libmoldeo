/*******************************************************************************

								moIODevice.h

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

#ifndef __MO_IO_DEVICE_H__
#define __MO_IO_DEVICE_H__
#include <moTypes.h>
#include <moMoldeoObject.h>
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

/*
template class LIBMOLDEO_API moDynamicArray<moIODevice*>;
typedef moDynamicArray<moIODevice*> moIODevicesArray;
*/
moDeclareExportedDynamicArray (moIODevice *, moIODevicesArray)
#endif							 /* __MO_IO_DEVICE_H__ */
