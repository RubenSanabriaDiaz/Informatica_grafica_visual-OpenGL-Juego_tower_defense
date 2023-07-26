#include "Cañon.h"

#include "igvInterfaz.h"

extern igvInterfaz interfaz; // los callbacks deben ser estaticos y se requiere este objeto para acceder desde
// ellos a las variables de la clase

Cañon::Cañon(int _nivel, float _x, float _y, float _z) : Estructura(_nivel, CAÑON, _x, _y, _z) {
	movBaseX = 0;
	movBaseZ = 0;
	giroRuedasZ = 0;
	giroCuerpoY = 0;
	alargCuerpoX = 1;
	lanzamietoBala = 0;
}

Cañon::Cañon(const Cañon& orig) :Estructura(orig.nivel, orig.tipo, orig.posicion.c[0], orig.posicion.c[1], orig.posicion.c[2]) {
	movBaseX = 0;
	movBaseZ = 0;
	giroRuedasZ = 0;
	giroCuerpoY = 0;
	alargCuerpoX = 1;
	lanzamietoBala = 0;
}

Cañon::Cañon() : Estructura(1, CAÑON, 0, 0, 0) {
	movBaseX = 0;
	movBaseZ = 0;
	giroRuedasZ = 0;
	giroCuerpoY = 0;
	alargCuerpoX = 1;
	lanzamietoBala = 0;
}

Cañon::~Cañon() {
}

void Cañon::crearLinea(float x, float z) {
	if (z > 0) {
		GLfloat rojo[] = { 1,0,0,1.0 };
		glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
	}
	else {
		GLfloat azul[] = { 0,0,1,1.0 };
		glMaterialfv(GL_FRONT, GL_EMISSION, azul);
	}

	glBegin(GL_LINES);
	glVertex3f(aux[X] + x, 0, 4);
	glVertex3f(aux[X] + x, 0, -4);
	glEnd();

}

void Cañon::crearBala() {
	glPushMatrix();
	glTranslated(0, 0, lanzamietoBala);

	glutSolidSphere(1, 100, 100);

	if ((lanzamietoBala > (30 + alargCuerpoX)) || (colision)) {
		lanzamietoBala = 0;
		interfaz.setTipoA(ENEMIGOS);
		interfaz.set_tDisparoFinEscena(clock());
		colision = false;

		interfaz.setColisionadoEscena(false);
	}
	glPopMatrix();

	if(!vuelta)
		posicionDisparo.set(getX(), getY() + 0.05, getZ() + lanzamietoBala/20);
	else
		posicionDisparo.set(getX(), getY() + 0.05, -getZ() + lanzamietoBala / 20);
}

void Cañon::crearCañon() {
	glPushMatrix();
		float x, z;
		
		if (aux[X] == -2.25 && aux[Z] == -1.25) { //terreno 1
			x = -0.45;  z = -1.25;

			glPushMatrix();
			glTranslated(0, 0.25, 0);
			crearLinea(x, z);
			glPopMatrix();

			posicion.set(aux[X] + x, 0.1, aux[Z]);

			glTranslated(aux[X] + x, 0.1, aux[Z] + z);
		}

		if (aux[X] == -0.75 && aux[Z] == -1.25) { //terreno 2
			x = -0.15;  z = -1.25;

			glPushMatrix();
			glTranslated(0, 0.25, 0);
			crearLinea(x, z);
			glPopMatrix();

			posicion.set(aux[X] + x, 0.1, aux[Z]);

			glTranslated(aux[X] + x, 0.1, aux[Z] + z);
		}

		if (aux[X] == 0.75 && aux[Z] == -1.25) { //terreno 3
			x = +0.15;  z = -1.25;

			glPushMatrix();
			glTranslated(0, 0.25, 0);
			crearLinea(x, z);
			glPopMatrix();

			posicion.set(aux[X] + x, 0.1, aux[Z]);

			glTranslated(aux[X] + x, 0.1, aux[Z] + z);
		}

		if (aux[X] == -1.5 && aux[Z] == 1.25) { //terreno 4
			x = -0.3;  z = +1.25;

			glPushMatrix();
			glTranslated(0, 0.25, 0);
			crearLinea(x, z);
			glPopMatrix();

			posicion.set(aux[X] + x, 0.1, aux[Z]);

			glTranslated(aux[X] + x, 0.1, aux[Z] + z);
		}

		if (aux[X] == 0 && aux[Z] == 1.25) { //terreno 5
			x = 0;  z = +1.25;

			glPushMatrix();
			glTranslated(0, 0.25, 0);
			crearLinea(x, z);
			glPopMatrix();

			posicion.set(aux[X] + x, 0.1, aux[Z]);

			glTranslated(aux[X] + x, 0.1, aux[Z] + z);
		}

		if (aux[X] == 1.5 && aux[Z] == 1.25) { //terreno 6
			x = +0.3;  z = +1.25;

			glPushMatrix();
			glTranslated(0, 0.25, 0);
			crearLinea(x, z);
			glPopMatrix();

			posicion.set(aux[X] + x, 0.1, aux[Z]);

			glTranslated(aux[X] + x, 0.1, aux[Z] + z);
		}

		glScaled(0.3, 0.3, 0.3);

		 if (aux[Z] < 0) {
			 glRotated(-90, 0, 1, 0);
			 vuelta = false;
		 }
		 else {
			 glRotated(90, 0, 1, 0);
			 vuelta = true;
		 }

		 glMaterialfv(GL_FRONT, GL_EMISSION, color);
		 glColor3fv(color);

		 glPushMatrix();

			  glTranslated(movBaseX, 0, movBaseZ);
			  glScaled(0.5, 0.5, 0.5);
			  crearBase();

			  glPushMatrix();
				crearRuedas();
			  glPopMatrix();

			  glPushMatrix();
				glRotated(giroCuerpoY, 0, 0, 1);
				crearCuerpo();
			  glPopMatrix();

   		 glPopMatrix();
	glPopMatrix();
}

