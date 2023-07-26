#include <cstdlib>
#include <stdio.h>

#include "igvEscena3D.h"

#include "igvInterfaz.h"

extern igvInterfaz interfaz; // los callbacks deben ser estaticos y se requiere este objeto para acceder desde
// ellos a las variables de la clase

GLfloat negro[] = { 0,0,0 };
GLfloat red[] = { 1,0,0 };
GLfloat green[] = { 0,1,0 };
GLfloat blue[] = { 0,0,1 };
GLfloat morado[] = { 0.7255,0.3137,0.8275 };

deque<TerrenoEstructuras> aux;

void pintarVariosQuads(float ini_x, float ini_z, 
					   float tam_x, float tam_z, 
					   float div_x, float div_z) 
{
	float ancho = tam_x / div_x;
	float alto = tam_z / div_z;

	for (int i = 0; i < div_x; i++) {
		for (int j = 0; j < div_z; j++) {
			float x1 = ini_x + (i * ancho);
			float z1 = ini_z + (j * alto);
			float x2 = x1 + ancho;
			float z2 = z1 + alto;

			glNormal3f(0, 1, 0);

			glBegin(GL_QUADS);
			glTexCoord2f((i / div_x), (j / div_z));

			glVertex3f(x1, 0.0, z1);
			glVertex3f(x1, 0.0, z2);
			glVertex3f(x2, 0.0, z2);
			glVertex3f(x2, 0.0, z1);
			glEnd();
		}
	}
}

// Metodos constructores 
igvEscena3D::igvEscena3D() { 
	ejes = false; 
	sentido = 0;
	numEnemigos = 6;
	numTerrenos = 6;
	consola = false;

	// Apartado C: inicializar los atributos para el control de los grados de libertad del modelo 
	grLib1 = 0; //grado de libertad 1: mueve el sombrero de arriba a abajo pulsando 's' (arriba) o 'S' (abajo)

	grLib2 = 0; //grado de libertad 2: mueve el brazo1 (izquierda) de arriba a abajo pulsando '2' (arriba) o '9' (abajo)
	grLib3 = 0; //grado de libertad 3: mueve el brazo1 (izquierda) de izquierda a derecha pulsando 3 (izquierda) o 8 (derecha)

	grLib4 = 0; //grado de libertad 4: mueve el brazo2 (derecha) de arriba a abajo pulsando '4' (arriba) o '7' (abajo)
	grLib5 = 0; //grado de libertad 5: mueve el brazo2 (derecha) de izquierda a derecha pulsando '5' (izquierda) o '6' (derecha)

	grLib6 = 0; //grado de libertad 6: mueve femur1 pulsando 'f' o 'F'
	grLib7 = 0; //grado de libertad 7: mueve femur2 pulsando 'g' o 'G'

	grLib8 = 0; //grado de libertad 8: el mu�eco se agacha pulsando 'm' y se estira pulsando 'M'

	float ejez = -0.75 + static_cast<float>(rand()) * static_cast<float>(0.75 - (-0.75)) / RAND_MAX; //valor aleatorio entre 0.75 y -0.75

	float r = 100, g = 100, b = 100;

	phong = -90;
	coefDifuso = 0;
	coefEspecular = 0;

	colisionado = false;
	principio = true;
	
	tInicio = 0;
	tFin = 0;
	tDisparoInicio = 0;
	tDisparoFin = 0;

	activarMaterial = false;
	autorizarMaterial = false;
	tipoSombreado = true;

	x = 0.2;
	radioFoco = 6;
	z = 0.2;

	int nLuzTerreno = 0;

	for (int i = 0; i < numTerrenos; i++) {
		igvPunto3D myPunto;
		switch (i) {
			case 0:
				myPunto.set(-2.25, -0.1, -1.25);
				nLuzTerreno = 0x4001;
				break;
				
			case 1:
				myPunto.set(-0.75, -0.1, -1.25);
				nLuzTerreno = 0x4002;
				break;

			case 2:
				myPunto.set(0.75, -0.1, -1.25);
				nLuzTerreno = 0x4003;
				break;

			case 3:
				myPunto.set(-1.5, -0.1, 1.25);
				nLuzTerreno = 0x4004;
				break;

			case 4:
				myPunto.set(0, -0.1, 1.25);
				nLuzTerreno = 0x4005;
				break;

			case 5:
				myPunto.set(1.5, -0.1, 1.25);
				nLuzTerreno = 0x4006;
				break;
		}

		TerrenoEstructuras* te = new TerrenoEstructuras(r / 255, g / 255, b / 255, myPunto);
		te->setNumeroLuz(nLuzTerreno);

		terrenos.push_back(te);

		r += 1;
		g += 1;
		b += 1;
	}

	terrenosOriginales = terrenos;
}

igvEscena3D::~igvEscena3D() {
	for (int i = 0; i < numTerrenos; i++) {
		delete terrenos[i];
		delete terrenosOriginales[i];
	}
}

