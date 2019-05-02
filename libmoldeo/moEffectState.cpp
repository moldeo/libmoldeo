/*******************************************************************************

                              moEffectState.cpp

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

#include "moEffectState.h"
#include <tinyxml.h>
#include "moDebugManager.h"

moEffectState::moEffectState() : moMobState() {
    Init();
}

moEffectState::~moEffectState() {
}


moEffectState::moEffectState(const moEffectState& src)  : moMobState( src ) {
    (*this) = src;
}

moEffectState&
moEffectState::operator=(const moEffectState& src) {

        m_Activated = src.m_Activated;
        m_Selected = src.m_Selected;
        on = src.on;
        synchronized = src.synchronized;
        enabled = src.enabled;
        magnitude = src.magnitude;
        amplitude = src.amplitude;
		alpha = src.alpha;

        tintr = src.tintr;
		tintg = src.tintg;
		tintb = src.tintb;

		tint = src.tint;
		tintc = src.tintc;
		tints = src.tints;

		stereo = src.stereo;
		stereoside = src.stereoside;
		fulldebug = src.fulldebug;

		tempo = src.tempo;

		mousex = src.mousex;
		mousey = src.mousey;
		mousefactor = src.mousefactor;

		return (*this);
}

MOboolean
moEffectState::Init() {

        on = MO_DEACTIVATED;
        synchronized = MO_ACTIVATED;
        enabled = MO_ACTIVATED;
        magnitude = 1.0;
        amplitude = 1.0;
		alpha = 1.0;

        tintr = 1.0;
		tintg = 1.0;
		tintb = 1.0;

		tint = 1.0;
		tintc = 0.0;
		tints = 0.0;

		stereo = MO_DEACTIVATED;
		stereoside = MO_STEREO_NONE;
		fulldebug = MO_DEACTIVATED;

		tempo.Init();

		mousex = 0.0;
		mousey = 0.0;
		mousefactor = 0.0;

		return true;
}

MOboolean
moEffectState::Init(MOswitch D) {

        Init();
		fulldebug = D;

		return true;
}


void moEffectState::SetColorRGB( MOfloat r, MOfloat g, MOfloat b)  {
    tintr = r;
    tintr = g;
    tintr = b;
}

void moEffectState::SetColorCSV( MOfloat c, MOfloat s, MOfloat t)  {
    tintc = c;
    tints = s;
    tint = t;
    CSV2RGB();
}

void
moEffectState::CSV2RGB() {
	short C,S,V;

	C= (short)((float)tintc * (float)360);
	if (C>360) C=360;
	if (C<0) C = 0;


	S = (short)((float)tints * (float)255);
	if (S>255) S=255;
	if (S<0) S = 0;

	V = (short)((float)tint * (float)255);
	if (V>255) V=255;
	if (V<0) V = 0;

	HSVtoRGB( C, S, V, &tintr, &tintg, &tintb);

}

const moText&
moEffectState::ToJSON() {

  moText fieldSeparation = ",";

  /** from mobstate */
  /**
  m_Activated = src.m_Activated;
  m_Selected = src.m_Selected;
  */
  fullJSON = "{";
  fullJSON+= "'activated': '"+IntToStr(m_Activated)+"'";
  fullJSON+= fieldSeparation+"'selected': '"+IntToStr(m_Selected)+"'";

  fullJSON+= fieldSeparation+ "'tempo': {";
  fullJSON+= "'tempo': '"+FloatToStr(tempo.getTempo())+"'";
  fullJSON+= fieldSeparation+ "'syncro': '"+FloatToStr(tempo.syncro)+"'";
  fullJSON+= fieldSeparation+ "'delta': '"+FloatToStr(tempo.delta)+"'";
  fullJSON+= fieldSeparation+ "'duration': '"+IntToStr(tempo.Duration())+"'";
  fullJSON+= fieldSeparation+ "'state': '"+tempo.StateToStr()+"'";
  fullJSON+= fieldSeparation+ "'started': '"+IntToStr(tempo.Started())+"'";
  fullJSON+= fieldSeparation+ "'factor': '"+FloatToStr(tempo.factor)+"'";

  fullJSON+= fieldSeparation+"'globaltimer_state': '" + moGetTimerStateStr() + "'";
  fullJSON+= fieldSeparation+"'globaltimer_duration': '" + IntToStr(moGetDuration())+"'";

  fullJSON+= "}";
  /** from state */


  fullJSON+= fieldSeparation + "'on': '"+IntToStr(on)+"'";
  fullJSON+= fieldSeparation + "'synchronized': '"+IntToStr(synchronized)+"'";

  fullJSON+= fieldSeparation + "'enabled': '"+IntToStr(enabled)+"'";

  /**  */
  fullJSON+= fieldSeparation + "'magnitude': '"+FloatToStr(magnitude)+"'";
  fullJSON+= fieldSeparation + "'amplitude': '"+FloatToStr(amplitude)+"'";

  /**COLORS*/
  fullJSON+= fieldSeparation + "'alpha': '"+FloatToStr(alpha)+"'";
  fullJSON+= fieldSeparation + "'tint': '"+FloatToStr(tint)+"'";
  fullJSON+= fieldSeparation + "'tintr': '"+FloatToStr(tintr)+"'";
  fullJSON+= fieldSeparation + "'tintg': '"+FloatToStr(tintg)+"'";
  fullJSON+= fieldSeparation + "'tintb': '"+FloatToStr(tintb)+"'";
  fullJSON+= fieldSeparation + "'tintc': '"+FloatToStr(tintc)+"'";
  fullJSON+= fieldSeparation + "'tints': '"+FloatToStr(tints)+"'";

  fullJSON+= fieldSeparation + "'stereo': '"+IntToStr(stereo)+"'";
  fullJSON+= fieldSeparation + "'stereoside': '"+IntToStr(stereoside)+"'";

  fullJSON+= fieldSeparation + "'fulldebug': '"+IntToStr(fulldebug)+"'";
  fullJSON+= fieldSeparation + "'mousex': '"+FloatToStr(mousex)+"'";
  fullJSON+= fieldSeparation + "'mousey': '"+FloatToStr(mousey)+"'";
  fullJSON+= fieldSeparation + "'mousefactor': '"+FloatToStr(mousefactor)+"'";

  fullJSON+= "}";
  //cout << fullJSON << endl;
  return fullJSON;
}


