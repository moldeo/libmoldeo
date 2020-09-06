/*******************************************************************************

                                moActions.h

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

#ifndef __MOACTIONS_H
#define __MOACTIONS_H

#include "moTypes.h"
#include "moValue.h"


#include <iostream>
#include <map>
#include <string>
using namespace std;

#define MO_ACTION_LISTENER_HOST_UNDEFINED 127.0.0.1
#define MO_ACTION_LISTENER_PORT_UNDEFINED 7777

#define MO_ACTION_MOLDEOAPI_EVENT_SEND 333
#define MO_ACTION_MOLDEOAPI_EVENT_RECEIVE 444


/// moMoldeoActionType
/**
* Tipos de acciones de la API de Moldeo.
*
* Estas acciones pueden ser disparadas de dos formas:
*
* Eventos:
*   publicando un evento en el IODeviceManager -> EventsList
*   o bien enviando por OSC al plugin de NetOscIn un mensaje formateado:
*
* Moldeo OSC API 1.0 , a trav�s de habilitar el plugin (IODevice) "netoscin" :
*   /moldeo/mensaje/parametro 1/parametro 2/parametro 3/parametro 4...
*
*/
enum moMoldeoActionType {

  MO_ACTION_UNDEFINED = -1,





  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  // VALUES
  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====

  /// "valueadd" > MO_ACTION_VALUE_ADD
  /**
  * Agrega un valor a un par�metro
  * Al definir el value index como -1, se agrega al final
  * Al definir un value index como 0 o N, se inserta antes de ese valor
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter id
  * param 3: -1 | value index | value index:sub value index
  * param 4: new value
  */
  MO_ACTION_VALUE_ADD = 100, /** add a value to existing param ( objectname|objectid, paramname|paramid, defaultvalue )*/

  /// "valuedelete" > MO_ACTION_VALUE_DELETE
  /**
  * Elimina un valor de un par�metro
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter id
  * param 3: value index | value index:sub value index
  */
  MO_ACTION_VALUE_DELETE, /** delete a value from existing param ( objectname|objectid, paramname|paramid )*/

  /// "valuesave" > MO_ACTION_VALUE_SAVE
  /**
  * Guarda f�sicamente el valor de un par�metro en el config en disco
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter id
  * param 3: value index | value index:sub value index
  */
  MO_ACTION_VALUE_SAVE, /** save a value ( objectname|objectid, paramname|paramid, valueid, value )*/

  /// "valuerefresh" > MO_ACTION_VALUE_REFRESH
  /**
  * Envia un mensaje para volver a cargar un valor de su origen. Si se trata del par�metro de una textura, este comando har� que
  * la imagen se vuelva a cargar.
  *
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter id
  * param 3: value index | value index:sub value index
  */
  MO_ACTION_VALUE_REFRESH, /** refresh a value ( objectname|objectid, paramname|paramid, valueid, value )*/

  /// "valueset" > MO_ACTION_VALUE_SET
  /**
  * Fija un valor
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter id
  * param 3: value index | value index:sub value index
  * param 4: new value
  */
  MO_ACTION_VALUE_SET,/** set the index of a value ( objectname|objectid, paramname|paramid, valueid, indexvalue ) */

  /// "valueget" > MO_ACTION_VALUE_GET
  /**
  * Obtiene un valor
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter id
  * param 3: value index | value index:sub value index
  */
  MO_ACTION_VALUE_GET,/** get the index of a value  ( objectname|objectid, paramname|paramid, valueid ) */

 /// "valueget" > MO_ACTION_VALUE_GET
  /**
  * Obtiene el mismo valor pero evaluado
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter id
  * param 3: value index | value index:sub value index
  */
  MO_ACTION_VALUE_GETCOMPUTED,/** get the fully computed value (in case of a function>use this one to know actual value!) ( objectname|objectid, paramname|paramid, valueid ) */





  /** MO_ACTION related to PARAMETERS */

