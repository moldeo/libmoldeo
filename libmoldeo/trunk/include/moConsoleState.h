/*******************************************************************************

                             moConsoleState.h

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
  Andr�s Colubri

*******************************************************************************/

#ifndef __MO_CONSOLE_STATE_H__
#define __MO_CONSOLE_STATE_H__

#include "moTypes.h"
#include "moAbstract.h"

class LIBMOLDEO_API moConsoleState : public moAbstract
{
	public:

		moConsoleState();
		virtual ~moConsoleState();

		MOboolean Init();
		MOboolean Finish();

    public:
		moTempo tempo;
		MOswitch pause;
		MOswitch automatic;
		MOswitch reinit;
		MOswitch finish;
		MOswitch setfps;
		MOuint fps, fps0, fps1;

		//especiales
		MOswitch savescreen;
		MOuint frame;

		MOuint m_nEffects;
		MOuint m_nPreEffects;
		MOuint m_nPostEffects;
		MOuint m_nMasterEffects;
		MOuint m_nAllEffects;
    
};

#endif
