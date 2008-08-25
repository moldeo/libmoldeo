/*******************************************************************************

                                moFileWrite.h

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

<<<<<<< .mine
// Network device for input //

#include "moConfig.h"
#include "moDeviceCode.h"
#include "moEventList.h"
#include "moIODeviceManager.h"
#include "moTypes.h"

//#include "sdlnet/SDL_net.h"
#include "SDL_thread.h"

#ifndef __MO_FILE_WRITE_H
#define __MO_FILE_WRITE_H

class LIBMOLDEO_API moFileWrite : public moIODevice 
{
public:
    moFileWrite(moEventList*);
    ~moFileWrite();
    
    void Update(moEventList*);
    MOboolean Init();
    MOswitch GetStatus(MOdevcode);
    MOswitch SetStatus( MOdevcode,MOswitch);
    MOint GetValue(MOdevcode);
    MOdevcode GetCode( moText);
    MOboolean Finish();

private:
    moConfig config;

    moEventList *events;
};

int file_write_thread_main(void *);

#endif
=======
// Network device for input //

#include "moConfig.h"
#include "ajCodigoDispositivo.h"
#include "moEventList.h"
#include "moIODeviceManager.h"
#include "moTypes.h"

//#include "sdlnet/SDL_net.h"
#include "SDL_thread.h"

#ifndef __MO_FILE_WRITE_H
#define __MO_FILE_WRITE_H

class LIBMOLDEO_API ajFileWrite : public moIODevice 
{
public:
    ajFileWrite(moEventList*);
    ~ajFileWrite();
    
    void Currentizar(moEventList*);
    MOboolean Init();
    MOswitch getEstado(MOcodigodisp);
    MOswitch setEstado( MOcodigodisp,MOswitch);
    MOint getValor(MOcodigodisp);
    MOcodigodisp getCodigo( moText);
    MOboolean Finish();

private:
    moConfig config;

    moEventList *eventos;
};

int file_write_thread_main(void *);

#endif
>>>>>>> .r166
