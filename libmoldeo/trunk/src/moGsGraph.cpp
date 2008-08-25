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
  Andrés Colubri

*******************************************************************************/

#include "moGsGraph.h"


#ifdef MO_GSTREAMER

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

GMainLoop *moGsGraph::loop = g_main_loop_new (NULL, FALSE);

moGsFramework::moGsFramework() {
   // m_pDevEnum = NULL;
   // m_pEnum = NULL;
}


moGsFramework::~moGsFramework() {



}

moCaptureDevices* moGsFramework::LoadCaptureDevices() {

    #ifdef MO_WIN32
        m_CaptureDevices.Add( moCaptureDevice( moText("Microsoft DV Camera and VCR"), moText("DV IEEE 1394"), moText("-") ) );
        m_CaptureDevices.Add( moCaptureDevice( moText("Laptop Integrated Webcam"), moText("webcam"), moText("-") ) );
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
        m_CaptureDevices.Add( moCaptureDevice( moText("DV"), moText("DV IEEE 1394"), moText("") ) );
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

//===========================================
//
//     Class: moGsGraph
//
//===========================================

moGsGraph::moGsGraph() {

    m_pGstBus = NULL;
    m_pGstPipeline = NULL;

    m_pFileSource = NULL;
    m_pDecoderBin = NULL;

    m_pTypeFind = NULL;
    m_pCapsFilter = NULL;
    m_pFakeSink = NULL;
    m_pIdentity = NULL;

    m_pBucketsPool = NULL;

    m_pAudioConverter = NULL;

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

    //opner en el main de la consola...
    //inicialización de la libreria gstreamer
    guint major, minor, micro, nano;
    GError *errores;

    cout << "Initializing GStreamer" << endl;
    //bool init_result = gst_init_check (NULL, NULL, &errores);
    gst_init(NULL,NULL);
    //gst_init(NULL, NULL);
    //init_result = init_result && gst_controller_init(NULL,NULL);

    gst_version (&major, &minor, &micro, &nano);
    cout << "GStreamer version" << major << "." << minor << "." << minor << endl;
    //char vers[10];
    //sprintf( vers, "version: %i.%i.%i.%i",major,minor, micro, nano);

    //if (init_result) MODebug->Push(moText("Initializing GStreamer:OK "));

//analogo a FilterGraph, con dos parametros para dar de alta el elemento: playbin
//playbin
//player
    cout << "creating pipeline" << endl;
    m_pGstPipeline = gst_pipeline_new ("pipeline");

    //buscar un tipo de filtro: factory = gst_element_factory_find ("fakesrc");
    //generarlo: gst_element_factory_make ( factory, "player");
    //o  gst_element_factory_make ("playbin", "player");
    //tomar el valor de una propiedad: g_object_get (G_OBJECT (element), "name", &name, NULL);

    cout << "creating bus interface" << endl;
    m_pGstBus = gst_pipeline_get_bus (GST_PIPELINE (m_pGstPipeline));
    gst_bus_add_watch (m_pGstBus, moGsGraph::bus_call, NULL);
    gst_object_unref (m_pGstBus);
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
    cout << "moGsGraph::Init result:" <<  ((m_pGstPipeline!=NULL) ? "success" : "failure") << endl;
	return (m_pGstPipeline!=NULL);
}


bool
moGsGraph::FinishGraph() {
    //g_main_loop_quit (loop);
    //gst_object_unref (GST_OBJECT (m_pGstPipeline));
	return false;
}


//FILTER METHODS
bool
moGsGraph::SetCaptureDevice( moText deviceport, MOint idevice) {


	return false;
}


bool
moGsGraph::BuildLiveGraph( moBucketsPool *pBucketsPool, moCaptureDevice p_capdev) {

    return BuildLiveWebcamGraph(  pBucketsPool, p_capdev.GetName() );
}


bool
moGsGraph::BuildLiveDVGraph( moBucketsPool *pBucketsPool, MOint idevice) {

	return true;
}



bool
moGsGraph::BuildLiveWebcamGraph( moBucketsPool *pBucketsPool, moText devicename ) {

    m_pBucketsPool = pBucketsPool;
    guint signal_id = 0;
    bool link_result = false;
    gchar* checkval;
    bool res = false;


    if (devicename.Length()>0)
    {

        #ifdef MO_WIN32
        m_pFileSource = gst_element_factory_make ("dshowvideosrc", "source");
        #else
        if (devicename==moText("DV"))
            m_pFileSource = gst_element_factory_make ("dv1394src", "source");
        else
            m_pFileSource = gst_element_factory_make ("v4l2src", "source");
        #endif

        if (m_pFileSource) {
           #ifdef MO_WIN32
            g_object_set (G_OBJECT (m_pFileSource), "device-name", (char*)devicename, NULL);
           #else
            if (devicename==moText("DV") )
                g_object_set (G_OBJECT (m_pFileSource), "port", 0, NULL);
            else
                g_object_set (G_OBJECT (m_pFileSource), "device-name", (char*)devicename, NULL);
           #endif
           //g_object_get (G_OBJECT (m_pFileSource), "location", &checkval, NULL);
           //GstElement *filter = gst_element_factory_make ("capsfilter", "filter");
           //g_object_set (G_OBJECT (m_pFileSource), "pattern", GST_VIDEO_TEST_SRC_SNOW, NULL);
           //res = gst_pad_set_caps( gst_element_get_pad( m_pFileSource, "src" ), NULL);


           res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pFileSource );

           m_pColorSpace = gst_element_factory_make ("ffmpegcolorspace", "color");
           if (m_pColorSpace) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pColorSpace );
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
               res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pCapsFilter );
           }
           //RetreivePads( m_pFileSource );

           m_pAudioConverter = gst_element_factory_make ("audioresample", "resample");

           if (m_pAudioConverter) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pAudioConverter );
           }


            m_pDecoderBin = gst_element_factory_make ("decodebin", "decoder");
            if (m_pDecoderBin) {
                signal_id = g_signal_connect (m_pDecoderBin, "new-decoded-pad", G_CALLBACK (cb_newpad), (gpointer)this);
                res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pDecoderBin );

                m_pFakeSink = gst_element_factory_make ("fakesink", "destout");
                //RetreivePads( m_pFakeSink );
                if (m_pFakeSink) {
                     res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pFakeSink );

                    //link_result = gst_element_link_many ( m_pFileSource, m_pDecoderBin, m_pFakeSink, NULL );
                    link_result = gst_element_link_many( m_pFileSource, m_pDecoderBin, NULL );
                    if (link_result) {

                        link_result = gst_element_link_many( m_pColorSpace, m_pCapsFilter, m_pFakeSink, NULL );

                        if (link_result) {

                            CheckState( gst_element_set_state (m_pGstPipeline, GST_STATE_PAUSED), false /*SYNCRUNASLI*/ );

                            WaitForFormatDefinition( 600 );

                            cout << "state gstreamer finish" << endl;

                            return true;

                        } else {
                            cout << "moGsGraph::error: m_pColorSpace m_pCapsFilter m_pFakeSink linking failed" << endl;
                            event_loop( m_pGstPipeline, false, GST_STATE_PAUSED);
                        }
                    } else {
                        cout << "moGsGraph::error: filesrc and decodebin linkage failed" << endl;
                        event_loop( m_pGstPipeline, false, GST_STATE_PAUSED);
                    }

                } else {
                    printf("moGsGraph::error: fakesink construction failed\n");
                    event_loop( m_pGstPipeline, false, GST_STATE_PAUSED);
                }
            } else {
                cout << "moGsGraph::error: decodebin construction failed" << endl;
                event_loop( m_pGstPipeline, false, GST_STATE_PAUSED);
            }
        } else {
            cout << "moGsGraph::error: file source failed" << endl;
            event_loop( m_pGstPipeline, false, GST_STATE_PAUSED);
        }
        return false;

        /*
        GstPad *pad;
        pad = gst_element_get_pad (m_pDecoderBin, "src0");
        gst_pad_add_buffer_probe (pad, G_CALLBACK (cb_have_data), NULL);
        gst_object_unref (pad);
        */


    }

	return true;
}

