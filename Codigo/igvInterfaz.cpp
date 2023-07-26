#include <cstdlib>
#include <stdio.h>
#include "igvInterfaz.h"

extern igvInterfaz interfaz; // los callbacks deben ser estaticos y se requiere este objeto para acceder desde
							 // ellos a las variables de la clase

// Metodos constructores -----------------------------------
igvInterfaz::igvInterfaz() {}

igvInterfaz::~igvInterfaz() {}

// Metodos publicos ----------------------------------------
void igvInterfaz::crear_mundo(void) {
	// crear camaras
	p0 = igvPunto3D(-4, 3, 5); //punto con la posicion de la camara / donde estoy posicionado Vpos
	r = igvPunto3D(0, 0, 0); //punto de referencia hacia donde mira la camara / hacio donde miro Vdir
	V = igvPunto3D(0, 1.0, 0); //vector arriba / direccion de la vertical Vup

	interfaz.tipoA = ENEMIGOS;

	//atributos de tipo de camaras
	interfaz.camara.porcentajeZoom = 1.75;
	interfaz.camara.tipo = IGV_PARALELA;
	interfaz.camara.vista = ORIGINAL;
	interfaz.cuatro = false;

	//parametros de las proyecciones de la camara
	interfaz.camara.angulo = 60.0;
	interfaz.camara.raspecto = 1.0;
	interfaz.camara.znear = -1;

	interfaz.camara.set(interfaz.camara.tipo, p0, r, V,
		-interfaz.camara.porcentajeZoom * 3, interfaz.camara.porcentajeZoom * 3,
		-interfaz.camara.porcentajeZoom * 3, interfaz.camara.porcentajeZoom * 3,
		interfaz.camara.znear, 200);
}

void igvInterfaz::configura_entorno(int argc, char** argv,
	int _ancho_ventana, int _alto_ventana,
	int _pos_X, int _pos_Y,
	string _titulo) 
{
	this->numOleadas = 1;
	this->numVidasReyes = 6;
	this->tienda = false;
	this->modo = modoInterfaz::visualizar;
	this->terrenoSeleccionado = -1;
	this->estructuraSeleccionada = -1;
	this->torreSeleccionada = -1;
	this->estructuraDisparo = -1;
	this->dineroActual = 300;
	this->colorTorres = false;
	this->disparoRelizado = false;

	// inicializacion de las variables de la interfaz																	
	ancho_ventana = _ancho_ventana;
	alto_ventana = _alto_ventana;

	// inicializacion de la ventana de visualizacion
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(_ancho_ventana, _alto_ventana);
	glutInitWindowPosition(_pos_X, _pos_Y);
	glutCreateWindow(_titulo.c_str());

	glEnable(GL_DEPTH_TEST); // activa el ocultamiento de superficies por z-buffer
	glClearColor(1.0, 1.0, 1.0, 0.0); // establece el color de fondo de la ventana

	glEnable(GL_LIGHTING); // activa la iluminacion de la escena
	glEnable(GL_NORMALIZE); // normaliza los vectores normales para calculo iluminacion

	crear_mundo(); // crea el mundo a visualizar en la ventana
}

void igvInterfaz::inicia_bucle_visualizacion() {
	glutMainLoop(); // inicia el bucle de visualizacion de OpenGL
}

