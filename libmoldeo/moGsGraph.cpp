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

  Gstreamer list of defined types:
  http://gstreamer.freedesktop.org/data/doc/gstreamer/head/pwg/html/section-types-definitions.html

*******************************************************************************/
#include "moGsGraph.h"

#include <gst/gst.h>

#ifndef GSTVERSION
#include <gst/interfaces/propertyprobe.h>
#else
#endif // GSTVERSION

#ifndef GSTVERSION
#define GSTVERSION
#endif

#include "moFileManager.h"

moLock BuildLock;
#ifndef MO_GSTREAMER
#define MO_GSTREAMER
#endif

#ifdef MO_GSTREAMER

#define CAMERABIN "wrappercamerabinsrc"

#ifdef GSTVERSION
  #include <gst/app/gstappsink.h>
  #define DECODEBIN "decodebin"
  #define VIDEOCONVERT "videoconvert"

  #ifdef MO_MACOSX
    //for mac osx > 10.4
    #define CAMERABINOSX "avfvideosrc"
  #endif
  //#define VIDEOCONVERT "glcolorconvert"
#else
  #define VIDEOCONVERT "ffmpegcolorspace"
  #ifdef MO_MACOSX
   #define DECODEBIN "decodebin"
  #else
    #define DECODEBIN "decodebin2"
  #endif
#endif

#define USING_SYNC_FRAMEBUFFER

static gboolean bus_call ( GstBus *bus, GstMessage *msg, void* user_data)
{
    //cout << "bus_call: new message" << endl;
    bus = NULL;
    moGsGraph* pGsGraph = (moGsGraph*) user_data;

    if (true) {
      const GstStructure *s;

      s = gst_message_get_structure ((GstMessage *)msg);

      /*
      pGsGraph->MODebug2->Message(
          moText("moGsGraph:: Got message from element  \"")
          + moText( GST_STR_NULL (GST_ELEMENT_NAME (GST_MESSAGE_SRC (msg)))  )
          + moText("\" (")
          + moText(gst_message_type_get_name (GST_MESSAGE_TYPE (msg)))
          + moText(")") );
*/
      if (s) {
        gchar *sstr;

        sstr = gst_structure_to_string (s);
        //pGsGraph->MODebug2->Message( moText(sstr) );
        //g_print ("%s\n", sstr);
        g_free (sstr);
      } else {

        //pGsGraph->MODebug2->Message( moText(" <no message details>") );
        //g_print ("no message details\n");
      }
    }

  switch (GST_MESSAGE_TYPE (msg))
  {
      case GST_MESSAGE_EOS:
      {
          //g_message ("End-of-stream");
          pGsGraph->MODebug2->Message(moText("moGsGraph:: EOS <End-of-stream> "));
          pGsGraph->SetEOS(true);
          //g_main_loop_quit (loop);
          break;
      }

      case GST_MESSAGE_ERROR:
      {
          gchar *debug;

          GError *err;

          gst_message_parse_error ((GstMessage *)msg, &err, &debug);
          pGsGraph->MODebug2->Error(moText("moGsGraph:: gst message error: ") + moText(debug));
          g_free (debug);
          //g_error ("%s", err->message);
          //g_error_free (err);
          //g_main_loop_quit (loop);

          break;
      }

      default:

      break;

}

  return true;
}


/*
///IMPORTANT TO KNOW WHAT TO RETREIVE FROM BUFFER IF NEEDED
typedef struct {
  GstMiniObject		 mini_object;

  // pointer to data and its size
  guint8		*data;
  guint			 size;

  // timestamp
  GstClockTime		 timestamp;
  GstClockTime		 duration;

  // the media type of this buffer
  GstCaps		*caps;

  // media specific offset
  guint64		 offset;
  guint64		 offset_end;

  guint8                *malloc_data;
} GstBuffer;
*/


moGBoolean
moGsGraph::cb_buffer_disconnected (
                                    moGPointer   u_data
                                    ) {

    moGsGraph* pGsGraph;

    if (u_data!=0) {
      pGsGraph = (moGsGraph*)u_data;
      if (pGsGraph) {
        pGsGraph->MODebug2->Error(moText("moGsGraph::cb_buffer_disconnected !!!"));
      }
    }

    //moAbstract::MODebug2->Error(moText("moGsGraph::cb_buffer_disconnected !!!"));
	return false;
}

/**
// g_signal_connect (data.appsink, "new-buffer", G_CALLBACK (appsink_new_buffer), &data);
void appsink_new_buffer (GstElement *sink, CustomData *data) {
  GstBuffer *buffer;

  // Retrieve the buffer
  g_signal_emit_by_name (sink, "pull-buffer", &buffer);
  if (buffer) {

    (((CustomData*)data)->appsink_buffer_count)++;

    //push buffer onto queue, to be processed in different thread
    if (GstBufferQueue->size() > GSTBUFFERQUEUE_SIZE) {
      //error message
      printf ("GstBufferQueue is full!\n");
      //release buffer
      gst_buffer_unref (buffer);
    } else {
      //push onto queue
      GstBufferQueue->push(buffer);
      //activate thread
      connectionDataAvailable_GstBufferQueue.notify_all();
    }
  }
}
*/

#ifdef GSTVERSION
moGstFlowReturn
moGsGraph::appsink_new_sample( moGstAppSink* appsink, moGPointer user_data ) {

  GstSample* sample;
  GstMapInfo mapinfo;
  GstAppSink* psink = (GstAppSink*) appsink;
  if (!psink) return GST_FLOW_ERROR;

  moGsGraph* pGsGraph = (moGsGraph*) user_data;
  int w = pGsGraph->GetVideoFormat().m_Width;
  int h = pGsGraph->GetVideoFormat().m_Height;

  if (!pGsGraph) return GST_FLOW_ERROR;
  //pGsGraph->MODebug2->Message("new sample");
    moBucket *pbucket=NULL;

//pGsGraph->MODebug2->Message(moText("new-sample") );

  if (1==1) {
      //GstSample* sample = gst_app_sink_try_pull_sample ( psink, 1000000  );
      GstSample* sample = gst_app_sink_pull_sample ( psink  );
      //g_signal_emit_by_name (psink, "pull-sample", &sample);
      //GstSample* sample = gst_base_sink_get_last_sample(GST_BASE_SINK(psink));
      if (!sample) {
            pGsGraph->MODebug2->Message(moText("no sample") );
            return GST_FLOW_ERROR;
      } else {
          //pGsGraph->MODebug2->Message(moText("sample!") );
      }

      GstCaps* bcaps = gst_sample_get_caps( sample );
      if (!bcaps) return GST_FLOW_OK;

      GstBuffer* Gbuffer = gst_sample_get_buffer (sample);
      int bsize = gst_buffer_get_size( Gbuffer );
      if (!( bsize>0 && (int)bsize<=(h*w*4) )) return GST_FLOW_ERROR;
      pGsGraph->MODebug2->Message(moText("Bucket receiving size: ") + IntToStr(bsize) );

    if (1==1) {

      //gst buffer to moldeo bucketpool

      if (!pGsGraph->m_pBucketsPool) return GST_FLOW_ERROR;
      if(pGsGraph->m_pBucketsPool->IsFull()) {
          //pGsGraph->MODebug2->Warning("appsink_new_sample > bckt full");
          gst_sample_unref(sample);
          return GST_FLOW_OK;
      }

      pbucket = new moBucket();
      if (pbucket==NULL) return GST_FLOW_ERROR;
    }

      if (Gbuffer) {
        gst_buffer_map ( Gbuffer, &mapinfo, GST_MAP_READ);
        //GstBuffer* Gbuffer2 = gst_buffer_ref (Gbuffer);
        //if (Gbuffer2) {
          //gst_buffer_map ( Gbuffer2, &mapinfo, GST_MAP_READ);

          //MOubyte color = mapinfo.data[0];
          //pGsGraph->MODebug2->Message(moText("color: ") + IntToStr(color) );
          if (bsize) {
            //pGsGraph->MODebug2->Message(moText("copying: ") + IntToStr(bsize) );
            //pGsGraph->m_Buckets[0].Copy( bsize, (MOubyte*)mapinfo.data );
            pbucket->SetBuffer( bsize,(MOubyte*)mapinfo.data );
            //pbucket->BuildBucket(bsize,128);
          } else {
            pGsGraph->MODebug2->Error(moText("m_Buckets  size: ") + IntToStr(pGsGraph->m_Buckets[0].GetSize()) + moText(" do not match with buffer size: ") + IntToStr(bsize) );
          }
          //gst_buffer_unmap ( Gbuffer2, &mapinfo );
          //gst_buffer_unref(Gbuffer2);

        //}
        gst_buffer_unmap ( Gbuffer, &mapinfo );
      }

    if (1==1) {


      bool added_bucket = pGsGraph->m_pBucketsPool->AddBucket( pbucket );
      if(!added_bucket)
        pGsGraph->MODebug2->Error(moText("appsink_new_sample > Bucket error"));
      //else pGsGraph->MODebug2->Message("bckt added!!"+IntToStr(pGsGraph->m_pBucketsPool->m_nBuckets) );
    }


      gst_sample_unref(sample);
  }
  return GST_FLOW_OK;
}

moGstFlowReturn
moGsGraph::appsink_new_preroll( moGstAppSink* appsink, moGPointer user_data ) {

  return 0;
}

void
moGsGraph::appsink_eos( moGstAppSink* appsink, moGPointer user_data ) {

}
#endif




#ifndef GSTVERSION
moGBoolean
moGsGraph::cb_have_data (moGstPad    *pad, moGstBuffer *buffer, moGPointer   u_data)
#else
moGstCallbackReturn
moGsGraph::cb_have_data (moGstPad    *pad, moGstPadProbeInfo *info, moGPointer   u_data)
#endif
{
    moGsGraph* pGsGraph;
    pad = NULL;
    GstStructure* str = NULL;
    GstBuffer* Gbuffer;
    GstCaps* caps = NULL;
    GstPad* Gpad = NULL;

#ifndef GSTVERSION
    Gbuffer = (GstBuffer*)buffer;
    caps = Gbuffer->caps;
#else
    GstPadProbeInfo* Ginfo = (GstPadProbeInfo*) info;
    Gbuffer = GST_PAD_PROBE_INFO_BUFFER ( Ginfo );
    Gpad = (GstPad*)pad;
    if (Gpad)
      caps = gst_pad_get_current_caps(  Gpad );
#endif

    if (caps)
      str = gst_caps_get_structure ( (caps), 0);
    else
      return false;

    if (str==NULL)
     return FALSE;


    const gchar *sstr;
    const gchar *strname;

    strname = gst_structure_get_name( str );
    sstr = gst_structure_to_string (str);

    //cout << "new data: timestamp: " << buffer->timestamp << " duration:" << buffer->duration << " size:" << buffer->size << " caps:" << sstr << endl;
    moAbstract::MODebug2->Message( moText(" moGsGraph:: cb_have_data") );

    gchar* isaudio =  NULL;
    gchar* isvideo =  NULL;

    isaudio = g_strrstr (strname, "audio");
    isvideo =  g_strrstr (strname, "video");

    if (u_data!=0) {
      pGsGraph = (moGsGraph*)u_data;

      if (isvideo) {
        if (pGsGraph->m_VideoFormat.m_WaitForFormat)
          pGsGraph->SetVideoFormat( caps, Gbuffer );
      }

      if (isaudio) {
        if (pGsGraph->m_AudioFormat.m_WaitForFormat)
          pGsGraph->SetAudioFormat( caps, Gbuffer );
      }
    } else {
      //moAbstract::MODebug2->Error( moText(" moGsGraph:: cb_have_data error: no user data!!") );
      return true;//siga intentando
    }

    //pGsGraph->MODebug2->Message(moText("moGsGraph::cb_have_data receiving..."));

    //return true;

    int w = pGsGraph->GetVideoFormat().m_Width;
    int h = pGsGraph->GetVideoFormat().m_Height;

    //cout << "w:" << w << "h:" << h << endl;

  if (Gbuffer ) {
    int bsize;
#ifndef GSTVERSION
    bsize = Gbuffer->size;
#else
    bsize = gst_buffer_get_size( Gbuffer );
#endif
    if (isvideo) {
      if ( bsize>0 && (int)bsize<=(h*w*4) ) {
        //g_passing buffer to bucketpool
        moBucket *pbucket=NULL;

        if (pGsGraph->m_pBucketsPool)
        if(!pGsGraph->m_pBucketsPool->IsFull()) {

              //pGsGraph->MODebug2->Message(moText("Bucket receiving size: ") + IntToStr(Gbuffer->size) );

            pbucket = new moBucket();
            if(pbucket!=NULL) {

                //pGsGraph->m_VideoFormat.m_BufferSize = Gbuffer->size;
                //pGsGraph->m_VideoFormat.m_TimePerFrame = Gbuffer->duration;

                gint value_numerator, value_denominator;
                gst_structure_get_fraction( str, "framerate", &value_numerator, &value_denominator );

                //MOuint frate = (value_numerator * 100) / value_denominator;
                //MODebug2->Push( " frate: "+ IntToStr(frate) + " timeperframe: " + IntToStr(Gbuffer->duration));
#ifndef GSTVERSION
                pbucket->SetBuffer( bsize,(MOubyte*)Gbuffer->data );
#else
                pbucket->SetBuffer( bsize,(MOubyte*)GST_PAD_PROBE_INFO_DATA(Ginfo) );
#endif
                //pbucket->BuildBucket( w*h*4, 100 );
                //MODebug2->Push("bucket created.");
                //gst_buffer_extract( Gbuffer, 0, pbucket->GetBuffer(), Gbuffer->size );


                if(!pGsGraph->m_pBucketsPool->AddBucket( pbucket )) {
                    pGsGraph->MODebug2->Error(moText("Bucket error"));
                }// else pGsGraph->MODebug2->Message("bucket Added.");
                else {
                  //cout << "bckt added!!" << bsize << " #" << pGsGraph->m_pBucketsPool->m_nBuckets << endl;
                  pGsGraph->MODebug2->Message("bucket Added.");

                }
            }

        }

      } else {
        pGsGraph->MODebug2->Error( moText(" moGsGraph:: cb_have_data error: wrong buffer size:")
                + IntToStr(bsize));

      }
    }
  } else {
    pGsGraph->MODebug2->Error( moText(" moGsGraph:: cb_have_data error: no Gbuffer data!!") );
  }

  gst_object_unref( caps );

  return TRUE;
}


#ifndef GSTVERSION
void
moGsGraph::on_rtsppadd_added( moGstElement *rtspsrc, moGstPad *pad, moGPointer u_data ) {

    rtspsrc = NULL;///unused
    GstCaps *caps = NULL;
    GstPadLinkReturn padlink;
    gchar* padname = NULL;
    const gchar* strname = NULL;
    const gchar* medianame = NULL;
    GstStructure *str = NULL;
    GstPad* Gpad = (GstPad*) pad;
    moGsGraph* pGsGraph;

    if (gst_pad_is_linked(Gpad)) {
      return;
    }
    if (u_data!=0) {
      pGsGraph = (moGsGraph*)u_data;

      /* check media type */
      caps = gst_pad_get_caps (Gpad);
      padname = gst_pad_get_name(Gpad);
      if (padname) {
        str = gst_caps_get_structure (caps, 0);

        const gchar *sstr;
          gchar *field = ¨media¨;
        sstr = gst_structure_to_string (str);
        strname = gst_structure_get_name (str);
        medianame = gst_structure_get_string (str, field);
        //strname = GST_STRUCTURE(str)->has_field("media");

        moText dbgstr = medianame;
        pGsGraph->MODebug2->Push( dbgstr );

        if (g_strrstr (medianame, "video")) {
            ///link video...
            if ( pGsGraph->m_pRTSPDepaySink ) {
                padlink = gst_pad_link ( Gpad, (GstPad*)pGsGraph->m_pRTSPDepaySink);
                if (padlink==GST_PAD_LINK_OK) {
                    ///all ok!!!
                }
            } else
            if ( pGsGraph->m_pHTTPSource ) {
                padlink = gst_pad_link ( Gpad, (GstPad*)pGsGraph->m_pDecoderBin );
                if (padlink==GST_PAD_LINK_OK) {
                    ///all ok!!!
                }
            }
        }

      }
    }

}

#else

#endif