// Metodos publicos 
void pintar_ejes(void) {
	GLfloat rojo[] = { 1,0,0,1.0 };
	GLfloat verde[] = { 0,1,0,1.0 };
	GLfloat azul[] = { 0,0,1,1.0 };

	glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
	glBegin(GL_LINES);
	glVertex3f(1000, 0, 0);
	glVertex3f(-1000, 0, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_EMISSION, verde);
	glBegin(GL_LINES);
	glVertex3f(0, 1000, 0);
	glVertex3f(0, -1000, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_EMISSION, azul);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 1000);
	glVertex3f(0, 0, -1000);
	glEnd();
}

void pintar_tubo() {
	GLUquadricObj* tubo;
	GLfloat color_tubo[] = { 0,0,0.5 };

	glMaterialfv(GL_FRONT, GL_EMISSION, color_tubo);

	tubo = gluNewQuadric();
	gluQuadricDrawStyle(tubo, GLU_FILL);

	glPushMatrix();
	glTranslatef(0, 0, -0.5);
	gluCylinder(tubo, 0.25, 0.25, 1, 20, 20);
	glPopMatrix();

	gluDeleteQuadric(tubo);
}

void crearFoco(int numTerreno, int _nivel, int numLuz) {
	igvFuenteLuz foco;

	float _x, _z;

	switch (numTerreno) {
	case 0:
		_x = -2.65;
		_z = -2.5;
		break;

	case 1:
		_x = -0.9;
		_z = -2.5;
		break;

	case 2:
		_x = 0.9;
		_z = -2.5;
		break;

	case 3:
		_x = -1.8;
		_z = 2.5;
		break;

	case 4:
		_x = 0;
		_z = 2.5;
		break;

	case 5:
		_x = 1.8;
		_z = 2.5;
		break;
	}

	igvPunto3D posFoco(_x, 6, _z);
	igvPunto3D dFoco(0, -1, 0);

	foco.setPosicion(posFoco);
	foco.setDireccionFoco(dFoco);

	if (_nivel == 1) {
		igvColor af(0, 0, 1, 1.0);
		igvColor df(0, 0, 1, 1.0);
		igvColor ef(0, 0, 1, 1.0);

		foco.set(af, df, ef);
	}

	if (_nivel == 2) {
		igvColor af(0.9765, 0.4745, 0.1686, 1.0);
		igvColor df(0.9765, 0.4745, 0.1686, 1.0);
		igvColor ef(0.9765, 0.4745, 0.1686, 1.0);

		foco.set(af, df, ef);
	}

	if (_nivel == 3) {
		igvColor af(1, 0.0, 0, 1.0);
		igvColor df(1, 0.0, 0, 1.0);
		igvColor ef(1, 0.0, 0, 1.0);

		foco.set(af, df, ef);
	}

	foco.setAtenuacion(0.1, 0.1, 0.1);

	foco.setId(numLuz);

	foco.encender();
	foco.aplicar(true);
}

void igvEscena3D::cargarExternos() {
	glPushMatrix();

	if (tipoSombreado == true) {
		glShadeModel(GL_SMOOTH);
	}
	else {
		glShadeModel(GL_FLAT);
	}

	GLfloat luz0[] = { 10,8,9,1 }; // luz puntual
	glLightfv(GL_LIGHT0, GL_POSITION, luz0);
	glEnable(GL_LIGHT0);

	//-------------------------------------------------------------

	focoGeneral.setId(GL_LIGHT7);

	igvPunto3D posFoco(2 + x, radioFoco, 1 + z);
	igvPunto3D dFoco(0, -1, 0);

	focoGeneral.setPosicion(posFoco);
	focoGeneral.setDireccionFoco(dFoco);

	igvColor af(1, 0, 1, 1.0);
	igvColor df(1, 0, 1, 1.0);
	igvColor ef(1, 0, 1, 1.0);

	focoGeneral.set(af, df, ef);

	focoGeneral.setAtenuacion(0.1, 0.1, 0.1);

	focoGeneral.encender();
	focoGeneral.aplicar(true);

	//-------------------------------------------------------------

	if (autorizarMaterial == true) {
		igvColor coefA(0.15, 0.0, 0.0, 1);
		igvColor coefD(0.5 + coefDifuso, 0.0, 0.0, 1);
		igvColor coefE(0.5 + coefEspecular, 0, 0, 1);

		double coefP = 120 + phong;

		igvMaterial material;
		material.set(coefA, coefD, coefE, coefP);

		material.aplicar(activarMaterial);
	}

	//-------------------------------------------------------------

	glMaterialfv(GL_FRONT, GL_EMISSION, green);

	//Textura1
	char name[] = "cesped.png";
	igvTextura textura(name);

	textura.aplicar();

	pintarVariosQuads(-4, -4, 8, 3, 15, 15); /* (float ini_x, float ini_z, float tam_x, float tam_z, float div_x, float div_z)*/

	//-------------------------------------------------------------

	//Textura2
	char name2[] = "cesped.png";
	igvTextura textura2(name2);

	textura2.aplicar();

	pintarVariosQuads(-4, 1, 8, 3, 15, 15); /* (float ini_x, float ini_z, float tam_x, float tam_z, float div_x, float div_z)*/

	glPopMatrix();
}

