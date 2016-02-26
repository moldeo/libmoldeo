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

#include "moArray.h"
moDefineDynamicArray(moCaptureDevices)


moCaptureDevice::moCaptureDevice() {
    m_Name = moText("");
    m_Description = moText("");
    m_Path  = moText("");
    m_LabelName = moText("");
    m_DevicePort = 0;
    m_bPresent = false;
    m_bPreferred = false;
    m_SourceWidth = m_SourceHeight = m_SourceBpp = m_SourceFlipH = m_SourceFlipV = 0;
    m_VideoFormat.m_Width = 0;
    m_VideoFormat.m_Height = 0;
}

moCaptureDevice::moCaptureDevice( const moText &p_name,
                                 const moText &p_description,
                                 const moText &p_path,
                                 MOint p_deviceport,
                                 MOint p_sourcewidth,
                                 MOint p_sourceheight,
                                 MOint p_bpp,
                                 MOint p_fliph,
                                 MOint p_flipv ) {
  m_Name = p_name;
  m_LabelName = p_name;
  m_Description = p_description;
  m_Path  = p_path;
  m_DevicePort = p_deviceport;
  m_bPresent = true;
  m_bPreferred = false;
  m_SourceWidth = p_sourcewidth;
  m_SourceHeight = p_sourceheight;
  m_SourceBpp = p_bpp;
  m_SourceFlipH = p_fliph;
  m_SourceFlipV = p_flipv;
  m_VideoFormat.m_Width = 0;
  m_VideoFormat.m_Height = 0;
}



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
		m_PreferredDevices.Add( p_pPreferredDevices->GetRef(i) );
	}



	//update all CaptureDevices availables, with preferred formats...
	for( j=0; j< m_PreferredDevices.Count(); j++) {
		PreferredDevice = m_PreferredDevices.GetRef(j);
		for( i=0; i< m_CaptureDevices.Count(); i++) {
			CapDev = m_CaptureDevices.GetRef(i);
			if ( CapDev.GetName() == PreferredDevice.GetName()) { //capdev founded and maybe present
				//set preferred videoformat...and codename
				CapDev.SetVideoFormat( PreferredDevice.GetVideoFormat() );
				CapDev.SetLabelName( PreferredDevice.GetLabelName() );
				//save new CapDev data
				m_CaptureDevices.Set( i, CapDev );
			}
		}
	}

}

void
moVideoFramework::SetPreferredFormat( const moCaptureDevice &m_CaptureDevice ) {

	moCaptureDevice PreferredDevice;
	moCaptureDevice CapDev;
	MOuint i;

	//search for preferred configurations for the specified CaptureDevice
	for( i=0; i< m_PreferredDevices.Count(); i++) {
		PreferredDevice = m_PreferredDevices.GetRef(i);
		//search by friendly name
		if ( PreferredDevice.GetName() == m_CaptureDevice.GetName())
			break;
	}

	for( i=0; i< m_CaptureDevices.Count(); i++) {
		CapDev = m_CaptureDevices.GetRef(i);
		if ( CapDev.GetPath() == m_CaptureDevice.GetPath()) { //capdev founded and maybe present
			//set preferred videoformat...and codename
			CapDev.SetVideoFormat( PreferredDevice.GetVideoFormat() );
			CapDev.SetLabelName( PreferredDevice.GetLabelName() );
			//save new CapDev data
			m_CaptureDevices.Set( i, CapDev );
		}
	}

}