void igvInterfaz::set_glutKeyboardFunc(unsigned char key, int x, int y) {
	igvPunto3D p0 = igvPunto3D(-4, 3, 5); //punto con la posicion de la camara / donde estoy posicionado Vpos
	igvPunto3D r = igvPunto3D(0, 0, 0); //punto de referencia hacia donde mira la camara / hacio donde miro Vdir
	igvPunto3D V = igvPunto3D(0, 1, 0); //vector arriba / direccion de la vertical Vup

	/* IMPORTANTE: en la implementacion de este metodo hay que cambiar convenientemente el estado
	   de los objetos de la aplicacion, pero no hacer llamadas directas a funciones de OpenGL */

	switch (key) {
	case 'p': // cambia el tipo de proyeccion de paralela a perspectiva y viceversa
		if (interfaz.camara.tipo == IGV_PERSPECTIVA) {
			interfaz.camara.tipo = IGV_PARALELA;
		}
		else {
			if (interfaz.camara.tipo == IGV_PARALELA) {
				interfaz.camara.tipo = IGV_PERSPECTIVA;
			}
			else {
				if (interfaz.camara.tipo == IGV_FRUSTUM) {
					interfaz.camara.tipo = IGV_PARALELA;
				}
			}
		}

		interfaz.camara.aplicar();

		break;

	case 'v': // cambia la posicion de la camara para mostrar las vistas planta, perfil, alzado o perspectiva
		if (interfaz.camara.vista == ORIGINAL) {
			p0.set(8, 0, 0);
			interfaz.camara.vista = ALZADO; //x
		}
		else {
			if (interfaz.camara.vista == ALZADO) {
				p0.set(IGV_EPSILON, 8, 0);
				interfaz.camara.vista = PLANTA; //y
			}
			else {
				if (interfaz.camara.vista == PLANTA) {
					p0.set(0, 0, 8);
					interfaz.camara.vista = PERFIL; //z
				}
				else {
					if (interfaz.camara.vista == PERFIL) {
						p0.set(-4, 3, 5);
						interfaz.camara.vista = ORIGINAL;
					}
				}
			}
		}

		interfaz.camara.set(interfaz.camara.tipo, p0, r, V,
			-interfaz.camara.porcentajeZoom * 3, interfaz.camara.porcentajeZoom * 3,
			-interfaz.camara.porcentajeZoom * 3, interfaz.camara.porcentajeZoom * 3,
			interfaz.camara.znear, 200);

		interfaz.camara.aplicar();

		break;

	case '-': // zoom in
		if (interfaz.camara.porcentajeZoom < 3) {
			interfaz.camara.porcentajeZoom = interfaz.camara.porcentajeZoom + 0.1;
			interfaz.camara.zoom(interfaz.camara.porcentajeZoom);
		}

		break;

	case '+': // zoom out
		if (interfaz.camara.porcentajeZoom > 1) {
			interfaz.camara.porcentajeZoom = interfaz.camara.porcentajeZoom - 0.1;
			interfaz.camara.zoom(interfaz.camara.porcentajeZoom);
		}

		break;

	case 'n': // incrementar la distancia del plano cercano
		if (interfaz.camara.znear < interfaz.camara.zfar / 2) {
			interfaz.camara.znear = interfaz.camara.znear + 0.2;
			interfaz.camara.aplicar();
		}

		break;

	case 'N': // decrementar la distancia del plano cercano
		if (interfaz.camara.znear > -5) {
			interfaz.camara.znear = interfaz.camara.znear - 0.2;
			interfaz.camara.aplicar();
		}

		break;

	case 'x': // rotar X positivo 
		interfaz.escena.incrementarAngRotX(10);
		break;

	case 'X': // rotar X negativo 
		interfaz.escena.incrementarAngRotX(-10);
		break;

	case 'y': // rotar Y positivo 
		interfaz.escena.incrementarAngRotY(10);
		break;

	case 'Y': // rotar y negativo 
		interfaz.escena.incrementarAngRotY(-10);
		break;

	case 'z': // rotar z positivo 
		interfaz.escena.incrementarAngRotZ(10);
		break;

	case 'Z': // rotar Z negativo 
		interfaz.escena.incrementarAngRotZ(-10);
		break;

	case 'b': // grLib1: subimos sombrero
		if (interfaz.escena.getGrLib1() < 1) {
			interfaz.escena.incrementarGrLib1(0.1);
		}

		break;

	case 'B': // grLib1: bajamos sombrero
		if (interfaz.escena.getGrLib1() > 0) {
			interfaz.escena.incrementarGrLib1(-0.1);
		}

		break;

	case 'q': // grLib2: bajamos brazo izquierdo
		if (interfaz.escena.getGrLib2() < 90 && interfaz.escena.getGrLib3() == 0) {
			interfaz.escena.incrementarGrLib2(10);
		}

		break;

	case 'Q': // grLib2: subimos brazo izquierdo
		if (interfaz.escena.getGrLib2() > -90 && interfaz.escena.getGrLib3() == 0) {
			interfaz.escena.incrementarGrLib2(-10);
		}

		break;

	case 'u': // grLib3: movemos brazo izquierdo hacia fuera
		if (interfaz.escena.getGrLib3() < 90 && interfaz.escena.getGrLib2() == 0) {
			interfaz.escena.incrementarGrLib3(10);
		}

		break;

	case 'U': // grLib3: movemos brazo izquierdo hacia dentro
		if (interfaz.escena.getGrLib3() > 0 && interfaz.escena.getGrLib2() == 0) {
			interfaz.escena.incrementarGrLib3(-10);
		}

		break;

	case 'e': // grLib4: bajamos brazo derecho
		if (interfaz.escena.getGrLib4() < 90 && interfaz.escena.getGrLib5() == 0) {
			interfaz.escena.incrementarGrLib4(10);
		}

		break;

	case 'E': // grLib4: subimos brazo derecho
		if (interfaz.escena.getGrLib4() > -90 && interfaz.escena.getGrLib5() == 0) {
			interfaz.escena.incrementarGrLib4(-10);
		}

		break;

	case '<': // grLib5: movemos brazo derecho hacia dentro
		if (interfaz.escena.getGrLib5() > -90 && interfaz.escena.getGrLib4() == 0) {
			interfaz.escena.incrementarGrLib5(-10);
		}

		break;

	case '>': // grLib5: movemos brazo derecho hacia fuera
		if (interfaz.escena.getGrLib5() < 0 && interfaz.escena.getGrLib4() == 0) {
			interfaz.escena.incrementarGrLib5(10);
		}

		break;

	case 'h': // grLib6: movemos pierna izquierda para atras
		if (interfaz.escena.getGrLib6() > -90 && interfaz.escena.getGrLib7() == 0) {
			interfaz.escena.incrementarGrLib6(-10);
		}

		break;

	case 'H': // grLib6: movemos pierna izquierda para adelante
		if (interfaz.escena.getGrLib6() < 60 && interfaz.escena.getGrLib7() == 0) {
			interfaz.escena.incrementarGrLib6(10);
		}

		break;

	case 'g': // grLib7: movemos pierna derecha para atras
		if (interfaz.escena.getGrLib7() > -90 && interfaz.escena.getGrLib6() == 0) {
			interfaz.escena.incrementarGrLib7(-10);
		}

		break;

	case 'G': // grLib7: movemos pierna derecha para adelante
		if (interfaz.escena.getGrLib7() < 60 && interfaz.escena.getGrLib6() == 0) {
			interfaz.escena.incrementarGrLib7(10);
		}

		break;

	case 'm': // grLib8: agachamos muneco
		if (interfaz.escena.getGrLib8() < 0.3) {
			interfaz.escena.incrementarGrLib8(0.1);
		}

		break;

	case 'M': // grLib8: levantamos muneco
		if (interfaz.escena.getGrLib8() > 0.1) {
			interfaz.escena.incrementarGrLib8(-0.1);
		}

		break;

	case 'P': // activa/desactiva la visualizacion de los ejes
		interfaz.escena.set_ejes(interfaz.escena.get_ejes() ? false : true);

		break;

	case 'W': //movemos camara para arriba(eje - z)
		interfaz.camara.mover(0, 0, 0.1);

		break;

	case 'S': //movemos camara para abajo (eje z)
		interfaz.camara.mover(0, 0, -0.1);

		break;

	case 'A': //movemos camara para izquierda (eje -x)
		interfaz.camara.mover(0.1, 0, 0);

		break;

	case 'D': //movemos camara para derecha (eje x)
		interfaz.camara.mover(-0.1, 0, 0);

		break;

	case 'w': //movemos camara (arcball (cabeceo)) para arriba
		interfaz.camara.orbital(0, 5);
		interfaz.camara.aplicar();

		break;

	case 's': //movemos camara (arcball (cabeceo)) para abajo
		interfaz.camara.orbital(0, -5);
		interfaz.camara.aplicar();

		break;

	case 'a': //movemos camara (orbital) para izquierda
		interfaz.camara.orbital(-5, 0);
		interfaz.camara.aplicar();

		break;

	case 'd': //movemos camara (orbital) para derecho
		interfaz.camara.orbital(5, 0);
		interfaz.camara.aplicar();

		break;

	case 'r': //movemos camara para arriba (eje y)
		interfaz.camara.mover(0, 0.1, 0);

		break;

	case 'R': //movemos camara para abajo (eje -y)
		interfaz.camara.mover(0, -0.1, 0);

		break;

	case 'i': //movemos foco morado de la escena para arriba (eje -z)
		if (interfaz.escena.get_z() > -5) {
			interfaz.escena.incrementarZ(-0.1);
		}

		break;

	case 'k': //movemos foco morado de la escena para abajo (eje z)
		if (interfaz.escena.get_z() < 3.2) {
			interfaz.escena.incrementarZ(0.1);
		}

		break;

	case 'j': //movemos foco morado de la escena para izquierda (eje -x)
		if (interfaz.escena.get_x() > -5.7) {
			interfaz.escena.incrementarX(-0.1);
		}

		break;

	case 'l': //movemos foco morado de la escena para derecha (eje x)
		if (interfaz.escena.get_x() < 2.2) {
			interfaz.escena.incrementarX(0.1);
		}

		break;

	case 'o': // aumentar en 0.1 la componente R del coeficiente difuso del material
		if (interfaz.escena.get_coefDifuso() < 2) {
			interfaz.escena.incrementarcoefDifuso(0.1);
		}

		break;

	case 'O': // disminuir en 0.1 la componente R del coeficiente difuso del material
		if (interfaz.escena.get_coefDifuso() > 0) {
			interfaz.escena.incrementarcoefDifuso(-0.1);
		}

		break;

	case 'f': // aumentar en 0.1 la componente R del coeficiente especular del material
		interfaz.escena.incrementarcoefEspecular(0.1);

		break;

	case 'F': // disminuir en 0.1 la componente R del coeficiente especular del material
		if (interfaz.escena.get_coefEspecular() > 0) {
			interfaz.escena.incrementarcoefEspecular(-0.1);
		}

		break;

	case 'T': // Aumentar en 10 el exponente de Phong del material
		if (interfaz.escena.get_phong() < -90) {
			interfaz.escena.incrementarPhong(5);
		}

		break;

	case 't': // disminuir en 10 el exponente de Phong del material
		if (interfaz.escena.get_phong() > -150) {
			interfaz.escena.incrementarPhong(-5);
		}

		break;

	case 'C': // Aumentar en 10 el exponente de Phong del material
		if (interfaz.escena.get_activarMaterial() == false) {
			interfaz.escena.set_activarMaterial(true);
		}
		else {
			interfaz.escena.set_activarMaterial(false);
		}

		break;

	case 'V': // Aumentar en 10 el exponente de Phong del material
		if (interfaz.escena.get_autorizarMaterial() == false) {
			interfaz.escena.set_autorizarMaterial(true);
		}

		break;

	case 'J': // Aumentar en 10 el exponente de Phong del material
		interfaz.escena.cambiarTipoSombreado();
		interfaz.escena.cargarExternos();

		break;

	case '1': // Disparar estructura del terreno1 (si hay)
		if (interfaz.escena.getTerrenos()[0]->getTipo() != NINGUNA) {
			if (interfaz.escena.getTerrenos()[0]->getTipo() == CAÑON) {
				interfaz.tipoA = BALA;
			}
			else if (interfaz.escena.getTerrenos()[0]->getTipo() == TORRE) {
				interfaz.tipoA = FLECHA;
			}

			interfaz.disparoRelizado = true;
			interfaz.estructuraDisparo = 0;
		}
		break;

	case '2': // Disparar estructura del terreno2 (si hay)
		if (interfaz.escena.getTerrenos()[1]->getTipo() != NINGUNA) {
			if (interfaz.escena.getTerrenos()[1]->getTipo() == CAÑON) {
				interfaz.tipoA = BALA;
			}
			else if (interfaz.escena.getTerrenos()[1]->getTipo() == TORRE) {
				interfaz.tipoA = FLECHA;
			}

			interfaz.disparoRelizado = true;
			interfaz.estructuraDisparo = 1;
		}
		break;

	case '3': // Disparar estructura del terreno3 (si hay)
		if (interfaz.escena.getTerrenos()[2]->getTipo() != NINGUNA) {
			if (interfaz.escena.getTerrenos()[2]->getTipo() == CAÑON) {
				interfaz.tipoA = BALA;
			}
			else if (interfaz.escena.getTerrenos()[2]->getTipo() == TORRE) {
				interfaz.tipoA = FLECHA;
			}

			interfaz.disparoRelizado = true;
			interfaz.estructuraDisparo = 2;
		}
		break;

	case '4': // Disparar estructura del terreno4 (si hay)
		if (interfaz.escena.getTerrenos()[3]->getTipo() != NINGUNA) {
			if (interfaz.escena.getTerrenos()[3]->getTipo() == CAÑON) {
				interfaz.tipoA = BALA;
			}
			else if (interfaz.escena.getTerrenos()[3]->getTipo() == TORRE) {
				interfaz.tipoA = FLECHA;
			}

			interfaz.disparoRelizado = true;
			interfaz.estructuraDisparo = 3;
		}
		break;

	case '5': // Disparar estructura del terreno5 (si hay)
		if (interfaz.escena.getTerrenos()[4]->getTipo() != NINGUNA) {
			if (interfaz.escena.getTerrenos()[4]->getTipo() == CAÑON) {
				interfaz.tipoA = BALA;
			}
			else if (interfaz.escena.getTerrenos()[4]->getTipo() == TORRE) {
				interfaz.tipoA = FLECHA;
			}

			interfaz.disparoRelizado = true;
			interfaz.estructuraDisparo = 4;
		}
		break;

	case '6': // Disparar estructura del terreno6 (si hay)
		if (interfaz.escena.getTerrenos()[5]->getTipo() != NINGUNA) {
			if (interfaz.escena.getTerrenos()[5]->getTipo() == CAÑON) {
				interfaz.tipoA = BALA;
			}
			else if (interfaz.escena.getTerrenos()[5]->getTipo() == TORRE) {
				interfaz.tipoA = FLECHA;
			}

			interfaz.disparoRelizado = true;
			interfaz.estructuraDisparo = 5;
		}
		break;

	case 27: // tecla de escape para SALIR
		exit(1);

		break;
	}

	glutPostRedisplay(); // renueva el contenido de la ventana de vision y redibuja la escena
}