const moText&
moEffectState::ToXML() {

  moText fieldSeparation = " ";

  /** from mobstate */
  /**
  m_Activated = src.m_Activated;
  m_Selected = src.m_Selected;
  */
  fullXML = "<moEffectState ";
  fullXML+= "Activated='"+IntToStr(m_Activated)+"'";
  fullXML+= fieldSeparation+"Selected='"+IntToStr(m_Selected)+"'";

  /** from state */


  fullXML+= fieldSeparation + "on='"+IntToStr(on)+"'";
  fullXML+= fieldSeparation + "synchronized='"+IntToStr(synchronized)+"'";

  fullXML+= fieldSeparation + "enabled='"+IntToStr(enabled)+"'";

  /**  */
  fullXML+= fieldSeparation + "magnitude='"+FloatToStr(magnitude)+"'";
  fullXML+= fieldSeparation + "amplitude='"+FloatToStr(amplitude)+"'";

  /**COLORS*/
  fullXML+= fieldSeparation + "alpha='"+FloatToStr(alpha)+"'";
  fullXML+= fieldSeparation + "tint='"+FloatToStr(tint)+"'";
  fullXML+= fieldSeparation + "tintr='"+FloatToStr(tintr)+"'";
  fullXML+= fieldSeparation + "tintg='"+FloatToStr(tintg)+"'";
  fullXML+= fieldSeparation + "tintb='"+FloatToStr(tintb)+"'";
  fullXML+= fieldSeparation + "tintc='"+FloatToStr(tintc)+"'";
  fullXML+= fieldSeparation + "tints='"+FloatToStr(tints)+"'";

  fullXML+= fieldSeparation + "stereo='"+IntToStr(stereo)+"'";
  fullXML+= fieldSeparation + "stereoside='"+IntToStr(stereoside)+"'";

  fullXML+= fieldSeparation + "fulldebug='"+IntToStr(fulldebug)+"' >";


  fullXML+= fieldSeparation+ "<moTempo ";
  fullXML+= "tempo='"+FloatToStr(tempo.getTempo())+"'";
  fullXML+= fieldSeparation+ "syncro='"+FloatToStr(tempo.syncro)+"'";
  fullXML+= fieldSeparation+ "delta='"+FloatToStr(tempo.delta)+"'";
  fullXML+= fieldSeparation+ "duration='"+IntToStr(tempo.Duration())+"'";
  fullXML+= fieldSeparation+ "state='"+tempo.StateToStr()+"'";
  fullXML+= fieldSeparation+ "started='"+IntToStr(tempo.Started())+"'";
  fullXML+= fieldSeparation+ "factor='"+FloatToStr(tempo.factor)+"'";

  fullXML+= fieldSeparation+"globaltimer_state='" + moGetTimerStateStr() + "'";
  fullXML+= fieldSeparation+"globaltimer_duration='" + IntToStr(moGetDuration())+"' >";
  fullXML+= "</moTempo>";


  fullXML+= "</moEffectState>";
  //cout << fullJSON << endl;
  return fullXML;
}