bool moGsGraph::BuildLiveQTVideoGraph( moText filename , moBucketsPool *pBucketsPool ) {

	return BuildLiveVideoGraph( filename, pBucketsPool );

}

/*
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
gboolean
moGsGraph::cb_have_data (GstPad    *pad, GstBuffer *buffer, gpointer   u_data)
{
  moGsGraph* pGsGraph;

    GstStructure* str;
    str = gst_caps_get_structure (buffer->caps, 0);

    const gchar *sstr;

    sstr = gst_structure_to_string (str);

    //cout << "new data: timestamp: " << buffer->timestamp << " duration:" << buffer->duration << " size:" << buffer->size << " caps:" << sstr << endl;


    if (u_data!=0) {
      pGsGraph = (moGsGraph*)u_data;
      if (pGsGraph->m_VideoFormat.m_WaitForFormat)
        pGsGraph->SetVideoFormat(buffer->caps, buffer);
    } else return true;//siga intentando

    int w = pGsGraph->GetVideoFormat().m_Width;
    int h = pGsGraph->GetVideoFormat().m_Height;

    //cout << "w:" << w << "h:" << h << endl;


  if ( buffer->size>0 && buffer->size<=(h*w*4) ) {
    //g_passing buffer to bucketpool
    moBucket *pbucket=NULL;

    if(!pGsGraph->m_pBucketsPool->IsFull()) {
        pbucket = new moBucket();
        if(pbucket!=NULL) {

            pGsGraph->m_VideoFormat.m_BufferSize = buffer->size;
            pGsGraph->m_VideoFormat.m_TimePerFrame = buffer->duration;
            pbucket->SetBuffer( buffer->size,(MOubyte*)buffer->data );

            if(!pGsGraph->m_pBucketsPool->AddBucket( pbucket )) {

                //pGsGraph->MODebug->Push(moText("Bucket error"));
            }
            // else cout << "bucket passed!!" << buffer->size << "timestamp:" << buffer->timestamp << endl;
        }
    }

  }


  return TRUE;
}


void
moGsGraph::cb_newpad (GstElement *decodebin, GstPad *pad, gboolean last, gpointer u_data)
{
  GstCaps *caps = NULL;
  GstPad  *videopad = NULL;
  GstPad  *audiopad = NULL;
  GstPad  *audiopadinconverter = NULL;
  GstPadLinkReturn padlink;
  gchar* padname = NULL;
  const gchar* strname = NULL;
  GstStructure *str = NULL;
  moGsGraph* pGsGraph;

  if (gst_pad_is_linked(pad)) {
      return;
  }


  if (u_data!=0) {
      pGsGraph = (moGsGraph*)u_data;
      /* check media type */
      caps = gst_pad_get_caps (pad);
      padname = gst_pad_get_name(pad);
      if (padname) {
        str = gst_caps_get_structure (caps, 0);

        const gchar *sstr;

        sstr = gst_structure_to_string (str);
        //cout << "cb_newpad: new pad: " << padname << "caps:" << sstr << endl;

        strname = gst_structure_get_name (str);

          if (g_strrstr (strname, "audio")) {
            pGsGraph->m_pAudioPad = pad;
            //cout << "cb_newpad: audio pad created" << endl;
            if (pGsGraph->m_pAudioConverter) {
                audiopadinconverter = gst_element_get_pad ( pGsGraph->m_pAudioConverter, "sink");
                gst_pad_link (pad, audiopadinconverter);
            }
          } else if (g_strrstr (strname, "video")) {
            pGsGraph->m_pVideoPad = pad;

            //cout << "cb_newpad: video pad created" << endl;
            videopad = gst_element_get_pad ( pGsGraph->m_pColorSpace, "sink");
            GstPad* srcRGB = gst_element_get_pad ( pGsGraph->m_pColorSpace, "src");
            //bool res = gst_pad_set_caps( gst_element_get_pad ( pGsGraph->m_pColorSpace, "src"), gst_caps_new_simple ("video/x-raw-rgb","bpp", G_TYPE_INT, 24, NULL)  );
            padlink = gst_pad_link( pad, videopad );

            if (padlink==GST_PAD_LINK_OK) {
                caps = gst_pad_get_caps( pad );
                //pGsGraph->SetVideoFormat(caps);
                gst_pad_add_buffer_probe ( srcRGB, G_CALLBACK (cb_have_data), pGsGraph);
                //cout << "cb_newpad: linked pads..." << endl;
            }
          }
      }

  }

}