#ifndef GSTVERSION
void
moGsGraph::cb_newpad ( moGstElement *decodebin, moGstPad *pad, moGBoolean last, moGPointer u_data)
#else
void
moGsGraph::cb_pad_added_new ( moGstElement *decodebin, moGstPad *pad, moGPointer u_data)
#endif
{
  decodebin = NULL; ///unused
#ifndef GSTVERSION
  last = false; ///unused
#endif
  GstCaps *caps = NULL;
  GstPad  *videopad = NULL;
//  GstPad  *audiopad = NULL;
  GstPad  *audiopadinconverter = NULL;
  GstPadLinkReturn padlink;
  gchar* padname = NULL;
  const gchar* strname = NULL;
  GstStructure *str = NULL;
  GstPad* Gpad = (GstPad*) pad;

  moGsGraph* pGsGraph = NULL;
  GstElement* SinkElement = NULL;

    cout << "cb_pad_added_new" << endl;


  if (gst_pad_is_linked(Gpad)) {
      cout << "cb_pad_added_new already linked!" << endl;
      return;
  }


  if (u_data!=0) {
      pGsGraph = (moGsGraph*)u_data;
      /* check media type */
#ifndef GSTVERSION
      caps = gst_pad_get_caps (Gpad);
#else
      caps = gst_pad_get_current_caps(Gpad);
#endif
      padname = gst_pad_get_name(Gpad);
      if (padname) {
        str = gst_caps_get_structure (caps, 0);

        const gchar *sstr=NULL;
	if (str) {
        	sstr = gst_structure_to_string (str);
            cout << "cb_pad_added_new: new pad: " << padname << "caps:" << sstr << endl;
	} else {
		MODebug2->Error(moText("moGsGraph::cb_pad_added_new > gst_caps_get_structure is empty")  );
	}

	if (sstr==NULL) {
		MODebug2->Error(moText("moGsGraph::cb_pad_added_new > sstr gst_structure_to_string is empty")  );
	} else strname = gst_structure_get_name (str);
	cout << "cb_newpad: new pad: " << padname << " strname: " << strname << endl;

  bool is_rtsp = false;
	if (g_strrstr (strname, "application/x-rtp")) {
	  is_rtsp = true;
    strname = gst_structure_get_string(str,"media");
    cout << "application/x-rtp: media: " << strname << endl;
  }


        bool forcing_video = false;
	bool is_video = false;
	bool is_audio = false;
        if (strname==NULL) {
		//cout << "cb_newpad: strname==NULL" << endl;
            MODebug2->Error(moText("moGsGraph::cb_pad_added_new > gst_structure_to_string is empty, forcing video!")  );
            //return;
		          forcing_video = true;
        } else {
		is_video = g_strrstr (strname, "video");
		is_audio = g_strrstr (strname, "audio");
	}




          if (is_audio) {
            pGsGraph->m_pAudioPad = Gpad;

            MODebug2->Message(moText("moGsGraph::cb_pad_added_new: audio pad created > building filters"));

            ///TODO: no anda en win
            ///pGsGraph->BuildAudioFilters();

            if (pGsGraph->m_pAudioConverter) {
#ifndef GSTVERSION
                audiopadinconverter = gst_element_get_pad ( (GstElement*) pGsGraph->m_pAudioConverter, "sink");
#else
                MODebug2->Message(moText("moGsGraph::cb_pad_added_new: get static pad sink audio converter"));
                audiopadinconverter = gst_element_get_static_pad ( (GstElement*) pGsGraph->m_pAudioConverter, "sink");
#endif
                MODebug2->Message(moText("moGsGraph::cb_pad_added_new: audio pad link"));
                padlink = gst_pad_link (Gpad, audiopadinconverter);

                MODebug2->Message(moText("moGsGraph::cb_pad_added_new: srcAudio"));
                GstPad* srcAudio = gst_element_get_static_pad ( (GstElement*)pGsGraph->m_pAudioConverter, "src");

                MODebug2->Message(moText("moGsGraph::cb_pad_added_new: pad link ok?"));
                if (padlink==GST_PAD_LINK_OK) {
                MODebug2->Message(moText("moGsGraph::cb_pad_added_new: pad link is ok GST_PAD_LINK_OK"));
#ifndef GSTVERSION
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_buffer_probe_full ( srcAudio, G_CALLBACK (cb_have_data), pGsGraph, (GDestroyNotify) (cb_buffer_disconnected) );
#else
                    /*pGsGraph->cb_have_data_handler_id = gst_pad_add_probe ( srcAudio,
                                                                            GST_PAD_PROBE_TYPE_BUFFER,
                                                                            (GstPadProbeCallback) cb_have_data,
                                                                            pGsGraph,
                                                                           (GDestroyNotify) (cb_buffer_disconnected) );*/
#endif
                }

            } else if (pGsGraph->m_pAudioSink) {
                audiopadinconverter = gst_element_get_static_pad ( (GstElement*) pGsGraph->m_pAudioSink, "sink");
                padlink = gst_pad_link (Gpad, audiopadinconverter);
            }


          } else if (is_video || forcing_video ) {
            pGsGraph->m_pVideoPad = Gpad;
            const gchar *v_format = "";
            v_format = gst_structure_get_string(str, "format");
            MODebug2->Message(moText("moGsGraph::cb_pad_added_new: video pad created, format: ")+moText(v_format));
            if (is_rtsp) {
              videopad = (GstPad*)pGsGraph->m_pRTSPDepaySink;
              if (videopad) {
                padlink = gst_pad_link( Gpad, videopad );
              }
              if (padlink==GST_PAD_LINK_OK) {
                cout << "moGsGraph::cb_pad_added_new: linked rtsp source with rtsp depay" << endl;
              } else {
                cout << "moGsGraph::cb_pad_added_new: ERROR: UNlinked rtsp source with rtsp depay" << endl;
              }
            } else
            if (pGsGraph->m_pVideoScale==NULL) {
                //version directa a videoscale
                if (!(GstElement*)pGsGraph->m_pColorSpaceInterlace) {
                    SinkElement = (GstElement*)pGsGraph->m_pColorSpace;
                } else {
                    SinkElement = (GstElement*)pGsGraph->m_pColorSpaceInterlace;
                }
#ifndef GSTVERSION
                videopad = gst_element_get_pad ( SinkElement, "sink");
                if (videopad) {
                    padlink = gst_pad_link( Gpad, videopad );
                }
#else

                if (v_format && (moText(v_format)==moText("BGRA") || moText(v_format)==moText("RGBA")) ) {
                  MODebug2->Message(moText("moGsGraph::cb_pad_added_new: setting caps for Alpha channel, RGBA"));
                  GstCaps *capsRGBA = gst_caps_new_simple ( "video/x-raw", "format", G_TYPE_STRING, "RGBA", NULL);
                  g_object_set (G_OBJECT (pGsGraph->m_pFakeSink), "caps", capsRGBA, NULL);
                  // TODO: relink appsink to get the format RGBA?
                }

                videopad = gst_element_get_static_pad( SinkElement, "sink");
                if (videopad) {
                    padlink = gst_pad_link( Gpad, videopad );
                }
#endif
                //version con deinterlace
                //videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pVideoDeinterlace, "sink");

                //bool res = gst_pad_set_caps( gst_element_get_pad ( pGsGraph->m_pColorSpace, "src"), gst_caps_new_simple ("video/x-raw-rgb","bpp", G_TYPE_INT, 24, NULL)  );

                if (padlink==GST_PAD_LINK_OK) {
//                    caps = gst_pad_get_caps( Gpad );
                    //pGsGraph->SetVideoFormat(caps);
#ifndef GSTVERSION
                    GstPad* srcRGB = gst_element_get_pad ( (GstElement*)pGsGraph->m_pColorSpace, "src");
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_buffer_probe_full ( srcRGB, G_CALLBACK (cb_have_data), pGsGraph, (GDestroyNotify) (cb_buffer_disconnected) );
#else

                    MODebug2->Message(moText("moGsGraph::cb_pad_added_new > padlink success, rock and rolling live video.")  );

                    GstPad* srcRGB = gst_element_get_static_pad ( (GstElement*)pGsGraph->m_pFakeSink, "sink");
                    /*
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_probe ( srcRGB,
                                                                             GST_PAD_PROBE_TYPE_BUFFER,
                                                                            (GstPadProbeCallback) cb_have_data,
                                                                             pGsGraph,
                                                                             (GDestroyNotify) (cb_buffer_disconnected) );
                                                                              */
#endif
                    //cout << "cb_newpad: linked pads..." << endl;
                } else {
                  /**
                  GST_PAD_LINK_OK	0
                  GST_PAD_LINK_WRONG_HIERARCHY	-1
                  GST_PAD_LINK_WAS_LINKED	-2
                  GST_PAD_LINK_WRONG_DIRECTION	-3
                  GST_PAD_LINK_NOFORMAT	-4
                  GST_PAD_LINK_NOSCHED	-5
                  GST_PAD_LINK_REFUSED	-6
                  */
                  moText padlink_returns = "GST_PAD_LINK_OK	0\
                  GST_PAD_LINK_WRONG_HIERARCHY	-1\
                  GST_PAD_LINK_WAS_LINKED	-2\
                  GST_PAD_LINK_WRONG_DIRECTION	-3\
                  GST_PAD_LINK_NOFORMAT	-4\
                  GST_PAD_LINK_NOSCHED	-5\
                  GST_PAD_LINK_REFUSED	-6";

                  MODebug2->Error(moText("moGsGraph::cb_pad_added_new > padlink BAD! padlink return:")+IntToStr((int)padlink) +padlink_returns );
                }

            } else {
                //version 2 con videoscale

                //version directa a videoscale
#ifndef GSTVERSION
videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pVideoScale, "sink");
#else
videopad = gst_element_get_static_pad ( (GstElement*)pGsGraph->m_pVideoScale, "sink");
#endif
                //version con deinterlace
                //videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pVideoDeinterlace, "sink");
                //bool res = gst_pad_set_caps( gst_element_get_pad ( pGsGraph->m_pColorSpace, "src"), gst_caps_new_simple ("video/x-raw-rgb","bpp", G_TYPE_INT, 24, NULL)  );

                padlink = gst_pad_link( Gpad, videopad );

                if (padlink==GST_PAD_LINK_OK) {
                    //caps = gst_pad_get_caps( Gpad );
                    //pGsGraph->SetVideoFormat(caps);
#ifndef GSTVERSION
                    GstPad* srcRGB = gst_element_get_pad ( (GstElement*)pGsGraph->m_pColorSpace, "src");
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_buffer_probe_full ( srcRGB, G_CALLBACK (cb_have_data), pGsGraph, (GDestroyNotify) (cb_buffer_disconnected) );
#else
                    GstPad* srcRGB = gst_element_get_static_pad ( (GstElement*)pGsGraph->m_pColorSpace, "src");
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_probe ( srcRGB,
                                                                           GST_PAD_PROBE_TYPE_BUFFER,
                                                                          (GstPadProbeCallback) cb_have_data,
                                                                           pGsGraph,
                                                                           (GDestroyNotify) (cb_buffer_disconnected) );
#endif
                    //cout << "cb_newpad: linked pads..." << endl;
                }
            }
          }
      }

  }

}


//#ifndef GSTVERSION
void
moGsGraph::cb_pad_added ( moGstElement *decodebin, moGstPad *pad, moGPointer u_data)
{
  decodebin = NULL;
  GstCaps *caps = NULL;
  GstPad  *videopad = NULL;
//  GstPad  *audiopad = NULL;
  GstPad  *audiopadinconverter = NULL;
  GstPadLinkReturn padlink;
  gchar* padname = NULL;
  const gchar* strname = NULL;
  GstStructure *str = NULL;
  GstPad* Gpad = (GstPad*) pad;

  moGsGraph* pGsGraph;
  GstElement* SinkElement = NULL;

    cout << "pad added" << endl;
  if (gst_pad_is_linked(Gpad)) {
      return;
  }


  if (u_data!=0) {
      pGsGraph = (moGsGraph*)u_data;
      /* check media type */
#ifndef GSTVERSION
      caps = gst_pad_get_caps (Gpad);
#else
      caps = gst_pad_get_current_caps(Gpad);
#endif
      padname = gst_pad_get_name(Gpad);
      if (padname) {
        str = gst_caps_get_structure (caps, 0);

        const gchar *sstr;

        sstr = gst_structure_to_string (str);
        cout << "moGsGraph::cb_pad_added: new pad: " << padname << " caps:" << sstr << endl;

        strname = gst_structure_get_name (str);
        cout << "moGsGraph::cb_pad_added: new pad: " << padname << " strname:" << strname << endl;


          if (g_strrstr (strname, "audio")) {
            pGsGraph->m_pAudioPad = Gpad;

            MODebug2->Message("moGsGraph::cb_pad_added: audio pad created > creating audio filters!");

            ///TODO: no andan en win
            ///pGsGraph->BuildAudioFilters();
            ///if (/*pGsGraph->m_pAudioConverter &&*/ 1==1) {
            if (pGsGraph->m_pAudioConverter) {

                gboolean link_audioresult = gst_element_link_many( (GstElement*)pGsGraph->m_pAudioConverter,
                                      (GstElement*)pGsGraph->m_pAudioVolume,
                                      (GstElement*)pGsGraph->m_pAudioPanorama,
                                      (GstElement*)pGsGraph->m_pAudioSink, NULL );
                if (link_audioresult) {
#ifndef GSTVERSION
                    audiopadinconverter = gst_element_get_pad ( (GstElement*) pGsGraph->m_pAudioConverter, "sink");
#else
                    audiopadinconverter = gst_element_get_static_pad( (GstElement*) pGsGraph->m_pAudioConverter, "sink");
#endif
                    padlink = gst_pad_link (Gpad, audiopadinconverter);

#ifndef GSTVERSION
                    GstPad* srcAudio = gst_element_get_pad ( (GstElement*)pGsGraph->m_pAudioConverter, "src");
#else
                    GstPad* srcAudio = gst_element_get_static_pad( (GstElement*)pGsGraph->m_pAudioConverter, "src");
#endif
                    if (padlink==GST_PAD_LINK_OK) {
#ifndef GSTVERSION
                        pGsGraph->cb_have_data_handler_id = gst_pad_add_buffer_probe_full ( srcAudio, G_CALLBACK (cb_have_data), pGsGraph, (GDestroyNotify) (cb_buffer_disconnected) );
#else
                        pGsGraph->cb_have_data_handler_id = gst_pad_add_probe( srcAudio,
                                                                              GST_PAD_PROBE_TYPE_BUFFER,
                                                                              (GstPadProbeCallback) cb_have_data,
                                                                              pGsGraph,
                                                                              (GDestroyNotify) (cb_buffer_disconnected) );
#endif

                    }
                }
            } else if (pGsGraph->m_pAudioSink && 1==1) {
#ifndef GSTVERSION
                audiopadinconverter = gst_element_get_pad ( (GstElement*) pGsGraph->m_pAudioSink, "sink");
#else
                audiopadinconverter = gst_element_get_static_pad ( (GstElement*) pGsGraph->m_pAudioSink, "sink");
#endif
                padlink = gst_pad_link (Gpad, audiopadinconverter);
            }


          } else if (g_strrstr (strname, "video")) {
            pGsGraph->m_pVideoPad = Gpad;

            //MODebug2->Push(moText("moGsGraph::cb_pad_added: video pad created"));
            cout << "is video" << endl;
            if (pGsGraph->m_pVideoScale==NULL) {
                //version directa a videoscale
                if (!(GstElement*)pGsGraph->m_pColorSpaceInterlace) {
                    SinkElement = (GstElement*)pGsGraph->m_pColorSpace;
                    cout << "SinkElement: m_pColorSpace" << endl;
                } else {
                    SinkElement = (GstElement*)pGsGraph->m_pColorSpaceInterlace;
                }
#ifndef GSTVERSION
                videopad = gst_element_get_pad ( SinkElement, "sink");
                if (videopad) {
                    padlink = gst_pad_link( Gpad, videopad );
                }
#else
                videopad = gst_element_get_static_pad( SinkElement, "sink");
                if (videopad) {
                    padlink = gst_pad_link( Gpad, videopad );
                }
#endif
                //version con deinterlace
                //videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pVideoDeinterlace, "sink");

                //bool res = gst_pad_set_caps( gst_element_get_pad ( pGsGraph->m_pColorSpace, "src"), gst_caps_new_simple ("video/x-raw-rgb","bpp", G_TYPE_INT, 24, NULL)  );

                if (padlink==GST_PAD_LINK_OK) {
#ifndef GSTVERSION
                    //pGsGraph->SetVideoFormat(caps);
                    GstPad* srcRGB = gst_element_get_pad ( (GstElement*)pGsGraph->m_pColorSpace, "src");
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_buffer_probe_full ( srcRGB, G_CALLBACK (cb_have_data), pGsGraph, (GDestroyNotify) (cb_buffer_disconnected) );
                    //cout << "cb_newpad: linked pads..." << endl;
#else
                    GstPad* srcRGB = gst_element_get_static_pad ( (GstElement*)pGsGraph->m_pColorSpace, "src");
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_probe ( srcRGB,
                                                                           GST_PAD_PROBE_TYPE_BUFFER,
                                                                           (GstPadProbeCallback) cb_have_data,
                                                                           pGsGraph,
                                                                           (GDestroyNotify) (cb_buffer_disconnected) );
                    //cout << "SinkElement linked" << endl;
#endif
                }
            } else {
                //version 2 con videoscale

                //version directa a videoscale
#ifndef GSTVERSION
videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pVideoScale, "sink");
#else
videopad = gst_element_get_static_pad ( (GstElement*)pGsGraph->m_pVideoScale, "sink");
#endif
                //version con deinterlace
                //videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pVideoDeinterlace, "sink");
                //bool res = gst_pad_set_caps( gst_element_get_pad ( pGsGraph->m_pColorSpace, "src"), gst_caps_new_simple ("video/x-raw-rgb","bpp", G_TYPE_INT, 24, NULL)  );

                padlink = gst_pad_link( Gpad, videopad );

                if (padlink==GST_PAD_LINK_OK) {
                    //caps = gst_pad_get_caps( Gpad );
                    //pGsGraph->SetVideoFormat(caps);
#ifndef GSTVERSION
                    GstPad* srcRGB = gst_element_get_pad ( (GstElement*)pGsGraph->m_pColorSpace, "src");
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_buffer_probe_full ( srcRGB, G_CALLBACK (cb_have_data), pGsGraph, (GDestroyNotify) (cb_buffer_disconnected) );
#else
                    GstPad* srcRGB = gst_element_get_static_pad ( (GstElement*)pGsGraph->m_pColorSpace, "src");
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_probe ( srcRGB,
                                                                           GST_PAD_PROBE_TYPE_BUFFER,
                                                                           (GstPadProbeCallback) cb_have_data,
                                                                           pGsGraph,
                                                                           (GDestroyNotify) (cb_buffer_disconnected) );
#endif
                    //cout << "cb_newpad: linked pads..." << endl;
                }
            }
          }
      }

  }

}
//#else
//#endif

#ifndef GSTVERSION
void
moGsGraph::cb_handoff (moGstElement *fakesrc,
	    moGstBuffer  *buffer,
	    moGstPad     *pad,
	    moGPointer    user_data)
{
    static gboolean white = FALSE;

    GstElement* Gfakesrc = (GstElement*)fakesrc;
    GstBuffer* Gbuffer = (GstBuffer*)buffer;
    GstPad* Gpad = (GstPad*)pad;
    Gpad = NULL;///unused
    Gfakesrc = NULL;///unused
    moGsGraph* pGsGraph;


    if (user_data!=0) {
      pGsGraph = (moGsGraph*)user_data;
      ///moAbstract::MODebug2->Push(moText("frame nbr"));
      //memset (GST_BUFFER_DATA (buffer), white ? 0x44 : 0x0, GST_BUFFER_SIZE (buffer));
      pGsGraph->CopyVideoFrame( GST_BUFFER_DATA (Gbuffer), GST_BUFFER_SIZE (Gbuffer) );
      //memcpy( GST_BUFFER_DATA (buffer), (void*)pGsGraph->GetVideoFrame(), GST_BUFFER_SIZE (buffer) );
    } else {
        ////moAbstract::MODebug2->Push(moText("black"));
        memset (GST_BUFFER_DATA (Gbuffer), white ? 0xff : 0x0, GST_BUFFER_SIZE (Gbuffer));
    }




    GstCaps *caps;

    caps = gst_caps_new_simple ("video/x-raw-rgb", "width", G_TYPE_INT, 400,
                                                    "height", G_TYPE_INT, 300,
                                                    "bpp", G_TYPE_INT, 24,
                                                    "depth", G_TYPE_INT, 24,
                                                    "framerate", GST_TYPE_FRACTION, 10, 1,
                                                   NULL);
    gst_buffer_set_caps (Gbuffer, caps);
    gst_caps_unref (caps);
    /* this makes the image black/white */


    white = !white;

}
#else
#endif

/* returns TRUE if there was an error or we caught a keyboard interrupt. */
static gboolean
event_loop (GstElement * pipeline, gboolean blocking, GstState target_state)
{
  GstBus *bus;
  GstMessage *message = NULL;
  gboolean res = FALSE;
  gboolean buffering = FALSE;

  bus = gst_element_get_bus (GST_ELEMENT (pipeline));

  if (!bus) exit(1);

  while (TRUE) {
    message = gst_bus_poll (bus, GST_MESSAGE_ANY, blocking ? -1 : 0);

    /* if the poll timed out, only when !blocking */
    if (message == NULL)
      goto exit;

    /* check if we need to dump messages to the console */
    if (true) {
      const GstStructure *s;

      s = gst_message_get_structure (message);

      g_print (("Got Message from element \"%s\" (%s): "),
          GST_STR_NULL (GST_ELEMENT_NAME (GST_MESSAGE_SRC (message))),
          gst_message_type_get_name (GST_MESSAGE_TYPE (message)));
      if (s) {
        gchar *sstr;

        sstr = gst_structure_to_string (s);
        g_print ("%s\n", sstr);
        g_free (sstr);
      } else {
        g_print ("no message details\n");
      }
    }

    switch (GST_MESSAGE_TYPE (message)) {

      case GST_MESSAGE_WARNING:{
        GError *gerror;
        gchar *debug;
        gchar *name = gst_object_get_path_string (GST_MESSAGE_SRC (message));

        /* dump graph on warning */
        GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pipeline),
            GST_DEBUG_GRAPH_SHOW_ALL, "gst-launch.warning");

        gst_message_parse_warning (message, &gerror, &debug);
        g_print (("WARNING: from element %s: %s\n"), name, gerror->message);
        if (debug) {
          g_print (("Additional debug info:\n%s\n"), debug);
        }
        g_error_free (gerror);
        g_free (debug);
        g_free (name);
        break;
      }
      case GST_MESSAGE_ERROR:{
        GError *gerror;
        gchar *debug;

        /* dump graph on error */
        GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pipeline),
            GST_DEBUG_GRAPH_SHOW_ALL, "gst-launch.error");

        gst_message_parse_error (message, &gerror, &debug);
        gst_object_default_error (GST_MESSAGE_SRC (message), gerror, debug);
        g_error_free (gerror);
        g_free (debug);
        /* we have an error */
        res = TRUE;
        goto exit;
      }
      case GST_MESSAGE_STATE_CHANGED:{
        GstState old, mnew, pending;

        gst_message_parse_state_changed (message, &old, &mnew, &pending);

        /* debug each state change
           GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "gst-launch");
         */

        /* we only care about pipeline state change messages */
        if (GST_MESSAGE_SRC (message) != GST_OBJECT_CAST (pipeline))
          break;

        /* debug only overall state changes
           {
           gchar *dump_name;

           dump_name = g_strdup_printf ("gst-launch.%s",gst_element_state_get_name (new);
           GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pipeline), GST_DEBUG_GRAPH_SHOW_ALL, dump_name);
           g_free (dump_name);
           }
         */

        /* ignore when we are buffering since then we mess with the states
         * ourselves. */
        if (buffering) {
          fprintf (stderr,
              ("Prerolled, waiting for buffering to finish...\n"));
          break;
        }

        /* if we reached the final target state, exit */
        if (target_state == GST_STATE_PAUSED && mnew == target_state)
          goto exit;

        /* else not an interesting message */
        break;
      }
      case GST_MESSAGE_BUFFERING:{
        gint percent;

        gst_message_parse_buffering (message, &percent);
        fprintf (stderr, ("buffering... %d  \r"), percent);

        /* no state management needed for live pipelines */
        /*
        if (is_live)
          break;
          */

        if (percent == 100) {
          /* a 100% message means buffering is done */
          buffering = FALSE;
          /* if the desired state is playing, go back */
          if (target_state == GST_STATE_PLAYING) {
            fprintf (stderr,
                ("Done buffering, setting pipeline to PLAYING ...\n"));
            gst_element_set_state (pipeline, GST_STATE_PLAYING);
          } else
            goto exit;
        } else {
          /* buffering busy */
          if (buffering == FALSE && target_state == GST_STATE_PLAYING) {
            /* we were not buffering but PLAYING, PAUSE  the pipeline. */
            fprintf (stderr, ("Buffering, setting pipeline to PAUSED ...\n"));
            gst_element_set_state (pipeline, GST_STATE_PAUSED);
          }
          buffering = TRUE;
        }
        break;
      }
      case GST_MESSAGE_APPLICATION:{
        const GstStructure *s;

        s = gst_message_get_structure (message);

        if (gst_structure_has_name (s, "GstLaunchInterrupt")) {
          /* this application message is posted when we caught an interrupt and
           * we need to stop the pipeline. */
          fprintf (stderr, ("Interrupt: Stopping pipeline ...\n"));
          /* return TRUE when we caught an interrupt */
          res = TRUE;
          goto exit;
        }
      }
      default:
        /* just be quiet by default */
        break;
    }
    if (message)
      gst_message_unref (message);
  }
  g_assert_not_reached ();

exit:
  {
    if (message)
      gst_message_unref (message);
    gst_object_unref (bus);
    return res;
  }
}



//===========================================
//
//     Class: moGsFramewwork
//
//===========================================

//GMainLoop *moGsGraph::loop = g_main_loop_new (NULL, FALSE);

moGsFramework::moGsFramework() {
   // m_pDevEnum = NULL;
   // m_pEnum = NULL;
}


moGsFramework::~moGsFramework() {



}