void Cañon::crearBase() {
	//Palos laterales
	glPushMatrix();
	glTranslated(0, 0.5, 0);
	glScaled(5, 2, 1);

	glPushMatrix();
	glTranslated(0, 0, 2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -2);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();

	//Palo transversal
	glPushMatrix();
	glScaled(1, 1, 4);

	glPushMatrix();
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
};

void Cañon::crearEjesRueda() {
	int ejeRotacion = 0;

	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glRotated(ejeRotacion, 0, 0, 1);
		glScaled(0.25, 2.5, 0.25);
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glutSolidSphere(0.3, 100, 100);
		glPopMatrix();

		ejeRotacion += 45;
	}
}
void Cañon::crearRuedas() {
	//Rueda 1
	glPushMatrix();
	glTranslated(2.25, 0, 2.75);
	glRotated(giroRuedasZ, 0, 0, 1);
	crearCilindro(1.35, 0.25, false, true);

	glPushMatrix();
	crearEjesRueda();
	glPopMatrix();
	glPopMatrix();

	//Rueda 2
	glPushMatrix();
	glTranslated(-2.25, 0, 2.75);
	glRotated(giroRuedasZ, 0, 0, 1);
	crearCilindro(1.35, 0.25, false, true);

	glPushMatrix();
	crearEjesRueda();
	glPopMatrix();
	glPopMatrix();

	//Rueda 3
	glPushMatrix();
	glTranslated(2.25, 0, -2.75);
	glRotated(giroRuedasZ, 0, 0, 1);
	crearCilindro(1.35, 0.25, true, false);

	glPushMatrix();
	crearEjesRueda();
	glPopMatrix();
	glPopMatrix();

	//Rueda 4
	glPushMatrix();
	glTranslated(-2.25, 0, -2.75);
	glRotated(giroRuedasZ, 0, 0, 1);
	crearCilindro(1.35, 0.25, true, false);

	glPushMatrix();
	crearEjesRueda();
	glPopMatrix();
	glPopMatrix();
};

void Cañon::crearCuerpo() {
	//Cuerpo
	glPushMatrix();
	glTranslated(-3.15, 1.25, 0);
	glRotated(90, 0, 1, 0);

	glPushMatrix();
		glScaled(1, 1, alargCuerpoX);
		crearCilindro(1.15, 8.5, false, true);
		glPopMatrix();

		glPushMatrix();
			crearBala();
		glPopMatrix();
	glPopMatrix();

	//Anilla
	glPushMatrix();
		glTranslated(0.8, 1.25, 0);
		glRotated(90, 0, 1, 0);
		crearCilindro(1.25, 1.5, false, true);
	glPopMatrix();

	//Esfera trasera
	glPushMatrix();
		glTranslated(-3.15, 1.25, 0);
		glutSolidSphere(1.13, 100, 100);

	//Pulsador esfera
		glPushMatrix();
			glTranslated(-0.95, 0, 0);
			glRotated(-90, 0, 1, 0);
			glutSolidCone(0.5, 0.5, 100, 100);
		glPopMatrix();
	glPopMatrix();
}