void igvInterfaz::set_glutReshapeFunc(int w, int h) {
	// dimensiona el viewport al nuevo ancho y alto de la ventana
	// guardamos valores nuevos de la ventana de visualizacion
	interfaz.set_ancho_ventana(w);
	interfaz.set_alto_ventana(h);

	// establece los parametros de la camara y de la proyeccion
	interfaz.camara.aplicar();
}

void igvInterfaz::set_glutDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borra la ventana y el z-buffer

	// se establece el viewport
	glViewport(0, 0, interfaz.get_ancho_ventana(), interfaz.get_alto_ventana());

	if (interfaz.modo == seleccionar) {
		glDisable(GL_LIGHTING); // desactiva la iluminacion de la escena
		glDisable(GL_DITHER);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);

		for (int i = 0; i < interfaz.escena.getTerrenos().size(); i++) {
			interfaz.escena.getTerrenos()[i]->setSeleccionados(false);
		}

		// Apartado A: aplica la camara
		interfaz.camara.aplicar();

		// Apartado A: visualiza los BV cada uno de un color
		interfaz.escena.visualizarVB();

		// Apartado A: Obtener el color del pixel seleccionado
		GLubyte pixelColor[3];
		glReadPixels(interfaz.cursorX, interfaz.get_alto_ventana() - interfaz.cursorY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixelColor);

		// Apartado A: Comprobar el color del objeto que hay en el cursor mirando en la tabla de colores y asigna otro color al objeto seleccionado
		if (!interfaz.tienda) {
			interfaz.terrenoSeleccionado = interfaz.buscarTerreno(pixelColor);
			if (interfaz.terrenoSeleccionado != -1) { //objeto ha sido seleccionado

				interfaz.escena.getTerrenos()[interfaz.terrenoSeleccionado]->setSeleccionados(true);

				interfaz.tienda = true;

				glutSwapBuffers(); // se utiliza, en vez de glFlush(), para evitar el parpadeo
			}

			interfaz.torreSeleccionada = interfaz.buscarTorre(pixelColor);
		}

		interfaz.modo = visualizar;

		glEnable(GL_LIGHTING);
	}
	else {
		glutIdleFunc(interfaz.animacion);

		interfaz.camara.aplicar();
		interfaz.escena.visualizar(); //visualiza la escena

		// refresca la ventana
		glutSwapBuffers(); // se utiliza, en vez de glFlush(), para evitar el parpadeo
	}
}

