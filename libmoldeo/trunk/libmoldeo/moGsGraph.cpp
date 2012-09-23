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

#include <gst/gst.h>
#include "moGsGraph.h"

#include "moFileManager.h"

#ifdef MO_GSTREAMER

static gboolean bus_call ( GstBus *bus, GstMessage *msg, void* user_data)
{
    //cout << "bus_call: new message" << endl;
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
          //pGsGraph->MODebug2->Error(moText("moGsGraph:: EOS <End-of-stream> "));
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

}


moGBoolean
moGsGraph::cb_have_data (moGstPad    *pad, moGstBuffer *buffer, moGPointer   u_data)
{
  moGsGraph* pGsGraph;

    GstStructure* str;
    GstBuffer* Gbuffer = (GstBuffer*)buffer;
    str = gst_caps_get_structure ( Gbuffer->caps, 0);

    const gchar *sstr;
    const gchar *strname;

    strname = gst_structure_get_name( str );
    sstr = gst_structure_to_string (str);

    //cout << "new data: timestamp: " << buffer->timestamp << " duration:" << buffer->duration << " size:" << buffer->size << " caps:" << sstr << endl;
    //moAbstract::MODebug2->Message( moText(" moGsGraph:: cb_have_data") );

    gchar* isaudio =  NULL;
    gchar* isvideo =  NULL;

    isaudio = g_strrstr (strname, "audio");
    isvideo =  g_strrstr (strname, "video");

    if (u_data!=0) {
      pGsGraph = (moGsGraph*)u_data;

      if (isvideo) {
        if (pGsGraph->m_VideoFormat.m_WaitForFormat)
          pGsGraph->SetVideoFormat( Gbuffer->caps, buffer );
      }

      if (isaudio) {
        if (pGsGraph->m_AudioFormat.m_WaitForFormat)
          pGsGraph->SetAudioFormat( Gbuffer->caps, buffer );
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

    if (isvideo)
    if ( Gbuffer->size>0 && (int)Gbuffer->size<=(h*w*4) ) {
      //g_passing buffer to bucketpool
      moBucket *pbucket=NULL;

      if (pGsGraph->m_pBucketsPool)
      if(!pGsGraph->m_pBucketsPool->IsFull()) {
          pbucket = new moBucket();
          if(pbucket!=NULL) {

              //pGsGraph->m_VideoFormat.m_BufferSize = Gbuffer->size;
              //pGsGraph->m_VideoFormat.m_TimePerFrame = Gbuffer->duration;
              gint value_numerator, value_denominator;
              gst_structure_get_fraction( str, "framerate", &value_numerator, &value_denominator );
              MOuint frate = (value_numerator * 100) / value_denominator;
              //MODebug2->Push( " frate: "+ IntToStr(frate) + " timeperframe: " + IntToStr(Gbuffer->duration));
              pbucket->SetBuffer( Gbuffer->size,(MOubyte*)Gbuffer->data );

              if(!pGsGraph->m_pBucketsPool->AddBucket( pbucket )) {
                  pGsGraph->MODebug2->Error(moText("Bucket error"));
              }
              // else cout << "bucket passed!!" << buffer->size << "timestamp:" << buffer->timestamp << endl;
          }
      }

    } else {
      pGsGraph->MODebug2->Error( moText(" moGsGraph:: cb_have_data error: wrong buffer size:")
              + IntToStr(Gbuffer->size));

    }
  } else {
    pGsGraph->MODebug2->Error( moText(" moGsGraph:: cb_have_data error: no Gbuffer data!!") );
  }

  return TRUE;
}


void
moGsGraph::on_rtsppadd_added( moGstElement *rtspsrc, moGstPad *pad, moGPointer u_data ) {

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

        sstr = gst_structure_to_string (str);
        strname = gst_structure_get_name (str);
        medianame = gst_structure_get_string (str, "media");
        //strname = GST_STRUCTURE(str)->has_field("media");

        if (g_strrstr (medianame, "video")) {
            ///link video...
            if ( pGsGraph->m_pRTSPDepaySink ) {
                padlink = gst_pad_link ( Gpad, (GstPad*)pGsGraph->m_pRTSPDepaySink);
                if (padlink==GST_PAD_LINK_OK) {
                    ///all ok!!!
                }
            }
        }

      }
    }

}


