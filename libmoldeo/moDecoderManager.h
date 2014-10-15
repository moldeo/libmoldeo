/*******************************************************************************

                                moDecoderManager.h

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

#ifndef __MO_DECODERMANAGER_H__
#define __MO_DECODERMANAGER_H__

#include <moTypes.h>
#include <moTempo.h>
#include <moText.h>
#include <moLock.h>
#include <moFBO.h>
#include <moTexture.h>
#include <moVideoGraph.h>
#include <moResourceManager.h>

typedef void moDecoderDevice;
typedef void moVideoMixer;
typedef void moVideoDecoder;

#ifdef MO_VDPAU
  //#include "avformat.h"
  //#include "avcodec.h"

  #include "moVdpauGraph.h"
  #include "vdpau/vdpau.h"
  #include "vdpau/vdpau_x11.h"
  #include "GL/glx.h"
#endif

enum moDecoderType {

    MO_DECODERTYPE_VDPAU,
    MO_DECODERTYPE_CUDA,
    MO_DECODERTYPE_OPENCL

};


class moDecoderPlaylist {


  moDecoderPlaylist();
  virtual ~moDecoderPlaylist();



};


/**
 * Esta clase implementa un administrador para la API de VDPAU de reproducción de videos
 * que hace uso en LINUX del hardware específico de NVidia con la arquitectura de chips VP1/VP2
 * Esta librería permite reproducir archivos de tipo MPEG2, H264, WMV, VC1, total o parcialmente
 * acelerados por esta librería.

 */
class LIBMOLDEO_API moDecoderManager : public moResource
{
    public:
        /**
         * Constructor por defecto.
         */
		moDecoderManager();
        /**
         * Destructor por defecto.
         */
		virtual ~moDecoderManager();

        /**
         * Inicializador del administrador.
         */
		virtual MOboolean Init();
        /**
         * Finalizador del administrador.
         */
		virtual MOboolean Finish();

    moDecoderDevice*        GetDecoderDevice();
    moVideoDecoder*         GetVideoDecoder();
    moVideoMixer*           GetVideoMixer();

    void SetDecoderType( moDecoderType type );
    moDecoderType GetDecoderType();


    MOboolean  OpenVideo( moText filename );

    private:

		moGLManager*            m_glmanager;
		moDecoderType           m_DecoderType;
    moDecoderDevice*        m_pDecoderDevice;
    moVideoDecoder*         m_pVideoDecoder;
    moVideoMixer*           m_pVideoMixer;

    moVideoGraph*           m_pVideoGraph;

#ifdef MO_VDPAU
    //AVFormatContext *pFormatCtx;

    VdpDevice								vdp_device;
    VdpGetProcAddress						*vdp_get_proc_address;
    VdpDeviceDestroy						*vdp_device_destroy;
    VdpVideoSurfaceCreate					*vdp_video_surface_create;
    VdpVideoSurfaceDestroy					*vdp_video_surface_destroy;
    VdpDecoderCreate						*vdp_decoder_create;
    VdpDecoderDestroy						*vdp_decoder_destroy;
    VdpDecoderRender						*vdp_decoder_render;
    VdpOutputSurfaceCreate					*vdp_output_surface_create;
    VdpOutputSurfaceDestroy					*vdp_output_surface_destroy;
    VdpOutputSurfaceGetBitsNative			*vdp_output_surface_get_bits_native;
    VdpVideoSurfaceGetBitsYCbCr				*vdp_video_surface_get_bits_y_cb_cr;
    VdpVideoMixerCreate						*vdp_video_mixer_create;
    VdpVideoMixerDestroy					*vdp_video_mixer_destroy;
    VdpVideoMixerRender						*vdp_video_mixer_render;
    VdpPresentationQueueCreate				*vdp_presentation_queue_create;
    VdpPresentationQueueDestroy				*vdp_presentation_queue_destroy;
    VdpPresentationQueueGetTime				*vdp_presentation_queue_get_time;
    VdpPresentationQueueTargetCreateX11		*vdp_presentation_queue_target_create_x11;
    VdpPresentationQueueQuerySurfaceStatus	*vdp_presentation_queue_query_surface_status;
    VdpPresentationQueueDisplay				*vdp_presentation_queue_display;
    VdpPresentationQueueBlockUntilSurfaceIdle	*vdp_presentation_queue_block_until_surface_idle;
    VdpGetErrorString *vdp_get_error_string;

    VdpDecoder						vdp_decoder;
    VdpVideoMixer					vdp_mixer;
    VdpPresentationQueue			vdp_presentation_queue;
    VdpPresentationQueueTarget		vdp_presentation_queue_target;
    int								vdp_pix_fmt;
    int								vdp_width;
    int								vdp_height;
#endif

};

#include "moGLManager.h"

#endif