  /// "paramadd" > MO_ACTION_PARAM_ADD
  /**
  * Fijar un parámetro: definición y configuración
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter id
  * param 3: paremeter info { "": "", "": "", "": "" }
  */
  MO_ACTION_PARAM_ADD, /** ( objectname|objectid, paramname|paramid, paraminfo ) > if passed name/type doest affect, but can be affected by 'interpolation' and 'duration', 'position' !! */

  /// "paramget" > MO_ACTION_PARAM_GET
  /**
  * Obtiene un parametro: definici�n y configuraci�n
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter id
  */
  MO_ACTION_PARAM_GET, /** ( objectname|objectid, paramname|paramid ) > Retreive full info of a parameters ( name, type and all values)*/

  /// "paramset" > MO_ACTION_PARAM_SET
  /**
  * Fijar un par�metro: definici�n y configuraci�n
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter id
  * param 3: paremeter info { "": "", "": "", "": "" }
  */
  MO_ACTION_PARAM_SET, /** ( objectname|objectid, paramname|paramid, paraminfo ) > if passed name/type doest affect, but can be affected by 'interpolation' and 'duration', 'position' !! */

  /// "paramsave" > MO_ACTION_PARAM_SAVE
  /**
  * Guardar la info completa de un par�metro en el archivo de configuraci�n
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter id
  */
  MO_ACTION_PARAM_SAVE, /** ( objectname|objectid, paramname|paramid) > Save actual parameter to file !? */





  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  // PRECONFIGS
  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====

  /// "preconfigadd" > MO_ACTION_PRECONFIG_ADD
  /**
  * Agregar una preconfiguraci�n
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter id
  * param 3: preconfig info
  */
  MO_ACTION_PRECONFIG_ADD, /** add a preconfig set to object ( uses actual param's value position ) ( objectname|objectid, paramname|paramid, preconfigname ) */

  /// "preconfigdelete" > MO_ACTION_PRECONFIG_DELETE
  /**
  * Eliminar una preconfiguraci�n
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter index
  * param 3: preconfig name | preconfig index
  */
  MO_ACTION_PRECONFIG_DELETE, /** delete a preconfig set ( objectname|objectid, paramname|paramid, preconfigname ) */

  /// "preconfigsave" > MO_ACTION_PRECONFIG_SAVE
  /**
  * Eliminar una preconfiguraci�n
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter index
  * param 3: preconfig name | preconfig index
  */
  MO_ACTION_PRECONFIG_SAVE, /** save a preconfig set ( objectname|objectid, paramname|paramid, preconfigname ) */

  /// "preconfigset" > MO_ACTION_PRECONFIG_SET
  /**
  * Fijar una preconfiguraci�n
  *
  * param 1: object name | object id
  * param 2: parameter name | parameter index
  * param 3: preconfig name | preconfig index
  * param 4: preconfig info
  */
  MO_ACTION_PRECONFIG_SET, /** set a preconfig set ( objectname|objectid, paramname|paramid, preconfigame ) */






  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  // OBJECTS
  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====

  /// "objects add" > MO_ACTION_OBJECTS_ADD
  /**
  * Agregar un objeto.
  * El primer par�metro define el punto de anclaje para este objeto.
  * si es "console" o -1. Lo agregar� en el objeto consola.
  * si es "escena xxx" o id>=101 entonces se trata de algun objeto donde agregar el objeto
  * solo se puede agregar un fx object a una escena, en cambio en una consola se pueden agregar
  * todas las categor�as de objetos ( fx, resource, iodevice )
  *
  * param 1: father object name | id
  * param 3: object info
  */
  MO_ACTION_OBJECT_ADD, /// 1:  father object name | id, 2: object info

  /// "objectduplicate" : MO_ACTION_OBJECT_DUPLICATE
  /**
  * Duplicar un objeto
  *
  * param 1: object name | object id
  */
  MO_ACTION_OBJECT_DUPLICATE, /// 1:  object name | id