void
moGsGraph::RetreivePads( GstElement* FilterElement) {

    GstIterator* piter;
    GstPad*        ppad;
    gchar* nname;
    gpointer ppointer;
    bool done;
    bool res = false;

    piter = gst_element_iterate_pads( FilterElement );

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

    MOulong time0 = moGetTicks();
    MOulong time1 = time0;

    cout << "waiting for format definition..." << timeout << endl;

    while((time1 - time0) < timeout) {
        if (!m_VideoFormat.m_WaitForFormat)
            break;
        time1 = moGetTicks();
         //cout << (time1 - time0) << endl;
        continue;
    }
    cout << "elapsed:" << (time1 - time0) << "m_WaitForFormat:" << m_VideoFormat.m_WaitForFormat << "w:" << m_VideoFormat.m_Width << " x h:" << m_VideoFormat.m_Height  << endl;

}

bool moGsGraph::BuildLiveSound( moText filename  ) {

    guint signal_id = 0;
    bool link_result = false;
    gchar* checkval;
    bool res = false;

    MODebug2->Push( moText("Building live sound:") + (moText)filename);

    if (filename.Length()>0)
    {

        m_pFileSource = gst_element_factory_make ("filesrc", "source");

        if (m_pFileSource) {

           g_object_set (G_OBJECT (m_pFileSource), "location", (char*)filename/*("///home/fabri/jp5.avi")*/, NULL);

           res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pFileSource );

           m_pAudioConverter = NULL;
/*
           m_pAudioConverter =  gst_element_factory_make ("audioconverter", "converter");

           if (m_pAudioConverter) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pAudioConverter );
           }
*/
           m_pAudioConverter = gst_element_factory_make ("audioresample", "resample");

           if (m_pAudioConverter) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pAudioConverter );
           }

           GstElement* m_pAudioSink = gst_element_factory_make ("directsoundsink", "audioout");

           if (m_pAudioSink) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pAudioSink );
           }


           m_pDecoderBin = gst_element_factory_make ("decodebin", "decoder");
            if (m_pDecoderBin) {
                signal_id = g_signal_connect (m_pDecoderBin, "new-decoded-pad", G_CALLBACK (cb_newpad), (gpointer)this);
                res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pDecoderBin );
            }


           //signal_id = g_signal_connect (m_pWavParser, "new-decoded-pad", G_CALLBACK (cb_newpad), (gpointer)this);
            link_result = gst_element_link_many( m_pFileSource, m_pDecoderBin, NULL );

            if (link_result) {
                link_result = gst_element_link_many( m_pAudioConverter, m_pAudioSink, NULL );

                if (link_result) {

                    CheckState( gst_element_set_state (m_pGstPipeline, GST_STATE_PAUSED), false /*SYNCRUNASLI*/ );

                    //WaitForFormatDefinition( 600 );

                    cout << "state gstreamer finish" << endl;

                    return true;

                } else {
                    MODebug2->Error(moText("moGsGraph::error: m_pAudioConverter m_pAudioResample m_pAudioSink linking failed"));
                    event_loop( m_pGstPipeline, false, GST_STATE_PAUSED);
                }
            } else {
               MODebug2->Error(moText("moGsGraph::error: m_pFileSource m_pWavParser linking failed"));
               event_loop( m_pGstPipeline, false, GST_STATE_PAUSED);
            }

        }




    }


}

