#ifndef MO_MOLDEO_HEADERS
#define MO_MOLDEO_HEADERS

#include "moTypes.h"
#include "moConsole.h"

/**
*   \mainpage
*
*   \if spanish
*   Bienvenidos a la documentación de Moldeo, una plataforma de desarrollo para aplicaciones multimediales interactivas en tiempo real.
*   \else
*   Welcome to the Moldeo's documentation, Moldeo is a software and a development platform for interactive multimedia applications in real time.
*   \endif
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

                //Update
                MoldeoConsole.Update();

                //Draw
                MoldeoConsole.Draw();
            }
         }
    }
*   \endcode
*
*
*
*   \author     Moldeo Interactive
*   \author     Fabricio Costa Alisedo
*
*
*/

#endif