  /// "objectimport" : MO_ACTION_OBJECT_IMPORT
  /**
  * Importar un objeto
  *
  * param 1: config full file path
  */
  MO_ACTION_OBJECT_IMPORT, /// 1:  config full file path

  /// "objectget" : MO_ACTION_OBJECT_GET
  /**
  * Obtener la info completa de un objeto
  *
  * param 1: object name | object id
  */
  MO_ACTION_OBJECT_GET,

  /// "objectgetstate" : OBJECT_GETSTATE
  /**
  * Obtener el estado completo de un objeto
  *
  * param 1: object name | object id
  *
  */
  MO_ACTION_OBJECT_GETSTATE,

  /// "objectget" : MO_ACTION_OBJECT_GETCONFIG
  /**
  * Obtener la info completa de la configuraci�n de un objeto
  *
  * param 1: object name | object id
  */
  MO_ACTION_OBJECT_GETCONFIG,

  /// "objectgetpreconfig" : MO_ACTION_OBJECT_GETPRECONFIG
  /**
  * Obtener la info completa de una preconfiguraci�n
  *
  * param 1: object name | object id
  * param 2: preconfig index
  */
  MO_ACTION_OBJECT_GETPRECONFIG,

  /// "objectgetpreconfig" : MO_ACTION_OBJECT_GETPRECONFIG
  /**
  * Obtener la info completa de todas las preconfiguraciones
  *
  * param 1: object name | object id
  */
  MO_ACTION_OBJECT_GETPRECONFIGS,

  /// "objectsave" : MO_ACTION_OBJECT_SAVE
  /**
  * Guardar el objeto completo en el archivo de configuraci�n
  *
  * param 1: object name | object id
  *
  */
  MO_ACTION_OBJECT_SAVE, /** EDITOR */

  /// "objectset" : MO_ACTION_OBJECT_SET
  /**
  * Asignar uno o varios valores al estado y la configuraci�n
  *
  * param 1: object name | object id
  * param 2: object info {}
  *
  */
  MO_ACTION_OBJECT_SET, ///test

  /// "objectset" : MO_ACTION_OBJECT_DELETE
  /**
  * Eliminar un objeto
  *
  * param 1: object name | object id
  *
  */
  MO_ACTION_OBJECT_DELETE, /// param 1: object name | object id

  /// "objetmove" : MO_ACTION_OBJECT_MOVE
  /**
  * Mover un objeto de posici�n en la consola
  * si el param 3 corresponde a una posici�n ya ocupada, la desplaza hacia arriba.
  *
  * param 1: object name | object id
  * param 1: new father object name | new father object id
  * param 3: new object position index
  */
  MO_ACTION_OBJECT_MOVE, ///

  /// "objetenable" : MO_ACTION_OBJECT_ENABLE
  /**
  * Activar un objeto
  *
  * param 1: object name | object id
  */
  MO_ACTION_OBJECT_ENABLE, ///

  /// "objetdisable" : MO_ACTION_OBJECT_DISABLE
  /**
  * Desactivar un objeto
  *
  * param 1: object name | object id
  */
  MO_ACTION_OBJECT_DISABLE, /// param 1: object name | object id

  /// "objecttoggle" : MO_ACTION_OBJECT_TOGGLE
  /**
  * Activa/Desactiva un objeto
  *
  * param 1: object name | object id
  */
  MO_ACTION_OBJECT_TOGGLE, /// param 1: object name | object id

  /// "objectconnect" : MO_ACTION_OBJECT_CONNECT
  /**
  * Conecta un objeto
  *
  * param 1: source object name | source object id
  * param 2: destination object name | destination object id
  * param 3 (optional): source param name | source param id
  * param 4 (optional): destination param name | destination param id
  */
  MO_ACTION_OBJECT_CONNECT, ///  param 1: source object name | source object id, param 2: destination object name | destination object id, param 3 (optional): source param name | source param id, param 4 (optional): destination param name | destination param id





  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  // EFFECTS moEffect derived from moMoldeoObject
  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====