void igvInterfaz::set_glutMouseFunc(GLint boton, GLint estado, GLint x, GLint y) {
	if (boton == GLUT_LEFT_BUTTON) {
		if (estado == GLUT_DOWN) {
			interfaz.boton_retenido = true;
			interfaz.modo = seleccionar;

			interfaz.cursorX = x;
			interfaz.cursorY = y;
		}
		else {
			interfaz.boton_retenido = false;
			interfaz.torreSeleccionada = -1;
		}

		set_glutDisplayFunc();
	}
}

void igvInterfaz::set_glutMotionFunc(GLint x, GLint y) {
	// Apartado B: si el boton esta retenido y hay algun objeto seleccionado,
	// comprobar el objeto seleccionado y la posicion del ratan y rotar
	// el objeto seleccionado utilziando el desplazamiento entre la posicion 
	// inicial y final del ratan
	int desplazamiento;

	if (interfaz.boton_retenido && interfaz.torreSeleccionada != -1) {
		desplazamiento = interfaz.cursorY - y;
		interfaz.escena.getTerrenos()[interfaz.torreSeleccionada]->getDefensaTorre()->setAlturaTorre(desplazamiento/5);
	}

	// Apartado B: guardar la nueva posicion del raton 
	interfaz.cursorX = x;
	interfaz.cursorY = y;

	// Apartado B: renovar el contenido de la ventana de vision 
	glutPostRedisplay();
}