void igvEscena3D::crearEnemigo() {
	int tipo = rand() % 2; //valor aleatorio entre 1 y 0

	tipoEnemigo tipoE = tipoEnemigo::TERRESTRE;
	float ejey = 0.15;

	if (tipo == 0) {
		tipoE = tipoEnemigo::TERRESTRE;
		ejey = 0.15;
	}

	if (tipo == 1) {
		tipoE = tipoEnemigo::VOLADOR;

		do {
			ejey = 0.15 + static_cast<float>(rand()) * static_cast<float>(3 - (0.15)) / RAND_MAX; //valor aleatorio entre 1.5 y 0.15
		} while (1.25 <= ejey && ejey <= 2.2);
	}

	anadirEnemigo(interfaz.getNumOleada(), tipoE, -4, ejey, 0);
	numEnemigos--;
}

double time1 = 0, time2 = 0;

void igvEscena3D::visualizar(void) {
	if (interfaz.getTienda() == false) {
		glRotated(this->ang_rot_x, 1, 0, 0);
		glRotated(this->ang_rot_y, 0, 1, 0);
		glRotated(this->ang_rot_z, 0, 0, 1);

		cargarExternos();

		for (int i = 0; i < numTerrenos; i++) {
			if (terrenos[i]->getTipo() != NINGUNA) {
				if (terrenos[i]->getTipo() == CAÑON) {
					Cañon* c = terrenos[i]->getDefensaCañon();
					c->crearCañon();

					crearFoco(i, terrenos[i]->getDefensaCañon()->getNivel(), terrenos[i]->getNumeroLuz());
				}
				else if (terrenos[i]->getTipo() == TORRE) {
					Torre* t = terrenos[i]->getDefensaTorre();
					t->crearTorre();

					crearFoco(i, terrenos[i]->getDefensaTorre()->getNivel(), terrenos[i]->getNumeroLuz());
				}
			} else {
				glDisable(terrenos[i]->getNumeroLuz());
			}
		}

		terreno();
		castillo();
		crearCorazones();
		
		if (tDisparoInicio == 0) {
			if (tInicio != 0 && tFin == 0) {
				tFin = clock();
			}
		}
		else {
			if (tDisparoFin != 0) {
				tInicio = tDisparoInicio;
				tFin = tDisparoFin;

				tDisparoInicio = 0;
				tDisparoFin = 0;
				interfaz.setEstructuraDisparo(-1);
			}
		}

		tEjecucion = clock();
		time1 = (double(tEjecucion - interfaz.getT0()) / CLOCKS_PER_SEC);

		if (tInicio != 0 && tFin != 0) {
			time2 = (double(tFin - tInicio) / CLOCKS_PER_SEC);
			time1 = time1 - time2;
		}

		if (this->numEnemigos > 0 && time1 > 4 && tDisparoInicio == 0) {
			crearEnemigo();

			interfaz.setT0(tEjecucion);		
			
			tInicio = 0;
			tFin = 0;

			time2 = 0;
		}
		else {
			if (principio == true) {
				if (time1 > 4) {
					crearEnemigo();
					principio = false;
					interfaz.setT0(tEjecucion);
				}
			}
			else {
				if (enemigos.size() == 0 && time1 > 4) { //nos esperamos 3 segs entre ronda y ronda
					interfaz.incrementarOleada(1);
					numEnemigos = 6;
					interfaz.quitarVida(-1);

					interfaz.setT0(tEjecucion);
				}
			}
		}

		if (interfaz.getNumVidas() != 0) {
			if (interfaz.getNumOleada() < 4) {
				for (int i = 0; i < enemigos.size(); i++) {
					if (enemigos.at(i)->getX() < 4) {
						enemigos.at(i)->crearEnemigo();
					}
					else {
						enemigos.erase(enemigos.begin() + (i)); //eliminamos elemento i (begin = 1 + (i-1) = i)
						interfaz.quitarVida(1);
					}
				}
			}
			else {
				glutIdleFunc(NULL);
				interfaz.setTipoA(REYES);
				glutIdleFunc(interfaz.animacion);
				if (!finalizar) {
					interfaz.reproducirSonido(VICTORIA);
					finalizar = true;
				}

			}
		}
		else {
			if (!finalizar) {
				interfaz.reproducirSonido(DERROTA);
				finalizar = true;
			}
		}

		// crear el modelo
		glPushMatrix(); // guarda la matriz de modelado

		// se pintan los ejes
		if (ejes) pintar_ejes();

		glPopMatrix(); // restaura la matriz de modelado
	
		glPushMatrix();
			visualizarVB();
		glPopMatrix();

		controlColisiones();
	}
	else {
		for (int i = 1; i < 8; i++) {
			glDisable(0x4000 + (0x0001 * i)); // desactiva la iluminacion de la escena
		}

		if (tInicio == 0) {	
			tInicio = clock();
		}
		
		glutIdleFunc(NULL);

		glPushMatrix();
			visualizarVB();
		glPopMatrix();

		interfaz.create_menu();

		bool seRealizo = realizarAccionesMenu();

		if (seRealizo) {
			interfaz.setTienda(false);

			glutDestroyMenu(glutGetMenu());

			interfaz.setMenuSelection(NADA);

			glutSwapBuffers();

			glutPostRedisplay(); // renew the content of the window

			for (int i = 1; i < 8; i++) {
				glEnable(0x4000 + (0x0001 * i)); // activa la iluminacion de la escena
			}
		}
	}	
}