  /// "effectenable" : MO_ACTION_EFFECT_ENABLE
  /**
  * Activar un efecto
  *
  * param 1: effect label name | effect id
  */
  MO_ACTION_EFFECT_ENABLE, /// param 1: effect label name | effect id

  /// "effectdisable" : MO_ACTION_EFFECT_DISABLE
  /**
  * Desactivar un efecto
  *
  * param 1: effect label name | effect id
  */
  MO_ACTION_EFFECT_DISABLE, /// param 1: effect label name | effect id

  /// "effectsetstate" : MO_ACTION_EFFECT_SETSTATE
  /**
  * Fijar el estado de un efecto
  *
  * param 1: effect name | object id
  * param 2: new effect info state
  *
  */
  MO_ACTION_EFFECT_SETSTATE, ///

  /// "effectgetstate" : MO_ACTION_EFFECT_SETSTATE
  /**
  * Obtiene el estado completo de un efecto
  *
  * param 1: effect name | object id
  *
  * return: new effect info state
  *
  */
  MO_ACTION_EFFECT_GETSTATE, ///

  /// "effectplay" : MO_ACTION_EFFECT_PLAY
  /**
  * Activa el reloj del efecto
  *
  * param 1: effect name | object id
  *
  */
  MO_ACTION_EFFECT_PLAY, ///

  /// "effectstop" : MO_ACTION_EFFECT_STOP
  /**
  * Para el reloj del efecto
  *
  * param 1: effect name | object id
  *
  */
  MO_ACTION_EFFECT_STOP, ///

  /// "effectpause" : MO_ACTION_EFFECT_PAUSE
  /**
  * Pausa el reloj del efecto
  *
  * param 1: effect name | object id
  *
  */
  MO_ACTION_EFFECT_PAUSE, ///





  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  // EVENTS
  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====

  /// "eventadd" : MO_ACTION_EVENT_ADD
  /**
  * Agrega un evento a la cola de eventos
  *
  * param 1: event info
  *
  */
  MO_ACTION_EVENT_ADD, /// param 1: event info




  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  // CONSOLE
  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====

  /// "consoleplay" : MO_ACTION_CONSOLE_PLAY
  /**
  * Activa el reloj de la consola
  *
  */
  MO_ACTION_CONSOLE_PLAY, ///

  /// "consoleplaysession" : MO_ACTION_CONSOLE_PLAY_SESSION
  /**
  *
  * Play a recorded session
  */
  MO_ACTION_CONSOLE_PLAY_SESSION, ///

  /// "consolerecordsession" : MO_ACTION_CONSOLE_RECORD_SESSION
  /**
  * Record a live session
  *
  */
  MO_ACTION_CONSOLE_RECORD_SESSION, ///

  /// "consolerendersession" : MO_ACTION_CONSOLE_RECORD_SESSION
  /**
  * Render a recorded session
  *
  */
  MO_ACTION_CONSOLE_RENDER_SESSION, ///

  /// "consolesavesessionas" : MO_ACTION_CONSOLE_SAVE_SESSION_AS
  /**
  * Save a recorded session as a xml file
  *
  */
  MO_ACTION_CONSOLE_SAVE_SESSION_AS, ///

  /// "consolepause" : MO_ACTION_CONSOLE_PAUSE
  /**
  * Pausa el reloj de la consola
  *
  */
  MO_ACTION_CONSOLE_PAUSE, ///

  /// "consolestop" : MO_ACTION_CONSOLE_STOP
  /**
  * Para el reloj de la consola
  *
  */
  MO_ACTION_CONSOLE_STOP, ///

  /// "consolesave" : MO_ACTION_CONSOLE_SAVE
  /**
  * Salva la configuraci�n de la consola en su archivo de configuraci�n (.mol)
  *
  */
  MO_ACTION_CONSOLE_SAVE, ///

  /// "consolesaves" : MO_ACTION_CONSOLE_SAVEAS
  /**
  * Salva la configuraci�n de la consola en su archivo de configuraci�n (.mol)
  *
  * param 1:  new location path and .mol name
  *
  */
  MO_ACTION_CONSOLE_SAVEAS, ///