int
moEffectState::Set( const moText& p_XmlText ) {

  TiXmlDocument   m_XMLDoc;
  //TiXmlHandle xmlHandle( &m_XMLDoc );
  TiXmlEncoding   xencoding = TIXML_ENCODING_LEGACY; ///or TIXML_ENCODING_UTF8

  m_XMLDoc.Parse((const char*) p_XmlText, 0, xencoding );
  ///convert xmltext to structure
  //TiXmlElement* rootKey = m_XMLDoc.FirstChildElement( "D" );
  TiXmlElement* effectStateNode = m_XMLDoc.FirstChildElement("moEffectState");

  //if (rootKey) {

    //TiXmlElement* sceneStateNode = rootKey->FirstChildElement("moSceneState");
    if (effectStateNode) {
      m_Activated = atoi( effectStateNode->Attribute("Activated") );
      m_Selected = atoi( effectStateNode->Attribute("Selected") );
      on = atoi( effectStateNode->Attribute("on") );
      synchronized = atoi( effectStateNode->Attribute("synchronized") );
      alpha = atof( effectStateNode->Attribute("alpha") );
      tint = atof( effectStateNode->Attribute("tint") );
      tintb = atof( effectStateNode->Attribute("tintb") );
      tintc = atof( effectStateNode->Attribute("tintc") );
      tintg = atof( effectStateNode->Attribute("tintg") );
      tintr = atof( effectStateNode->Attribute("tintr") );
      tints = atof( effectStateNode->Attribute("tints") );

      TiXmlElement* keyNode = effectStateNode->FirstChildElement("moTempo");
      if(keyNode) {

        tempo.ang = atof(moText( keyNode->Attribute("tempo") ));
        tempo.syncro = atof( moText( keyNode->Attribute("syncro") ) );
        tempo.delta = atof( moText( keyNode->Attribute("delta") ) );
        tempo.duration = atoi( moText( keyNode->Attribute("duration") ) );
        tempo.state_str = moText( keyNode->Attribute("state") );
        tempo.factor = atof( moText( keyNode->Attribute("factor") ) );
        if (atoi( moText( keyNode->Attribute("started") ) ) == 1 ) {
          if (tempo.Paused()) tempo.Continue();
          else if (!tempo.Started()) tempo.Start();

          //if (tempo.state_str=="")
        }

        //tempo.factor = atoi( moText( keyNode->Attribute("globaltimer_state") ) );
        //tempo.factor = atoi( moText( keyNode->Attribute("globaltimer_duration") ) );


      }
      return 0;
    } else moDebugManager::Log( "No XML moEffectState in: " + p_XmlText );

  //} else moDebugManager::Error();
  return -1;
}
