/*******************************************************************************

                              moVideoGraph.cpp

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


#include "moVideoGraph.h"

#include <moArray.h>
moDefineDynamicArray(moCaptureDevices)


moVideoGraph::moVideoGraph() {

}

moVideoGraph::~moVideoGraph() {

}

bool
moVideoGraph::FinishGraph() {
	return true;
}

moVideoMode moVideoGraph::GetVideoMode() {

	return m_VideoFormat.m_VideoMode;

}

moVideoFormat moVideoGraph::GetVideoFormat() {

	return m_VideoFormat;

}

moStreamState    moVideoGraph::GetState() {

   return MO_STREAMSTATE_UNKNOWN;

}

moText  moVideoGraph::StateToText( moStreamState state ) {

    switch((int)state) {
        case MO_STREAMSTATE_UNKNOWN:
            return moText("UNKNOWN");
            break;
        case MO_STREAMSTATE_PAUSED:
            return moText("PAUSED");
            break;
        case MO_STREAMSTATE_PLAYING:
            return moText("PLAYING");
            break;
        case MO_STREAMSTATE_WAITING:
            return moText("WAITING");
            break;
        case MO_STREAMSTATE_READY:
            return moText("READY");
            break;
        case MO_STREAMSTATE_STOPPED:
            return moText("STOPPED");
            break;
    }

    return moText("invalid state");
}

//==========================================================================
// moVideoFramework
//==========================================================================


moVideoFramework::moVideoFramework() {

}


moVideoFramework::~moVideoFramework() {
	m_CaptureDevices.Empty();
	m_PreferredDevices.Empty();
}


void
moVideoFramework::SetPreferredDevices( moCaptureDevices* p_pPreferredDevices ) {

	moCaptureDevice PreferredDevice;
	moCaptureDevice CapDev;
	MOuint i,j;

	//m_PreferredDevices.Copy( (*p_pPreferredDevices) );
	m_PreferredDevices.Empty();
	for( i=0; i < p_pPreferredDevices->Count(); i++ ) {
		m_PreferredDevices.Add( p_pPreferredDevices->Get(i) );
	}



	//update all CaptureDevices availables, with preferred formats...
	for( j=0; j< m_PreferredDevices.Count(); j++) {
		PreferredDevice = m_PreferredDevices.Get(j);
		for( i=0; i< m_CaptureDevices.Count(); i++) {
			CapDev = m_CaptureDevices.Get(i);
			if ( CapDev.GetName() == PreferredDevice.GetName()) { //capdev founded and maybe present
				//set preferred videoformat...and codename
				CapDev.SetVideoFormat( PreferredDevice.GetVideoFormat() );
				CapDev.SetCodeName( PreferredDevice.GetCodeName() );
				//save new CapDev data
				m_CaptureDevices.Set( i, CapDev );
			}
		}
	}

}

void
moVideoFramework::SetPreferredFormat( moCaptureDevice &m_CaptureDevice ) {

	moCaptureDevice PreferredDevice;
	moCaptureDevice CapDev;
	MOuint i;

	//search for preferred configurations for the specified CaptureDevice
	for( i=0; i< m_PreferredDevices.Count(); i++) {
		PreferredDevice = m_PreferredDevices.Get(i);
		//search by friendly name
		if ( PreferredDevice.GetName() == m_CaptureDevice.GetName())
			break;
	}

	for( i=0; i< m_CaptureDevices.Count(); i++) {
		CapDev = m_CaptureDevices.Get(i);
		if ( CapDev.GetPath() == m_CaptureDevice.GetPath()) { //capdev founded and maybe present
			//set preferred videoformat...and codename
			CapDev.SetVideoFormat( PreferredDevice.GetVideoFormat() );
			CapDev.SetCodeName( PreferredDevice.GetCodeName() );
			//save new CapDev data
			m_CaptureDevices.Set( i, CapDev );
		}
	}

}
