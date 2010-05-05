/*******************************************************************************

                              moGsGraph.cpp

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


#include "moVdpauGraph.h"


#ifdef MO_LINUX
#ifdef MO_VDPAU
#include "vdpau/vdpau.h"
#endif
#endif


#ifdef MO_VDPAU



moVdpauGraph::moVdpauGraph() {

}


moVdpauGraph::~moVdpauGraph() {


}

//================================================
//	INITIALIZATION AND FINALIZATION METHODS
//================================================
bool moVdpauGraph::InitGraph() {
  return false;
}


bool moVdpauGraph::FinishGraph() {
  return false;
}


//================================================
//	FILTER METHODS CONSTRUCTION
//================================================
bool moVdpauGraph::SetCaptureDevice( moText deviceport , MOint idevice) {
  return false;
}


  bool moVdpauGraph::BuildLiveGraph( moBucketsPool* bpool, moCaptureDevice cdev ) {
    return false;
    }

  bool moVdpauGraph::BuildLiveVideoGraph( moText livevid, moBucketsPool* bpool ) {
    return false;
    }

  bool moVdpauGraph::BuildLiveQTVideoGraph( moText tx, moBucketsPool* bpool ) {
    return false;
    }

  bool moVdpauGraph::BuildRecordGraph( moText tx, moBucketsPool* bpool ) {
    return false;
    }

//================================================
//	CONTROL METHODS
//================================================

moStreamState moVdpauGraph::GetState() {
  return MO_STREAMSTATE_UNKNOWN;
}


	void moVdpauGraph::Play() {
	  }

	void moVdpauGraph::Stop() {

  }

	void moVdpauGraph::Pause() {

  }

	void moVdpauGraph::Seek( MOuint frame ) {
	  }

	bool moVdpauGraph::IsRunning() {
      return false;
	}

	MOulong	moVdpauGraph::GetFramesLength() {
    return 0;
  }

	MObyte* moVdpauGraph::GetFrameBuffer(MOlong *size) {
    return NULL;
  }

  MOulong moVdpauGraph::GetDuration() {
    return 0;
  }

  MOulong moVdpauGraph::GetPosition() {
    return 0;
  }



#endif