bool moGsGraph::BuildLiveVideoGraph( moText filename , moBucketsPool *pBucketsPool ) {

    m_pBucketsPool = pBucketsPool;
    guint signal_id = 0;
    bool link_result = false;
    gchar* checkval;
    bool res = false;


    if (filename.Length()>0)
    {

        m_pFileSource = gst_element_factory_make ("filesrc", "source");

        if (m_pFileSource) {
           g_object_set (G_OBJECT (m_pFileSource), "location", (char*)filename/*("///home/fabri/jp5.avi")*/, NULL);
           //g_object_get (G_OBJECT (m_pFileSource), "location", &checkval, NULL);
           //GstElement *filter = gst_element_factory_make ("capsfilter", "filter");
           //g_object_set (G_OBJECT (m_pFileSource), "pattern", GST_VIDEO_TEST_SRC_SNOW, NULL);
           //res = gst_pad_set_caps( gst_element_get_pad( m_pFileSource, "src" ), NULL);


           res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pFileSource );

           m_pColorSpace = gst_element_factory_make ("ffmpegcolorspace", "color");
           if (m_pColorSpace) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pColorSpace );
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
               res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pCapsFilter );
           }
           //RetreivePads( m_pFileSource );

           m_pAudioConverter = gst_element_factory_make ("audioresample", "resample");

           if (m_pAudioConverter) {
                res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pAudioConverter );
           }


            m_pDecoderBin = gst_element_factory_make ("decodebin", "decoder");
            if (m_pDecoderBin) {
                signal_id = g_signal_connect (m_pDecoderBin, "new-decoded-pad", G_CALLBACK (cb_newpad), (gpointer)this);
                res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pDecoderBin );

                m_pFakeSink = gst_element_factory_make ("fakesink", "destout");
                //RetreivePads( m_pFakeSink );
                if (m_pFakeSink) {
                     res = gst_bin_add (GST_BIN (m_pGstPipeline), m_pFakeSink );

                    //link_result = gst_element_link_many ( m_pFileSource, m_pDecoderBin, m_pFakeSink, NULL );
                    link_result = gst_element_link_many( m_pFileSource, m_pDecoderBin, NULL );
                    if (link_result) {

                        link_result = gst_element_link_many( m_pColorSpace, m_pCapsFilter, m_pFakeSink, NULL );

                        if (link_result) {

                            CheckState( gst_element_set_state (m_pGstPipeline, GST_STATE_PAUSED), false /*SYNCRUNASLI*/ );

                            WaitForFormatDefinition( 600 );

                            cout << "state gstreamer finish" << endl;

                            return true;

                        } else {
                            cout << "moGsGraph::error: m_pColorSpace m_pCapsFilter m_pFakeSink linking failed" << endl;
                            event_loop( m_pGstPipeline, false, GST_STATE_PAUSED);
                        }
                    } else {
                        cout << "moGsGraph::error: filesrc and decodebin linkage failed" << endl;
                        event_loop( m_pGstPipeline, false, GST_STATE_PAUSED);
                    }

                } else {
                    printf("moGsGraph::error: fakesink construction failed\n");
                    event_loop( m_pGstPipeline, false, GST_STATE_PAUSED);
                }
            } else {
                cout << "moGsGraph::error: decodebin construction failed" << endl;
                event_loop( m_pGstPipeline, false, GST_STATE_PAUSED);
            }
        } else {
            cout << "moGsGraph::error: file source failed" << endl;
            event_loop( m_pGstPipeline, false, GST_STATE_PAUSED);
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
moGsGraph::SetVideoFormat( GstCaps* caps, GstBuffer* buffer ) {

    bool isfixed = false;

    isfixed = gst_caps_is_fixed(caps);


    if (!isfixed) {

      return;
    }

    GstStructure* str;
    str = gst_caps_get_structure (caps, 0);

    const gchar *sstr;

    sstr = gst_structure_to_string (str);

    cout << "SetVideoFormat: we have a format!!" << sstr << endl;

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
            m_VideoFormat.m_TimePerFrame = buffer->duration;
            m_VideoFormat.m_BufferSize = buffer->size;
        }
        m_VideoFormat.SetVideoMode();
        m_VideoFormat.m_WaitForFormat = false;

    }

}