/**
GList*
gst_camera_capturer_enum_devices(gchar* device_name)
{
  GstElement* device;
  GstPropertyProbe* probe;
  GValueArray* va;
  GList* list=NULL;
  guint i=0;

  device = gst_element_factory_make (device_name, "source");
  gst_element_set_state(device, GST_STATE_READY);
  gst_element_get_state(device, NULL, NULL, 5 * GST_SECOND);
  if (!device || !GST_IS_PROPERTY_PROBE(device))
    goto finish;
  probe = GST_PROPERTY_PROBE (device);
  va = gst_property_probe_get_values_name (probe, "device-name");
  if (!va)
    goto finish;
  for(i=0; i < va->n_values; ++i) {
    GValue* v = g_value_array_get_nth(va, i);
    list = g_list_append(list, g_string_new(g_value_get_string(v)));
  }
  g_value_array_free(va);

finish:
  {
    gst_element_set_state (device, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT (device));
    return list;
  }
}


GList*
+gst_camera_capturer_enum_devices(gchar* device_name)
+{
+  GstElement* device;
+  GstPropertyProbe* probe;
+  GValueArray* va;
+  GList* list=NULL;
+  guint i=0;
+
+  device = gst_element_factory_make (device_name, "source");
+  gst_element_set_state(device, GST_STATE_READY);
+  gst_element_get_state(device, NULL, NULL, 5 * GST_SECOND);
+  if (!device || !GST_IS_PROPERTY_PROBE(device))
+    goto finish;
+  probe = GST_PROPERTY_PROBE (device);
+  va = gst_property_probe_get_values_name (probe, "device-name");
+  if (!va)
+    goto finish;
+  for(i=0; i < va->n_values; ++i) {
+    GValue* v = g_value_array_get_nth(va, i);
+    list = g_list_append(list, g_string_new(g_value_get_string(v)));
+  }
+  g_value_array_free(va);
+
+finish:
+ {
+  gst_element_set_state (device, GST_STATE_NULL);
+  gst_object_unref(GST_OBJECT (device));
+  return list;
+ }
+}
*/


moCaptureDevices* moGsFramework::LoadCaptureDevices() {

        GstElement* device;
        #ifndef GSTVERSION
        GstPropertyProbe* probe;
        #endif
        GValueArray* va;
        GList *plist;
        GParamSpec* pm;
        GValue* vdefault;
        GValue valDef = { 0, };
        //GList* list=NULL;
        //guint i=0;
        gchar* device_name;

        MODebug2->Message( "moGsFramework::LoadCaptureDevices running..." );

        m_CaptureDevices.Empty();

        for( int i=0; i<m_PreferredDevices.Count(); i++) {
            MODebug2->Message( moText("moGsFramework::PreferredDevices: ") + IntToStr(i)
            + moText(" Name: ") + m_PreferredDevices[i].GetName()
            + moText(" LabelName: ") + m_PreferredDevices[i].GetLabelName()
            + moText(" Path: ") + m_PreferredDevices[i].GetPath()
            + moText(" Port: ") + IntToStr(m_PreferredDevices[i].GetPort())
            + moText(" W: ") + IntToStr(m_PreferredDevices[i].GetSourceWidth())
            + moText(" H: ") + IntToStr(m_PreferredDevices[i].GetSourceHeight())
            + moText(" FlipH: ") + IntToStr(m_PreferredDevices[i].GetSourceFlipH())
            + moText(" FlipV: ") + IntToStr(m_PreferredDevices[i].GetSourceFlipV())
            + moText(" Bpp: ") + IntToStr(m_PreferredDevices[i].GetSourceBpp()) );
        }

if (m_PreferredDevices.Count()==0) {
	    moText cap_dev_name = moText("default");
            moCaptureDevice newdev;
            newdev.Present(true);

            newdev.SetName(cap_dev_name);
            newdev.SetLabelName("LIVEIN"+IntToStr(m_CaptureDevices.Count()));

	    m_PreferredDevices.Add( newdev );
	}
  #ifdef MO_WIN32
        //m_CaptureDevices.Add( moCaptureDevice( moText("Laptop Integrated Webcam"), moText("webcam"), moText("-") ) );
        //m_CaptureDevices.Add( moCaptureDevice( moText("Default"), moText("-"), moText("-") ) );
        #ifdef GSTVERSION
        moText dname( "ksvideosrc" );
        #else
        moText dname( "dshowvideosrc" );
        #endif
        device_name = dname;

        for( MOuint i=0; i<m_PreferredDevices.Count();i++) {
            moCaptureDevice CaptDev = m_PreferredDevices[i];
            CaptDev.SetLabelName("LIVEIN"+IntToStr(m_CaptureDevices.Count()));
            AddCaptureDevice( CaptDev );
            MODebug2->Message( "moGsFramework::LoadCaptureDevices > Added preferred device: " + CaptDev.GetLabelName() );
        }
        //m_CaptureDevices.Add( moCaptureDevice( moText("Laptop Integrated Webcam"), moText("webcam"), moText("-") ) );
        //m_CaptureDevices.Add( moCaptureDevice( moText("Microsoft DV Camera and VCR"), moText("DV IEEE 1394"), moText("-"), 0 ) );
        //m_CaptureDevices.Add( moCaptureDevice( moText("VideoCAM Messenger"), moText("webcam"), moText("-") ) );
        //DIRECT SHOW TEST//
        /*
        HRESULT hr;

        // Create the System Device Enumerator.
        if(m_pDevEnum==NULL) {
            HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
                CLSCTX_INPROC_SERVER, IID_ICreateDevEnum,
                reinterpret_cast<void**>(&m_pDevEnum));

            if(SUCCEEDED(hr) && m_pEnum==NULL)
            {
                // Create an enumerator for the video capture category.
                hr = m_pDevEnum->CreateClassEnumerator(
                    CLSID_VideoInputDeviceCategory,
                    &m_pEnum, 0);
            } else {
                ShowError(hr);
                return &m_CaptureDevices;
            }
        }
        */
    #else
        #ifdef MO_MACOSX
        device_name = CAMERABINOSX;
        #else
        device_name = moText("v4l2src");
        //m_CaptureDevices.Add( moCaptureDevice( moText("Default"), moText("default"), moText("/dev/video0") ) );
        #endif
        // in linux: for v4l2src   device could be  /dev/video0   -   /dev/video1   etc...
        //m_CaptureDevices.Add( moCaptureDevice( moText("Default"), moText("default") );
    	//m_CaptureDevices.Add( moCaptureDevice( moText("Laptop Integrated Webcam"), moText("webcam"), moText("/dev/video0") ) );
        //m_CaptureDevices.Add( moCaptureDevice( moText("DV"), moText("DV IEEE 1394"), moText("-"), 0 ) );

        for(int i=0; i<m_PreferredDevices.Count();i++) {
            moCaptureDevice CaptDev = m_PreferredDevices[i];
            CaptDev.SetLabelName("LIVEIN"+IntToStr(m_CaptureDevices.Count()));
            AddCaptureDevice( CaptDev );
            MODebug2->Message( "moGsFramework::LoadCaptureDevices > Added preferred device: " + CaptDev.GetLabelName() );
        }



    #endif


#ifndef GSTVERSION
  try {

    device = gst_element_factory_make (device_name, "source");
    gst_element_get_state(device, NULL, NULL, 5 * GST_SECOND);
     moText probepname = "device-name";
    if (!device || !GST_IS_PROPERTY_PROBE(device))
      goto finish;
    probe = GST_PROPERTY_PROBE (device);
    if (probe) {
        plist = (GList *)gst_property_probe_get_properties( probe );
        if (plist) {
            plist = (GList *)g_list_first(plist);
            do {
                pm = (GParamSpec *)plist->data;
                if (pm) {
                    if (pm->name) {
                        probepname = moText((char*)pm->name);
                        MODebug2->Message( "moGsFramework::LoadCaptureDevices > probe property:"+probepname);
                        va = gst_property_probe_get_values(probe, pm);
                        if (va) {
                            MODebug2->Message( "moGsFramework::LoadCaptureDevices > probe property:"+probepname+" has values!");
                        }
                    }
                }
            } while( plist=g_list_next(plist) );
        }
    }
    va = gst_property_probe_get_values_name (probe, (char*)probepname);
    //va = gst_property_probe_get_values_name (probe, "device");
    if (!va) {
        //TRY TO SET DEFAULT VALUE FROM PARAM SPEC
        g_value_init( &valDef, G_PARAM_SPEC_VALUE_TYPE(pm) );
        //vdefault = g_param_spec_get_default_value ( pm );
        g_param_value_set_default( pm, &valDef );
        vdefault = &valDef;
        if (vdefault) {
            moText defaultText(g_value_get_string( vdefault ));
            MODebug2->Message("moGsFramework::LoadCaptureDevices > Default value for: \""+moText((char*)probepname)+"\" is "+defaultText);
            //G_VALUE_TYPE_NAME(vdefault);
            moText cap_dev_name = defaultText;
            moCaptureDevice newdev;
            newdev.Present(true);

            newdev.SetName(cap_dev_name);
            newdev.SetLabelName("LIVEIN"+IntToStr(m_CaptureDevices.Count()));

            m_CaptureDevices.Add( newdev );

            MODebug2->Message( "moGsFramework::LoadCaptureDevices > AUTO Added Default capture device: " + newdev.GetName() + " label:" + newdev.GetLabelName() );
        }
    }
    if (!va)
      goto finish;
    for(guint i=0; i < va->n_values; ++i) {
      GValue* v = g_value_array_get_nth(va, i);
      //GArray* v = g_array_index(va, i);
      GString* stv = g_string_new( g_value_get_string(v) );
      if (stv) {
          moText cap_dev_name = moText((char*)stv->str);
          moCaptureDevice newdev;
          newdev.Present(true);

          newdev.SetName(cap_dev_name);
          newdev.SetLabelName("LIVEIN"+IntToStr(m_CaptureDevices.Count()));

          m_CaptureDevices.Add( newdev );
          MODebug2->Message( "moGsFramework::LoadCaptureDevices > AUTO Added" );
      }
      //list = g_list_append(list, );
    }
    g_value_array_free(va);

    finish:
    {
    gst_element_set_state (device, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT (device));
    }
  }
  catch(...) {
    MODebug2->Error("moGsFramework::LoadCaptureDevices > exception error.");
  }
#else
#if (GST_VERSION_MINOR >= 8)
  GstDeviceMonitor *monitor = NULL;
  GList *devices = NULL;
  GstStructure* properties = NULL;

  monitor = gst_device_monitor_new();
  GstCaps *mon_caps = gst_caps_new_empty_simple ("video/x-raw");
  gst_device_monitor_add_filter (monitor, "Video/Source", mon_caps);
  gst_caps_unref (mon_caps);

  if (!gst_device_monitor_start (monitor))
      g_error ("Failed to start device monitor!");

    devices = gst_device_monitor_get_devices (monitor);
    int idev = 0;
    if (devices != NULL) {
      while (devices != NULL) {
        GstDevice *deviced = (GstDevice*)devices->data;

        gchar *device_class, *caps_str, *name;
        moText t_device_path;
        GstCaps *caps;
        guint i, size = 0;

        caps = gst_device_get_caps (deviced);
        if (caps != NULL)
          size = gst_caps_get_size (caps);

        name = gst_device_get_display_name (deviced);
        properties = gst_device_get_properties(deviced);
        if (properties) {
//          gchar *propstr = gst_structure_to_string(properties);
            gchar dpath[] = "device.path";
          t_device_path = (char*)gst_structure_get_string(properties,dpath);
//          if (propstr) {
//            MODebug2->Message( moText("moGsFramework::LoadCaptureDevice > properties: ") + moText(propstr));
//          }
        }

        MODebug2->Message( moText("moGsFramework::LoadCaptureDevice > name: ") + moText(name)+ " path:"+moText(t_device_path));

        device_class = gst_device_get_device_class (deviced);

        for (i = 0; i < size; ++i) {
            GstStructure *s = gst_caps_get_structure (caps, i);
            caps_str = gst_structure_to_string (s);
            //g_print ("\t%s %s\n", (i == 0) ? "caps  :" : "       ", caps_str);
            MODebug2->Message( moText("moGsFramework::LoadCaptureDevice > ")+ moText(" caps: ") + moText(caps_str) + " device_class:" + moText(device_class) );
            g_free (caps_str);
        }

        moText cap_dev_name = name;

        if (m_CaptureDevices.Count()>idev) {

          moCaptureDevice upddev = m_CaptureDevices.Get(idev);

          if (idev>0) upddev.SetName(cap_dev_name);
          upddev.m_Path = t_device_path;
          upddev.Present(true);
          upddev.SetLabelName( "LIVEIN" + IntToStr(idev) );
          m_CaptureDevices.Set(idev,upddev);

        } else {

          moCaptureDevice newdev;

          newdev.Present(true);
          newdev.SetName(cap_dev_name);
          newdev.m_Path = t_device_path;
          newdev.SetLabelName( "LIVEIN" + IntToStr(idev) );

          m_CaptureDevices.Add( newdev );
        }



        //device_added (device);
        gst_object_unref (deviced);
        devices = g_list_remove_link (devices, devices);
        idev++;
      }
    } else {
      g_print ("No devices found!\n");
  }
#endif // GST_VERSION_MINOR
#endif
  ///iterate thru list and populate m_CaptureDevices (best as we can)


	return &m_CaptureDevices;

}



moCaptureDevices* moGsFramework::UpdateCaptureDevices() {


	return &m_CaptureDevices;

}


bool
moGsFramework::CheckCaptureDevice( int i ) {
  /// TODO: implement CheckCaptureDevice
  i = 0;///unused

	return false;

}

bool
moGsFramework::AddCaptureDevice(  moCaptureDevice& p_capdev ) {
    ///try to load: then if couldn't load it return false....

    for(int i=0; i<(int)m_CaptureDevices.Count(); i++) {
        if ( m_CaptureDevices[i].GetName() ==  p_capdev.GetName() ) {
            return false;
        }
    }

    m_CaptureDevices.Add( p_capdev );

    MODebug2->Message( moText("moGsFramework::AddCaptureDevice > Added capture device:") + p_capdev.GetName() + " label:" + p_capdev.GetLabelName() );

    return true;
}

//===========================================
//
//     Class: moGsGraph
//
//===========================================

moGsGraph::moGsGraph() {

    m_pGstBus = NULL;
    m_pGMainLoop = NULL;
    m_pGMainContext = NULL;
    m_pGstPipeline = NULL;
    m_pGsFramework = NULL;

    m_pFileSource = NULL;
    m_pFinalSource = NULL;
    m_pFileSink = NULL;
    m_pRTSPSource = NULL;
    m_pRTSPDepay = NULL;
    m_pHTTPSource = NULL;
    m_pMultipartDemux = NULL;
    m_pJpegDecode = NULL;
    m_pDecoderBin = NULL;
    m_pEncoder = NULL;

    m_pTypeFind = NULL;
    m_pCapsFilter = NULL;
    m_pFakeSink = NULL;
    m_pFakeSource = NULL;
    m_pIdentity = NULL;

    m_pBucketsPool = NULL;
    m_pVideoScale = NULL;
    m_pVideoFlip = NULL;
    m_pVideoBalance = NULL;

    m_pVideoDeinterlace = NULL;
    m_pColorSpaceInterlace = NULL;
    m_pColorSpace = NULL;

    m_pAudioConverter = NULL;
    m_pAudioConverter2 = NULL;
    m_pAudioConverter3 = NULL;
    m_pAudioConverter4 = NULL;
    m_pAudioEcho =  NULL;
    m_pAudioPanorama =  NULL;
    m_pAudioAmplify =  NULL;
    m_pAudioSpeed =  NULL;
    m_pAudioVolume =  NULL;
    m_pAudioSink = NULL;

    m_pAudioPad = NULL;
    m_pVideoPad = NULL;

    signal_newpad_id = 0;
    signal_handoff_id = 0;
    cb_have_data_handler_id = 0;
    m_bEOS = false;

}

moGsGraph::~moGsGraph() {
	//last try to release objects
	FinishGraph();
}


/*
  GstElementFactory *factory;
  GstElement * element;

  // init GStreamer
  gst_init (&argc, &argv);

  // create element, method #2
  factory = gst_element_factory_find ("fakesrc");
  if (!factory) {
    g_print ("Failed to find factory of type 'fakesrc'\n");
    return -1;
  }
  element = gst_element_factory_create (factory, "source");
  if (!element) {
    g_print ("Failed to create element, even though its factory exists!\n");
    return -1;
  }

  gst_object_unref (GST_OBJECT (element));

  return 0;

*/

	//INIT METHODS
bool
moGsGraph::InitGraph() {

    signal_newpad_id = 0;
    signal_handoff_id = 0;
    cb_have_data_handler_id = 0;
    m_BusWatchId = 0;
    m_bEOS = false;

    //opner en el main de la consola...
    //inicialización de la libreria gstreamer
    //guint major, minor, micro, nano;
    //GError *errores;

    MODebug2->Message( moText("Initializing GStreamer"));
    //bool init_result = gst_init_check (NULL, NULL, &errores);

    //gst_init(NULL, NULL);
    //init_result = init_result && gst_controller_init(NULL,NULL);

    //gst_version (&major, &minor, &micro, &nano);
    //MODebug2->Message( moText("GStreamer version") + IntToStr(major) + moText(".") + IntToStr(minor) + moText(".") + IntToStr(minor));
    //char vers[10];
    //sprintf( vers, "version: %i.%i.%i.%i",major,minor, micro, nano);

    //if (init_result) MODebug2->Push(moText("Initializing GStreamer:OK "));

//analogo a FilterGraph, con dos parametros para dar de alta el elemento: playbin
//playbin
//player
    MODebug2->Message( moText("creating pipeline"));
    m_pGstPipeline = gst_pipeline_new ("pipeline");

    //buscar un tipo de filtro: factory = gst_element_factory_find ("fakesrc");
    //generarlo: gst_element_factory_make ( factory, "player");
    //o  gst_element_factory_make ("playbin", "player");
    //tomar el valor de una propiedad: g_object_get (G_OBJECT (element), "name", &name, NULL);

    MODebug2->Message( moText("creating bus interface"));
    m_pGstBus = gst_pipeline_get_bus (GST_PIPELINE (m_pGstPipeline));
    m_BusWatchId = gst_bus_add_watch ( (GstBus*)m_pGstBus, bus_call, this );
    gst_object_unref (m_pGstBus);
    m_pGstBus = NULL;

/*
    GMainLoop *loop = g_main_loop_new( NULL, FALSE);
    m_pGMainLoop = (moGMainLoop*) loop;
    if (loop) {
      m_pGMainContext = (moGMainContext*) g_main_loop_get_context( loop );
    }
    */
    m_pGMainContext = (moGMainContext*) g_main_context_default();
    //fin inicialización

/*
    m_pGstPipeline = gst_element_factory_make ("playbin", "play");
    g_object_set (G_OBJECT (m_pGstPipeline), "uri", "file:///home/fabri/plasma.mpg", NULL);

    m_pGstBus = gst_pipeline_get_bus (GST_PIPELINE (m_pGstPipeline));
    gst_bus_add_watch (m_pGstBus, bus_call, loop);
    gst_object_unref (m_pGstBus);


    CheckState( gst_element_set_state (m_pGstPipeline, GST_STATE_PAUSED), true );
*/
    /* now run */

    //g_main_loop_run (moGsGraph::loop);
    MODebug2->Message( moText("moGsGraph::Init result:") + moText(((m_pGstPipeline!=NULL) ? "success" : "failure")) );
    m_bInitialized = m_pGstPipeline!=NULL;
	return (m_bInitialized);
}


bool
moGsGraph::FinishGraph() {

    if (IsRunning()) {
        Stop();
    }

    if (m_BusWatchId!=0) {
        if (!g_source_remove(m_BusWatchId)) {
            MODebug2->Error(moText("Error releasing bus call watch:") + IntToStr(m_BusWatchId));
        } else m_BusWatchId = 0;
    }


    if (m_pGMainLoop) {

      g_main_loop_quit( (GMainLoop*) m_pGMainLoop );
      g_main_loop_unref( (GMainLoop*) m_pGMainLoop);

      m_pGMainLoop = NULL;
      m_pGMainContext = NULL;
    }

    if (m_pColorSpace) {
#ifndef GSTVERSION
        GstPad* srcRGB = gst_element_get_pad ( (GstElement*)m_pColorSpace, "src");
        if (srcRGB && cb_have_data_handler_id) gst_pad_remove_buffer_probe ( srcRGB, cb_have_data_handler_id );
#endif
        cb_have_data_handler_id = 0;
    }

    if (m_pColorSpaceInterlace) {
#ifndef GSTVERSION
        GstPad* srcRGB = gst_element_get_pad ( (GstElement*)m_pColorSpaceInterlace, "src");
        if (srcRGB && cb_have_data_handler_id) gst_pad_remove_buffer_probe ( srcRGB, cb_have_data_handler_id );
#endif
        cb_have_data_handler_id = 0;
    }


    if (m_pFileSource) {
        //gst_object_unref( (GstElement*) m_pFileSource);
        m_pFileSource = NULL;
    }

    if (m_pJpegDecode) {
        //gst_object_unref( (GstElement*) m_pJpegDecode);
        m_pJpegDecode = NULL;
    }

    if (m_pMultipartDemux) {
        //gst_object_unref( (GstElement*) m_pMultipartDemux);
        m_pMultipartDemux = NULL;
    }

    if (m_pHTTPSource) {
        //gst_object_unref( (GstElement*) m_pHTTPSource);
        m_pHTTPSource = NULL;
    }

    if (m_pRTSPDepay) {
        //gst_object_unref( (GstElement*) m_pRTSPDepay);
        m_pRTSPDepay = NULL;
    }

    if (m_pRTSPSource) {
        //gst_object_unref( (GstElement*) m_pRTSPSource);
        m_pRTSPSource = NULL;
    }

    /** this is only a reference doesnt destroy it, just pass to null**/
    if (m_pFinalSource) {
        m_pFinalSource = NULL;
    }

    if (m_pColorSpace) {
        //gst_object_unref( (GstElement*) m_pColorSpace);
        m_pColorSpace = NULL;
    }

    if (m_pColorSpaceInterlace) {
        //gst_object_unref( (GstElement*) m_pColorSpaceInterlace);
        m_pColorSpaceInterlace = NULL;
    }

    if (m_pCapsFilter) {
        //gst_object_unref( (GstElement*) m_pCapsFilter);
        m_pCapsFilter = NULL;
    }

    if (m_pDecoderBin) {
        if (g_signal_handler_is_connected((GstElement*)m_pDecoderBin, signal_newpad_id))
            g_signal_handler_disconnect ( (GstElement*)m_pDecoderBin, signal_newpad_id );
        signal_newpad_id = 0;
        //gst_object_unref( (GstElement*) m_pDecoderBin);
        m_pDecoderBin = NULL;
    }

    if (m_pFakeSink) {
        //gst_object_unref( (GstElement*) m_pFakeSink);
        m_pFakeSink = NULL;
    }

    if (m_pAudioConverter) {
        //gst_object_unref( (GstElement*) m_pAudioConverter);
        m_pAudioConverter = NULL;
    }

    if (m_pAudioSink) {
        //gst_object_unref( (GstElement*) m_pAudioSink);
        m_pAudioSink = NULL;
    }

    if (m_pAudioPad) {
        //gst_object_unref( (GstPad*) m_pAudioPad);
        m_pAudioPad = NULL;
    }

    if (m_pVideoPad) {
        //gst_object_unref( (GstPad*) m_pVideoPad);
        m_pVideoPad = NULL;
    }

    if (m_pFakeSource) {
        if (g_signal_handler_is_connected((GstElement*)m_pFakeSource, signal_handoff_id))
            g_signal_handler_disconnect ( (GstElement*)m_pFakeSource, signal_handoff_id );
        signal_handoff_id = 0;
        //gst_object_unref( (GstElement*) m_pFakeSource);
        m_pFakeSource = NULL;
    }

    if (m_pFileSink) {
        //gst_object_unref( (GstElement*) m_pFileSink);
        m_pFileSink = NULL;
    }

    if (m_pGstBus) {
        //gst_object_unref( (GstElement*) m_pGstBus);
        m_pGstBus = NULL;
    }

    if (m_pVideoDeinterlace) {
        gst_object_unref( (GstElement*) m_pVideoDeinterlace);
        m_pVideoDeinterlace = NULL;
    }

    if (m_pVideoScale) {
        //gst_object_unref( (GstElement*) m_pVideoScale);
        m_pVideoScale = NULL;
    }

    if (m_pVideoFlip) {
        //gst_object_unref( (GstElement*) m_pVideoScale);
        m_pVideoFlip = NULL;
    }

    ///FINALLY UNREFERENCE PIPELINE, DESTROY ALL
    if (m_pGstPipeline) {
        gst_object_unref( (GstElement*) m_pGstPipeline);
        m_pGstPipeline = NULL;
    }


	return false;
}