void
moGsGraph::cb_newpad ( moGstElement *decodebin, moGstPad *pad, moGBoolean last, moGPointer u_data)
{
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

        sstr = gst_structure_to_string (str);
        //cout << "cb_newpad: new pad: " << padname << "caps:" << sstr << endl;

        strname = gst_structure_get_name (str);

          if (g_strrstr (strname, "audio")) {
            pGsGraph->m_pAudioPad = Gpad;

            //MODebug2->Push(moText("moGsGraph::cb_newpad: audio pad created"));

            if (pGsGraph->m_pAudioConverter) {
                audiopadinconverter = gst_element_get_pad ( (GstElement*) pGsGraph->m_pAudioConverter, "sink");
                padlink = gst_pad_link (Gpad, audiopadinconverter);

                GstPad* srcAudio = gst_element_get_pad ( (GstElement*)pGsGraph->m_pAudioConverter, "src");

                if (padlink==GST_PAD_LINK_OK) {
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_buffer_probe_full ( srcAudio, G_CALLBACK (cb_have_data), pGsGraph, (GDestroyNotify) (cb_buffer_disconnected) );
                }

            } else if (pGsGraph->m_pAudioSink) {
                audiopadinconverter = gst_element_get_pad ( (GstElement*) pGsGraph->m_pAudioSink, "sink");
                padlink = gst_pad_link (Gpad, audiopadinconverter);
            }


          } else if (g_strrstr (strname, "video")) {
            pGsGraph->m_pVideoPad = Gpad;

            MODebug2->Push(moText("moGsGraph::cb_newpad: video pad created"));
            if (pGsGraph->m_pVideoScale==NULL) {
                //version directa a videoscale
                if (!(GstElement*)pGsGraph->m_pColorSpaceInterlace) {
                    videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pColorSpace, "sink");
                } else {
                    videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pColorSpaceInterlace, "sink");
                }
                //version con deinterlace
                //videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pVideoDeinterlace, "sink");

                GstPad* srcRGB = gst_element_get_pad ( (GstElement*)pGsGraph->m_pColorSpace, "src");
                //bool res = gst_pad_set_caps( gst_element_get_pad ( pGsGraph->m_pColorSpace, "src"), gst_caps_new_simple ("video/x-raw-rgb","bpp", G_TYPE_INT, 24, NULL)  );
                padlink = gst_pad_link( Gpad, videopad );

                if (padlink==GST_PAD_LINK_OK) {
                    caps = gst_pad_get_caps( Gpad );
                    //pGsGraph->SetVideoFormat(caps);
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_buffer_probe_full ( srcRGB, G_CALLBACK (cb_have_data), pGsGraph, (GDestroyNotify) (cb_buffer_disconnected) );
                    //cout << "cb_newpad: linked pads..." << endl;
                }
            } else {
                //version 2 con videoscale

                //version directa a videoscale
                videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pVideoScale, "sink");

                //version con deinterlace
                //videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pVideoDeinterlace, "sink");
                GstPad* srcRGB = gst_element_get_pad ( (GstElement*)pGsGraph->m_pColorSpace, "src");
                //bool res = gst_pad_set_caps( gst_element_get_pad ( pGsGraph->m_pColorSpace, "src"), gst_caps_new_simple ("video/x-raw-rgb","bpp", G_TYPE_INT, 24, NULL)  );

                padlink = gst_pad_link( Gpad, videopad );

                if (padlink==GST_PAD_LINK_OK) {
                    caps = gst_pad_get_caps( Gpad );
                    //pGsGraph->SetVideoFormat(caps);
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_buffer_probe_full ( srcRGB, G_CALLBACK (cb_have_data), pGsGraph, (GDestroyNotify) (cb_buffer_disconnected) );
                    //cout << "cb_newpad: linked pads..." << endl;
                }
            }
          }
      }

  }

}

void
moGsGraph::cb_pad_added ( moGstElement *decodebin, moGstPad *pad, moGPointer u_data)
{
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

        sstr = gst_structure_to_string (str);
        //cout << "cb_newpad: new pad: " << padname << "caps:" << sstr << endl;

        strname = gst_structure_get_name (str);

          if (g_strrstr (strname, "audio")) {
            pGsGraph->m_pAudioPad = Gpad;

            //MODebug2->Push(moText("moGsGraph::cb_pad_added: audio pad created"));

            //pGsGraph->BuildAudioFilters();

            if (pGsGraph->m_pAudioConverter) {
                audiopadinconverter = gst_element_get_pad ( (GstElement*) pGsGraph->m_pAudioConverter, "sink");
                padlink = gst_pad_link (Gpad, audiopadinconverter);

                GstPad* srcAudio = gst_element_get_pad ( (GstElement*)pGsGraph->m_pAudioConverter, "src");

                if (padlink==GST_PAD_LINK_OK) {
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_buffer_probe_full ( srcAudio, G_CALLBACK (cb_have_data), pGsGraph, (GDestroyNotify) (cb_buffer_disconnected) );
                }

            } else if (pGsGraph->m_pAudioSink) {
                audiopadinconverter = gst_element_get_pad ( (GstElement*) pGsGraph->m_pAudioSink, "sink");
                padlink = gst_pad_link (Gpad, audiopadinconverter);
            }


          } else if (g_strrstr (strname, "video")) {
            pGsGraph->m_pVideoPad = Gpad;

            //MODebug2->Push(moText("moGsGraph::cb_pad_added: video pad created"));
            if (pGsGraph->m_pVideoScale==NULL) {
                //version directa a videoscale
                if (!(GstElement*)pGsGraph->m_pColorSpaceInterlace) {
                    videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pColorSpace, "sink");
                } else {
                    videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pColorSpaceInterlace, "sink");
                }
                //version con deinterlace
                //videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pVideoDeinterlace, "sink");

                GstPad* srcRGB = gst_element_get_pad ( (GstElement*)pGsGraph->m_pColorSpace, "src");
                //bool res = gst_pad_set_caps( gst_element_get_pad ( pGsGraph->m_pColorSpace, "src"), gst_caps_new_simple ("video/x-raw-rgb","bpp", G_TYPE_INT, 24, NULL)  );
                padlink = gst_pad_link( Gpad, videopad );

                if (padlink==GST_PAD_LINK_OK) {
                    caps = gst_pad_get_caps( Gpad );
                    //pGsGraph->SetVideoFormat(caps);
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_buffer_probe_full ( srcRGB, G_CALLBACK (cb_have_data), pGsGraph, (GDestroyNotify) (cb_buffer_disconnected) );
                    //cout << "cb_newpad: linked pads..." << endl;
                }
            } else {
                //version 2 con videoscale

                //version directa a videoscale
                videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pVideoScale, "sink");

                //version con deinterlace
                //videopad = gst_element_get_pad ( (GstElement*)pGsGraph->m_pVideoDeinterlace, "sink");
                GstPad* srcRGB = gst_element_get_pad ( (GstElement*)pGsGraph->m_pColorSpace, "src");
                //bool res = gst_pad_set_caps( gst_element_get_pad ( pGsGraph->m_pColorSpace, "src"), gst_caps_new_simple ("video/x-raw-rgb","bpp", G_TYPE_INT, 24, NULL)  );

                padlink = gst_pad_link( Gpad, videopad );

                if (padlink==GST_PAD_LINK_OK) {
                    caps = gst_pad_get_caps( Gpad );
                    //pGsGraph->SetVideoFormat(caps);
                    pGsGraph->cb_have_data_handler_id = gst_pad_add_buffer_probe_full ( srcRGB, G_CALLBACK (cb_have_data), pGsGraph, (GDestroyNotify) (cb_buffer_disconnected) );
                    //cout << "cb_newpad: linked pads..." << endl;
                }
            }
          }
      }

  }

}

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