  /// "consolescreenshot" : MO_ACTION_CONSOLE_SCREENSHOT
  /**
  * Captura una imagen de pantalla y la guarda en la carpeta de screenshots del proyecto
  *
  * param 1:  new location path for screenshots
  *
  * return: image path ( C:\\molproject\\moldeo_screenshots\\01-02-1410:00:01.png )
  */
  MO_ACTION_CONSOLE_SCREENSHOT, ///

  /// "consolepreviewshot" : MO_ACTION_CONSOLE_PREVIEW_SHOT
  /**
  * Captura una secuencia de 4 im�genes y las guarda en la carpeta del proyecto
  * asignando estas en el par�metro "previews".
  * Una vez definidas estas servir�n para la previsualizaci�n del proyecto.
  *
  * param 1:  new location path for screenshots
  *
  * return: image path ( C:\\molproject\\moldeo_screenshots\\01-02-1410:00:01.png )
  */
  MO_ACTION_CONSOLE_PREVIEW_SHOT, ///

  /// "consolepresentation" : MO_ACTION_CONSOLE_PRESENTATION
  /**
  * Se�al de cambiar de modo de presentaci�n: fullscreen, fullscreen_secondary, windowed
  *
  */
  MO_ACTION_CONSOLE_PRESENTATION, /// Se�al de cambiar de modo de presentaci�n: fullscreen, fullscreen_secondary, windowed


  /// "consolefullscreen" : MO_ACTION_CONSOLE_FULLSCREEN
  /**
  * Pasar el modo de presentaci�n a pantalla completa
  *
  */
  MO_ACTION_CONSOLE_FULLSCREEN,

  /// "consolefullscreensecondary" : MO_ACTION_CONSOLE_FULLSCREEN_SECONDARY
  /**
  * Pasar el modo de presentaci�n a pantalla completa en el segundo monitor
  *
  */
  MO_ACTION_CONSOLE_FULLSCREEN_SECONDARY,

  /// "consolewindowed" : MO_ACTION_CONSOLE_WINDOWED
  /**
  * Pasar el modo de presentaci�n a ventana
  *
  */
  MO_ACTION_CONSOLE_WINDOWED,

  /// "consolereset" : MO_ACTION_CONSOLE_RESET
  /**
  * TODO: Reset ?? Reinicia el modo de pantalla y el proyecto???
  *
  */
  MO_ACTION_CONSOLE_RESET,

  /// "consoleget" : MO_ACTION_CONSOLE_GET
  /**
  * Obtiene la info completa de la consola
  *
  * return: full console info (state+config) (json format)
  */
  MO_ACTION_CONSOLE_GET, ///

  /// "consolegetstate" : MO_ACTION_CONSOLE_GETSTATE
  /**
  * Obtiene la info completa de la consola: state + config
  *
  * return: full console state
  */
  MO_ACTION_CONSOLE_GETSTATE, ///

  /// "consolesetstate" : MO_ACTION_CONSOLE_SETSTATE
  /**
  * Fija el estado de la consola
  *
  * param 1: full console state
  *
  */
  MO_ACTION_CONSOLE_SETSTATE, ///

  /// "consolegetplugins" : MO_ACTION_CONSOLE_GET_PLUGINS
  /**
  * Devuelve la lista de definiciones
  *
  *
  */
  MO_ACTION_CONSOLE_GET_PLUGINS, /// Devuelve la lista de definiciones





  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  // CONSOLE PRESETS
  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====

  /// "consolepresetadd" : MO_ACTION_CONSOLE_PRESET_ADD
  /**
  * Agregar un preset a la consola
  *
  * param 1: preset position index | -1
  * param 2: preset info
  *
  */
  MO_ACTION_CONSOLE_PRESET_ADD, ///

