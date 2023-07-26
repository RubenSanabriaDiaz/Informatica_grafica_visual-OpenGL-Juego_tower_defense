#include <cstdlib>

#include "igvInterfaz.h"

// Objetos globales
// objeto que contiene la configuración y la interacción con la ventana de visualización
igvInterfaz interfaz;

int main (int argc, char** argv) {
    // inicializa la ventana de visualizacion
    interfaz.configura_entorno(argc,argv,
                               500,500, // tama–o de la ventana
                               100,100, // posicion de la ventana
                               "Informatica Grafica y Visualizacion. Proyecto de Prácticas IGV." // titulo de la ventana
                               );

    interfaz.setT0(0);
    
    // establece las funciones callbacks para la gestion de los eventos
    interfaz.inicializa_callbacks();

    // inicia el bucle de visualizacion de OpenGL
    interfaz.inicia_bucle_visualizacion();

    
    return(0);
}