/* returns TRUE if there was an error or we caught a keyboard interrupt. */
static gboolean
event_loop (GstElement * pipeline, gboolean blocking, GstState target_state)
{
  GstBus *bus;
  GstMessage *message = NULL;
  gboolean res = FALSE;
  gboolean buffering = FALSE;

  bus = gst_element_get_bus (GST_ELEMENT (pipeline));


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


moCaptureDevices* moGsFramework::LoadCaptureDevices() {

    #ifdef MO_WIN32
        //m_CaptureDevices.Add( moCaptureDevice( moText("Laptop Integrated Webcam"), moText("webcam"), moText("-") ) );
        //m_CaptureDevices.Add( moCaptureDevice( moText("Default"), moText("-"), moText("-") ) );

        for( MOuint i=0; i<m_PreferredDevices.Count();i++) {
            AddCaptureDevice( m_PreferredDevices[i] );
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
        // in linux: for v4l2src   device could be  /dev/video0   -   /dev/video1   etc...
        //m_CaptureDevices.Add( moCaptureDevice( moText("Default"), moText("-"), moText("-") ) );
        for(int i=0; i<m_PreferredDevices.Count();i++) {
            AddCaptureDevice( m_PreferredDevices[i] );
        }        //m_CaptureDevices.Add( moCaptureDevice( moText("Laptop Integrated Webcam"), moText("webcam"), moText("/dev/video0") ) );
        //m_CaptureDevices.Add( moCaptureDevice( moText(""), moText("webcam"), moText("/dev/video0") ) );
        //m_CaptureDevices.Add( moCaptureDevice( moText("DV"), moText("DV IEEE 1394"), moText("-"), 0 ) );
    #endif

	return &m_CaptureDevices;

}



moCaptureDevices* moGsFramework::UpdateCaptureDevices() {


	return &m_CaptureDevices;

}


bool
moGsFramework::CheckCaptureDevice( int i ) {


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

    MODebug2->Message( moText("Added capture device:") + p_capdev.GetName() );

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
    m_pDecoderBin = NULL;
    m_pEncoder = NULL;

    m_pTypeFind = NULL;
    m_pCapsFilter = NULL;
    m_pFakeSink = NULL;
    m_pFakeSource = NULL;
    m_pIdentity = NULL;

    m_pBucketsPool = NULL;
    m_pVideoScale = NULL;
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
    guint major, minor, micro, nano;
    //GError *errores;

    MODebug2->Message( moText("Initializing GStreamer"));
    //bool init_result = gst_init_check (NULL, NULL, &errores);
    gst_init(NULL,NULL);
    //gst_init(NULL, NULL);
    //init_result = init_result && gst_controller_init(NULL,NULL);

    gst_version (&major, &minor, &micro, &nano);
    MODebug2->Message( moText("GStreamer version") + IntToStr(major) + moText(".") + IntToStr(minor) + moText(".") + IntToStr(minor));
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


    GMainLoop *loop = g_main_loop_new( NULL, FALSE);
    m_pGMainLoop = (moGMainLoop*) loop;
    if (loop) {
      m_pGMainContext = (moGMainContext*) g_main_loop_get_context( loop );
    }

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
        GstPad* srcRGB = gst_element_get_pad ( (GstElement*)m_pColorSpace, "src");
        if (srcRGB && cb_have_data_handler_id) gst_pad_remove_buffer_probe ( srcRGB, cb_have_data_handler_id );
        cb_have_data_handler_id = 0;
    }

    if (m_pColorSpaceInterlace) {
        GstPad* srcRGB = gst_element_get_pad ( (GstElement*)m_pColorSpaceInterlace, "src");
        if (srcRGB && cb_have_data_handler_id) gst_pad_remove_buffer_probe ( srcRGB, cb_have_data_handler_id );
        cb_have_data_handler_id = 0;
    }


    if (m_pFileSource) {
        gst_object_unref( (GstElement*) m_pFileSource);
        m_pFileSource = NULL;
    }

    if (m_pRTSPDepay) {
        gst_object_unref( (GstElement*) m_pRTSPDepay);
        m_pRTSPDepay = NULL;
    }

    if (m_pRTSPSource) {
        gst_object_unref( (GstElement*) m_pRTSPSource);
        m_pRTSPSource = NULL;
    }

    /** this is only a reference doesnt destroy it, just pass to null**/
    if (m_pFinalSource) {
        m_pFinalSource = NULL;
    }

    if (m_pColorSpace) {
        gst_object_unref( (GstElement*) m_pColorSpace);
        m_pColorSpace = NULL;
    }

    if (m_pColorSpaceInterlace) {
        gst_object_unref( (GstElement*) m_pColorSpaceInterlace);
        m_pColorSpaceInterlace = NULL;
    }

    if (m_pCapsFilter) {
        gst_object_unref( (GstElement*) m_pCapsFilter);
        m_pCapsFilter = NULL;
    }

    if (m_pDecoderBin) {
        if (g_signal_handler_is_connected((GstElement*)m_pDecoderBin, signal_newpad_id))
            g_signal_handler_disconnect ( (GstElement*)m_pDecoderBin, signal_newpad_id );
        signal_newpad_id = 0;
        gst_object_unref( (GstElement*) m_pDecoderBin);
        m_pDecoderBin = NULL;
    }

    if (m_pFakeSink) {
        gst_object_unref( (GstElement*) m_pFakeSink);
        m_pFakeSink = NULL;
    }

    if (m_pAudioConverter) {
        gst_object_unref( (GstElement*) m_pAudioConverter);
        m_pAudioConverter = NULL;
    }

    if (m_pAudioSink) {
        gst_object_unref( (GstElement*) m_pAudioSink);
        m_pAudioSink = NULL;
    }

    if (m_pAudioPad) {
        gst_object_unref( (GstPad*) m_pAudioPad);
        m_pAudioPad = NULL;
    }

    if (m_pVideoPad) {
        gst_object_unref( (GstPad*) m_pVideoPad);
        m_pVideoPad = NULL;
    }

    if (m_pFakeSource) {
        if (g_signal_handler_is_connected((GstElement*)m_pFakeSource, signal_handoff_id))
            g_signal_handler_disconnect ( (GstElement*)m_pFakeSource, signal_handoff_id );
        signal_handoff_id = 0;
        gst_object_unref( (GstElement*) m_pFakeSource);
        m_pFakeSource = NULL;
    }

    if (m_pFileSink) {
        gst_object_unref( (GstElement*) m_pFileSink);
        m_pFileSink = NULL;
    }

    if (m_pGstBus) {
        gst_object_unref( (GstElement*) m_pGstBus);
        m_pGstBus = NULL;
    }

    if (m_pVideoDeinterlace) {
        gst_object_unref( (GstElement*) m_pVideoDeinterlace);
        m_pVideoDeinterlace = NULL;
    }

    if (m_pVideoScale) {
        gst_object_unref( (GstElement*) m_pVideoScale);
        m_pVideoScale = NULL;
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

*/
bool
moGsGraph::BuildLiveStreamingGraph( moBucketsPool *pBucketsPool, moText p_location ) {

    return false;
}

/**
grab at the same time with:
gst-launch-0.10 v4l2src ! queue ! ffmpegcolorspace ! theoraenc quality=1 ! queue ! oggmux name=mux alsasrc ! audio/x-raw-int,rate=8000,channels=1,depth=8 ! queue ! audioconvert ! vorbisenc ! queue ! mux. mux. ! queue ! tee name= t ! queue ! filesink location=test.ogg t. ! queue ! shout2send ip=giss.tv port=8000 password=pass

*/
bool
moGsGraph::BuildRecordGraph( moText filename, moBucketsPool *pBucketsPool ) {

    m_pBucketsPool = pBucketsPool;
    bool link_result = false;

    bool b_sourceselect = false;
    bool b_forcevideoscale = false;
    bool b_forcevideoflip = false;
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
            signal_handoff_id = g_signal_connect (m_pFakeSource, "handoff", G_CALLBACK (cb_handoff), this);

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
           "red_mask",G_TYPE_INT, 255,
           "green_mask",G_TYPE_INT, 65280,
           "blue_mask",G_TYPE_INT, 16711680,
           "endianness", G_TYPE_INT, 4321,
           NULL), NULL);
           //depth=(int)24, red_mask=(int)16711680, green_mask=(int)65280, blue_mask=(int)255, endianness=(int)4321
           res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pCapsFilter );
        }


       m_pColorSpace = gst_element_factory_make ("ffmpegcolorspace", "color");
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
    bool link_result = false;

    bool b_sourceselect = false;
    bool b_forcevideoscale = false;
    bool b_forcevideoflip = false;

    bool b_forcevideointerlace = false;

    //gchar* checkval;
    bool res = false;
    GstPadLinkReturn ret_padlink;

    moGstElement* m_pColorSpaceSource = NULL;

    moGstElement* m_pCapsFilterSource = NULL;
    moGstElement* m_pCapsFilter2 = NULL;

    moText codename;
    moText devicename;
    MOint p_sourcewidth;
    MOint p_sourceheight;
    MOint p_sourcebpp;
    MOint p_forcewidth;
    MOint p_forceheight;
    MOint p_forceflipH;
    MOint p_forceflipV;
    moText colormode;

    codename = p_capdev.GetCodeName();

    devicename = p_capdev.GetName();
    ( p_capdev.GetVideoFormat().m_ColorMode==YUV ) ? colormode = moText("video/x-raw-yuv") : colormode = moText("video/x-raw-rgb");
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

    if (devicename.Length()>0)
    {

        if (codename==moText("RTSP")) {

            m_pRTSPSource = gst_element_factory_make ("rtspsrc", "source");
            m_pRTSPDepay = gst_element_factory_make ("rtpmp4vdepay", "depay");

            if (m_pRTSPDepay) {
                m_pRTSPDepaySink = gst_element_get_static_pad ( (GstElement*)m_pRTSPDepay, "sink"  );
                signal_rtsppad_added_id = g_signal_connect (m_pRTSPSource, "pad-added", G_CALLBACK (on_rtsppadd_added), (gpointer)this);
            }

        } else {

            #ifdef MO_WIN32
            m_pFileSource = gst_element_factory_make ("dshowvideosrc", "source");
            #else
            if (devicename==moText("DV"))
                m_pFileSource = gst_element_factory_make ("dv1394src", "source");
            else
                m_pFileSource = gst_element_factory_make ("v4l2src", "source");
            #endif

            m_pFinalSource = m_pFileSource;
        }

        if (m_pRTSPDepay && m_pRTSPSource) {
            if (devicename.Length() > 0 && ( devicename!=moText("default")) ) {
                g_object_set (G_OBJECT (m_pRTSPSource), "location", (char*)devicename, NULL);
                g_object_set (G_OBJECT (m_pRTSPSource), "latency", (guint) 0, NULL);
                g_object_set (G_OBJECT (m_pRTSPSource), "debug", (gboolean) true, NULL);

                g_object_set (G_OBJECT (m_pRTSPSource), "protocols", (guint) 0x00000004, NULL);

            }
            res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pRTSPSource );
            if (res) {
                ///ok
                res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pRTSPDepay );
                if (res) {
                    ///ok
                    ///link_result = gst_element_link_many( (GstElement*) m_pRTSPSource, (GstElement*) m_pRTSPDepay, NULL );

                    ///try to link later dynamically
                    link_result = true;


                }

            }

            if (link_result) {
                m_pFinalSource = m_pRTSPDepay;
            } else {
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
                    if (devicename.Find( "/dev/" )==0 ) {
                        g_object_set (G_OBJECT (m_pFileSource), "device", (char*)devicename, NULL);
                    } else {
                        g_object_set (G_OBJECT (m_pFileSource), "device-name", (char*)devicename, NULL);
                    }
                }
            }
           #endif

           res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pFileSource );

           m_pFinalSource = m_pFileSource;
       }

        if (m_pFinalSource) {
           //g_object_get (G_OBJECT (m_pFileSource), "location", &checkval, NULL);
           //GstElement *filter = gst_element_factory_make ("capsfilter", "filter");
           //g_object_set (G_OBJECT (m_pFileSource), "pattern", GST_VIDEO_TEST_SRC_SNOW, NULL);
           //res = gst_pad_set_caps( gst_element_get_pad( m_pFileSource, "src" ), NULL);

           GstIterator* iterator = NULL;
           iterator = gst_element_iterate_src_pads( (GstElement*) m_pFinalSource );

           gpointer item;
           GstCaps* itemcaps = NULL;

           GstPad* srcpad = NULL;
           GstPad* sinkpad = NULL;

           moText padname;
           moText icapsstr;

           bool done = FALSE;
           while (!done) {
             switch (gst_iterator_next (iterator, &item)) {
               case GST_ITERATOR_OK:
                 //... use/change item here...

                 srcpad = (GstPad*)item;
                 padname = gst_object_get_name((GstObject*) srcpad );

                 MODebug2->Message( moText("filesrc src pad: checking caps: ") + (moText)padname );

                 itemcaps = gst_pad_get_caps( srcpad );
                 if (itemcaps) {

                     icapsstr = moText( gst_caps_to_string(itemcaps) );
                     MODebug2->Message(icapsstr);
                 }
                 //gst_object_unref (item);

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



           if (b_sourceselect) {
               MODebug2->Message(moText("moGsGraph:: sourceselect:") + (moText)colormode + moText(" ") + IntToStr(p_sourcewidth) + moText("X") + IntToStr(p_sourceheight)+ moText(" bpp:") + IntToStr(p_sourcebpp));
               m_pCapsFilterSource = gst_element_factory_make ("capsfilter", "filtsource");

               if (m_pCapsFilterSource) {

									  m_pColorSpaceSource = gst_element_factory_make ("ffmpegcolorspace", "colorsource");
									  if (m_pCapsFilterSource) {
									  	res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pColorSpaceSource );
									  }

                   g_object_set (G_OBJECT (m_pCapsFilterSource), "caps", gst_caps_new_simple ( colormode,
                   "width", G_TYPE_INT, p_sourcewidth,
                   "height", G_TYPE_INT, p_sourceheight,
                   NULL), NULL);
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

               }
           }

           if (b_forcevideoscale) {

               m_pVideoScale = gst_element_factory_make ("videoscale", "scale");
               if (m_pVideoScale) {
                   int  method = 0;

                   g_object_set (G_OBJECT (m_pVideoScale), "method", &method, NULL);
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
               m_pColorSpaceInterlace = gst_element_factory_make ("ffmpegcolorspace", "colordeinterlace");
               if (m_pColorSpaceInterlace) {
                    res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pColorSpaceInterlace );
               }


               m_pVideoDeinterlace = gst_element_factory_make ("ffdeinterlace", "deinterlace");
               if (m_pVideoDeinterlace) {
                    int  tff = 2;//bottom field first
                    //g_object_set (G_OBJECT (m_pVideoDeinterlace), "tff", &tff, NULL);
                    res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pVideoDeinterlace );
               }
           }

           m_pColorSpace = gst_element_factory_make ("ffmpegcolorspace", "color");
           if (m_pColorSpace) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pColorSpace );
           }

           m_pCapsFilter = gst_element_factory_make ("capsfilter", "filt");
           if (m_pCapsFilter) {
               g_object_set (G_OBJECT (m_pCapsFilter), "caps", gst_caps_new_simple ("video/x-raw-rgb",
               "bpp", G_TYPE_INT, 24,
               "depth", G_TYPE_INT, 24,
               "red_mask",G_TYPE_INT, 255,
               "green_mask",G_TYPE_INT, 65280,
               "blue_mask",G_TYPE_INT, 16711680,
               "endianness", G_TYPE_INT, 4321,
               NULL), NULL);
               //depth=(int)24, red_mask=(int)16711680, green_mask=(int)65280, blue_mask=(int)255, endianness=(int)4321
               res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pCapsFilter );
           }

           //RetreivePads( m_pFileSource );
