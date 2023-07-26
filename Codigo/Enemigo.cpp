#include "Enemigo.h"

Enemigo::Enemigo(int _nivel, tipoEnemigo _tipo, float _x, float _y, float _z) {
	nivel = _nivel;
	tipo = _tipo;

	posicion.set(_x, _y, _z);

	if (nivel == 1) { //verde
		color[0] = 0;
		color[1] = 1;
		color[2] = 0;

		salud = 90;
		velocidad = 120;
		dineroOtorgado = 70;
	}

	if (nivel == 2) { //naranja
		color[0] = 0.9765;
		color[1] = 0.4745;
		color[2] = 0.1686;

		salud = 160;
		velocidad = 90;
		dineroOtorgado = 100;
	}

	if (nivel == 3) { //rojo
		color[0] = 1;
		color[1] = 0;
		color[2] = 0;

		salud = 190;
		velocidad = 70;
		dineroOtorgado = 120;
	}
}

Enemigo::~Enemigo(){
}

void Enemigo::mover() {
	posicion.c[0] = posicion.c[0] + (1 / velocidad);
	//cout << "equis: " << x<<endl;
}

void Enemigo::decrementarSalud(float dano) {
	salud = salud - dano;
}

void Enemigo::crearEnemigo() {
	glPushMatrix();//enemigo 
		glMaterialfv(GL_FRONT, GL_EMISSION, color);
		glTranslated(posicion.c[0], posicion.c[1], posicion.c[2]);
		glutSolidSphere(0.25, 100, 80);
	glPopMatrix();
}