void igvEscena3D::visualizarVB() {
	if (!interfaz.getTienda()) {
		glPushMatrix();
		glTranslated(0, -0.2, 0);
		glScaled(4, 0.1, 4);
		glScaled(0.3, 10, 0.5);
		glTranslated(0, 0.2, 0);

		for (int i = 0; i < numTerrenos; i++) {
			glPushMatrix();
				terrenos[i]->visualizar();
			glPopMatrix();
		}

		glTranslated(0, 0.2, 0);
		glScaled(0.25, 10, 0.25);
		glScaled(3.33, 0.1, 2);
		glTranslated(0, -0.2, 0);
		glPopMatrix();

		if (interfaz.getColorTorres())
			cambiarColorTorres();

		for (int i = 0; i < numTerrenos; i++) {
			if (terrenos[i]->getTipo() != NINGUNA) {
				if (terrenos[i]->getTipo() == CAÑON) {
					Cañon* c = terrenos[i]->getDefensaCañon();
					c->crearCañon();
				}
				else if (terrenos[i]->getTipo() == TORRE) {
					Torre* t = terrenos[i]->getDefensaTorre();
					t->crearTorre();
				}
			}
		}
	}
	else {
		terrenos[interfaz.getTerrenoSeleccionado()]->getMenu()->mostrarMenu();
	}
}

void igvEscena3D::controlColisiones() {
	tipoEstructura tipo = NINGUNA;
	igvPunto3D posicionBala;
	Cañon* c = nullptr;
	Torre* t = nullptr;
	int nivelEstructura;

	if (interfaz.getEstructuraDisparo() != - 1) {
		tipo = terrenos[interfaz.getEstructuraDisparo()]->getTipo();

		float dano = 0;

		if (tipo == CAÑON) {
			c = terrenos[interfaz.getEstructuraDisparo()]->getDefensaCañon();
			posicionBala = c->getPosicionDisparo();
			nivelEstructura = c->getNivel();
			dano = c->getDano();
		}
		else if (tipo == TORRE) {
			t = terrenos[interfaz.getEstructuraDisparo()]->getDefensaTorre();
			posicionBala = t->getPosicionDisparo();
			nivelEstructura = t->getNivel();
			dano = t->getDano();
		}

		deque<Enemigo*>::iterator i;
		i = enemigos.begin();

		while (i != enemigos.end()) {
			if ((abs(i.operator*()->getX() - posicionBala[X]) +
				abs(i.operator*()->getY() - posicionBala[Y]) +
				abs(i.operator*()->getZ() - posicionBala[Z])) < 0.25 && colisionado == false) {

				i.operator*()->decrementarSalud(dano);

				if (i.operator*()->getSalud() <= 0) {
					if (enemigos.size() != 0)
						interfaz.incrementarDinero(i.operator*()->getDineroOtorgado());

					i = enemigos.erase(i);

					interfaz.reproducirSonido(MUERTE);
				}

				colisionado = true;

				if (tipo == CAÑON)
					c->setColision(true);
				else if (tipo == TORRE)
					t->setColision(true);
			}
			else
				++i;
		}
	}
}

bool igvEscena3D::realizarAccionesMenu() {
	bool seRealizo = false;

	switch (interfaz.getMenuSelection()) {
	case CONSTRUIRCAÑON:
		seRealizo = comprar(CAÑON);
		break;

	case CONSTRUIRTORRE:
		seRealizo = comprar(TORRE);
		break;

	case MEJORARCAÑON:
		seRealizo = mejorar(CAÑON);
		break;

	case MEJORARTORRE:
		seRealizo = mejorar(TORRE);
		break;

	case VENDERCAÑON:
		seRealizo = vender(CAÑON);
		break;

	case VENDERTORRE:
		seRealizo = vender(TORRE);
		break;

	case SALIR:
		return true;
		break;
	}

	if (interfaz.getMenuSelection() != NADA && interfaz.getMenuSelection() != SALIR)
		interfaz.setColorTorres(true);

	return seRealizo;
}

bool igvEscena3D::comprar(tipoEstructura tipo) {
	float precioConstruir = 100;

	if ((terrenos[interfaz.getTerrenoSeleccionado()]->getTipo() == NINGUNA)) {
		if (interfaz.getDinero() >= precioConstruir) {
			anadirEstructura(1, tipo, interfaz.getTerrenoSeleccionado());
			interfaz.setDinero(interfaz.getDinero() - precioConstruir);
			return true;
		}
	}

	return false;
}