bool
moGsGraph::IsEOS() {
  return m_bEOS;
}

void
moGsGraph::SetEOS(bool iseos) {
  m_bEOS = iseos;
}



//FILTER METHODS
bool
moGsGraph::SetCaptureDevice( moText deviceport, MOint idevice) {

  /// TODO: implement SetCaptureDevice
  deviceport = "";
  idevice = 0;
	return false;
}


void
moGsGraph::CopyVideoFrame( void* bufferdst, int size ) {

    //int ttid = m_pDirectorCore->GetResourceManager()->GetTextureMan()->GetTextureMOId( moText("preview_texture"), false);
    if (m_pBucketsPool) {
        moBucket* pBucket = m_pBucketsPool->RetreiveBucket();

        if (pBucket) {
            void* pbuf = pBucket->GetBuffer();

            pBucket->Lock();
            memcpy( bufferdst, (void*)pbuf, size );
            pBucket->Unlock();
            m_pBucketsPool->DestroyRetreivedBucket();

        }

    }

}

/**
selecciona entrada!!!

v4lctl setnorm NTSC

y

v4lctl setinput Composite1

*/
bool
moGsGraph::BuildLiveGraph( moBucketsPool *pBucketsPool, moCaptureDevice p_capdev) {

    return BuildLiveWebcamGraph(  pBucketsPool, p_capdev );
}

/**
SEND
gst-launch -m filesrc location=test-aac.3gp ! qtdemux ! rtpmp4gpay ! udpsink  host=172.20.122.9 port=19790

RECEIVE
gst-launch -m udpsrc port=19790 !  rtpmp4gdepay ! faad ! queue ! amrnbenc ! rtpamrpay pt=98 ! udpsink  host=172.20.122.23 port=2006

SEND:
gst-launch -m filesrc location=test.wav ! wavparse ! audioconvert ! audioresample !alawenc !  rtppcmapay ! udpsink  host=127.0.0.1 port=19790

RECEIVE:
gst-launch -m udpsrc port=19790 !  rtppcmadepay ! alawdec ! queue ! amrnbenc ! rtpamrpay pt=98 ! udpsink  host=127.0.0.1 port=2006


BRIEF demo of a SKYPE "like"  video::audio  Gstreamer Desktop APP setup between 2 operators...one using a WINDOWS 10 laptop, and the other using a  LINUX LUBUNTU 18.04 laptop

all video and audio streams created with Gstreamer scripts
https://gstreamer.freedesktop.org/

WINDOWS 10 GSTREAMER SCRIPTS:
*********************************************************************
AUDIO TRANSMIT from MICROPHONE
gst-launch-1.0 -v directsoundsrc !  audiorate ! audioconvert ! audioresample !  opusenc frame-size=5  bitrate-type=1 ! rtpopuspay  ! queue ! udpsink host=192.168.1.101 port=4001

AUDIO RECEIVE from other Gstreamer OP:
gst-launch-1.0 -v udpsrc port=4002 caps="application/x-rtp,channels=2" !  rtpjitterbuffer   ! rtpopusdepay ! opusdec plc=true  ! audioconvert ! directsoundsink  buffer-time=100000

SEND VIDEO to yourself and the other Gstreamer OP
gst-launch-1.0 -v ksvideosrc ! "image/jpeg,width=1280, height=720,framerate=30/1" ! rtpjpegpay ! multiudpsink clients=192.168.1.101:5006,192.168.1.142:5007

RECEIVE your own video stream
gst-launch-1.0  -v udpsrc port=5007 ! application/x-rtp, encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegparse ! jpegdec ! autovideosink

RECEIVE video from other Gstreamer OP
gst-launch-1.0  -v udpsrc port=5003 ! application/x-rtp, encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegparse ! jpegdec ! autovideosink



LINUX GSTREAMER SCRIPTS:
******************************************************************
AUDIO RECEIVE from other Gstreamer OP
gst-launch-1.0 -v udpsrc port=4001 caps="application/x-rtp,channels=2" !  rtpjitterbuffer   ! rtpopusdepay ! opusdec plc=true  ! audioconvert ! jackaudiosink  buffer-time=100000

AUDIO TRANSMIT from MICROPHONE
gst-launch-1.0 -v jackaudiosrc !  audiorate ! audioconvert ! audioresample !  opusenc frame-size=5 bitrate-type=1 ! rtpopuspay  !  queue ! udpsink host=192.168.1.142 port=4002

SEND VIDEO to yourself and the other Gstreamer OP
gst-launch-1.0 -v v4l2src device=/dev/video0 ! "image/jpeg,width=640, height=480,framerate=30/1" ! rtpjpegpay ! multiudpsink clients=192.168.1.142:5003,192.168.1.101:5004

receive your own video stream
gst-launch-1.0 -e -v udpsrc port=5004 ! application/x-rtp, encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegparse ! jpegdec ! autovideosink

receive the other Gstreamer OP's video stream
gst-launch-1.0 -e -v udpsrc port=5006 ! application/x-rtp, encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegparse ! jpegdec ! autovideosink

NOTE: on the video transmit scripts...MULTIUDP sink is used, so that when only using 1 webcam...you can send your own video stream from the webcam to yourself to monitor your cam and position it  etc... and to the other Gstreamer OP


*/
bool
moGsGraph::BuildLiveStreamingGraph( moBucketsPool *pBucketsPool, moText p_location ) {
    /// TODO: implement BuildLiveStreamingGraph
    p_location = "";
    pBucketsPool = NULL;
    return false;
}

/**
grab at the same time with:
gst-launch-0.10 v4l2src ! queue ! ffmpegcolorspace ! theoraenc quality=1 ! queue ! oggmux name=mux alsasrc ! audio/x-raw-int,rate=8000,channels=1,depth=8 ! queue ! audioconvert ! vorbisenc ! queue ! mux. mux. ! queue ! tee name= t ! queue ! filesink location=test.ogg t. ! queue ! shout2send ip=giss.tv port=8000 password=pass

*/
bool
moGsGraph::BuildRecordGraph( moText filename, moBucketsPool *pBucketsPool ) {
    /// TODO: implement BuildRecordGraph
    m_pBucketsPool = pBucketsPool;
    bool link_result = false;
    /*
    bool b_sourceselect = false;
    bool b_forcevideoscale = false;
    bool b_forcevideoflip = false;
    */
    //gchar* checkval;
    bool res = false;


    if (filename.Length()>0)
    {

        m_pFakeSource = gst_element_factory_make ("fakesrc", "source");

        /* setup fake source */
        if (m_pFakeSource) {
            g_object_set (G_OBJECT (m_pFakeSource),
                "signal-handoffs", TRUE,
                "sizemax", 400 * 300 * 3,
                "silent", TRUE,
                "sync", TRUE,
                "num-buffers", 30*200,
                "sizetype", 2, NULL);
            #ifndef GSTVERSION
            signal_handoff_id = g_signal_connect (m_pFakeSource, "handoff", G_CALLBACK (cb_handoff), this);
            #endif

            res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pFakeSource );
        }


        m_pCapsFilter = gst_element_factory_make ("capsfilter", "filtsource");
        if (m_pCapsFilter) {
           g_object_set (G_OBJECT (m_pCapsFilter), "caps", gst_caps_new_simple ("video/x-raw-rgb",
           "width", G_TYPE_INT, 400,
           "height", G_TYPE_INT, 300,
           "framerate", GST_TYPE_FRACTION, 10, 1,
           "bpp", G_TYPE_INT, 24,
           "depth", G_TYPE_INT, 24,
           "red_mask",G_TYPE_INT, 255,/** 0x000000FF */
           "green_mask",G_TYPE_INT, 65280,/** 0x0000FF00 */
           "blue_mask",G_TYPE_INT, 16711680,/** 0x00FF0000 */
           "endianness", G_TYPE_INT, 4321,
           NULL), NULL);
           //depth=(int)24, red_mask=(int)16711680, green_mask=(int)65280, blue_mask=(int)255, endianness=(int)4321
           res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pCapsFilter );
        }


       m_pColorSpace = gst_element_factory_make (VIDEOCONVERT, "color");
       if (m_pColorSpace) {
            res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pColorSpace );
       }

       link_result = gst_element_link_many( (GstElement*) m_pFakeSource, (GstElement*) m_pCapsFilter, (GstElement*) m_pColorSpace, NULL );

       if (link_result) {

           m_pEncoder = gst_element_factory_make( "ffenc_mpeg1video", "encoder");
           if (m_pEncoder) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pEncoder );
           }

           m_pMultiplexer = gst_element_factory_make( "ffmux_mpeg", "multiplexer");
           if (m_pMultiplexer) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pMultiplexer );
           }

           m_pFileSink = gst_element_factory_make( "filesink", "filesink");
           if (m_pFileSink) {
                g_object_set (G_OBJECT (m_pFileSink), "location", (char*)filename, NULL);
                res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pFileSink );
           }

           link_result = gst_element_link_many( (GstElement*) m_pColorSpace, (GstElement*) m_pEncoder, (GstElement*) m_pMultiplexer, (GstElement*) m_pFileSink, NULL );
           //link_result = gst_element_link_many( (GstElement*) m_pColorSpace, (GstElement*) m_pEncoder, NULL );
           //link_result = gst_element_link_many( (GstElement*) m_pColorSpace, (GstElement*) m_pEncoder, (GstElement*) m_pMultiplexer, NULL );

            if (link_result) {
                //if (CheckState( gst_element_set_state ((GstElement*) m_pGstPipeline, GST_STATE_PLAYING), false /*SYNCRUNASLI*/ )) {
                gst_element_set_state ( (GstElement*) m_pGstPipeline, GST_STATE_PLAYING);

                    return true;
                //}
            }
       } else return false;

    }

    return false;
}


bool
moGsGraph::BuildLiveDVGraph( moBucketsPool *pBucketsPool, moCaptureDevice &p_capdev ) {

  /// TODO: no need to implement ?!
  moCaptureDevice pp = p_capdev;
  pBucketsPool = NULL;
	return true;
}

/**
gstreamer example tests: gst-launch -v dshowvideosrc ! ffmpegcolorspace ! video/x-raw-rgb,bpp=16  ! videoscale ! video/x-raw-rgb,width=160,height=120 ! autovideosink

gstreamer example tests: gst-launch -v dshowvideosrc ! ffmpegcolorspace ! video/x-raw-rgb,bpp=16  ! videoscale ! video/x-raw-rgb,width=160,height=120 ! autovideosink

gst-launch rtspsrc location="rtsp://192.168.0.101/live.sdp" latency=100 ! queue ! rtpmp4vdepay ! queue ! ffdec_mpeg4 ! ffmpegcolorspace ! autovideosink


usando TCP se soluciona la desconección del server automática....

gst-launch rtspsrc location="rtsp://192.168.0.101/live.sdp" latency=100 debug=true protocols=0x00000004 ! queue ! rtpmp4vdepay ! queue ! ffdec_mpeg4 ! ffmpegcolorspace ! autovideosink


referencias:

http://nuald.blogspot.com/2009/07/creating-video-conference-application.html
http://stackoverflow.com/questions/1884204/playing-an-incoming-rtp-stream-with-gstreamer

*/

