/*******************************************************************************

								moFileRead.h

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
  Andrs Colubri

*******************************************************************************/

<<<<<<<.mine
/*==============================================

Class:   moFileRead

Description: use to read files from hard disc or other local devices.
Specially useful for long videos or data files, because it implements
asynchronous reads with a thread for each file.

Todo:	Multiple threads for multiple files

Example:

 //set the file to read from
moFileRead::moIODevice::SettValue( CHUNKS_SIZE,  120000 );
moFileRead::moIODevice::SetValue( ADD_FILE_TO_READ,(MOpointer*)pFileName );

//get the chunks
if(moFileRead::moIODevice::GetState( CHUNK_AVAILABLE )
	moFileRead::moIODevice::GetValue( FILE_CHUNK,(MOpointer*)pChunk )

==============================================*/
#include "moConfig.h"
#include "moDeviceCode.h"
#include "moEventList.h"
#include "moIODeviceManager.h"
#include "moTypes.h"

//#include "sdlnet/SDL_net.h"
#include "SDL_thread.h"
#ifndef __MO_FILE_READ_H
#define __MO_FILE_READ_H

//typedef FileInThreadData* pFileInThreadData;
class LIBMOLDEO_API moFileRead:public moIODevice
{
	public:moFileRead (moEventList *);
	~moFileRead ();
	void Update (moEventList *);
	MOboolean Init ();
	MOswitch GetStatus (MOdevcode);
	MOswitch SetStatus (MOdevcode, MOswitch);
	MOint GetValue (MOdevcode);
	MOdevcode GetCode (moText);
	MOboolean Finish ();
	private:moConfig config;
	moEventList * events;
};

int file_read_thread_main (void *);
#endif							 /*  */
== == == =
/*==============================================

Class:   ajFileRead

Description: use to read files from hard disc or other local devices.
Specially useful for long videos or data files, because it implements
asynchronous reads with a thread for each file.

Todo:	Multiple threads for multiple files

Example:

 //set the file to read from
ajFileRead::moIODevice::SettValue( CHUNKS_SIZE,  120000 );
ajFileRead::moIODevice::SetValue( ADD_FILE_TO_READ,(MOpointer*)pFileName );

//get the chunks
if(ajFileRead::moIODevice::GetState( CHUNK_AVAILABLE )
	ajFileRead::moIODevice::GetValue( FILE_CHUNK,(MOpointer*)pChunk )

==============================================*/
#include "moConfig.h"
#include "ajCodigoDispositivo.h"
#include "moEventList.h"
#include "moIODeviceManager.h"
#include "moTypes.h"

//#include "sdlnet/SDL_net.h"
#include "SDL_thread.h"
#ifndef __MO_FILE_READ_H
#define __MO_FILE_READ_H

//typedef FileInThreadData* pFileInThreadData;
class LIBMOLDEO_API ajFileRead:public moIODevice
{
	public:ajFileRead (moEventList *);
	~ajFileRead ();
	void Currentizar (moEventList *);
	MOboolean Init ();
	MOswitch getEstado (MOcodigodisp);
	MOswitch setEstado (MOcodigodisp, MOswitch);
	MOint getValor (MOcodigodisp);
	MOcodigodisp getCodigo (moText);
	MOboolean Finish ();
	private:moConfig config;
	moEventList * eventos;
};

int file_read_thread_main (void *);
#endif							 /*  */
>>>>>>>.r166