bool igvEscena3D::vender(tipoEstructura tipo) {
	if (terrenos[interfaz.getTerrenoSeleccionado()]->getTipo() == CAÑON)
		interfaz.setDinero(interfaz.getDinero() + terrenos[interfaz.getTerrenoSeleccionado()]->getDefensaCañon()->getPrecioVenta());

	else if (terrenos[interfaz.getTerrenoSeleccionado()]->getTipo() == TORRE)
		interfaz.setDinero(interfaz.getDinero() + terrenos[interfaz.getTerrenoSeleccionado()]->getDefensaTorre()->getPrecioVenta());

	quitarEstructura(interfaz.getTerrenoSeleccionado());

	return true;
}

bool igvEscena3D::mejorar(tipoEstructura tipo) {
	TerrenoEstructuras* myTerreno = terrenos[interfaz.getTerrenoSeleccionado()];

	if (terrenos[interfaz.getTerrenoSeleccionado()]->getTipo() == CAÑON) {
		if (myTerreno->getDefensaCañon()->getNivel() < 3) {
			if (interfaz.getDinero() >= myTerreno->getDefensaCañon()->getPrecioMejora()) {
				interfaz.setDinero(interfaz.getDinero() - myTerreno->getDefensaCañon()->getPrecioMejora());
				anadirEstructura(myTerreno->getDefensaCañon()->getNivel() + 1, tipo, interfaz.getTerrenoSeleccionado());
				return true;
			}
		}
	}

	else if (terrenos[interfaz.getTerrenoSeleccionado()]->getTipo() == TORRE) {
		if (myTerreno->getDefensaTorre()->getNivel() < 3) {
			if (interfaz.getDinero() >= myTerreno->getDefensaTorre()->getPrecioMejora()) {
				interfaz.setDinero(interfaz.getDinero() - myTerreno->getDefensaTorre()->getPrecioMejora());
				anadirEstructura(myTerreno->getDefensaTorre()->getNivel() + 1, tipo, interfaz.getTerrenoSeleccionado());
				return true;
			}
		}
	}

	return false;
}

void igvEscena3D::anadirEnemigo(int nivel, tipoEnemigo _tipo, float _x, float _y, float _z) {
	Enemigo* ene = new Enemigo(nivel, _tipo, _x, _y, _z);
	this->enemigos.push_back(ene);
}

void igvEscena3D::anadirEstructura(int nivel, tipoEstructura _tipo, int nTerreno) {
	igvPunto3D posTerreno = terrenos[nTerreno]->getPosicion();
	Cañon* c = new Cañon();
	Torre* t = new Torre();

	if (_tipo == CAñON) {
		c = new Cañon(nivel, posTerreno.c[0], posTerreno.c[1], posTerreno.c[2]);
		terrenos[nTerreno]->anadirEstructura(c);
	}
	else if (_tipo == TORRE) {
		t = new Torre(nivel, posTerreno.c[0], posTerreno.c[1], posTerreno.c[2]);
		terrenos[nTerreno]->anadirEstructura(t);
	}
}

void igvEscena3D::quitarEstructura(int nTerreno) {
	if (terrenos[nTerreno]->getTipo() != NINGUNA)
		terrenos[nTerreno]->quitarEstructura();
}

void igvEscena3D::cambiarColorTorres() {
	float varSuma = 1;

	for (int i = 0; i < numTerrenos; i++) {
		float r, g, b;
		if (terrenos[i]->getTipo() == TORRE) {
			Torre* myTorre = terrenos[i]->getDefensaTorre();
			GLfloat colorOrig[3];

			colorOrig[0] = myTorre->getColor()[0];
			colorOrig[1] = myTorre->getColor()[1];
			colorOrig[2] = myTorre->getColor()[2];

			r = (colorOrig[0] * 255 + varSuma) / 255;
			g = (colorOrig[1] * 255 + varSuma) / 255;
			b = (colorOrig[2] * 255 + varSuma) / 255;

			myTorre->setColor(r, g, b);

			varSuma += 2;
		}
	}

	interfaz.setColorTorres(false);
}

void igvEscena3D::corazones() {
	glPushMatrix();
		glTranslated(0, -3, 1);
		glRotated(90, 0, 1, 0);

		glPushMatrix();
			glTranslated(0, 0, -0.1);
			glutSolidSphere(0.3, 100, 100);
		glPopMatrix();

		glPushMatrix();
			glTranslated(0, 0, 0.1);
			glutSolidSphere(0.3, 100, 100);
		glPopMatrix();

		glPushMatrix();
			glTranslated(0, -0.1, 0);
			glRotated(90, 1, 0, 0);
			glScaled(0.55, 0.8, 1);
			glutSolidCone(0.5, 0.5, 100, 100);
		glPopMatrix();
	glPopMatrix();
}