bool
moGsGraph::BuildLiveWebcamGraph( moBucketsPool *pBucketsPool, moCaptureDevice &p_capdev ) {

    m_pBucketsPool = pBucketsPool;
    GstCaps *caps = NULL;
    GstCaps *rsrc_caps = NULL;
    bool link_result = false;

    bool b_sourceselect = false;
    bool b_forcevideoscale = false;
    bool b_forcevideoflip = false;

    bool b_forcevideointerlace = false;

    //gchar* checkval;
    bool res = false;
    //GstPadLinkReturn ret_padlink;

    moGstElement* m_pColorSpaceSource = NULL;

    moGstElement* m_pCapsFilterSource = NULL;
    moGstElement* m_pCapsFilter2 = NULL;

    moText labelname;
    moText devicename;
    moText devicepath;
    MOint p_sourcewidth;
    MOint p_sourceheight;
    MOint p_sourcebpp;
    MOint p_forcewidth;
    MOint p_forceheight;
    MOint p_forceflipH;
    MOint p_forceflipV;
    moText colormode;
    moText devinfo;

    labelname = p_capdev.GetLabelName();

    devicename = p_capdev.GetName();
    devicepath = p_capdev.GetPath();
    switch( p_capdev.GetVideoFormat().m_ColorMode) {
      case YUV:
        colormode = moText("video/x-raw-yuv");
        break;
      case RGB:
        colormode = moText("video/x-raw-rgb");
        break;
      default:
        colormode = "";
        break;
    };
    p_sourcewidth = p_capdev.GetSourceWidth();
    p_sourceheight = p_capdev.GetSourceHeight();
    p_sourcebpp = p_capdev.GetSourceBpp();

    p_forcewidth = p_capdev.GetVideoFormat().m_Width;
    p_forceheight = p_capdev.GetVideoFormat().m_Height;
    p_forceflipH = p_capdev.GetSourceFlipH();
    p_forceflipV = p_capdev.GetSourceFlipV();

    if (p_forcewidth!=0 || p_forceheight!=0) {
        b_forcevideoscale = true;
    }

    if (p_forceflipH!=0 || p_forceflipV!=0) {
        b_forcevideoflip = true;
    }

    if (p_sourcewidth!=0 || p_sourceheight!=0) {
        b_sourceselect = true;
    }

    devinfo = moText("Label/Texture ") + labelname;
    devinfo+= moText("; DeviceName ") + devicename;
    devinfo+= moText("; DevicePath ") + devicepath;
    devinfo+= moText("; colormode ") + colormode;
    devinfo+= moText("; width ") + IntToStr(p_sourcewidth);
    devinfo+= moText("; height ") + IntToStr(p_sourceheight);
    devinfo+= moText("; flipH ") + IntToStr(p_forceflipH);
    devinfo+= moText("; flipV ") + IntToStr(p_forceflipV);
    if (devicename.Length()>0)
    {

        std::string dname;

        dname = devicename;

        //if (labelname==moText("RTSP")) {
        int rtspindex = -1;
        int httpindex = -1;
        rtspindex = dname.find("rtsp");
        httpindex = dname.find("http");
        //devicename.Find("http")==0
        //devicename.Find("rtsp");
        if ( labelname==moText("RTSP") || rtspindex == 0 ) {

            m_pRTSPSource = gst_element_factory_make ("rtspsrc", "source");
            m_pRTSPDepay = gst_element_factory_make ("rtph264depay", "depay");
            m_pMultipartDemux = gst_element_factory_make ("h264parse", "parse");

            if (m_pRTSPDepay && m_pMultipartDemux) {
                m_pRTSPDepaySink = gst_element_get_static_pad ( (GstElement*)m_pRTSPDepay, "sink"  );
                signal_rtsppad_added_id = g_signal_connect (m_pRTSPSource, "pad-added", G_CALLBACK (cb_pad_added_new), (gpointer)this);
#ifndef GSTVERSION
signal_rtsppad_added_id = g_signal_connect (m_pRTSPSource, "pad-added", G_CALLBACK (on_rtsppadd_added), (gpointer)this);
#endif
            }
        } else if (labelname==moText("HTTP") || httpindex==0 ) {
            m_pHTTPSource = gst_element_factory_make ("souphttpsrc", "source");
            //needed for decodebin2 TODO: check this in gstreamer 1.0
            //m_pMultipartDemux = gst_element_factory_make ("multipartdemux", "demux");
            if ( m_pHTTPSource && m_pMultipartDemux ) {
                //signal_rtsppad_added_id = g_signal_connect ( m_pMultipartDemux, "pad-added", G_CALLBACK (on_rtsppadd_added), (gpointer)this);
            }
        }
        else {

            #ifdef MO_WIN32
              #ifdef GSTVERSION
                m_pFileSource = gst_element_factory_make ("ksvideosrc", "source");
              #else
                m_pFileSource = gst_element_factory_make ("dshowvideosrc", "source");
              #endif
            #else
		#ifdef GSTVERSION
			#ifdef MO_MACOSX
				m_pFileSource = gst_element_factory_make ( CAMERABINOSX, "source");
            cout << CAMERABINOSX << " created!" << endl;
			#else
				if (devicename==moText("DV"))
					m_pFileSource = gst_element_factory_make ("dv1394src", "source");
				else {
	      		        	//m_pFileSource = gst_element_factory_make ("rpicamsrc", "source");
					//g_object_set (G_OBJECT (m_pFileSource), "preview", (bool)false, NULL);
					//g_object_set (G_OBJECT (m_pFileSource), "sensor-mode", (int)6, NULL);
					m_pFileSource = gst_element_factory_make ("v4l2src", "source");
				}
			#endif

		#else
			if (devicename==moText("DV"))
				m_pFileSource = gst_element_factory_make ("dv1394src", "source");
			else
		        	m_pFileSource = gst_element_factory_make ("v4l2src", "source");
            	#endif
            #endif

            m_pFinalSource = m_pFileSource;
        }

        if (m_pRTSPDepay && m_pRTSPSource && m_pMultipartDemux) {
            if (devicename.Length() > 0 && ( devicename!=moText("default")) ) {
                g_object_set (G_OBJECT (m_pRTSPSource), "location", (char*)devicename, NULL);
                g_object_set (G_OBJECT (m_pRTSPSource), "latency", (guint) 0, NULL);
                g_object_set (G_OBJECT (m_pRTSPSource), "debug", (gboolean) false, NULL);

                //g_object_set (G_OBJECT (m_pRTSPSource), "protocols", (guint) 0x00000004, NULL);
                // caps = "application/x-rtp\,\ media\=\(string\)video\,\ payload\=\(int\)96\,
                //\ clock-rate\=\(int\)90000\,\ encoding-name\=\(string\)H264\,\ packetization-mode\=\(string\)1\,\ profile-level-id\=\(string\)4d001e\,\ sprop-parameter-sets\=\(string\)\"Z00AHp2oKAv+WbgICAoAAAMAAgAAAwAfCA\\\=\\\=\\\,aO48gA\\\=\\\=\""
                /*rsrc_caps = gst_caps_new_simple ( "application/x-rtp","media", G_TYPE_STRING, "video",
                                                        "clock-rate", G_TYPE_INT, 90000,
                                                        "encoding-name", G_TYPE_STRING, "H264",
                                                        "payload", G_TYPE_INT, 96, NULL);*/
                //g_object_set (G_OBJECT (m_pRTSPSource), "caps", rsrc_caps, NULL);

            }
            res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pRTSPSource );
            if (res) {
                ///ok
                res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pRTSPDepay );
                if (res) {
                    ///ok
                    res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pMultipartDemux );
                    /*if (caps!=NULL and m_pRTSPSource) {
                        m_pCapsFilterSource = gst_element_factory_make ("capsfilter", "filtsource");
                        if (m_pCapsFilterSource) {
                          res = gst_bin_add ( GST_BIN (m_pGstPipeline), (GstElement*) m_pCapsFilterSource);
                          g_object_set (G_OBJECT (m_pCapsFilterSource), "caps", caps, NULL);

                        }
                     }*/
                     link_result = gst_element_link_many( (GstElement*)m_pRTSPDepay, (GstElement*)m_pMultipartDemux, NULL );
                    //(GstElement*) m_pRTSPSource,
                    //                                      (GstElement*) m_pCapsFilterSource,
                     //                                     (GstElement*) m_pRTSPDepay,
                     //NULL );
                    ///try to link later dynamically
                    //link_result = true;


                }

            }

            if (link_result) {
                //m_pFinalSource = m_pRTSPDepay;
                m_pFinalSource = m_pMultipartDemux;
            } else {
                m_pFinalSource = NULL;
            }

        }

        ///SOUP HTTP source (MJPEG/HTTP)   videofeed
        if ( m_pHTTPSource /*&& m_pMultipartDemux*/ ) {

            g_object_set (G_OBJECT (m_pHTTPSource), "location", (char*)devicename, NULL);
            g_object_set (G_OBJECT (m_pHTTPSource), "automatic-redirect", TRUE, NULL);

            //g_object_set (G_OBJECT (m_pRTSPSource), "latency", (guint) 0, NULL);
            //g_object_set (G_OBJECT (m_pRTSPSource), "debug", (gboolean) true, NULL);
            //g_object_set (G_OBJECT (m_pRTSPSource), "protocols", (guint) 0x00000004, NULL);


            res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pHTTPSource );
            //needed for decoderbin2 (version 2) not for version 1
            //res = res && gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pMultipartDemux );
            link_result  = false;
            if (res) {
                //link_result = gst_element_link_many( (GstElement*) m_pHTTPSource, (GstElement*) m_pMultipartDemux, NULL );
                link_result = true;
            }

            if ( link_result ) {
                //m_pFinalSource = m_pMultipartDemux;
                m_pFinalSource = m_pHTTPSource;
                m_pDecoderBin = gst_element_factory_make ( "decodebin", "decoder");
            } else {
                MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > SOUP HTTP source failed linking with MultipartDemux"));
                m_pFinalSource = NULL;
            }

        }


       ///STANDAR DEVICES....
       if (m_pFileSource) {
           #ifdef MO_WIN32
           devicename.ToLower();
           if (devicename.Length() > 0 && ( devicename!=moText("default")) ) {
                g_object_set (G_OBJECT (m_pFileSource), "device-name", (char*)devicename, NULL);
           }
           #else
            if (devicename==moText("DV") ) {
                g_object_set (G_OBJECT (m_pFileSource), "port", 0, NULL);
            } else {
                devicename.ToLower();
                if ( devicename.Length() > 0 && ( devicename!=moText("default") ) ) {
                    if (dname.find( "/dev/" )==0 ) {
                        g_object_set (G_OBJECT (m_pFileSource), "device", (char*)devicename, NULL);
                    } else if (devicepath.Find("/dev/" )==0) {
                        g_object_set (G_OBJECT (m_pFileSource), "device", (char*)devicepath, NULL);
                    } else {
                        g_object_set (G_OBJECT (m_pFileSource), "device-name", (char*)devicename, NULL);
                    }
                }
            }
           #endif

           res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pFileSource );
           MODebug2->Message( moText("filesrc created! > devicename: ") + (moText)devicename );
           m_pFinalSource = m_pFileSource;
       }

       if (m_pFinalSource) {
           //g_object_get (G_OBJECT (m_pFileSource), "location", &checkval, NULL);
           //GstElement *filter = gst_element_factory_make ("capsfilter", "filter");
           //g_object_set (G_OBJECT (m_pFileSource), "pattern", GST_VIDEO_TEST_SRC_SNOW, NULL);
           //res = gst_pad_set_caps( gst_element_get_pad( m_pFileSource, "src" ), NULL);

           GstIterator* iterator = NULL;
           iterator = gst_element_iterate_src_pads( (GstElement*) m_pFinalSource );

#ifndef GSTVERSION
           gpointer item;
#else
           GValue item = G_VALUE_INIT;
#endif
           GstPad* srcpad = NULL;
           GstCaps* itemcaps = NULL;
           GstCaps* capstpl = NULL;
           GstCaps* capsQuery = NULL;
           GstPad* peerPad = NULL;


           //GstPad* sinkpad = NULL;

           moText padname;
           moText icapsstr;

           bool done = FALSE;
           while (!done) {
            #ifndef GSTVERSION
            switch (gst_iterator_next (iterator, &item)) {
            #else
            switch (gst_iterator_next (iterator, &item)) {
            #endif
               case GST_ITERATOR_OK:
                //... use/change item here...
                #ifndef GSTVERSION
                  srcpad = (GstPad*)item;
                #else
                  srcpad = (GstPad*)g_value_dup_object (&item);
                #endif
                 padname = gst_object_get_name((GstObject*) srcpad );

                 MODebug2->Message( moText("filesrc src pad: checking caps: ") + (moText)padname );

                #ifndef GSTVERSION
                 itemcaps = gst_pad_get_caps( srcpad );
                #else
                 itemcaps = gst_pad_get_current_caps( srcpad );
                 capstpl = gst_pad_get_pad_template_caps( srcpad );
                 capsQuery = gst_pad_query_caps( srcpad, NULL );
                 peerPad = gst_pad_get_peer( srcpad );
                 //if (peerPad==NULL)

                 //gst_pad_peer_query_caps()
                #endif

                 if (capsQuery) {

                     icapsstr = moText( gst_caps_to_string(capsQuery) );
                     MODebug2->Message(icapsstr);
                 }
                 //gst_object_unref (item);
#ifdef GSTVERSION
                g_value_reset (&item);
#endif
                 break;
               case GST_ITERATOR_RESYNC:
                 //...rollback changes to items...
                 gst_iterator_resync (iterator);
                 break;
               case GST_ITERATOR_ERROR:
                 //...wrong parameters were given...
                 done = TRUE;
                 break;
               case GST_ITERATOR_DONE:
                 done = TRUE;
                 break;
             }
           }

           gst_iterator_free (iterator);

            //queue = gst_element_factory_make("queue", "vqueue");
//b_sourceselect = true;
//colormode = "";

           if (b_sourceselect) {
              #ifdef MO_WIN32
              #ifdef GSTVERSION
              b_sourceselect = false;
              #endif // GSTVERSION
              #endif // WIN32
           }

           if (b_sourceselect) {
               MODebug2->Message(moText("moGsGraph:: sourceselect: colormode: ") + (moText)colormode
                                 + moText(" wXh: ") + IntToStr(p_sourcewidth)
                                 + moText("X") + IntToStr(p_sourceheight)
                                 + moText(" bpp:") + IntToStr(p_sourcebpp));
               m_pCapsFilterSource = gst_element_factory_make ("capsfilter", "filtsource");

               if (m_pCapsFilterSource) {
/**
									  m_pColorSpaceSource = gst_element_factory_make ( VIDEOCONVERT, "colorsource");
									  if (m_pCapsFilterSource) {
									  	res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pColorSpaceSource );
									  }*/
#ifndef GSTVERSION
                  MODebug2->Message("colormode: "+ colormode );
                  if (colormode=="") colormode = "video/x-raw-yuv";
		  //if (colormode=="") colormode = "video/x-raw-rgb";
                   g_object_set (G_OBJECT (m_pCapsFilterSource), "caps", gst_caps_new_simple ( colormode,
                   "width", G_TYPE_INT, p_sourcewidth,
                   "height", G_TYPE_INT, p_sourceheight,
                    "depth", G_TYPE_INT, 24,
                   "red_mask",G_TYPE_INT, 16711680,
                   "green_mask",G_TYPE_INT, 65280,
                   "blue_mask",G_TYPE_INT, 255,
                   NULL), NULL);
#else
//
                    moText colormodef = "";

                    int opt_framerate = 15;
                  if (colormode=="") {
                    colormode = "video/x-raw";
                      /*
                     colormodef = "BGR";
                     moText fullf = colormode+ ","+ colormodef;
                     MODebug2->Message("moGsGraph::BuildLiveWebcamGraph > p_sourcewidth:" + fullf );

                      g_object_set (G_OBJECT (m_pCapsFilterSource), "caps", gst_caps_new_simple ( colormode,
                    "format", G_TYPE_STRING, (char*)colormodef,
                     "width", G_TYPE_INT, p_sourcewidth,
                     "height", G_TYPE_INT, p_sourceheight,
                     "framerate", GST_TYPE_FRACTION, opt_framerate, 1,
                     NULL), NULL);
                      */
                      //colormodef = "UYVY";
                      colormodef = "RGB";
                      moText fullf = colormode+ ","+ colormodef;
                      MODebug2->Message( moText("moGsGraph::BuildLiveWebcamGraph > (colormode, format): (") + fullf + moText(")") );
                      //opt_framerate = 30;
                      /*g_object_set (G_OBJECT (m_pCapsFilterSource), "caps", gst_caps_new_simple ( colormode,
                                                                                                 "format", G_TYPE_STRING, (char*)colormodef,
                                                                                                 "width", G_TYPE_INT, p_sourcewidth,
                                                                                                 "height", G_TYPE_INT, p_sourceheight,
												 "framerate", GST_TYPE_FRACTION, opt_framerate, 1,
                                                                                                 NULL), NULL);
                                                                                                 */
                      g_object_set (G_OBJECT (m_pCapsFilterSource), "caps", gst_caps_new_simple ( colormode,
                                                                                                 //"format", G_TYPE_STRING, (char*)colormodef,
                                                                                                 "width", G_TYPE_INT, p_sourcewidth,
                                                                                                 "height", G_TYPE_INT, p_sourceheight,
                                                                                                 NULL), NULL);

                  } else {

                    colormode="video/x-raw-yuv";

                    if (colormode=="video/x-raw-rgb") {
                      colormodef = "RGB";
                    } else if (colormode=="video/x-raw-yuv") {
                      colormodef = "YUV";
                    }

                    colormode="video/x-raw";

                    g_object_set (G_OBJECT (m_pCapsFilterSource), "caps", gst_caps_new_simple ( colormode,
                     //"format", G_TYPE_STRING, "I420",
                     /*"format", G_TYPE_STRING, (char*)colormodef,*/
                     "width", G_TYPE_INT, p_sourcewidth,
                     "height", G_TYPE_INT, p_sourceheight,
                     NULL), NULL);

                  }


#endif
                   //depth=(int)24, red_mask=(int)16711680, green_mask=(int)65280, blue_mask=(int)255, endianness=(int)4321
                   /*
                   "bpp", G_TYPE_INT, p_sourcebpp,
                    "depth", G_TYPE_INT, 24,
                   "red_mask",G_TYPE_INT, 255,
                   "green_mask",G_TYPE_INT, 65280,
                   "blue_mask",G_TYPE_INT, 16711680,
                   "endianness", G_TYPE_INT, 4321
                   */
                   res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pCapsFilterSource );
                   if (res) { MODebug2->Message(moText("moGsGraph:: added capsfilter source!") ) ; }
                   else MODebug2->Error(moText("moGsGraph:: adding capsfilter source..."));
               }
           }


          if (b_forcevideoflip) {
          /**
          https://gstreamer.freedesktop.org/documentation/videofilter/videoflip.html?gi-language=c
          Members
          none (0) – Identity (no rotation)
          clockwise (1) – Rotate clockwise 90 degrees
          rotate-180 (2) – Rotate 180 degrees
          counterclockwise (3) – Rotate counter-clockwise 90 degrees
          horizontal-flip (4) – Flip horizontally
          vertical-flip (5) – Flip vertically
          upper-left-diagonal (6) – Flip across upper left/lower right diagonal
          upper-right-diagonal (7) – Flip across upper right/lower left diagonal
          automatic (8) – Select flip method based on image-orientation tag
          Video-orientation-method
          Members
          identity (0) – GST_VIDEO_ORIENTATION_IDENTITY
          90r (1) – GST_VIDEO_ORIENTATION_90R
          180 (2) – GST_VIDEO_ORIENTATION_180
          90l (3) – GST_VIDEO_ORIENTATION_90L
          horiz (4) – GST_VIDEO_ORIENTATION_HORIZ
          vert (5) – GST_VIDEO_ORIENTATION_VERT
          ul-lr (6) – GST_VIDEO_ORIENTATION_UL_LR
          ur-ll (7) – GST_VIDEO_ORIENTATION_UR_LL
          auto (8) – GST_VIDEO_ORIENTATION_AUTO
          custom (9) – GST_VIDEO_ORIENTATION_CUSTOM
          */
            m_pVideoFlip = gst_element_factory_make ("videoflip", "flip");
            if (m_pVideoFlip) {
               int  method = 0;//identity
               if (p_forceflipH==1 && p_forceflipV==1) {
                method = 2;
               } else if (p_forceflipH==1) {
                method = 4;
               } else if (p_forceflipV==1) {
                method = 5;
               }
#ifndef GSTVERSION
               g_object_set (G_OBJECT (m_pVideoScale), "method", &method, NULL);
#endif
               res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pVideoFlip );
               g_object_set (G_OBJECT (m_pVideoFlip), "method", (int)method, NULL);


           }
          }


            b_forcevideoscale = false;
           if (b_forcevideoscale) {

               m_pVideoScale = gst_element_factory_make ("videoscale", "scale");
               if (m_pVideoScale) {
                   int  method = 0;
                   colormode = "video/x-raw";
                   MODebug2->Message(moText("moGsGraph:: creating videoscale!") ) ;
#ifndef GSTVERSION
                   g_object_set (G_OBJECT (m_pVideoScale), "method", &method, NULL);
#endif
                   res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pVideoScale );

                    m_pCapsFilter2 = gst_element_factory_make ("capsfilter", "filt2");
                    if (m_pCapsFilter2) {
                        if (b_forcevideoscale) {
                            g_object_set (G_OBJECT (m_pCapsFilter2), "caps", gst_caps_new_simple ( colormode,
                                "width", G_TYPE_INT, p_forcewidth,
                                "height", G_TYPE_INT, p_forceheight,
                                NULL), NULL);
                        } else {
                            g_object_set (G_OBJECT (m_pCapsFilter2), "caps", gst_caps_new_simple ( colormode,
                                "width", G_TYPE_INT, 240,
                                "height", G_TYPE_INT, 160,
                                NULL), NULL);
                        }
                        //depth=(int)24, red_mask=(int)16711680, green_mask=(int)65280, blue_mask=(int)255, endianness=(int)4321
                        res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pCapsFilter2 );
                    }


               }
           }

            b_forcevideointerlace = false;
           if (b_forcevideointerlace) {
               m_pColorSpaceInterlace = gst_element_factory_make (VIDEOCONVERT, "colordeinterlace");
               if (m_pColorSpaceInterlace) {
                    MODebug2->Message(moText("moGsGraph:: created videoconvert before deinterlace!") ) ;
                    res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pColorSpaceInterlace );
                } else {
                  MODebug2->Error(moText("moGsGraph:: couldn't create m_pColorSpaceInterlace convert! ")+moText(VIDEOCONVERT) );
                  return false;
                }


               m_pVideoDeinterlace = gst_element_factory_make ("ffdeinterlace", "deinterlace");
               if (m_pVideoDeinterlace) {
                    //int  tff = 2;//bottom field first
                    //g_object_set (G_OBJECT (m_pVideoDeinterlace), "tff", &tff, NULL);
                    MODebug2->Message(moText("moGsGraph:: created ffdeinterlace!") ) ;
                    res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pVideoDeinterlace );
                } else {
                  MODebug2->Error(moText("moGsGraph:: couldn't create m_pVideoDeinterlace convert! ")+moText("ffdeinterlace") );
                  return false;
                }
           }

           m_pColorSpace = gst_element_factory_make (VIDEOCONVERT, "color");
           if (m_pColorSpace) {
                MODebug2->Message(moText("moGsGraph:: created videoconvert for final color!") ) ;
                res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pColorSpace );
           } else {
             MODebug2->Error(moText("moGsGraph:: couldn't create m_pColorSpace convert! ")+moText(VIDEOCONVERT) );
             return false;
           }

           m_pCapsFilter = gst_element_factory_make ("capsfilter", "filt");
           if (m_pCapsFilter) {
                MODebug2->Message(moText("moGsGraph:: created last capsfilter!") ) ;
#ifndef GSTVERSION
               g_object_set (G_OBJECT (m_pCapsFilter), "caps", gst_caps_new_simple ("video/x-raw-rgb",
               "bpp", G_TYPE_INT, 24,
               "depth", G_TYPE_INT, 24,
               "red_mask",G_TYPE_INT, 255,
               "green_mask",G_TYPE_INT, 65280,
               "blue_mask",G_TYPE_INT, 16711680,
               "endianness", G_TYPE_INT, 4321,
               NULL), NULL);
               res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pCapsFilter );
#else
              caps = gst_caps_new_simple ( "video/x-raw",
                   "format", G_TYPE_STRING, "RGB",
                   NULL);
              g_object_set (G_OBJECT (m_pCapsFilter), "caps", caps, NULL);
              res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pCapsFilter );

#endif
               //depth=(int)24, red_mask=(int)16711680, green_mask=(int)65280, blue_mask=(int)255, endianness=(int)4321

           } else {
             MODebug2->Error(moText("moGsGraph:: couldn't create m_pCapsFilter! "));
             return false;
           }

           //RetreivePads( m_pFileSource );
/*
           m_pAudioConverter = gst_element_factory_make ("audioresample", "resample");

           if (m_pAudioConverter) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pAudioConverter );
           }
           */


            if (m_pDecoderBin==NULL) m_pDecoderBin = gst_element_factory_make ( DECODEBIN, "decoder");
            if (m_pDecoderBin) {
                MODebug2->Message(moText("moGsGraph:: created decoder bin! ") + DECODEBIN ) ;
                res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pDecoderBin );
#ifndef GSTVERSION
                signal_newpad_id = g_signal_connect (m_pDecoderBin, "new-decoded-pad", G_CALLBACK (cb_newpad), (gpointer)this);
#else
                signal_newpad_id = g_signal_connect (m_pDecoderBin, "pad-added", G_CALLBACK (cb_pad_added_new), (gpointer)this);
                MODebug2->Message( moText("moGsGraph:: added signal to Decoder Bin, \"pad-added\": ") + IntToStr(signal_newpad_id) ) ;
#endif

#ifndef GSTVERSION
                m_pFakeSink = gst_element_factory_make ("fakesink", "destout");
#else
                cout << "creating FakeSink from appsink" << endl;
                m_pFakeSink = gst_element_factory_make ("appsink", "destout");
