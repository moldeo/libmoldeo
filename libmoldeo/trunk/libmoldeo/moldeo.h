#ifndef MO_MOLDEO_HEADERS
#define MO_MOLDEO_HEADERS

#include "moTypes.h"
#include "moConsole.h"

/**
*   \mainpage
*   \if spanish
*
*   \image html MoldeoHeaderLogoTransicionTierra2.png "Moldeo     http://www.moldeo.org"
*
*   Bienvenidos a la documentación de Moldeo, una plataforma de desarrollo para aplicaciones multimediales interactivas en tiempo real.
*
*   \code

    #include "moldeo.h"

    void main() {

        moConsole MoldeoConsole;

        // Inicializamos en primer lugar
        if ( MoldeoConsole.Init( moText("test"),
                                moText("test.mol") ) ) {

            // Ciclo principal
            // Mientras interactuamos
            while( MoldeoConsole.Interaction() ) {

                //Actualizamos
                MoldeoConsole.Update();

                //Dibujamos
                MoldeoConsole.Draw();
            }
         }
    }
*   \endcode
*
*
*
*   \author     Fabricio Costa Alisedo
*   \author     Gustavo Orrillo
*   \author
*   \author     Cristian Rocha
*   \author     Christian Parsons
*   \author     Arturo Bianchetti
*   \author     Gustavo Lado
*
*   \endif
*
*/

#endif
