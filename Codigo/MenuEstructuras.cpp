#include "MenuEstructuras.h"

MenuEstructuras::MenuEstructuras() {
	posicion.set(0, 0, 0);
	c = new Cañon();
	t = new Torre();
}

MenuEstructuras::MenuEstructuras(Cañon* _c, Torre* _t) {
	posicion.set(0, 0, 0);
	c = _c;
	t = _t;
}

MenuEstructuras::~MenuEstructuras() {
	c = nullptr;
	t = nullptr;
	myTerreno = nullptr;
}

void MenuEstructuras::mostrarMenu() {
	if (myTerreno->getDefensaCaÑon() != nullptr || myTerreno->getDefensaTorre() != nullptr) {
		if (myTerreno->getTipo() == CAÑON) {
			c = new Cañon(*myTerreno->getDefensaCañon());
			c->setAux(0, 0, 0);
		}
		else if (myTerreno->getTipo() == TORRE) {
			t = new Torre(*myTerreno->getDefensaTorre());
			t->setAux(0, 0, 0);
		}
	}
	
	glPushMatrix();
	glTranslated(2.5, 0, 0);
	glScaled(0.65, 0.65, 0.65);

	glPushMatrix();
	glTranslated(posicion.c[0], posicion.c[1], posicion.c[2]);

	glPushMatrix();
	glTranslated(-2.5, 0, 0);
	glRotated(180, 0, 1, 0);
	glScaled(3, 3, 3);
	c->crearCañon();
	glPopMatrix();

	glPushMatrix();
	glTranslated(2.5, 0, 0);
	glScaled(3, 3, 3);
	t->crearTorre();
	glPopMatrix();

	//Barras horizontales
	GLfloat* colorMenu = new GLfloat[3];
	colorMenu[0] = 0;
	colorMenu[1] = 0;
	colorMenu[2] = 0;

	glMaterialfv(GL_FRONT, GL_EMISSION, colorMenu);
	glColor3fv(colorMenu);
	glPushMatrix();
	glScaled(10, 0.2, 0.2);

	glPushMatrix();
	glTranslated(0, 20, 0);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, -20, 0);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//Barras verticales
	glPushMatrix();
	glScaled(0.2, 8.1, 0.2);

	glPushMatrix();
	glTranslated(24.5, 0, 0);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslated(-24.5, 0, 0);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//Boton Menu
	GLfloat* colorBoton = new GLfloat[3];
	colorBoton[0] = 0;
	colorBoton[1] = 255;
	colorBoton[2] = 255;

	glMaterialfv(GL_FRONT, GL_EMISSION, colorBoton);
	glColor3fv(colorBoton);

	glPushMatrix();
		glTranslated(-12.5, 5, 0);
		glutSolidCube(1);
	glPopMatrix();
}