#endif
                //RetreivePads( m_pFakeSink );
                if (m_pFakeSink) {
                     MODebug2->Message(moText("moGsGraph:: created FakeSink! ") ) ;
#ifdef GSTVERSION
                     g_object_set (G_OBJECT (m_pFakeSink), "caps", caps, NULL);
                     g_object_set (G_OBJECT (m_pFakeSink), "sync", false, NULL);
                     g_object_set (G_OBJECT (m_pFakeSink), "drop", true, NULL);
#endif
                     res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pFakeSink );


                    MODebug2->Message(moText("moGsGraph:: Try linkage!! sourceselect?: ") + IntToStr(b_sourceselect) ) ;
                    //b_sourceselect = true;
                    if (b_sourceselect) {
                        cout << "linking m_pFinalSource, m_pCapsFilterSource, m_pDecoderBin" << endl;
                        if (b_forcevideoflip) {
                          link_result = gst_element_link_many(    (GstElement*) m_pFinalSource,
                        /**(GstElement*) m_pColorSpaceSource,*/ (GstElement*) m_pCapsFilterSource,
                                                                (GstElement*) m_pVideoFlip,
                                                                (GstElement*) m_pDecoderBin, NULL );
                        } else {
                          link_result = gst_element_link_many(    (GstElement*) m_pFinalSource,
                        /**(GstElement*) m_pColorSpaceSource,*/ (GstElement*) m_pCapsFilterSource,
                                                                (GstElement*) m_pDecoderBin, NULL );
                        }
                     } else {
                         if (b_forcevideoflip) {
                          link_result = gst_element_link_many( (GstElement*) m_pFinalSource,
                                                              (GstElement*)m_pVideoFlip,
                                                              (GstElement*) m_pDecoderBin, NULL );
                         } else {
                          cout << "linking m_pFinalSource, m_pDecoderBin" << endl;
                          link_result = gst_element_link_many( (GstElement*) m_pFinalSource,
                                                               (GstElement*) m_pDecoderBin, NULL );
                         }
                    }


                    if (link_result) {
                        MODebug2->Message(moText("moGsGraph:: Source linkage ok! ")+devinfo ) ;
                        if (b_forcevideoscale) {
                            cout << "linking forcing videoscale" << endl;
                            if (b_forcevideointerlace)
                                link_result = gst_element_link_many( (GstElement*) m_pVideoScale, (GstElement*)m_pCapsFilter2, (GstElement*) m_pColorSpaceInterlace, (GstElement*) m_pVideoDeinterlace, (GstElement*) m_pColorSpace, (GstElement*) m_pCapsFilter, (GstElement*) m_pFakeSink, NULL );
                            else
                                link_result = gst_element_link_many( (GstElement*) m_pVideoScale, (GstElement*)m_pCapsFilter2, (GstElement*) m_pColorSpace, (GstElement*) m_pCapsFilter, (GstElement*) m_pFakeSink, NULL );

                            //old deinterlace
                            //link_result = gst_element_link_many( (GstElement*) m_pVideoDeinterlace, (GstElement*) m_pVideoScale, (GstElement*)m_pCapsFilter2, (GstElement*) m_pColorSpace, (GstElement*) m_pCapsFilter, (GstElement*) m_pFakeSink, NULL );
                        } else {
                            cout << "linking no videoscale" << endl;
                            //link_result = gst_element_link_many( (GstElement*) m_pVideoDeinterlace, (GstElement*) m_pColorSpace, (GstElement*) m_pCapsFilter, (GstElement*) m_pFakeSink, NULL );
                            if (b_forcevideointerlace) {
                                cout << "linking m_pColorSpaceInterlace, m_pVideoDeinterlace, m_pColorSpace, m_pCapsFilter, m_pFakeSink" << endl;
                                link_result = gst_element_link_many( (GstElement*) m_pColorSpaceInterlace,
                                                                      (GstElement*) m_pVideoDeinterlace,
                                                                      (GstElement*)m_pColorSpace,
                                                                      (GstElement*) m_pCapsFilter,
                                                                      (GstElement*) m_pFakeSink,
                                                                      NULL );
                            } else {
                                cout << "linking m_pColorSpace, /*m_pCapsFilter*/, m_pFakeSink" << endl;
                                link_result = gst_element_link_many(
                                        (GstElement*) m_pColorSpace,
#ifndef GSTVERSION
                                        (GstElement*) m_pCapsFilter,
#endif
                                        (GstElement*) m_pFakeSink, NULL );


                            }
                                //link_result = gst_element_link_filtered( (GstElement*) m_pColorSpace, (GstElement*) m_pFakeSink, NULL );
                                //link_result = gst_element_link_many( (GstElement*) m_pColorSpace, (GstElement*) m_pCapsFilter, (GstElement*) m_pFakeSink, NULL );
                        }

                        if (link_result) {
                            MODebug2->Message( moText("moGsGraph::BuildLiveWebcamGraph > play pipeline > ")+devinfo);
                            bool ret = CheckState( gst_element_set_state ((GstElement*) m_pGstPipeline, GST_STATE_PLAYING), true /*SYNCRUNASLI*/ );
                            if (ret==false) ret = CheckState( gst_element_set_state ((GstElement*) m_pGstPipeline, GST_STATE_PLAYING), true /*SYNCRUNASLI*/ );
                            if (ret==false) {
                              MODebug2->Error( moText("moGsGraph::BuildLiveWebcamGraph > No playing. ")+devinfo);
                            } else {
                              MODebug2->Message( moText("moGsGraph::BuildLiveWebcamGraph > GST_STATE_PLAYING > OK.")+devinfo);
                            }

                            //GetState();

#ifdef GSTVERSION

                            GstSample *sample=NULL;
                            //g_signal_emit_by_name ( m_pFakeSink, "pull-sample", &sample, NULL);

                            if (ret) {
                              MODebug2->Message( moText("moGsGraph::BuildLiveWebcamGraph > gst_app_sink_pull_preroll for appsink. ")+devinfo);
                              sample = gst_app_sink_pull_preroll( (GstAppSink*) m_pFakeSink );
                            }

                            if (sample) {
                                MODebug2->Message( moText("moGsGraph::BuildLiveWebcamGraph > RECEIVED sample from gst_app_sink_pull_preroll!")+devinfo);
                                GstBuffer *Gbuffer;
                                GstCaps *bcaps;
                                GstStructure *bstr;

                                /// get the snapshot buffer format now. We set the caps on the appsink so
                                 /// that it can only be an rgb buffer. The only thing we have not specified
                                 /// on the caps is the height, which is dependant on the pixel-aspect-ratio
                                 /// of the source material
                                bcaps = gst_sample_get_caps( sample );
                                if (bcaps) {
                                  Gbuffer = gst_sample_get_buffer (sample);
                                  SetVideoFormat( bcaps, Gbuffer );
                                  gst_app_sink_set_emit_signals((GstAppSink*)m_pFakeSink, true);
                                  gst_app_sink_set_drop((GstAppSink*)m_pFakeSink, true);
                                  gst_app_sink_set_wait_on_eos ((GstAppSink*)m_pFakeSink, false);
                                  //g_object_set (G_OBJECT (m_pFakeSink), "sync", false, NULL);
                                  gst_app_sink_set_max_buffers((GstAppSink*)m_pFakeSink, 1);
                                  g_signal_connect( (GstElement*)m_pFakeSink, "new-sample", G_CALLBACK (appsink_new_sample), (gpointer)this );
                                  //gst_app_sink_set_callbacks( (GstAppSink*)m_pFakeSink,  )

                                }
                            } else MODebug2->Error( moText("moGsGraph::BuildLiveWebcamGraph > NO sample from gst_app_sink_pull_preroll!")+devinfo);

                            MODebug2->Message( moText("moGsGraph::BuildLiveWebcamGraph > gst_app_sink_pull_preroll for appsink ended.")+devinfo);
#else
                            WaitForFormatDefinition( 1600 );
#endif

                            MODebug2->Message( moText("moGsGraph::BuildLiveWebcamGraph > graph builded.")+devinfo);
                            //cout << "state gstreamer finish" << endl;

                            //event_loop( (GstElement*) m_pGstPipeline, false, GST_STATE_PAUSED);

                            return true;

                        } else {
                            MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > m_pColorSpace m_pCapsFilter m_pFakeSink linking failed.")+devinfo);
                            //MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > m_pColorSpace")+IntToStr((int)m_pColorSpace) );
                            //MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > m_pFakeSink")+IntToStr((int)m_pFakeSink) );
                            event_loop( (GstElement*) m_pGstPipeline, false, GST_STATE_PAUSED);
                        }
                    } else {
                        MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > src and decodebin linkage failed:")+devinfo);
                        event_loop( (GstElement*) m_pGstPipeline, false, GST_STATE_PAUSED);
                    }

                } else {
                    MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > fakesink construction failed.")+devinfo);
                    event_loop( (GstElement*) m_pGstPipeline, false, GST_STATE_PAUSED);
                }
            } else {
                MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > decodebin construction failed.")+devinfo);
                event_loop( (GstElement*) m_pGstPipeline, false, GST_STATE_PAUSED);
            }
        } else {
            MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > final source failed.")+devinfo);
            event_loop( (GstElement*) m_pGstPipeline, false, GST_STATE_PAUSED);
        }
        return false;


    }

	return true;
}

bool moGsGraph::BuildLiveQTVideoGraph( moText filename , moBucketsPool *pBucketsPool ) {

	return BuildLiveVideoGraph( filename, pBucketsPool );

}


void
moGsGraph::RetreivePads( moGstElement* FilterElement) {

    GstIterator* piter;
    GstPad*        ppad;
    gchar* nname;
#ifndef GSTVERSION
    gpointer ppointer;
#else
    GValue gvalue = G_VALUE_INIT;
#endif
    bool done;
    bool res = false;

    piter = gst_element_iterate_pads( (GstElement*)FilterElement );

    done = FALSE;
    while (!done) {
#ifndef GSTVERSION
        switch (gst_iterator_next (piter, &ppointer)) {
#else
        switch (gst_iterator_next (piter, &gvalue)) {
#endif
            case GST_ITERATOR_OK:
            //... use/change item here...
#ifndef GSTVERSION
                ppad = (GstPad*) ppointer;
#else
                ppad = (GstPad*) g_value_dup_object( &gvalue );
#endif
                nname = gst_pad_get_name(ppad);
                res = gst_pad_is_active(ppad);
                res = gst_pad_is_linked(ppad);
                res = gst_pad_is_blocking(ppad);
#ifndef GSTVERSION
                gst_object_unref (ppointer);
#else
                g_value_reset( &gvalue );
#endif
            break;

            case GST_ITERATOR_RESYNC:
            //...rollback changes to items...
                gst_iterator_resync (piter);
            break;

            case GST_ITERATOR_ERROR:
            //...wrong parameter were given...
                done = TRUE;
            break;

            case GST_ITERATOR_DONE:
                done = TRUE;
            break;
        }
    }
    gst_iterator_free (piter);done = FALSE;

    return;
}
/*
bool
moGsGraph::BuildTestGraph( moBucketsPool *pBucketsPool ) {
typedef enum {
  GST_VIDEO_TEST_SRC_SMPTE,
  GST_VIDEO_TEST_SRC_SNOW,
  GST_VIDEO_TEST_SRC_BLACK,
  GST_VIDEO_TEST_SRC_WHITE,
  GST_VIDEO_TEST_SRC_RED,
  GST_VIDEO_TEST_SRC_GREEN,
  GST_VIDEO_TEST_SRC_BLUE,
  GST_VIDEO_TEST_SRC_CHECKERS1,
  GST_VIDEO_TEST_SRC_CHECKERS2,
  GST_VIDEO_TEST_SRC_CHECKERS4,
  GST_VIDEO_TEST_SRC_CHECKERS8,
  GST_VIDEO_TEST_SRC_CIRCULAR,
  GST_VIDEO_TEST_SRC_BLINK
} GstVideoTestSrcPattern;

}*/

void
moGsGraph::WaitForFormatDefinition( MOulong timeout ) {

    MOulong time0 = moGetTicksAbsolute();
    MOulong time1 = time0;

    //cout << "waiting for format definition..." << timeout << endl;

    while((time1 - time0) < timeout) {
        if (!m_VideoFormat.m_WaitForFormat) {
          return;
        }
        time1 = moGetTicksAbsolute();
         //cout << (time1 - time0) << endl;
        continue;
    }
    //cout << "elapsed:" << (time1 - time0) << "m_WaitForFormat:" << m_VideoFormat.m_WaitForFormat << "w:" << m_VideoFormat.m_Width << " x h:" << m_VideoFormat.m_Height  << endl;
    MODebug2->Error("moGsGraph::WaitForFormatDefinition > time out !!! " + IntToStr(timeout) + " ms elapsed!");
}


bool moGsGraph::BuildLiveSound( moText filename  ) {

    bool link_result = false;
//    gchar* checkval;
    bool res = false;

    MODebug2->Push( moText("Building live sound:") + (moText)filename);

    moFile SoundFile( filename );

    if ( !SoundFile.Exists() ) return false;

    if (filename.Length()>0)
    {

        moText extension = filename;
        extension.Right(4);

        m_pFileSource = gst_element_factory_make ("filesrc", "source");

        if (m_pFileSource) {

           g_object_set (G_OBJECT (m_pFileSource), "location", (char*)filename/*("///home/fabri/jp5.avi")*/, NULL);

           res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pFileSource );

           m_pAudioConverter = NULL;
/*


           if (m_pAudioConverter) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pAudioConverter );
           }
*/
           if (extension==moText(".wav")) {
              m_pAudioConverter = gst_element_factory_make ("audioresample", "resample");
           //    MODebug2->Push( "moGsGraph:: wav file" );
           } else {
              m_pAudioConverter =  gst_element_factory_make ("audioconvert", "converter");
           }

           if (m_pAudioConverter) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioConverter );
           }

           m_pAudioSink = gst_element_factory_make ("autoaudiosink", "audioout");

           if (m_pAudioSink) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioSink );
           }

            /**
              AUDIOFX
            */

            m_pAudioSpeed = gst_element_factory_make ("speed", "speed");

            if (m_pAudioSpeed) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioSpeed );
           }

           m_pAudioVolume = gst_element_factory_make ("volume", "volume");

           if (m_pAudioVolume) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioVolume );
           }

           m_pAudioPanorama = gst_element_factory_make ("audiopanorama", "audiopanorama");

           if (m_pAudioPanorama) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioPanorama );
           }

           m_pAudioConverter2 = gst_element_factory_make ("audioconvert", "audioconvert2");

           if (m_pAudioConverter2) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioConverter2 );
           }

           m_pAudioConverter3 = gst_element_factory_make ("audioconvert", "audioconvert3");

           if (m_pAudioConverter3) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioConverter3 );
           }

/*
          m_pAudioEcho = gst_element_factory_make ("audioecho", "audioecho");


           if (m_pAudioEcho) {
            res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioEcho );
            unsigned long long max_delay,delay;
            max_delay = 2000000000;
            delay = 0;
            float intensity = 0.0;

            g_object_set ( (GstElement*)m_pAudioEcho, "max-delay", max_delay, NULL);
            g_object_set ( (GstElement*)m_pAudioEcho, "delay", delay, NULL);
            g_object_set ( (GstElement*)m_pAudioEcho, "intensity", intensity, NULL);
           }
*/

           m_pAudioConverter4 = gst_element_factory_make ("audioconvert", "audioconvert4");

           if (m_pAudioConverter4) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioConverter4 );
           }

           m_pDecoderBin = gst_element_factory_make ( DECODEBIN, "decoder");
            if (m_pDecoderBin) {
#ifndef GSTVERSION
                signal_newpad_id = g_signal_connect ((GstElement*)m_pDecoderBin, "new-decoded-pad", G_CALLBACK (cb_newpad), (gpointer)this);
#else
                signal_newpad_id = g_signal_connect ((GstElement*)m_pDecoderBin, "pad-added", G_CALLBACK (cb_pad_added_new), (gpointer)this);
#endif
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pDecoderBin );
            }


           //signal_id = g_signal_connect (m_pWavParser, "new-decoded-pad", G_CALLBACK (cb_newpad), (gpointer)this);
            link_result = gst_element_link_many( (GstElement*)m_pFileSource, (GstElement*)m_pDecoderBin, NULL );

            if (link_result) {
                /*
                if (m_pAudioConverter) link_result = gst_element_link_many(
                                                                           (GstElement*)m_pAudioConverter,
                                                                           (GstElement*)m_pAudioSpeed,
                                                                           (GstElement*)m_pAudioConverter2,
                                                                           (GstElement*)m_pAudioPanorama,
                                                                           (GstElement*)m_pAudioConverter3,
                                                                           (GstElement*)m_pAudioEcho,
                                                                           (GstElement*)m_pAudioConverter4,
                                                                           (GstElement*)m_pAudioVolume,
                                                                           (GstElement*)m_pAudioSink,
                                                                           NULL
                                                                           );
                */
                if (m_pAudioConverter) link_result = gst_element_link_many(
                                                                           (GstElement*)m_pAudioConverter,
                                                                           (GstElement*)m_pAudioSpeed,
                                                                           (GstElement*)m_pAudioConverter2,
                                                                           (GstElement*)m_pAudioPanorama,
                                                                           (GstElement*)m_pAudioConverter3,
                                                                           (GstElement*)m_pAudioVolume,
                                                                           (GstElement*)m_pAudioConverter4,
                                                                           (GstElement*)m_pAudioSink,
                                                                           NULL
                                                                           );
                //else link_result = gst_element_link_many( (GstElement*)m_pAudioSink, NULL );

                if (link_result) {

                    CheckState( gst_element_set_state ((GstElement*)m_pGstPipeline, GST_STATE_PAUSED), true /*SYNCRUNASLI*/ );

                    //WaitForFormatDefinition( 1600 );

                    cout << "state gstreamer finish" << endl;

                    return true;

                } else {
                    MODebug2->Error(moText("moGsGraph::error: m_pAudioConverter m_pAudioResample m_pAudioSink linking failed"));
                    event_loop( (GstElement*)m_pGstPipeline, false, GST_STATE_PAUSED);
                }
            } else {
               MODebug2->Error(moText("moGsGraph::error: m_pFileSource m_pWavParser linking failed"));
               event_loop( (GstElement*)m_pGstPipeline, false, GST_STATE_PAUSED);
            }

        }




    }

    return false;
}


void moGsGraph::BuildAudioFilters() {

    //BuildLock.Lock();
    bool res = false;

    if (m_pGstPipeline) {
           m_pAudioConverter = gst_element_factory_make ("audioconvert", "convert");

           if (m_pAudioConverter) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioConverter );
           }

           m_pAudioVolume = gst_element_factory_make ("volume", "volume");

           if (m_pAudioVolume) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioVolume );
           }

           m_pAudioPanorama = gst_element_factory_make ("audiopanorama", "balance");

           if (m_pAudioPanorama) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioPanorama );
           }

           m_pAudioSink = gst_element_factory_make ("autoaudiosink", "audioout");

           if (m_pAudioSink) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioSink );
           }

           bool link_audio_result = gst_element_link_many( (GstElement*)m_pAudioConverter, (GstElement*)m_pAudioVolume, (GstElement*)m_pAudioPanorama, (GstElement*)m_pAudioSink, NULL );
           MODebug2->Message("BuildAudioFilters: link_audio_result: "+IntToStr(int(link_audio_result)) );
    }
    //BuildLock.Unlock();

}


bool moGsGraph::BuildLiveVideoGraph( moText filename , moBucketsPool *pBucketsPool ) {

    m_pBucketsPool = pBucketsPool;
    bool link_result = false;
//    gchar* checkval;
    bool res = false;

    /**
    * FULL nvidia gpu hardware acceleration for h264/h265/avc/vp8
    * based on vaapi (untested)
    * based on nvdec (tested on GTX 1660 Ti Mobile)
    *
    * https://developer.ridgerun.com/wiki/index.php?title=GStreamer_modify_the_elements_rank
    * GST_RANK_NONE (0) – will be chosen last or not at all
    * GST_RANK_MARGINAL (64) – unlikely to be chosen
    * GST_RANK_SECONDARY (128) – likely to be chosen
    * GST_RANK_PRIMARY (256) – will be chosen first
    *
    *
    */
    GstRegistry* plugins_register = gst_registry_get();

    GstPluginFeature* vaapi_decode = NULL;
    GstPluginFeature* nvdec_decode = NULL;
    GstPluginFeature* nvdec_gldownload = NULL;

    if (1==2) {

      vaapi_decode = gst_registry_lookup_feature(plugins_register, "vaapidecode");
      nvdec_decode = gst_registry_lookup_feature(plugins_register, "nvdec");
      nvdec_gldownload = gst_registry_lookup_feature(plugins_register, "gldownload");

      if(vaapi_decode != NULL) {
        MODebug2->Message("VAAPI (vaapidecode) founded, setting plugin rank to GST_RANK_PRIMARY");
        gst_plugin_feature_set_rank(vaapi_decode, GST_RANK_PRIMARY - 1);
        gst_object_unref(vaapi_decode);
      }

      if(nvdec_decode != NULL) {
        MODebug2->Message("NVDEC (nvdec) founded, setting plugin rank to GST_RANK_PRIMARY");
        gst_plugin_feature_set_rank(nvdec_decode, GST_RANK_PRIMARY - 1);
        gst_object_unref(nvdec_decode);
      }

      if (nvdec_decode || vaapi_decode) {
        GstPluginFeature* avdec_h264 = gst_registry_lookup_feature(plugins_register, "avdec_h264");
        if (avdec_h264) {
          MODebug2->Message("Switch to NVIDIA DECODING, downranking LIBAV (avdec_h264) founded, setting plugin rank to GST_RANK_NONE");
          gst_plugin_feature_set_rank(avdec_h264, GST_RANK_NONE);
          gst_object_unref(avdec_h264);
        }
      }
    }

    moFile VideoFile( filename );
    m_VideoFile = VideoFile;

    MODebug2->Message( moText("moGsGraph::BuildLiveVideoGraph > loading: ")+filename ) ;

    if ( !VideoFile.Exists() ) return false;

    //if (filename.Length()>0)
    {

        m_pFileSource = gst_element_factory_make ("filesrc", "source");

        if (m_pFileSource) {
           g_object_set (G_OBJECT (m_pFileSource), "location", (char*)filename/*("///home/fabri/jp5.avi")*/, NULL);
           //g_object_get (G_OBJECT (m_pFileSource), "location", &checkval, NULL);
           //GstElement *filter = gst_element_factory_make ("capsfilter", "filter");
           //g_object_set (G_OBJECT (m_pFileSource), "pattern", GST_VIDEO_TEST_SRC_SNOW, NULL);
           //res = gst_pad_set_caps( gst_element_get_pad( m_pFileSource, "src" ), NULL);


           res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pFileSource );


           m_pColorSpaceInterlace = gst_element_factory_make (VIDEOCONVERT, "color0");
           if (m_pColorSpaceInterlace) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pColorSpaceInterlace );
           }

           m_pVideoBalance = gst_element_factory_make ("videobalance", "videobalance");
           if (m_pVideoBalance) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pVideoBalance );
           }

           m_pColorSpace = gst_element_factory_make (VIDEOCONVERT, "color");
           if (m_pColorSpace) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pColorSpace );
           }
/*
           m_pCapsFilter = gst_element_factory_make ("capsfilter", "filt");
           if (m_pCapsFilter) {
#ifndef GSTVERSION

               g_object_set (G_OBJECT ((GstElement*)m_pCapsFilter), "caps", gst_caps_new_simple ("video/x-raw-rgb",
               "bpp", G_TYPE_INT, 24,
               "depth", G_TYPE_INT, 24,
               "red_mask",G_TYPE_INT, 255,
               "green_mask",G_TYPE_INT, 65280,
               "blue_mask",G_TYPE_INT, 16711680,
               "endianness", G_TYPE_INT, 4321,
               NULL), NULL);
               //depth=(int)24, red_mask=(int)16711680, green_mask=(int)65280, blue_mask=(int)255, endianness=(int)4321
               res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pCapsFilter );

#else
               g_object_set (G_OBJECT (m_pCapsFilter), "caps", gst_caps_new_simple ( "video/x-raw",
                                                                                    "format", G_TYPE_STRING, "RGB",
                                                                                    NULL), NULL);
               res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pCapsFilter );                                                                           NULL), NULL);
#endif

            }*/
           //RetreivePads( m_pFileSource );

          ///SOUND...
            //BuildAudioFilters();

           ///FIN SOUND

            m_pDecoderBin = gst_element_factory_make ( DECODEBIN, "decoder");
            if (m_pDecoderBin) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pDecoderBin );

                m_pFakeSink = gst_element_factory_make ("fakesink", "destout");
#ifndef GSTVERSION
                //signal_newpad_id = g_signal_connect (m_pDecoderBin, "pad-added", G_CALLBACK (cb_pad_added), (gpointer)this);
                signal_newpad_id = g_signal_connect (m_pDecoderBin, "new-decoded-pad", G_CALLBACK (cb_newpad), (gpointer)this);