void igvEscena3D::crearCorazones() {
	GLfloat rojo[3] = { 1, 0, 0 };
	GLfloat gris[3] = { 0.512 , 0.512, 0.512 };

	if (interfaz.getNumVidas() == 0)
		glMaterialfv(GL_FRONT, GL_EMISSION, gris);
	else
		glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
	corazones();

	glPushMatrix();
		glTranslated(1, 0, 0);
			if (interfaz.getNumVidas() <= 1)
				glMaterialfv(GL_FRONT, GL_EMISSION, gris);
			else
				glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
			corazones();

			glPushMatrix();
				glTranslated(1, 0, 0);
				if (interfaz.getNumVidas() <= 2)
					glMaterialfv(GL_FRONT, GL_EMISSION, gris);
				else
					glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
				corazones();

				glPushMatrix();
					glTranslated(1, 0, 0);
					if (interfaz.getNumVidas() <= 3)
						glMaterialfv(GL_FRONT, GL_EMISSION, gris);
					else
						glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
					corazones();
					glPushMatrix();
						glTranslated(1, 0, 0);
						if (interfaz.getNumVidas() <= 4)
							glMaterialfv(GL_FRONT, GL_EMISSION, gris);
						else
							glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
						corazones();
						glPushMatrix();
						glTranslated(1, 0, 0);
						if (interfaz.getNumVidas() <= 5)
							glMaterialfv(GL_FRONT, GL_EMISSION, gris);
						else
							glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
						corazones();
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}


void igvEscena3D::terreno() {
	GLfloat colCamino[] = { 0.7,0.7,0 }; //amarillo
	GLfloat colCampo[] = { 0,0.2,0 }; //verde

	// crear el modelo
	glPushMatrix(); // guarda la matriz de modelado
		glMaterialfv(GL_FRONT, GL_EMISSION, colCampo);
		glTranslated(0, -0.2, 0);
		glScaled(4, 0.1, 4);

		glutSolidCube(2); //campo

		glScaled(0.333333333, 10, 0.5);
		glTranslated(0, 0.2, 0);

		glPushMatrix(); //pared derecha
			glTranslated(0, 0, 2);
			glScaled(3, 0.1, 0.1);
			glutSolidCube(2);
		glPopMatrix(); // restaura la matriz de modelado

		glPushMatrix(); // pared izquierda
			glTranslated(0, 0, -2);
			glScaled(3, 0.1, 0.1);
			glutSolidCube(2);
		glPopMatrix(); // restaura la matriz de modelado

		glPushMatrix(); //camino de los enemigos
			glMaterialfv(GL_FRONT, GL_EMISSION, colCamino);
			glTranslated(0, -0.1, 0);
			glScaled(3, 0.01, 0.5);
			glutSolidCube(2);
		glPopMatrix(); // restaura la matriz de modelado

	glPopMatrix(); // restaura la matriz de modelado
}

void igvEscena3D::castillo() {
	GLfloat castillo[] = { 0,0,1 }; //azul
	GLfloat col_base_rey1[] = { 0.7,0,1 }; //azul
	GLfloat col_base_rey2[] = { 0,0.3,1 }; //azul

	glMaterialfv(GL_FRONT, GL_EMISSION, castillo);

	glPushMatrix(); // guarda la matriz de modelado
	glTranslated(3.25, 0, 0);

	glPushMatrix(); //base del castillo
	glPushMatrix(); //base derecha del castillo 
	glTranslated(0, 0.7, 1.5);
	glScaled(0.25, 1, 0.5);

	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix(); //base izquierda del castillo 
	glTranslated(0, 0.7, -1.5);
	glScaled(0.25, 1, 0.5);

	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix(); //base superior del castillo 
	glTranslated(0, 2 * 0.5 + 0.7, 0);
	glScaled(0.5, 0.25, 3);

	glutSolidCube(2);

	glScaled(2, 4, 0.33333333);

	glPushMatrix(); //base1 derecha del rey1 del castillo 
	glMaterialfv(GL_FRONT, GL_EMISSION, col_base_rey1);
	glTranslated(0, 0.25, 1.25);
	glScaled(0.45, 0.1, 1);
	glutSolidCube(2);

	glScaled(2, 10, 1);

	glTranslated(0, 0.92, 0);
	glScaled(0.25, 0.25, 0.25);
	glRotated(-90, 0, 1, 0);

	glPushMatrix();
	rey();
	glPopMatrix();

	glPopMatrix();

	glTranslated(0, 1.7, 0);

	glPushMatrix(); //base2 izquierda del rey2 del castillo 
	glMaterialfv(GL_FRONT, GL_EMISSION, col_base_rey2);
	glTranslated(0, 0.25, -1.25);
	glScaled(0.45, 0.1, 1);
	glutSolidCube(2);

	glScaled(2, 10, 1);

	glTranslated(0, 0.92, 0);
	glScaled(0.25, 0.25, 0.25);
	glRotated(-90, 0, 1, 0);

	glPushMatrix();
	rey();
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix(); // restaura la matriz de modelado
	glPopMatrix(); // restaura la matriz de modelado
}