void igvInterfaz::animacion() {
	glutIdleFunc(NULL);

	if (interfaz.tipoA == BALA) {
		if (interfaz.escena.get_tDisparoInicio() == 0) 
			interfaz.escena.set_tDisparoInicio(clock());

		if(interfaz.disparoRelizado)
			interfaz.reproducirSonido(DISPAROCAÑON);

		interfaz.balaCañonIdleFunc();
	}

	if (interfaz.tipoA == FLECHA) {
		if (interfaz.escena.get_tDisparoInicio() == 0)
			interfaz.escena.set_tDisparoInicio(clock());

		if (interfaz.disparoRelizado)
			interfaz.reproducirSonido(DISPAROTORRE);

		interfaz.flechaTorreIdleFunc();
	}

	if (interfaz.tipoA == ENEMIGOS) {
		interfaz.enemigoIdleFunc();
	}

	if (interfaz.tipoA == REYES) {
		interfaz.reyIdleFunc();
	}
}

void igvInterfaz::reproducirSonido(tipoSonido sonido) {
	if (sonido == DERROTA) {
		PlaySound(TEXT("Derrota.wav"), NULL, SND_ASYNC | SND_FILENAME);

	} else if(sonido == DISPAROCAÑON) {
		PlaySound(TEXT("Cañon.wav"), NULL, SND_ASYNC | SND_FILENAME);

	} else if (sonido == DISPAROTORRE) {
		PlaySound(TEXT("Flecha.wav"), NULL, SND_ASYNC | SND_FILENAME);

	} else if (sonido == MUERTE) {
		PlaySound(TEXT("Muerte.wav"), NULL, SND_ASYNC | SND_FILENAME);

	} else if (sonido == VICTORIA) {
		 PlaySound(TEXT("Victoria.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
}

void igvInterfaz::inicializa_callbacks() {
	glutKeyboardFunc(set_glutKeyboardFunc);
	glutReshapeFunc(set_glutReshapeFunc);
	glutDisplayFunc(set_glutDisplayFunc);

	glutMouseFunc(set_glutMouseFunc);
	glutMotionFunc(set_glutMotionFunc);
}

bool igvInterfaz::gradosLibActivos() {
	if (interfaz.escena.getGrLib2() != 0 || interfaz.escena.getGrLib3() != 0|| interfaz.escena.getGrLib4() != 0 || interfaz.escena.getGrLib5() != 0) {
		return true;
	}

	return false;
}

int igvInterfaz::buscarTerreno(GLubyte pixel[]) {
	//GLfloat colTerreno[] = { 1, 0.1 ,0 };
	for (int i = 0; i < escena.getTerrenos().size(); i++) {
		GLubyte colorByte[3];

		colorByte[0] = (GLubyte)(escena.getTerrenos()[i]->getColor()[0] * 255);
		colorByte[1] = (GLubyte)(escena.getTerrenos()[i]->getColor()[1] * 255);
		colorByte[2] = (GLubyte)(escena.getTerrenos()[i]->getColor()[2] * 255);

		if ((pixel[0] == colorByte[0]) &&
		(pixel[1] == colorByte[1]) &&
		(pixel[2] == colorByte[2])) {
			return i;
		}
	}

	return -1;
}

int igvInterfaz::buscarTorre(GLubyte pixel[]) {
	for (int i = 0; i < escena.getTerrenos().size(); i++) {
		if (escena.getTerrenos()[i]->getTipo() == TORRE) {
			GLubyte colorByte[3];

			colorByte[0] = (GLubyte)(escena.getTerrenos()[i]->getDefensaTorre()->getColor()[0] * 255);
			colorByte[1] = (GLubyte)(escena.getTerrenos()[i]->getDefensaTorre()->getColor()[1] * 255);
			colorByte[2] = (GLubyte)(escena.getTerrenos()[i]->getDefensaTorre()->getColor()[2] * 255);

			if ((pixel[0] == colorByte[0]) && (pixel[1] == colorByte[1]) && (pixel[2] == colorByte[2])) {
				return i;
			}
		}
	}

	return -1;
}

void igvInterfaz::create_menu() {
	if (tienda) {
		int menu_id = glutCreateMenu(menuHandle);

		TerrenoEstructuras* myTerreno = interfaz.escena.getTerrenos()[interfaz.getTerrenoSeleccionado()];
		Torre* myTorre = new Torre();
		Cañon* myCañon = new Cañon();

		bool hayTorre = false;
		bool hayCañon = false;

		if (myTerreno->getDefensaCañon() != nullptr || myTerreno->getDefensaTorre() != nullptr) {
			if (myTerreno->getTipo() == CAÑON) {
				hayCañon = true;
				myCañon = myTerreno->getDefensaCañon();
			}
			else if (myTerreno->getTipo() == TORRE) {
				hayTorre = true;
				myTorre = myTerreno->getDefensaTorre();
			}
		}

		string myLinea;

		myLinea = "Terreno: " + to_string(interfaz.getTerrenoSeleccionado());
		glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), NADA);

		myLinea = "Dinero actual: " + to_string(interfaz.getDinero());
		glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), NADA);

		myLinea = "";
		glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), NADA);

		if (hayTorre == true) {
			myLinea = "------ Torre ------";
			glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), NADA);

			if (myTorre->getNivel() < 3) {
				myLinea = "Mejorar: " + to_string(myTorre->getPrecioMejora());
				glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), MEJORARTORRE);
			}

			myLinea = "Vender: " + to_string(myTorre->getPrecioVenta());
			glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), VENDERTORRE);
		}
		else {
			if (hayCañon == true) {
				myLinea = "------ Cañon ------";
				glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), NADA);

				if (myCañon->getNivel() < 3) {
					myLinea = "Mejorar: " + to_string(myCañon->getPrecioMejora());
					glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), MEJORARCAÑON);
				}

				myLinea = "Vender: " + to_string(myCañon->getPrecioVenta());
				glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), VENDERCAÑON);
			}
			else {
				myLinea = "------ Cañon ------";
				glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), NADA);

				myLinea = "Construir: " + to_string(myCañon->getPrecioConstruccion());
				glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), CONSTRUIRCAÑON);

				myLinea = "";
				glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), NADA);

				myLinea = "------ Torre ------";
				glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), NADA);

				myLinea = "Construir: " + to_string(myTorre->getPrecioConstruccion());
				glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), CONSTRUIRTORRE);

			}
		}

		myLinea = "";
		glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), NADA);

		myLinea = "Salir";
		glutAddMenuEntry(const_cast<char*>(myLinea.c_str()), SALIR);

		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}
}

