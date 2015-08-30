#ifndef MO_MOLDEO_HEADERS
#define MO_MOLDEO_HEADERS

#include "moTypes.h"
#include "moConsole.h"

/**
*   \mainpage
*   \if spanish
*
*   \link http://www.moldeo.org/documentation#head
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
*   \author     Moldeo Interactive
*   \author
*   \author     Fabricio Costa Alisedo
*   \author
*   \endif
*
*/

#endif
