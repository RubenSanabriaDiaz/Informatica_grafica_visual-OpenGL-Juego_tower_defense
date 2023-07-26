#include "Estructura.h"

Estructura::Estructura(int _nivel, tipoEstructura _tipo, float _x, float _y, float _z) {
	nivel = _nivel;
	tipo = _tipo;
	color = new GLfloat[3];
	posicion.set(_x, _y, _z);
	aux.set(_x, _y, _z);
	colision = false;
	vuelta = false;

	this->precioConstruccion = 100;

	if (nivel == 1) { //verde
		color[0] = 0;
		color[1] = 0.9;
		color[2] = 0;

		if (tipo == CAÑON) {
			this->dano = 100;
			this->precioMejora = 150;
			this->precioVenta = 50;
		}
		else {
			if (tipo == TORRE) {
				this->dano = 80;
				this->precioMejora = 150;
				this->precioVenta = 50;
			}		
		}
		
	}

	if (nivel == 2) { //naranja
		color[0] = 0.9;
		color[1] = 0.5;
		color[2] = 0.15;

		if (tipo == CAÑON) {
			this->dano = 180;
			this->precioMejora = 250;
			this->precioVenta = 150;
		}
		else {
			if (tipo == TORRE) {
				this->dano = 140;
				this->precioMejora = 350;
				this->precioVenta = 150;
			}
		}
	}

	if (nivel == 3) { //rojo
		color[0] = 0.9;
		color[1] = 0;
		color[2] = 0;

		if (tipo == CAÑON) {
			this->dano = 200;
			this->precioMejora = 0;
			this->precioVenta = 250;
			this->precioConstruccion = 300;
		}
		else {
			if (tipo == TORRE) {
				this->dano = 170;
				this->precioMejora = 0;
				this->precioVenta = 250;
				this->precioConstruccion = 300;
			}
		}
	}
}

Estructura::~Estructura() {
	delete[] color;
}

void Estructura::crearTapa(float tam_cilindro) {
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

void Estructura::crearCilindro(float tam_cilindro, float altura, bool tapaExt, bool tapaInt) {
	glPushMatrix();
	GLUquadric* cyl = gluNewQuadric();

	gluCylinder(cyl, tam_cilindro, tam_cilindro, altura, 20, 5);

	gluDeleteQuadric(cyl);
	cyl = nullptr;

	if (tapaInt)
		crearTapa(tam_cilindro); //tapa de la base del cilindro

	glTranslated(0, 0, altura);

	if (tapaExt)
		crearTapa(tam_cilindro); //tapa de la parte de arriba del cilindro

	glPopMatrix();
}