gboolean
moGsGraph::bus_call (GstBus *bus, GstMessage *msg, gpointer user_data)
{
    cout << "bus_call: new message" << endl;

    if (true) {
      const GstStructure *s;

      s = gst_message_get_structure (msg);

      /*g_print (("Got Message from element \"%s\" (%s): "),
          GST_STR_NULL (GST_ELEMENT_NAME (GST_MESSAGE_SRC (msg))),
          gst_message_type_get_name (GST_MESSAGE_TYPE (msg)));*/
      if (s) {
        gchar *sstr;

        sstr = gst_structure_to_string (s);
        //moGsGraph::MODebug->Push(moText(sstr));
        //g_print ("%s\n", sstr);
        g_free (sstr);
      } else {
        //g_print ("no message details\n");
      }
    }

  switch (GST_MESSAGE_TYPE (msg))
  {
      case GST_MESSAGE_EOS:
      {
          g_message ("End-of-stream");
          //moAbstract::MODebug->Push(moText("End-of-stream"));
          //g_main_loop_quit (loop);
          break;
      }

      case GST_MESSAGE_ERROR:
      {
          gchar *debug;

          GError *err;

          gst_message_parse_error (msg, &err, &debug);
          //moAbstract::MODebug->Push(moText(debug));
          g_free (debug);
          g_error ("%s", err->message);
          g_error_free (err);
          //g_main_loop_quit (loop);

          break;
      }

      default:

      break;

}
  return true;
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
moGsGraph::CheckState( GstStateChangeReturn state_change_result, bool waitforsync) {

  if (!waitforsync)
  switch(state_change_result) {
    case GST_STATE_CHANGE_FAILURE:
        //MODebug->Push(moText("GST_STATE_CHANGE_FAILURE"));
        return false;
        break;
    case GST_STATE_CHANGE_SUCCESS:
        //MODebug->Push(moText("GST_STATE_CHANGE_SUCCESS"));
        return true;
        break;
    case GST_STATE_CHANGE_ASYNC:
        //MODebug->Push(moText("GST_STATE_CHANGE_ASYNC"));
        return true;
        break;
    case GST_STATE_CHANGE_NO_PREROLL:
        //MODebug->Push(moText("GST_STATE_CHANGE_NO_PREROLL"));
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

//CONTROL METHODS
void
moGsGraph::Play() {
  /* start the pipeline */
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
  //CheckState( gst_element_set_state (GST_ELEMENT (m_pGstPipeline), GST_STATE_PAUSED));
}

#define MO_INFINITE -1
void
moGsGraph::Seek( MOuint frame ) {

  gint64     time_nanoseconds;
  bool        res;

  if (m_FramesLength>0 && m_FramesLength<(MOulong)MO_INFINITE) {
    time_nanoseconds = (gint64) frame * m_VideoFormat.m_TimePerFrame;

    //cout << "seeking frame:" << frame << " in " << time_nanoseconds << endl;
    /*res = gst_element_seek (m_pGstPipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                         GST_SEEK_TYPE_SET, time_nanoseconds,
                         GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
                         */
    res = gst_element_seek_simple( m_pGstPipeline, GST_FORMAT_TIME, (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), time_nanoseconds );
    //cout << "success:" << res << endl;
    //this->Pause();
  }

}

MOulong
moGsGraph::GetFramesLength() {

	GstFormat fmt = GST_FORMAT_TIME;

    gint64 len;

    if (gst_element_query_duration (m_pGstPipeline, &fmt, &len)) {
    /*g_print ("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r",
         GST_TIME_ARGS (pos), GST_TIME_ARGS (len));*/
         if (m_VideoFormat.m_TimePerFrame)
            m_FramesLength = ( len / ( m_VideoFormat.m_TimePerFrame ) ) - 5;
         //cout << "gsgraph: len: ns: " << len  << " frames:" << m_FramesLength << endl;
         return m_FramesLength;
    }

	return 0;
}

MOulong
moGsGraph::GetDuration() {

	GstFormat fmt = GST_FORMAT_TIME;

    gint64 dur;

    if (gst_element_query_duration (m_pGstPipeline, &fmt, &dur)) {
         m_Duration = dur / 1000000; //in milliseconds  1ms = 1 million ns
         //cout << "gsgraph: dur: ns: " <<  dur  << endl;
         return dur;
    }

	return 0;
}

MOulong
moGsGraph::GetPosition() {

    GstFormat fmt = GST_FORMAT_TIME;
    gint64 pos;

    if (gst_element_query_position (m_pGstPipeline, &fmt, &pos)) {
         return (MOulong)(pos / m_VideoFormat.m_TimePerFrame );
    }
    return 0;
}

bool
moGsGraph::IsRunning() {
    if (gst_element_get_state (m_pGstPipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE ) return false;
    return true;
}

MObyte *
moGsGraph::GetFrameBuffer( MOlong *size ) {

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