/*
           m_pAudioConverter = gst_element_factory_make ("audioresample", "resample");

           if (m_pAudioConverter) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pAudioConverter );
           }
           */


            m_pDecoderBin = gst_element_factory_make ("decodebin", "decoder");
            if (m_pDecoderBin) {
                signal_newpad_id = g_signal_connect (m_pDecoderBin, "new-decoded-pad", G_CALLBACK (cb_newpad), (gpointer)this);
                res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pDecoderBin );

                m_pFakeSink = gst_element_factory_make ("fakesink", "destout");
                //RetreivePads( m_pFakeSink );
                if (m_pFakeSink) {
                     res = gst_bin_add (GST_BIN (m_pGstPipeline), (GstElement*) m_pFakeSink );

                    if (b_sourceselect) {
                        link_result = gst_element_link_many( (GstElement*) m_pFinalSource, (GstElement*) m_pColorSpaceSource, (GstElement*) m_pCapsFilterSource, (GstElement*) m_pDecoderBin, NULL );
                     } else {
                        link_result = gst_element_link_many( (GstElement*) m_pFinalSource, (GstElement*) m_pDecoderBin, NULL );
                    }


                    if (link_result) {

                        if (b_forcevideoscale) {
                            if (b_forcevideointerlace)
                                link_result = gst_element_link_many( (GstElement*) m_pVideoScale, (GstElement*)m_pCapsFilter2, (GstElement*) m_pColorSpaceInterlace, (GstElement*) m_pVideoDeinterlace, (GstElement*) m_pColorSpace, (GstElement*) m_pCapsFilter, (GstElement*) m_pFakeSink, NULL );
                            else
                                link_result = gst_element_link_many( (GstElement*) m_pVideoScale, (GstElement*)m_pCapsFilter2, (GstElement*) m_pColorSpace, (GstElement*) m_pCapsFilter, (GstElement*) m_pFakeSink, NULL );

                            //old deinterlace
                            //link_result = gst_element_link_many( (GstElement*) m_pVideoDeinterlace, (GstElement*) m_pVideoScale, (GstElement*)m_pCapsFilter2, (GstElement*) m_pColorSpace, (GstElement*) m_pCapsFilter, (GstElement*) m_pFakeSink, NULL );
                        } else {
                            //link_result = gst_element_link_many( (GstElement*) m_pVideoDeinterlace, (GstElement*) m_pColorSpace, (GstElement*) m_pCapsFilter, (GstElement*) m_pFakeSink, NULL );
                            if (b_forcevideointerlace)
                                link_result = gst_element_link_many( (GstElement*) m_pColorSpaceInterlace, (GstElement*) m_pVideoDeinterlace, (GstElement*)m_pColorSpace, (GstElement*) m_pCapsFilter, (GstElement*) m_pFakeSink, NULL );
                            else
                                link_result = gst_element_link_many( (GstElement*) m_pColorSpace, (GstElement*) m_pCapsFilter, (GstElement*) m_pFakeSink, NULL );
                        }

                        if (link_result) {

                            CheckState( gst_element_set_state ((GstElement*) m_pGstPipeline, GST_STATE_PLAYING), true /*SYNCRUNASLI*/ );

                            WaitForFormatDefinition( 1600 );

                            MODebug2->Message( moText("moGsGraph::BuildLiveWebcamGraph > graph builded"));
                            //cout << "state gstreamer finish" << endl;

                            //event_loop( (GstElement*) m_pGstPipeline, false, GST_STATE_PAUSED);

                            return true;

                        } else {
                            MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > m_pColorSpace m_pCapsFilter m_pFakeSink linking failed"));
                            event_loop( (GstElement*) m_pGstPipeline, false, GST_STATE_PAUSED);
                        }
                    } else {
                        MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > src and decodebin linkage failed: ") + devicename );
                        event_loop( (GstElement*) m_pGstPipeline, false, GST_STATE_PAUSED);
                    }

                } else {
                    MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > fakesink construction failed"));
                    event_loop( (GstElement*) m_pGstPipeline, false, GST_STATE_PAUSED);
                }
            } else {
                MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > decodebin construction failed"));
                event_loop( (GstElement*) m_pGstPipeline, false, GST_STATE_PAUSED);
            }
        } else {
            MODebug2->Error(moText("moGsGraph::BuildLiveWebcamGraph > file source failed"));
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
    gpointer ppointer;
    bool done;
    bool res = false;

    piter = gst_element_iterate_pads( (GstElement*)FilterElement );

    done = FALSE;
    while (!done) {
        switch (gst_iterator_next (piter, &ppointer)) {
            case GST_ITERATOR_OK:
            //... use/change item here...
                ppad = (GstPad*) ppointer;
                nname = gst_pad_get_name(ppad);
                res = gst_pad_is_active(ppad);
                res = gst_pad_is_linked(ppad);
                res = gst_pad_is_blocking(ppad);
                gst_object_unref (ppointer);
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
        if (!m_VideoFormat.m_WaitForFormat)
            break;
        time1 = moGetTicksAbsolute();
         //cout << (time1 - time0) << endl;
        continue;
    }
    //cout << "elapsed:" << (time1 - time0) << "m_WaitForFormat:" << m_VideoFormat.m_WaitForFormat << "w:" << m_VideoFormat.m_Width << " x h:" << m_VideoFormat.m_Height  << endl;

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

           m_pAudioConverter4 = gst_element_factory_make ("audioconvert", "audioconvert4");

           if (m_pAudioConverter4) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pAudioConverter4 );
           }

           m_pDecoderBin = gst_element_factory_make ("decodebin", "decoder");
            if (m_pDecoderBin) {
                signal_newpad_id = g_signal_connect ((GstElement*)m_pDecoderBin, "new-decoded-pad", G_CALLBACK (cb_newpad), (gpointer)this);
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pDecoderBin );
            }


           //signal_id = g_signal_connect (m_pWavParser, "new-decoded-pad", G_CALLBACK (cb_newpad), (gpointer)this);
            link_result = gst_element_link_many( (GstElement*)m_pFileSource, (GstElement*)m_pDecoderBin, NULL );

            if (link_result) {
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
                //else link_result = gst_element_link_many( (GstElement*)m_pAudioSink, NULL );

                if (link_result) {

                    CheckState( gst_element_set_state ((GstElement*)m_pGstPipeline, GST_STATE_PAUSED), true /*SYNCRUNASLI*/ );

                    //WaitForFormatDefinition( 600 );

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
    }


}