void igvEscena3D::rey() {
	glPushMatrix();
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_EMISSION, blue); //colores arriba del todo del fichero igvEscena3D.cpp

			glScalef(1, 1.5, 1);
			glTranslated(0, -grLib8, 0);
			glutSolidCube(1.5);//cuerpo
			glScalef(1 / 1, 1 / 1.5, 1 / 1);

			glPushMatrix();
				parteArriba();
				costadoIzquierdo();
				costadoDerecho();

				glTranslated(0, +grLib8, 0);

				inferiorIzquierda();
				inferiorDerecha();
			glPopMatrix();
		glPopMatrix(); // restaura la matriz de modelado
	glPopMatrix();
}

void igvEscena3D::crearTapa(float tam_cilindro) {
	glPushMatrix();

		glBegin(GL_POLYGON);

		for (int i = 0; i < 100; i++) {
			float x = (float)cos(i * 2 * 3.14 / 100);
			float y = (float)sin(i * 2 * 3.14 / 100);
			glVertex2f(x / (1 / tam_cilindro), y / (1 / tam_cilindro));
			glColor3f(1.0f, 0.8f, 0.2f);
		}

		glEnd();
		
	glPopMatrix();
}

void igvEscena3D::crearCilindro(float tam_cilindro, float altura) {
	glPushMatrix();
		GLUquadric* cyl = gluNewQuadric();

		gluCylinder(cyl, tam_cilindro, tam_cilindro, altura, 20, 5);

		gluDeleteQuadric(cyl);
		cyl = nullptr;

		crearTapa(tam_cilindro); //tapa de la base del cilindro

		glTranslated(0, 0, altura);

		crearTapa(tam_cilindro); //tapa de la parte de arriba del cilindro
	glPopMatrix();
}

