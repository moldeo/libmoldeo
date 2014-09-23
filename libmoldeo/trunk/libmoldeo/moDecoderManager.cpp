/*******************************************************************************

                              moDecoderManager.cpp

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

#include "moDecoderManager.h"



//===========================================
//
//				moVdpauManager
//
//===========================================

moDecoderManager::moDecoderManager()
{
	SetType( MO_OBJECT_RESOURCE );
	SetResourceType( MO_RESOURCETYPE_DECODER );
  SetName("decodermanager");
  SetLabelName("decodermanager");

  m_pDecoderDevice = NULL;
  m_pVideoDecoder = NULL;
  m_pVideoMixer = NULL;

  m_pVideoGraph = NULL;

  #ifdef MO_VDPAU
  m_DecoderType = MO_DECODERTYPE_VDPAU;
  #endif

}

moDecoderManager::~moDecoderManager()
{
	Finish();
}

MOboolean moDecoderManager::Init()
{
	if ( m_pResourceManager ) {
		//m_glmanager = m_pResourceManager->GetGLMan();
		#ifdef MO_VDPAU

      //m_pDecoderDevice = (moDecoderDevice*) new VdpDevice();

      //typedef VdpStatus VdpDeviceCreateX11(Display *display, int screen,VdpDevice *device, VdpGetProcAddress **get_proc_address)
      //VdpGetProcAddress *vdp_get_proc_add;
      Display* vdp_xdisplay;
      vdp_xdisplay = NULL;
      VdpStatus vdp_st;
      int screen;
      screen = 0;
      //VdpDevice vdp_device;
      VdpDecoderProfile vdp_decoder_profile;
      //VdpGetProcAddress* vdp_get_proc_address;
      //VdpDecoder  vdp_decoder;

      vdp_device = 0;
      vdp_get_proc_address = NULL;

      int width,height;

      width = 1920;
      height = 1080;
      vdp_decoder_profile = VDP_DECODER_PROFILE_H264_HIGH;

      vdp_xdisplay = glXGetCurrentDisplay();
      if (vdp_xdisplay) {
        MODebug2->Message( moText("Display:") + IntToStr((long)vdp_xdisplay));
        screen = DefaultScreen(vdp_xdisplay);
        if (screen>=0) {
          MODebug2->Message( moText("Screen:") + IntToStr(screen) );
          MODebug2->Message( moText("Creating VdpDevice"));
          vdp_st = vdp_device_create_x11( vdp_xdisplay, screen, &vdp_device, &vdp_get_proc_address );
          if (vdp_st==VDP_STATUS_OK) {

            vdp_st = vdp_get_proc_address(vdp_device, VDP_FUNC_ID_DECODER_CREATE, (void **)&vdp_decoder_create);
            vdp_st = vdp_get_proc_address(vdp_device, VDP_FUNC_ID_GET_ERROR_STRING, (void **)&vdp_get_error_string);

            MODebug2->Message( moText("VdpStatus: OK => ") + IntToStr(vdp_st) );
            MODebug2->Message( moText("Creating VdpDecoder"));
            vdp_st = vdp_decoder_create( vdp_device, vdp_decoder_profile, width, height, 2, &vdp_decoder );
            if (vdp_st==VDP_STATUS_OK) {
              MODebug2->Message( moText("VdpStatus: OK => ") + IntToStr(vdp_st) );
            } else {
              MODebug2->Message( moText("VdpStatus: Error:") + moText(vdp_get_error_string(vdp_st)) );
            }

          } else {
            MODebug2->Message( moText("VdpStatus: Error:") + IntToStr(vdp_st) );
          }
        }
      }






      //MODebug2->Message( moText("Screen:") + IntToStr(screen) );

      //vdp_st = vdp_device_create_x11( vdp_xdisplay, screen, &vdp_device, &vdp_get_proc_address );
      /*
      m_pVideoDecoder = (moVideoDecoder*) new VdpDecoder(  );
      m_pVideoGraph = (moVideoGraph*) new moVdpauGraph();
      */
      //VdpDecoderProfile profile = 0;

      ///Depends on imageformat!!!
      //profile = VDP_DECODER_PROFILE_H264_MAIN;
      /*
              vdp_st = vdp_decoder_create(
            vdp_device,
            vdp_decoder_profile,
            vid_width,
            vid_height,
            max_reference_frames,
            &decoder
        );
        */
      //VdpStatus Status = VdpDecoderCreate( m_pDecoderDevice, profile, );

		#endif
	}
	return (m_pResourceManager!=NULL);
}

MOboolean moDecoderManager::OpenVideo( moText filename ) {
      MODebug2->Message("moDecoderManager::OpenVideo > filename: " + filename );
#ifdef MO_VDPAU
      avcodec_init();
      avcodec_register_all();
      av_register_all();


      const char      *pfilename="test.avi";

      // Open video file
      if(av_open_input_file(&pFormatCtx, pfilename, NULL, 0, NULL)!=0) {
          //handleerror();
          MODebug2->Message("Error");
      }
#endif
    return false;
}


MOboolean moDecoderManager::Finish()
{

	return true;
}