void igvInterfaz::menuHandle(int value) {
	interfaz.menuSelection = value;
	glutPostRedisplay(); // renew the content of the window
}

void igvInterfaz::reyIdleFunc() {
	if (interfaz.escena.get_sentido() == 0) { //para abajo (principio)
		interfaz.escena.incrementarGrLib8(0.3 / 10);
		interfaz.escena.incrementarGrLib2(90 / 10);
		interfaz.escena.incrementarGrLib4(90 / 10);

		if (interfaz.escena.getGrLib8() >= 0.3 && interfaz.escena.getGrLib2() >= 90 && interfaz.escena.getGrLib4() >= 90) {
			interfaz.escena.set_sentido(2);
		}

		glutPostRedisplay();

		Sleep(70);
	}

	if (interfaz.escena.get_sentido() == 1) { //para abajo
		interfaz.escena.incrementarGrLib8(0.3 / 10);
		interfaz.escena.incrementarGrLib2(180 / 10);
		interfaz.escena.incrementarGrLib4(180 / 10);

		if (interfaz.escena.getGrLib8() >= 0.3 && interfaz.escena.getGrLib2() >= 90 && interfaz.escena.getGrLib4() >= 90) {
			interfaz.escena.set_sentido(2);
		}

		glutPostRedisplay();

		Sleep(70);
	}

	if (interfaz.escena.get_sentido() == 2) { //para arriba
		interfaz.escena.incrementarGrLib8(-0.3 / 10);
		interfaz.escena.incrementarGrLib2(-180 / 10);
		interfaz.escena.incrementarGrLib4(-180 / 10);

		if (interfaz.escena.getGrLib8() <= 0 && interfaz.escena.getGrLib2() <= 0 && interfaz.escena.getGrLib4() <= 0) {
			interfaz.escena.set_sentido(3);
		}

		glutPostRedisplay();

		Sleep(40);
	}

	if (interfaz.escena.get_sentido() == 3) {
		interfaz.escena.incrementarGrLib1(0.1);

		if (interfaz.escena.getGrLib1() >= 0.9) {
			interfaz.escena.set_sentido(4);
		}

		glutPostRedisplay();

		Sleep(40);
	}

	if (interfaz.escena.get_sentido() == 4) {
		interfaz.escena.incrementarGrLib1(-0.1);

		if (interfaz.escena.getGrLib1() <= 0.1) {
			interfaz.escena.set_sentido(1);
		}

		glutPostRedisplay();

		Sleep(90);
	}
}