#else
                signal_newpad_id = g_signal_connect (m_pDecoderBin, "pad-added", G_CALLBACK (cb_pad_added_new), (gpointer)this);
                MODebug2->Message( moText("moGsGraph:: added signal to Decoder Bin, \"pad-added\": ") + IntToStr(signal_newpad_id) ) ;
#endif

#ifndef GSTVERSION
                m_pFakeSink = gst_element_factory_make ("fakesink", "destout");
#else
                cout << "creating FakeSink from appsink" << endl;
                m_pFakeSink = gst_element_factory_make ("appsink", "destout");
#endif
                //RetreivePads( m_pFakeSink );
                if (m_pFakeSink) {
                    MODebug2->Message(moText("moGsGraph:: created FakeSink! ") ) ;
#ifdef GSTVERSION
                    GstCaps *caps = NULL;
                    moGstElement* m_pGlDownload = NULL;
                    if (nvdec_decode) {
                      //video/x-raw(memory:GLMemory),format=(string)RGBA"
                      caps = gst_caps_new_simple ( "video/x-raw", "format", G_TYPE_STRING, "RGBA", NULL);
                      /*caps = gst_caps_new_simple ( "video/x-raw(memory:GLMemory)",
                           "format", G_TYPE_STRING, "RGBA",
                           NULL);*/
                      /*caps = gst_caps_new_simple ( "video/x-raw(memory:GLMemory)",
                                "format", G_TYPE_STRING, "RGB",
                                NULL);*/
                      m_pGlDownload = gst_element_factory_make ("gldownload", "gldownload");
                      if (m_pGlDownload)
                        res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pGlDownload );
                    } else {
                      caps = gst_caps_new_simple ( "video/x-raw", "format", G_TYPE_STRING, "RGB", NULL);
                    }
                    g_object_set (G_OBJECT (m_pFakeSink), "caps", caps, NULL);
                    g_object_set (G_OBJECT (m_pFakeSink), "sync", (bool)true, NULL);
                    g_object_set (G_OBJECT (m_pFakeSink), "drop", true, NULL);
                    //gst_app_sink_set_emit_signals( (GstAppSink*)m_pFakeSink, true);
                    gst_app_sink_set_max_buffers( (GstAppSink*)m_pFakeSink, 100 );
#else
                     ///marcamos el sync a true para que reproduzca en sync.
                    g_object_set (G_OBJECT (m_pFakeSink), "sync", (bool)true, NULL);

#endif
                     res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pFakeSink );

                    link_result = gst_element_link_many( (GstElement*)m_pFileSource, (GstElement*)m_pDecoderBin, NULL );
                    if (link_result) {
#ifndef GSTVERSION
                        if (m_pVideoBalance)
                            link_result = gst_element_link_many( (GstElement*)m_pColorSpaceInterlace, (GstElement*)m_pVideoBalance, (GstElement*)m_pColorSpace, (GstElement*)m_pCapsFilter, (GstElement*)m_pFakeSink, NULL );
                        else
                            link_result = gst_element_link_many( (GstElement*)m_pColorSpaceInterlace, (GstElement*)m_pColorSpace, (GstElement*)m_pCapsFilter, (GstElement*)m_pFakeSink, NULL );
#else
                        if (m_pGlDownload) {
                          link_result = gst_element_link_many( (GstElement*)m_pColorSpaceInterlace, (GstElement*)m_pColorSpace,(GstElement*)m_pGlDownload, (GstElement*)m_pFakeSink, NULL );
                        } else {
                          if (m_pVideoBalance)
                              link_result = gst_element_link_many( (GstElement*)m_pColorSpaceInterlace, (GstElement*)m_pVideoBalance, (GstElement*)m_pColorSpace, (GstElement*)m_pFakeSink, NULL );
                          else
                              link_result = gst_element_link_many( (GstElement*)m_pColorSpaceInterlace, (GstElement*)m_pColorSpace, (GstElement*)m_pFakeSink, NULL );
                        }
#endif
                        ///agrega sonido en sincro

                        //if (m_pAudioConverter)
                        //  bool link_audio_result = gst_element_link_many( (GstElement*)m_pAudioConverter, (GstElement*)m_pAudioVolume, (GstElement*)m_pAudioPanorama, (GstElement*)m_pAudioSink, NULL );

                        if (link_result) {
                          /*GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pipeline),
                              GST_DEBUG_GRAPH_SHOW_ALL, "gst-launch.warning");*/
                            MODebug2->Message("graph to dot");
                            gst_debug_bin_to_dot_file( GST_BIN(m_pGstPipeline), GST_DEBUG_GRAPH_SHOW_ALL, "moldeo_gsgraph" );
                            CheckState( gst_element_set_state ((GstElement*)m_pGstPipeline, GST_STATE_PAUSED), true /*SYNCRUNASLI*/ );
                            MODebug2->Message( moText("moGsGraph::BuildLiveVideoGraph > GST_STATE_PAUSED > OK"));
                            //CheckState( gst_element_set_state ((GstElement*)m_pGstPipeline, GST_STATE_NULL), true /*SYNCRUNASLI*/ );
                            //MODebug2->Message( moText("moGsGraph::BuildLiveVideoGraph > GST_STATE_NULL > OK"));
                            ///TODO: not working in win
                            ///Pause();
                            ///Seek(0);
#ifdef GSTVERSION
                            GstSample *sample;
                            MODebug2->Message( moText("moGsGraph::BuildLiveVideoGraph > gst_app_sink_pull_preroll for appsink"));
                            //g_signal_emit_by_name ( m_pFakeSink, "pull-sample", &sample, NULL);

                            sample = gst_app_sink_pull_preroll( (GstAppSink*) m_pFakeSink );
                            if (sample) {
                                GstBuffer *Gbuffer;
                                GstCaps *bcaps;
                                GstStructure *bstr;

                                /// get the snapshot buffer format now. We set the caps on the appsink so
                                /// that it can only be an rgb buffer. The only thing we have not specified
                                /// on the caps is the height, which is dependant on the pixel-aspect-ratio
                                /// of the source material
                                bcaps = gst_sample_get_caps( sample );
                                if (bcaps) {
                                    Gbuffer = gst_sample_get_buffer (sample);
                                    SetVideoFormat( bcaps, Gbuffer );
                                    gst_app_sink_set_emit_signals((GstAppSink*)m_pFakeSink, true);
                                    gst_app_sink_set_drop((GstAppSink*)m_pFakeSink, true);
                                    gst_app_sink_set_wait_on_eos ((GstAppSink*)m_pFakeSink, false);
                                    //g_object_set (G_OBJECT (m_pFakeSink), "sync", false, NULL);
                                    gst_app_sink_set_max_buffers((GstAppSink*)m_pFakeSink, 100 );
                                  #ifndef USING_SYNC_FRAMEBUFFER
                                    g_signal_connect( (GstElement*)m_pFakeSink, "new-sample", G_CALLBACK (appsink_new_sample), (gpointer)this );
                                  #endif
                                    //g_signal_connect( (GstElement*)m_pFakeSink, "new-sample", G_CALLBACK (appsink_new_sample), (gpointer)this );
                                    //gst_app_sink_set_callbacks( (GstAppSink*)m_pFakeSink,  )
                                    //g_signal_connect (G_OBJECT (bus), "message::error", (GCallback)error_cb, &data);

                                }
                                MODebug2->Message("graph to dot");
                                gst_debug_bin_to_dot_file( GST_BIN(m_pGstPipeline), GST_DEBUG_GRAPH_SHOW_ALL, "moldeo_gsgraph_builded" );
                            } else {
                                MODebug2->Error( moText("moGsGraph::BuildLiveVideoGraph > no sample!"));
                                cout << "gst_app_sink_is_eos: " << gst_app_sink_is_eos((GstAppSink*)m_pFakeSink) << endl;
                                cout << "gst_app_sink_get_emit_signals: " << gst_app_sink_get_emit_signals((GstAppSink*)m_pFakeSink) << endl;
                                cout << "gst_app_sink_get_max_buffers: " << gst_app_sink_get_max_buffers((GstAppSink*)m_pFakeSink) << endl;
                            }
#else
                            WaitForFormatDefinition( 3000 );
#endif

                            MODebug2->Message( moText("moGsGraph::BuildLiveVideoGraph > graph builded"));

                            //event_loop( (GstElement*)m_pGstPipeline, false, GST_STATE_PAUSED);

                            return true;

                        } else {
                            MODebug2->Error( moText("moGsGraph::BuildLiveVideoGraph > m_pColorSpace m_pCapsFilter m_pFakeSink linking failed"));
                            event_loop( (GstElement*)m_pGstPipeline, false, GST_STATE_PAUSED);
                        }
                    } else {
                        MODebug2->Error( moText("moGsGraph::BuildLiveVideoGraph > filesrc and decodebin linkage failed: ") + filename );
                        event_loop( (GstElement*)m_pGstPipeline, false, GST_STATE_PAUSED);
                    }

                } else {
                    MODebug2->Error( moText("moGsGraph::BuildLiveVideoGraph > fakesink construction failed"));
                    event_loop( (GstElement*)m_pGstPipeline, false, GST_STATE_PAUSED);
                }
            } else {
                MODebug2->Error( moText("moGsGraph::BuildLiveVideoGraph > decodebin construction failed"));
                event_loop( (GstElement*)m_pGstPipeline, false, GST_STATE_PAUSED);
            }
        } else {
            MODebug2->Error( moText("moGsGraph::BuildLiveVideoGraph > file source failed: ") + filename);
            event_loop( (GstElement*)m_pGstPipeline, false, GST_STATE_PAUSED);
        }
        return false;

        /*
        GstPad *pad;
        pad = gst_element_get_pad (m_pDecoderBin, "src0");
        gst_pad_add_buffer_probe (pad, G_CALLBACK (cb_have_data), NULL);
        gst_object_unref (pad);
        */


    }

    /*SETTING SOURCE*/
    /*
    // set the source audio file
    g_object_set (player, "location", "helloworld.ogg", NULL);
    */

    /*more complex*/
    /*
    // create elements
      pipeline = gst_pipeline_new ("my_pipeline");
      source = gst_element_factory_make ("filesrc", "source");
      g_object_set (source, "location", argv[1], NULL);
      demux = gst_element_factory_make ("oggdemux", "demuxer");

      // you would normally check that the elements were created properly

      // put together a pipeline
      gst_bin_add_many (GST_BIN (pipeline), source, demux, NULL);
      gst_element_link_pads (source, "src", demux, "sink");

      // listen for newly created pads
      g_signal_connect (demux, "pad-added", G_CALLBACK (cb_new_pad), NULL);
*/

/*
  // create elements
  source = gst_element_factory_make ("fakesrc", "source");
  filter = gst_element_factory_make ("identity", "filter");
  sink = gst_element_factory_make ("fakesink", "sink");

  // must add elements to pipeline before linking them
  gst_bin_add_many (GST_BIN (pipeline), source, filter, sink, NULL);

  // link
  if (!gst_element_link_many (source, filter, sink, NULL)) {
    g_warning ("Failed to link elements!");
  }

*/


/*putting pipelines into pipelines!!!! */
/*
  //
  pipeline = gst_pipeline_new ("my_pipeline");
  bin = gst_pipeline_new ("my_bin");
  source = gst_element_factory_make ("fakesrc", "source");
  sink = gst_element_factory_make ("fakesink", "sink");

  // set up pipeline
  gst_bin_add_many (GST_BIN (bin), source, sink, NULL);
  gst_bin_add (GST_BIN (pipeline), bin);
  gst_element_link (source, sink);
  */



  /*getting specific properties of caps*/
/*
  static void
read_video_props (GstCaps *caps)
{
  gint width, height;
  const GstStructure *str;

  g_return_if_fail (gst_caps_is_fixed (caps));

  str = gst_caps_get_structure (caps, 0);
  if (!gst_structure_get_int (str, "width", &width) ||
      !gst_structure_get_int (str, "height", &height)) {
    g_print ("No width/height available\n");
    return;
  }

  g_print ("The video size of this set of capabilities is %dx%d\n",
	   width, height);
}
*/

/*CREATE GHOST PAD FOR A BIN ATTACHED TO THE FIRST IN-PAD*/
/*
  GstElement *bin, *sink;
  GstPad *pad;

  // init
  gst_init (&argc, &argv);

  // create element, add to bin
  sink = gst_element_factory_make ("fakesink", "sink");
  bin = gst_bin_new ("mybin");
  gst_bin_add (GST_BIN (bin), sink);

  //add ghostpad
  pad = gst_element_get_pad (sink, "sink");
  gst_element_add_pad (bin, gst_ghost_pad_new ("sink", pad));
  gst_object_unref (GST_OBJECT (pad));

*/
	return false;

}


/*
tipicos de gstreamer:
format(fourcc);
bpp=(int)32,
depth=(int)24,
endianness=(int)4321,
red_mask=(int)-16777216,
green_mask=(int)16711680,
blue_mask=(int)65280,
width=(int)[ 2, 2147483647 ],
height=(int)[ 2, 2147483647 ],
framerate=(fraction)[ 0/1, 2147483647/1 ];
*/

void
moGsGraph::SetAudioFormat( moGstCaps* caps, moGstBuffer* buffer ) {

    bool isfixed = false;
    GstBuffer* Gbuffer =  (GstBuffer*)buffer;

    isfixed = gst_caps_is_fixed((GstCaps*)caps);


    if (!isfixed) {

      return;
    }

    GstStructure* str;
    str = gst_caps_get_structure ((GstCaps*)caps, 0);

    const gchar *sstr;

    sstr = gst_structure_to_string (str);

    //cout << "SetVideoFormat: we have a format!!" << sstr << endl;

    if (g_strrstr( sstr, "channels" )) {

        //to calculate framerate
        gint width, depth;
        //gint value_numerator, value_denominator;
        gint channels, rate;

        gst_structure_get_int( str, "width", &width);
        gst_structure_get_int( str, "depth", &depth);
        gst_structure_get_int( str, "channels", &channels);
        gst_structure_get_int( str, "rate", &rate);
        //gst_structure_get_int( str, "height", &height);
        //gst_structure_get_fraction( str, "framerate", &value_numerator, &value_denominator );

        m_AudioFormat.m_Width = (MOuint)width;
        m_AudioFormat.m_Depth = (MOuint)depth;
        m_AudioFormat.m_Channels = (MOuint)channels;
        m_AudioFormat.m_SampleRate = (MOuint)rate;
        m_AudioFormat.m_SampleSize = (MOuint)depth;
/*
        m_AudioFormat.m_Width = (MOuint)width;
        m_AudioFormat.m_Height = (MOuint)height;
        m_AudioFormat.m_FrameRate = (value_numerator * 100) / value_denominator;
        //cout << "Width:" << m_AudioFormat.m_Width << endl;
        //cout << "Height:" << m_AudioFormat.m_Height << endl;
        //cout << "Framerate:" << m_AudioFormat.m_FrameRate << endl;

        //m_AudioFormat.m_BitCount = pVih->bmiHeader.biBitCount;
        //m_AudioFormat.m_BitRate = pVih->dwBitRate;
        */
        if (Gbuffer!=NULL) {
            m_AudioFormat.m_TimePerSample = Gbuffer->duration;
#ifndef GSTVERSION
            m_AudioFormat.m_BufferSize = Gbuffer->size;
#else
            m_AudioFormat.m_BufferSize = gst_buffer_get_size( Gbuffer );
#endif
        }
        //m_AudioFormat.SetVideoMode();
        m_AudioFormat.m_WaitForFormat = false;

    }

    MODebug2->Message(
                        "SetAudioFormat: we have a format!! "
                      + IntToStr(m_AudioFormat.m_Channels)
                      + " Channels, "
                      + IntToStr(m_AudioFormat.m_SampleRate)
                      + " Hz, "
                      + IntToStr(m_AudioFormat.m_SampleSize)
                      + " bits, "
                      + IntToStr(m_AudioFormat.m_BufferSize)
                      + " bytes per buffer, "
                      + IntToStr(m_AudioFormat.m_TimePerSample)
                      + " nanoseconds per sample "

                      );


}

void
moGsGraph::SetVideoFormat( moGstCaps* caps, moGstBuffer* buffer ) {

    bool isfixed = false;
    GstBuffer* Gbuffer =  (GstBuffer*)buffer;

    isfixed = gst_caps_is_fixed((GstCaps*)caps);


    if (!isfixed) {

      return;
    }

    GstStructure* str;
    str = gst_caps_get_structure ((GstCaps*)caps, 0);

    const gchar *sstr;

    sstr = gst_structure_to_string (str);

    //cout << "SetVideoFormat: we have a format!!" << sstr << endl;

    if (g_strrstr( sstr, "width" )) {

        //to calculate framerate
        gint width, height, value_numerator, value_denominator, redmask, greenmask, bluemask, bitcount;

        gst_structure_get_int( str, "width", &width);
        gst_structure_get_int( str, "height", &height);
        gst_structure_get_fraction( str, "framerate", &value_numerator, &value_denominator );
        gst_structure_get_int( str, "red_mask", &redmask );
        gst_structure_get_int( str, "green_mask", &greenmask );
        gst_structure_get_int( str, "blue_mask", &bluemask );
        gst_structure_get_int( str, "bpp", &bitcount );

        m_VideoFormat.m_Width = (MOuint)width;
        m_VideoFormat.m_Height = (MOuint)height;
        m_VideoFormat.m_FrameRate = (value_numerator * 100) / value_denominator;
        m_VideoFormat.m_RedMask = (MOuint) redmask;
        m_VideoFormat.m_GreenMask = (MOuint) greenmask;
        m_VideoFormat.m_BlueMask = (MOuint) bluemask;
        m_VideoFormat.m_BitCount = (MOuint) bitcount;
        m_VideoFormat.m_ColorMode = RGB;

        const gchar* vformat = gst_structure_get_string( str, "format");
        MODebug2->Message( moText("vformat: [") + moText(vformat) + moText("]") );
        if (moText(vformat)==moText("RGBA") || moText(vformat)==moText("BGRA")) {
          m_VideoFormat.m_ColorMode = RGBA;
        }


        //cout << "Width:" << m_VideoFormat.m_Width << endl;
        //cout << "Height:" << m_VideoFormat.m_Height << endl;
        //cout << "Framerate:" << m_VideoFormat.m_FrameRate << endl;

        //m_VideoFormat.m_BitCount = pVih->bmiHeader.biBitCount;
        //m_VideoFormat.m_BitRate = pVih->dwBitRate;
        if (buffer!=NULL) {
            m_VideoFormat.m_TimePerFrame = Gbuffer->duration;
#ifndef GSTVERSION
            m_VideoFormat.m_BufferSize = Gbuffer->size;
#else
            m_VideoFormat.m_BufferSize = gst_buffer_get_size( Gbuffer );
#endif
        }
        m_VideoFormat.SetVideoMode();
        m_VideoFormat.m_WaitForFormat = false;

        m_Buckets[0].BuildBucket( m_VideoFormat.m_BufferSize, 0 );
        m_Buckets[1].BuildBucket( m_VideoFormat.m_BufferSize, 0 );
        m_Buckets[2].BuildBucket( m_VideoFormat.m_BufferSize, 0 );

    }

    MODebug2->Message(
                        "SetVideoFormat: we have a format!! For: "
                      + m_VideoFile.GetFullName()
                      + " Resolution: "
                      + IntToStr(m_VideoFormat.m_Width)
                      + " X "
                      + IntToStr(m_VideoFormat.m_Height)
                      + " m_BitCount: "
                      + IntToStr(m_VideoFormat.m_BitCount)
                      + " m_BufferSize: "
                      + IntToStr(m_VideoFormat.m_BufferSize)
                      + " m_ColorMode: "
                      + moText(moColorModeIndex[m_VideoFormat.m_ColorMode])
                      + " buffer duration: "
                      + IntToStr(m_VideoFormat.m_TimePerFrame)
                      + " m_FrameRate: "
                      + IntToStr(m_VideoFormat.m_FrameRate)
                      + " m_RedMask: "
                      + IntToStr(m_VideoFormat.m_RedMask)
                      + " Hex: "
                      + IntToHex(m_VideoFormat.m_RedMask)
                      //+ " string: "
                      //+ moText(gst_structure_get_string( str, "red_mask"))
                      + " m_GreenMask: "
                      + IntToHex(m_VideoFormat.m_GreenMask)
                      + " m_BlueMask: "
                      + IntToHex(m_VideoFormat.m_BlueMask)

                      );


}






/*
    *

      GST_STATE_NULL: this is the default state. This state will deallocate all resources held by the element.
    *

      GST_STATE_READY: in the ready state, an element has allocated all of its global resources, that is, resources that can be kept
      within streams. You can think about opening devices, allocating buffers and so on. However,
      the stream is not opened in this state, so the stream positions is automatically zero.
      If a stream was previously opened, it should be closed in this state, and position, properties and such should be reset.
    *

      GST_STATE_PAUSED: in this state, an element has opened the stream, but is not actively processing it. An element is allowed to
      modify a stream's position, read and process data and such to prepare for playback as soon as state is changed to PLAYING,
      but it is not allowed to play the data which would make the clock run. In summary, PAUSED is the same as PLAYING but without
      a running clock.

      Elements going into the PAUSED state should prepare themselves for moving over to the PLAYING state as soon as possible.
      Video or audio outputs would, for example, wait for data to arrive and queue it so they can play it right after the state change.
      Also, video sinks can already play the first frame (since this does not affect the clock yet). Autopluggers could use this same
      state transition to already plug together a pipeline. Most other elements, such as codecs or filters, do not need to explicitely
      do anything in this state, however.
    *

      GST_STATE_PLAYING: in the PLAYING state, an element does exactly the same as in the PAUSED state, except that the clock now runs.

*/