bool moGsGraph::BuildLiveVideoGraph( moText filename , moBucketsPool *pBucketsPool ) {

    m_pBucketsPool = pBucketsPool;
    bool link_result = false;
//    gchar* checkval;
    bool res = false;

    moFile VideoFile( filename );

    if ( !VideoFile.Exists() ) return false;

    if (filename.Length()>0)
    {

        m_pFileSource = gst_element_factory_make ("filesrc", "source");

        if (m_pFileSource) {
           g_object_set (G_OBJECT (m_pFileSource), "location", (char*)filename/*("///home/fabri/jp5.avi")*/, NULL);
           //g_object_get (G_OBJECT (m_pFileSource), "location", &checkval, NULL);
           //GstElement *filter = gst_element_factory_make ("capsfilter", "filter");
           //g_object_set (G_OBJECT (m_pFileSource), "pattern", GST_VIDEO_TEST_SRC_SNOW, NULL);
           //res = gst_pad_set_caps( gst_element_get_pad( m_pFileSource, "src" ), NULL);


           res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pFileSource );


           m_pColorSpaceInterlace = gst_element_factory_make ("ffmpegcolorspace", "color0");
           if (m_pColorSpaceInterlace) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pColorSpaceInterlace );
           }

           m_pVideoBalance = gst_element_factory_make ("videobalance", "videobalance");
           if (m_pVideoBalance) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pVideoBalance );
           }

           m_pColorSpace = gst_element_factory_make ("ffmpegcolorspace", "color");
           if (m_pColorSpace) {
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pColorSpace );
           }

           m_pCapsFilter = gst_element_factory_make ("capsfilter", "filt");
           if (m_pCapsFilter) {
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
           }
           //RetreivePads( m_pFileSource );

          ///SOUND...
          BuildAudioFilters();

           ///FIN SOUND

            m_pDecoderBin = gst_element_factory_make ("decodebin2", "decoder");
            if (m_pDecoderBin) {
                signal_newpad_id = g_signal_connect (m_pDecoderBin, "pad-added", G_CALLBACK (cb_pad_added), (gpointer)this);
                res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pDecoderBin );

                m_pFakeSink = gst_element_factory_make ("fakesink", "destout");
                //RetreivePads( m_pFakeSink );
                if (m_pFakeSink) {
                     ///marcamos el sync a true para que reproduzca en sync.
                     g_object_set (G_OBJECT (m_pFakeSink), "sync", (bool)true, NULL);

                     res = gst_bin_add (GST_BIN ((GstElement*)m_pGstPipeline), (GstElement*)m_pFakeSink );

                    link_result = gst_element_link_many( (GstElement*)m_pFileSource, (GstElement*)m_pDecoderBin, NULL );
                    if (link_result) {

                        if (m_pVideoBalance)
                            link_result = gst_element_link_many( (GstElement*)m_pColorSpaceInterlace, (GstElement*)m_pVideoBalance, (GstElement*)m_pColorSpace, (GstElement*)m_pCapsFilter, (GstElement*)m_pFakeSink, NULL );
                        else
                            link_result = gst_element_link_many( (GstElement*)m_pColorSpaceInterlace, (GstElement*)m_pColorSpace, (GstElement*)m_pCapsFilter, (GstElement*)m_pFakeSink, NULL );
                        ///agrega sonido en sincro
                        if (m_pAudioConverter) link_result = link_result && gst_element_link_many( (GstElement*)m_pAudioConverter, (GstElement*)m_pAudioVolume, (GstElement*)m_pAudioPanorama, (GstElement*)m_pAudioSink, NULL );

                        if (link_result) {

                            CheckState( gst_element_set_state ((GstElement*)m_pGstPipeline, GST_STATE_PAUSED), true /*SYNCRUNASLI*/ );

                            WaitForFormatDefinition( 3000 );

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
        gint width, depth, value_numerator, value_denominator;
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
            m_AudioFormat.m_BufferSize = Gbuffer->size;
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
        gint width, height, value_numerator, value_denominator;

        gst_structure_get_int( str, "width", &width);
        gst_structure_get_int( str, "height", &height);
        gst_structure_get_fraction( str, "framerate", &value_numerator, &value_denominator );

        m_VideoFormat.m_Width = (MOuint)width;
        m_VideoFormat.m_Height = (MOuint)height;
        m_VideoFormat.m_FrameRate = (value_numerator * 100) / value_denominator;
        //cout << "Width:" << m_VideoFormat.m_Width << endl;
        //cout << "Height:" << m_VideoFormat.m_Height << endl;
        //cout << "Framerate:" << m_VideoFormat.m_FrameRate << endl;

        //m_VideoFormat.m_BitCount = pVih->bmiHeader.biBitCount;
        //m_VideoFormat.m_BitRate = pVih->dwBitRate;
        if (buffer!=NULL) {
            m_VideoFormat.m_TimePerFrame = Gbuffer->duration;
            m_VideoFormat.m_BufferSize = Gbuffer->size;
        }
        m_VideoFormat.SetVideoMode();
        m_VideoFormat.m_WaitForFormat = false;

    }

    MODebug2->Message(
                        "SetVideoFormat: we have a format!!"
                      + IntToStr(m_VideoFormat.m_Width)
                      + " X "
                      + IntToStr(m_VideoFormat.m_Height)
                      + " buffer duration: "
                      + IntToStr(m_VideoFormat.m_TimePerFrame)
                      + " m_FrameRate: "
                      + IntToStr(m_VideoFormat.m_FrameRate)

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

  while(waitforsync) {
      state_wait = gst_element_get_state(GST_ELEMENT (m_pGstPipeline),&current_state, &pending_state, time_out);
      switch(state_wait) {
          case GST_STATE_CHANGE_SUCCESS:
            waitforsync = false;
            return true;
            break;
          case GST_STATE_CHANGE_FAILURE:
            waitforsync = false;
            return false;
            break;
          case GST_STATE_CHANGE_ASYNC:
            waitforsync = true;
            break;
          case GST_STATE_CHANGE_NO_PREROLL:
            waitforsync = true;
            break;
      }
  }

  return false;

}

moStreamState moGsGraph::GetState() {

    GstStateChangeReturn state_wait;
    GstState current_state, pending_state;
    GstClockTime  time_out = GST_CLOCK_TIME_NONE;

    GstPad* srcRGB = NULL;
    bool padactive = false;
    bool padlinked = false;
    bool padblocked = false;
    bool padblocking = false;


    if (m_pColorSpace) {

      srcRGB = gst_element_get_pad ( (GstElement*)m_pColorSpace, "src");
      padactive = gst_pad_is_active( srcRGB );
      padlinked =  gst_pad_is_linked( srcRGB );
      padblocked =  gst_pad_is_blocked( srcRGB );
      padblocking =  gst_pad_is_blocking( srcRGB );
    }

   if (g_main_context_iteration( (GMainContext*)m_pGMainContext, false )) {
      //MODebug2->Message(  moText("moGsGraph ::GetState (events)") );
   } else {
      //MODebug2->Message(  moText("moGsGraph ::GetState (no events!!)"));
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
            return MO_STREAMSTATE_UNKNOWN;
            break;
          case GST_STATE_NULL:
            return MO_STREAMSTATE_STOPPED;
            break;
          case GST_STATE_READY:
            return MO_STREAMSTATE_READY;
            break;
          case GST_STATE_PAUSED:
            return MO_STREAMSTATE_PAUSED;
            break;
          case GST_STATE_PLAYING:
            return MO_STREAMSTATE_PLAYING;
            break;
  }

  return MO_STREAMSTATE_UNKNOWN;

}

//CONTROL METHODS
void
moGsGraph::Play() {
  /* start the pipeline */
  SetEOS(false);
  CheckState( gst_element_set_state (GST_ELEMENT (m_pGstPipeline), GST_STATE_PLAYING), true );
}

void
moGsGraph::Stop() {
  /*set state to NULL*/
  CheckState( gst_element_set_state (GST_ELEMENT (m_pGstPipeline), GST_STATE_NULL) );
}

void
moGsGraph::Pause() {
/*set state to NULL*/
  ///TODO: for live-stream pause works ok... not for others
  if (m_VideoFormat.m_TimePerFrame==0 || GetState()==MO_STREAMSTATE_PLAYING ) {
    CheckState( gst_element_set_state (GST_ELEMENT (m_pGstPipeline), GST_STATE_PAUSED));
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

    if (gst_element_query_duration ((GstElement*)m_pGstPipeline, &fmt, &len)) {
    /*g_print ("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r",
         GST_TIME_ARGS (pos), GST_TIME_ARGS (len));*/
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

    if (gst_element_query_duration ((GstElement*)m_pGstPipeline, &fmt, &len)) {
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

    if (gst_element_query_duration ((GstElement*)m_pGstPipeline, &fmt, &dur)) {
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

    if (gst_element_query_position ((GstElement*)m_pGstPipeline, &fmt, &pos)) {
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

    if (gst_element_query_position ((GstElement*)m_pGstPipeline, &fmt, &pos)) {
        return (MOulong)GST_TIME_AS_MSECONDS(pos);
    }
    return 0;
}


bool
moGsGraph::IsRunning() {
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
