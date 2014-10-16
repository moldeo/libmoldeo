/*******************************************************************************

                                moDeviceCode.h

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


#ifndef __MODEVICECODE_H
#define __MODEVICECODE_H

#include "moTypes.h"
#include "moAbstract.h"

class LIBMOLDEO_API moDeviceCode {
public:
	MOint device;
	MOint devicecode;
	MOint value;
	moDeviceCode *next;
	moDeviceCode *previous;

	moDeviceCode();
	moDeviceCode(MOint,MOint,MOint=0);
};

class LIBMOLDEO_API moDeviceCodeList : public moAbstract {
public:

	moDeviceCode *First;
	moDeviceCode *Last;

	moDeviceCodeList();
	virtual ~moDeviceCodeList();

	MOboolean Init();
	void Add(MOint,MOint,MOint = 0);
	MOboolean Delete(moDeviceCode *);
	void Copy(moDeviceCodeList *);
	MOboolean Finish();

};

#endif