  /// "consolepresetdelete" : MO_ACTION_CONSOLE_PRESET_DELETE
  /**
  * Eliminar un preset a la consola
  *
  * param 1: preset position index | -1
  *
  */
  MO_ACTION_CONSOLE_PRESET_DELETE, ///

  /// "consolepresetsave" : MO_ACTION_CONSOLE_PRESET_SAVE
  /**
  * Guardar el preset en el archivo de configuraci�n
  * si se elige -1, guarda todos los presets en memoria en el archivo
  *
  * param 1: preset position index | -1
  *
  */
  MO_ACTION_CONSOLE_PRESET_SAVE, ///

  /// "consolepresetset" : MO_ACTION_CONSOLE_PRESET_SET
  /**
  * Fija un preset
  *
  * param 1: preset position index | -1
  * param 2: new preset info
  *
  */
  MO_ACTION_CONSOLE_PRESET_SET,  ///









  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====
  // ACTIONS - REACTIONS
  //==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ====

  /** Subscribe specific connection to receive REACTION MESSAGES */
  MO_ACTION_LISTEN_TO_REACTIONS, /** ( hostname|ipname, portname|portnumber, actiontypename|actiontypeid, objectname|objectid, paramname|paramid ) */
  MO_ACTION_STOP_LISTENING_TO_REACTIONS /** ( hostname|ipname, portname|portnumber, objectname|objectid, paramname|paramid ) */



};


enum moMoldeoReactionType {

  MO_REACTION_UNDEFINED = -1,
  /** specified value of some object was updated */
  MO_REACTION_VALUE_UPDATED = 500, /** ( objectname|objectid, paramname|paramid, valuename|valueid, { FULL VALUE INFO } ) */
  MO_REACTION_VALUE_ERROR,

  /** specified parameter of some object was updated (a param has changed a property/add or deletion of inner values ) */
  MO_REACTION_PARAM_UPDATED, /** ( objectname|objectid, paramname|paramid, { FULL PARAM INFO }  ) */
  MO_REACTION_PARAM_ERROR,

  /** specified object was updated, (params were added ?! label of object was changed ) */
  MO_REACTION_OBJECT_UPDATED, /** ( objectname|objectid, { FULL OBJECT INFO } ) */
  MO_REACTION_OBJECT_ERROR,

  /** console has to be fully reloaded ( objects are added, moved or removed ) */
  MO_REACTION_CONSOLE_UPDATED, /** (  ) */
  MO_REACTION_CONSOLE_ERROR
};





/**
  IMPERATIVE ORDERS
  imperative orders can be sent as messages from OSC like [ /moldeo loadproject C:\test.mol ]
  or just [ /moldeo setcurrentvalue icono1 alpha 2 ]
  or [ /moldeo valuesave icono1 alpha 2 ]
*/
class moAction {

  public:
    moAction();
    moAction( moMoldeoActionType p_ActionType );
    virtual ~moAction();

    moMoldeoActionType m_ActionType;
    moDataMessage m_Arguments;
};

class moReaction {

  public:
    moReaction();
    moReaction( moMoldeoReactionType p_ReactionType );
    virtual ~moReaction();

    moMoldeoReactionType m_ReactionType;

};



/**
  This class is a connection to a reaction listener
*/
class moReactionListener {

  public:
    moReactionListener();
    moReactionListener( const moText &p_host, int p_port );
    virtual ~moReactionListener();

    moText m_hostname;
    int m_port;

};

moDeclareExportedDynamicArray( moReactionListener*, moReactionListeners );

typedef std::map< std::string, moAction > TMapStrToAction;
typedef std::map< std::string, int> TMapStrToActionType;

class moReactionListenerManager : public moAbstract {

  public:
    moReactionListenerManager();
    virtual ~moReactionListenerManager();

    /**List of connections */
    moReactionListeners m_ReactionListeners;
    static moMoldeoActionType StrToActionType( const moText& p_action_type_str );

    static TMapStrToAction      m_MapStrToAction;
    static TMapStrToActionType  m_MapStrToActionType;

};

#endif