/*
typedef enum {
  GST_STATE_CHANGE_FAILURE             = 0,
  GST_STATE_CHANGE_SUCCESS             = 1,
  GST_STATE_CHANGE_ASYNC               = 2,
  GST_STATE_CHANGE_NO_PREROLL          = 3
} GstStateChangeReturn;
*/

bool
moGsGraph::CheckState( moGstStateChangeReturn state_change_result, bool waitforsync) {


  GstStateChangeReturn Gstate_change_result = (GstStateChangeReturn)state_change_result;

  if (!waitforsync)
  switch(Gstate_change_result) {
    case GST_STATE_CHANGE_FAILURE:
        //MODebug2->Push(moText("GST_STATE_CHANGE_FAILURE"));
        return false;
        break;
    case GST_STATE_CHANGE_SUCCESS:
        //MODebug2->Push(moText("GST_STATE_CHANGE_SUCCESS"));
        return true;
        break;
    case GST_STATE_CHANGE_ASYNC:
        //MODebug2->Push(moText("GST_STATE_CHANGE_ASYNC"));
        return true;
        break;
    case GST_STATE_CHANGE_NO_PREROLL:
        //MODebug2->Push(moText("GST_STATE_CHANGE_NO_PREROLL"));
        return false;
        break;
  }

  GstStateChangeReturn state_wait;
  GstState current_state, pending_state;
  GstClockTime  time_out = GST_CLOCK_TIME_NONE;
  time_out = GST_SECOND;

  while(waitforsync) {
      MODebug2->Message("while wait for sync");
      state_wait = gst_element_get_state(GST_ELEMENT (m_pGstPipeline),&current_state, &pending_state, time_out);
      MODebug2->Message("state_wait result: " + IntToStr( state_wait  ) );
      switch(state_wait) {
          case GST_STATE_CHANGE_SUCCESS:
            waitforsync = false;
            MODebug2->Message("change success");
            return true;
            break;
          case GST_STATE_CHANGE_FAILURE:
            waitforsync = false;
            MODebug2->Message("change failure!");
            return false;
            break;
          case GST_STATE_CHANGE_ASYNC:
            waitforsync = true;
            MODebug2->Message("change async!");
            break;
          default:
            waitforsync = true;
            MODebug2->Message("waitforsync");
            break;
            /*
          case GST_STATE_CHANGE_NO_PREROLL:
            waitforsync = true;
            break;
            */
      }
  }

  return false;

}

moStreamState moGsGraph::GetState() {

    GstStateChangeReturn state_wait;
    GstState current_state, pending_state;
    GstClockTime  time_out = GST_CLOCK_TIME_NONE;
    time_out = GST_SECOND;

    GstPad* srcRGB = NULL;
    bool padactive = false;
    bool padlinked = false;
    bool padblocked = false;
    bool padblocking = false;


    if (m_pColorSpace) {
#ifndef GSTVERSION
      srcRGB = gst_element_get_pad ( (GstElement*)m_pColorSpace, "src");
#else
      srcRGB = gst_element_get_static_pad ( (GstElement*)m_pColorSpace, "src" );
#endif

      padactive = gst_pad_is_active( srcRGB );
      padlinked =  gst_pad_is_linked( srcRGB );
      padblocked =  gst_pad_is_blocked( srcRGB );
      padblocking =  gst_pad_is_blocking( srcRGB );
    }

    if (m_pGMainContext) {
       if (g_main_context_iteration( (GMainContext*)m_pGMainContext, false )) {
          //MODebug2->Message(  moText("moGsGraph ::GetState (events)") );
       } else {
          //MODebug2->Message(  moText("moGsGraph ::GetState (no events!!)"));
       }
    }
/*
      MODebug2->Message(  moText(" Position:")
                          + IntToStr( this->GetPosition())
                         //+ moText(" pad active: ")
                         // + IntToStr((int)padactive)
                         // + moText(" pad linked: ")
                         // + IntToStr((int)padlinked)
                         // + moText(" pad blocked: ")
                         // + IntToStr((int)padblocked)
                         // + moText(" pad blocking: ")
                         // + IntToStr((int)padblocking)
                          );
*/
    //MODebug2->Message(  moText("moGsGraph ::GetState > gst_element_get_state"));
    state_wait = gst_element_get_state(GST_ELEMENT (m_pGstPipeline),&current_state, &pending_state, time_out);
  /*g_main_context_iteration
      GST_STATE_VOID_PENDING        = 0,
  GST_STATE_NULL                = 1,
  GST_STATE_READY               = 2,
  GST_STATE_PAUSED              = 3,
  GST_STATE_PLAYING             = 4
  */

    switch(current_state) {
          case GST_STATE_VOID_PENDING:
            //MODebug2->Message(  moText("moGsGraph ::GetState GST_STATE_VOID_PENDING"));
            return MO_STREAMSTATE_UNKNOWN;
            break;
          case GST_STATE_NULL:
            //MODebug2->Message(  moText("moGsGraph ::GetState GST_STATE_NULL"));
            return MO_STREAMSTATE_STOPPED;
            break;
          case GST_STATE_READY:
            //MODebug2->Message(  moText("moGsGraph ::GetState GST_STATE_READY"));
            return MO_STREAMSTATE_READY;
            break;
          case GST_STATE_PAUSED:
            //MODebug2->Message(  moText("moGsGraph ::GetState GST_STATE_PAUSED"));
            return MO_STREAMSTATE_PAUSED;
            break;
          case GST_STATE_PLAYING:
            //MODebug2->Message(  moText("moGsGraph ::GetState GST_STATE_PLAYING"));
            return MO_STREAMSTATE_PLAYING;
            break;
  }

  //MODebug2->Message(  moText("moGsGraph ::GetState MO_STREAMSTATE_UNKNOWN"));

  return MO_STREAMSTATE_UNKNOWN;

}

//CONTROL METHODS
void
moGsGraph::Play() {
  /* start the pipeline */
  //MODebug2->Message(moText("moGsGraph::Play()"));
  //MODebug2->Message(moText("moGsGraph::Play( SetEOS)"));
  SetEOS(false);
  //MODebug2->Message(moText("moGsGraph::Play() calling CheckState -> GST_STATE_PLAYING"));
  CheckState( gst_element_set_state (GST_ELEMENT (m_pGstPipeline), GST_STATE_PLAYING), true );
  //MODebug2->Message(moText("moGsGraph::Play() returnin CheckState."));
}

void
moGsGraph::Stop() {
  /*set state to NULL*/
  SetEOS(false);
  CheckState( gst_element_set_state (GST_ELEMENT (m_pGstPipeline), GST_STATE_NULL) );
  //moGsGraph::Pause();
}

void
moGsGraph::Pause() {
/*set state to NULL*/
  ///TODO: for live-stream pause works ok... not for others
  if (m_VideoFormat.m_TimePerFrame==0 || GetState()==MO_STREAMSTATE_PLAYING ) {
    MODebug2->Message( "moGsGraph::Pause() pausing" );
    int b = 2;
    while(b!=0) {
        b = gst_element_is_locked_state (GST_ELEMENT (m_pGstPipeline));
        MODebug2->Message( moText("moGsGraph::Pause() check locked ") + IntToStr(b) );
    }
    MODebug2->Message( moText("moGsGraph::Pause() set state"));
    GstStateChangeReturn st = gst_element_set_state (GST_ELEMENT (m_pGstPipeline), GST_STATE_PAUSED);
    MODebug2->Message( moText("moGsGraph::Pause() check state ") + IntToStr((int)st) );
    CheckState( st, true );
    MODebug2->Message( "moGsGraph::Pause() passed" );

  }
}

#define MO_INFINITE -1

///TODO:
/// TimeSeek()
/// SampleSeek()
void
moGsGraph::Seek( MOuint frame, float rate ) {

  gint64     time_nanoseconds;
  bool        res;
  rate = 1.0; /// TODO: unused rate!!!
  //MODebug2->Message(moText("moGsGraph :: Seeking:") + IntToStr(frame) );

  if (m_VideoFormat.m_TimePerFrame!=0 && m_FramesLength>0 && m_FramesLength<(MOulong)MO_INFINITE) {

    ///no leemos el ultimo cuadro para no generar un EOS
    ///esto depende del modo del play, si esta PLAYING o si esta en PAUSE
    if ( (GetState()==MO_STREAMSTATE_PAUSED) && frame >= (m_FramesLength - 1) ) {
        frame = m_FramesLength - 1;
    }

    time_nanoseconds = (gint64) frame * m_VideoFormat.m_TimePerFrame;
    //MODebug2->Message(" Seeking frame: " + IntToStr(frame) + " time (ns): " + IntToStr(time_nanoseconds) + " timeperframe:" + IntToStr(m_VideoFormat.m_TimePerFrame) );
    //cout << "seeking frame:" << frame << " in " << time_nanoseconds << endl;
    /*res = gst_element_seek (m_pGstPipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                         GST_SEEK_TYPE_SET, time_nanoseconds,
                         GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
                         */
    res = gst_element_seek_simple(
                                  (GstElement*)m_pGstPipeline,
                                  GST_FORMAT_TIME,
                                  (GstSeekFlags)(
                                                 GST_SEEK_FLAG_FLUSH
                                                 | GST_SEEK_FLAG_KEY_UNIT
                                                 //| GST_SEEK_FLAG_ACCURATE
                                                 ),
                                  time_nanoseconds );
    //cout << "success:" << res << endl;
    //this->Pause();
  } else {
      ///TIME SEEK: frame as time in milliseconds
      time_nanoseconds = frame * GST_MSECOND;
      res = gst_element_seek_simple( (GstElement*)m_pGstPipeline, GST_FORMAT_TIME, (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT ), time_nanoseconds );
      if (res!=true) {
            MODebug2->Error("moGsGraph :: Seek (time) error");
      }
      ///res = gst_element_seek( (GstElement*)m_pGstPipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_NONE,
      ///      GST_SEEK_TYPE_SET, 10 * GST_SECOND,
      ///      GST_SEEK_TYPE_SET, 20 * GST_SECOND  );
  }

}


MOulong
moGsGraph::GetFramesLength() {

	GstFormat fmt = GST_FORMAT_TIME;

    gint64 len,lenF;
#ifndef GSTVERSION
    if (gst_element_query_duration ((GstElement*)m_pGstPipeline, &fmt, &len)) {
#else
    if (gst_element_query_duration ((GstElement*)m_pGstPipeline, fmt, &len)) {
#endif
    /*g_print ("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r",
         GST_TIME_ARGS (pos), GST_TIME_ARGS (len));*/
        //if (m_VideoFormat.m_TimePerFrame)  m_VideoFormat.m_TimePerFrame = 25;
         if (m_VideoFormat.m_TimePerFrame)
            lenF = ( len / ( m_VideoFormat.m_TimePerFrame ) );
         //cout << "gsgraph: len: ns: " << len  << " frames:" << m_FramesLength << endl;
         MODebug2->Message( "Total length (miliseconds):" + IntToStr(len/GST_MSECOND) + " (frames): " + IntToStr(lenF));
         m_FramesLength = lenF;
         return m_FramesLength;
    }

	return 0;
}

MOulong
moGsGraph::GetSamplesLength() {
	GstFormat fmt = GST_FORMAT_TIME;

    gint64 len,lenF;
#ifndef GSTVERSION
    if (gst_element_query_duration ((GstElement*)m_pGstPipeline, &fmt, &len)) {
#else
    if (gst_element_query_duration ((GstElement*)m_pGstPipeline, fmt, &len)) {
#endif
    /*g_print ("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r",
         GST_TIME_ARGS (pos), GST_TIME_ARGS (len));*/
         if (m_AudioFormat.m_TimePerSample)
            lenF = ( len / ( m_AudioFormat.m_TimePerSample ) );
         //cout << "gsgraph: len: ns: " << len  << " frames:" << m_FramesLength << endl;
         MODebug2->Message( "Total length (miliseconds):" + IntToStr(len/GST_MSECOND) + " (samples): " + IntToStr(lenF));
         m_SamplesLength = lenF;
         return m_SamplesLength;
    }

	return 0;
}

MOulong
moGsGraph::GetDuration() {

	GstFormat fmt = GST_FORMAT_TIME;

    gint64 dur;

#ifndef GSTVERSION
    if (gst_element_query_duration ((GstElement*)m_pGstPipeline, &fmt, &dur)) {
#else
    if (gst_element_query_duration ((GstElement*)m_pGstPipeline, fmt, &dur)) {
#endif
          m_Duration = GST_TIME_AS_MSECONDS(dur); //in milliseconds  1ms = 1 million ns
         //cout << "gsgraph: dur: ns: " <<  dur  << endl;
         return m_Duration;
    }

	return 0;
}

///TODO:GetFramePosition vs GetSamplePosition
/// sample position = duration / samplerate

MOulong
moGsGraph::GetPosition() {

    GstFormat fmt = GST_FORMAT_TIME;
    gint64 pos,frame;

#ifndef GSTVERSION
    if (gst_element_query_position ((GstElement*)m_pGstPipeline, &fmt, &pos)) {
#else
    if (gst_element_query_position ((GstElement*)m_pGstPipeline, fmt, &pos)) {
#endif
        if (m_VideoFormat.m_TimePerFrame==0) {
            return (pos / 1000000);
        }
        frame = pos / (gint64) m_VideoFormat.m_TimePerFrame;
        return (MOulong)frame;
    }
    return 0;
}

MOulong
moGsGraph::GetPositionMS() {

    GstFormat fmt = GST_FORMAT_TIME;
    gint64 pos;

#ifndef GSTVERSION
    if (gst_element_query_position ((GstElement*)m_pGstPipeline, &fmt, &pos)) {
#else
    if (gst_element_query_position ((GstElement*)m_pGstPipeline, fmt, &pos)) {
#endif
        return (MOulong)GST_TIME_AS_MSECONDS(pos);
    }
    return 0;
}


bool
moGsGraph::IsRunning() {
    if (!m_pGstPipeline) return false;
    if (gst_element_get_state ((GstElement*)m_pGstPipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE ) return false;
    return true;
}

void
moGsGraph::SetVolume( float volume ) {
  if (m_pAudioVolume && m_bInitialized ) {
    g_object_set ( (GstElement*)m_pAudioVolume, "volume", volume, NULL);
  }
}

void
moGsGraph::SetBalance( float balance ) {
  if (m_pAudioPanorama && m_bInitialized ) {
    g_object_set ( (GstElement*)m_pAudioPanorama, "panorama", balance, NULL);
  }
}

void
moGsGraph::SetPitch( float pitch ) {
  if (m_pAudioSpeed && m_bInitialized ) {
    //Pause();
    g_object_set ( (GstElement*)m_pAudioSpeed, "speed", pitch, NULL);
    //Play();
  }
}

void
moGsGraph::SetEchoDelay( float delay ) {
  unsigned long long delayl = delay;
  if (m_pAudioEcho && m_bInitialized ) {
    g_object_set ( (GstElement*)m_pAudioEcho, "delay", delayl, NULL);
  }
}

void
moGsGraph::SetEchoIntensity( float intensity ) {
  if (m_pAudioEcho && m_bInitialized ) {
    g_object_set ( (GstElement*)m_pAudioEcho, "intensity", intensity, NULL);
  }
}

void
moGsGraph::SetEchoFeedback( float feedback ) {
  if (m_pAudioEcho && m_bInitialized ) {
    g_object_set ( (GstElement*)m_pAudioEcho, "feedback", feedback, NULL);
  }
}


void  moGsGraph::SetBrightness( float brightness ) {
  if (m_pVideoBalance && m_bInitialized ) {
    g_object_set ( (GstElement*)m_pVideoBalance, "brightness", brightness, NULL);
  }
}



void  moGsGraph::SetContrast( float contrast ) {
  if (m_pVideoBalance && m_bInitialized ) {
    g_object_set ( (GstElement*)m_pVideoBalance, "contrast", contrast, NULL);
  }
}



void  moGsGraph::SetHue( float hue ) {
  if (m_pVideoBalance && m_bInitialized ) {
    g_object_set ( (GstElement*)m_pVideoBalance, "hue", hue, NULL);
  }
}



void  moGsGraph::SetSaturation( float saturation ) {
  if (m_pVideoBalance && m_bInitialized ) {
    g_object_set ( (GstElement*)m_pVideoBalance, "saturation", saturation, NULL);
  }
}



MObyte *
moGsGraph::GetFrameBuffer( MOlong *size )  {
  /// TODO: ??  GetFrameBuffer
  #ifdef USING_SYNC_FRAMEBUFFER
  size = NULL;
  GstAppSink* psink;
  GstSample* sample;
  psink = (GstAppSink*) m_pFakeSink;
  if (psink && !this->m_VideoFormat.m_WaitForFormat) {
      //GstSample* sample = gst_app_sink_try_pull_sample ( psink, 1000000000  );
      //GstSample* sample = gst_app_sink_pull_sample ( psink);
      sample = gst_app_sink_pull_sample ( psink);
      GstMapInfo mapinfo;
      int w = m_VideoFormat.m_Width;
      int h = m_VideoFormat.m_Height;
      moBucket *pbucket=NULL;
      GstCaps* bcaps = gst_sample_get_caps( sample );
      if (!bcaps) return NULL;

      GstBuffer* Gbuffer = gst_sample_get_buffer (sample);
      int bsize = gst_buffer_get_size( Gbuffer );
      if (!( bsize>0 && (int)bsize<=(h*w*4) )) return NULL;

      if (!m_pBucketsPool) return NULL;
      if(m_pBucketsPool->IsFull()) {
          gst_sample_unref(sample);
          return NULL;
      }

      pbucket = new moBucket();
      if (pbucket==NULL) return NULL;

      if (Gbuffer) {
        gst_buffer_map ( Gbuffer, &mapinfo, GST_MAP_READ);
          if (bsize) {
            //pGsGraph->MODebug2->Message(moText("copying: ") + IntToStr(bsize) );
            //pGsGraph->m_Buckets[0].Copy( bsize, (MOubyte*)mapinfo.data );
            pbucket->SetBuffer( bsize,(MOubyte*)mapinfo.data );
            //pbucket->BuildBucket(bsize,128);
          } else {
            //MODebug2->Error(moText("m_Buckets  size: ") + IntToStr(pGsGraph->m_Buckets[0].GetSize()) + moText(" do not match with buffer size: ") + IntToStr(bsize) );
          }
        gst_buffer_unmap ( Gbuffer, &mapinfo );

          bool added_bucket = m_pBucketsPool->AddBucket( pbucket );
          if(!added_bucket)
            MODebug2->Error(moText("appsink_new_sample > Bucket error"));
          gst_sample_unref(sample);

      }
    }
#endif
    return NULL;
}


/*GET SOURCES FROM FILTER*/
/*
GstElement* tee;
GstPad * pad;
  gchar *name;

  pad = gst_element_get_request_pad (tee, "src%d");
  name = gst_pad_get_name (pad);
  g_print ("A new pad %s was created\n", name);
  g_free (name);
*/


/*CONNECTING TOW PINS OR PADS WITH SOME CAPABLITIES SET*/
/*
  gboolean link_ok;
  GstCaps *caps;

  caps = gst_caps_new_full (
      gst_structure_new ("video/x-raw-yuv",
			 "width", G_TYPE_INT, 384,
			 "height", G_TYPE_INT, 288,
			 "framerate", GST_TYPE_FRACTION, 25, 1,
			 NULL),
      gst_structure_new ("video/x-raw-rgb",
			 "width", G_TYPE_INT, 384,
			 "height", G_TYPE_INT, 288,
			 "framerate", GST_TYPE_FRACTION, 25, 1,
			 NULL),
      NULL);

  link_ok = gst_element_link_filtered (element1, element2, caps);
  gst_caps_unref (caps);

  if (!link_ok) {
    g_warning ("Failed to link element1 and element2!");
  }
*/



/*
#include <gst/gst.h>

[.. my_bus_callback goes here ..]

static gboolean
idle_exit_loop (gpointer data)
{
  g_main_loop_quit ((GMainLoop *) data);


  return FALSE;
}

static void
cb_typefound (GstElement *typefind,
	      guint       probability,
	      GstCaps    *caps,
	      gpointer    data)
{
  GMainLoop *loop = data;
  gchar *type;

  type = gst_caps_to_string (caps);
  g_print ("Media type %s found, probability %d%%\n", type, probability);
  g_free (type);

  g_idle_add (idle_exit_loop, loop);
}

gint
main (gint   argc,
      gchar *argv[])
{
  GMainLoop *loop;
  GstElement *pipeline, *filesrc, *typefind;
  GstBus *bus;

  //
  gst_init (&argc, &argv);
  loop = g_main_loop_new (NULL, FALSE);

  //
  if (argc != 2) {
    g_print ("Usage: %s <filename>\n", argv[0]);
    return -1;
  }


  pipeline = gst_pipeline_new ("pipe");

  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  gst_bus_add_watch (bus, my_bus_callback, NULL);
  gst_object_unref (bus);


  filesrc = gst_element_factory_make ("filesrc", "source");
  g_object_set (G_OBJECT (filesrc), "location", argv[1], NULL);
  typefind = gst_element_factory_make ("typefind", "typefinder");
  g_signal_connect (typefind, "have-type", G_CALLBACK (cb_typefound), loop);


  gst_bin_add_many (GST_BIN (pipeline), filesrc, typefind, NULL);
  gst_element_link (filesrc, typefind);
  gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);
  g_main_loop_run (loop);


  gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (pipeline));

  return 0;
}

*/
#endif