void igvInterfaz::enemigoIdleFunc() {
	for (int i = 0; i < interfaz.escena.getEnemigos().size(); i++) {
		if (interfaz.escena.getEnemigos().at(i)->getX() <= 4) {
			interfaz.escena.getEnemigos().at(i)->mover();
		}
	}

	glutPostRedisplay();

	Sleep(10);
}

void igvInterfaz::balaCañonIdleFunc() {
	if (interfaz.estructuraDisparo != -1) {
		if (interfaz.escena.getTerrenos()[interfaz.estructuraDisparo]->getTipo() == CAÑON) {
			Cañon* c = interfaz.escena.getTerrenos()[interfaz.estructuraDisparo]->getDefensaCañon();
			c->set_lanzamietoBala(0.5);
			interfaz.disparoRelizado = false;
		}
	}

	glutPostRedisplay();

	Sleep(10);
}

void igvInterfaz::flechaTorreIdleFunc() {
	if (interfaz.estructuraDisparo != -1) {
		if (interfaz.escena.getTerrenos()[interfaz.estructuraDisparo]->getTipo() == TORRE) {
			Torre* t = interfaz.escena.getTerrenos()[interfaz.estructuraDisparo]->getDefensaTorre();
			t->set_dispararFlecha(0.5);
			interfaz.disparoRelizado = false;
		}
	}

	glutPostRedisplay();

	Sleep(10);
}