void igvEscena3D::costadoIzquierdo() {
	glPushMatrix(); //Costado Izquierdo
		glTranslated(1, 0.75, 0);

		glRotated(grLib3, 0, 1, 0);
		glRotated(grLib2, 1, 0, 0);

		glMaterialfv(GL_FRONT, GL_EMISSION, morado); //colores arriba del todo del fichero igvEscena3D.cpp

		glPushMatrix();//hombro_izq
			glutSolidSphere(0.25, 10, 8);

			glMaterialfv(GL_FRONT, GL_EMISSION, red); //colores arriba del todo del fichero igvEscena3D.cpp

			glPushMatrix();//brazo izquierdo entero
				glPushMatrix();//br1
					glRotated(45, 1, 0, 0);
					crearCilindro(0.25, 1);
					glRotated(-45, 1, 0, 0);

					glMaterialfv(GL_FRONT, GL_EMISSION, morado); //colores arriba del todo del fichero igvEscena3D.cpp

					glPushMatrix();//codo_izq
						glTranslated(0, -0.75, 0.75);
						glutSolidSphere(0.25, 10, 8);

						glMaterialfv(GL_FRONT, GL_EMISSION, negro); //colores arriba del todo del fichero igvEscena3D.cpp

						glPushMatrix();//ant_br1
							crearCilindro(0.25, 1);

							glMaterialfv(GL_FRONT, GL_EMISSION, green); //colores arriba del todo del fichero igvEscena3D.cpp

							glPushMatrix();//m1
								glTranslated(0, 0, 1);
								glutSolidSphere(0.25, 10, 8);

								glMaterialfv(GL_FRONT, GL_EMISSION, blue); //colores arriba del todo del fichero igvEscena3D.cpp

								glPushMatrix();//d4
									glRotated(30, 0, 0.95, 0);
									crearCilindro(0.05, 0.5);
								glPopMatrix();

								glPushMatrix();//d2
									glRotated(-30, 0, 0.95, 0);
									crearCilindro(0.05, 0.5);
								glPopMatrix();

								glPushMatrix();//d1
									glRotated(60, 0, 0.95, 0);
									crearCilindro(0.05, 0.5);
								glPopMatrix();

								glPushMatrix();//d3
									crearCilindro(0.05, 0.5);
								glPopMatrix();
							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void igvEscena3D::costadoDerecho() {
	glPushMatrix(); //Costado Derecho
	glTranslated(-1, 0.75, 0);

	glRotated(grLib5, 0, 1, 0);
	glRotated(grLib4, 1, 0, 0);

	glMaterialfv(GL_FRONT, GL_EMISSION, morado); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//hombro_der
	glutSolidSphere(0.25, 10, 8);

	glMaterialfv(GL_FRONT, GL_EMISSION, red); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//brazo derecho entero
	glPushMatrix();//br2
	glRotated(45, 1, 0, 0);
	crearCilindro(0.25, 1);
	glRotated(-45, 1, 0, 0);

	glMaterialfv(GL_FRONT, GL_EMISSION, morado); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//codo_der
	glTranslated(0, -0.75, 0.75);
	glutSolidSphere(0.25, 10, 8);

	glMaterialfv(GL_FRONT, GL_EMISSION, negro); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//ant_br2
	crearCilindro(0.25, 1);

	glMaterialfv(GL_FRONT, GL_EMISSION, green); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//m2
	glTranslated(0, 0, 1);
	glutSolidSphere(0.25, 10, 8);

	glMaterialfv(GL_FRONT, GL_EMISSION, blue); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//d5
	glRotated(30, 0, 1, 0);
	crearCilindro(0.05, 0.5);
	glPopMatrix();

	glPushMatrix();//d6
	glRotated(-30, 0, 1, 0);
	crearCilindro(0.05, 0.5);
	glPopMatrix();

	glPushMatrix();//d7
	glRotated(-60, 0, 1, 0);
	crearCilindro(0.05, 0.5);
	glPopMatrix();

	glPushMatrix();//d8
	crearCilindro(0.05, 0.5);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void igvEscena3D::inferiorIzquierda() {
	glPushMatrix(); //inferior Izquierda
	glTranslated(0.5, (-1.5 * 1.5) / 2 - grLib8, 0);

	glMaterialfv(GL_FRONT, GL_EMISSION, morado); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//abductor1
	glutSolidSphere(0.25, 10, 8);

	glMaterialfv(GL_FRONT, GL_EMISSION, red); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//pi1
	glRotated(75 - (grLib8 * 100) - grLib6, 1, 0, 0);
	crearCilindro(0.25, 1);

	glMaterialfv(GL_FRONT, GL_EMISSION, morado); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//rodilla1
	glRotated(-75 + (grLib8 * 100), 1, 0, 0);
	glTranslated(0, -1 + grLib8, 0.3 + (grLib8 * 1.5));
	glutSolidSphere(0.25, 10, 8);

	glMaterialfv(GL_FRONT, GL_EMISSION, negro); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//femur1
	glTranslated(0, -1 + (grLib8), -0.3 - (grLib8 * 1.5));
	glRotated(-75 + (grLib8 * 100), 1, 0, 0);
	crearCilindro(0.25, 1);

	glMaterialfv(GL_FRONT, GL_EMISSION, blue); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//pie1
	glRotated(75 - (grLib8 * 100), 1, 0, 0);
	glTranslated(0, -0.1 + (grLib8 / 2), 0.1);
	glScaled(1, 0.75, 2);
	glutSolidCube(0.5);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void igvEscena3D::inferiorDerecha() {
	glPushMatrix(); //inferior derecha
	glTranslated(-0.5, ((-1.5 * 1.5) / 2) - grLib8, 0);

	glMaterialfv(GL_FRONT, GL_EMISSION, morado); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//abductor2
	glutSolidSphere(0.25, 10, 8);

	glMaterialfv(GL_FRONT, GL_EMISSION, red); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//pi2
	glRotated(75 - (grLib8 * 100) - grLib7, 1, 0, 0);
	crearCilindro(0.25, 1);

	glMaterialfv(GL_FRONT, GL_EMISSION, morado); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//rodilla2
	glRotated(-75 + (grLib8 * 100), 1, 0, 0);
	glTranslated(0, -1 + grLib8, 0.3 + (grLib8 * 1.5));
	glutSolidSphere(0.25, 10, 8);

	glMaterialfv(GL_FRONT, GL_EMISSION, negro); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//femur2
	glTranslated(0, -1 + (grLib8), -0.3 - (grLib8 * 1.5));
	glRotated(-75 + (grLib8 * 100), 1, 0, 0);
	crearCilindro(0.25, 1);

	glMaterialfv(GL_FRONT, GL_EMISSION, blue); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//pie2
	glRotated(75 - (grLib8 * 100), 1, 0, 0);
	glTranslated(0, -0.1 + (grLib8 / 2), 0.1);
	glScaled(1, 0.75, 2);
	glutSolidCube(0.5);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void igvEscena3D::parteArriba() {
	glPushMatrix(); //parte de arriba
	glTranslated(0, 1, 0);

	glMaterialfv(GL_FRONT, GL_EMISSION, morado); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//cabeza
	glTranslated(0, 0.9, 0);
	glutSolidSphere(0.9, 10, 8);

	glMaterialfv(GL_FRONT, GL_EMISSION, red); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//ojo1
	glTranslated(-0.30, 0.45, 0.7);
	glutSolidSphere(0.2, 10, 8);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, red); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//ojo2
	glTranslated(0.30, 0.45, 0.7);
	glutSolidSphere(0.2, 10, 8);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, blue); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//nariz
	glTranslated(0, 0, 0.85);
	glRotated(270, 1, 0, 0);
	glutSolidCone(0.1, 0.45 / 2, 10, 8);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, negro); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//boca
	glTranslated(0, -0.3, 0.85);
	glutSolidTorus(0.1, 0.1, 8, 10);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, blue); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//base_sombrero
	glTranslated(0, grLib1, 0);

	glTranslated(0, 1.7 + 1, 0);
	glTranslated(0, -1.9, 0);
	glRotated(270, 1, 0, 0);
	crearCilindro(1, 0.25);

	glMaterialfv(GL_FRONT, GL_EMISSION, negro); //colores arriba del todo del fichero igvEscena3D.cpp

	glPushMatrix();//sombrero
	crearCilindro(0.5, 1.5);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
