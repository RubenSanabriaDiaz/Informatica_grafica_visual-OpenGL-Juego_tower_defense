#include "Torre.h"

#include "igvInterfaz.h"

extern igvInterfaz interfaz; // los callbacks deben ser estaticos y se requiere este objeto para acceder desde
// ellos a las variables de la clase

Torre::Torre(const Torre& orig) : Estructura(orig.nivel, orig.tipo, orig.posicion.c[0], orig.posicion.c[1], orig.posicion.c[2]) {
	alturaTorreY = 0;
	disparoFlecha = 0;
}

Torre::Torre(int _nivel, float _x, float _y, float _z) : Estructura(_nivel, TORRE, _x, _y, _z) {
	alturaTorreY = 0;
	this->disparoFlecha = 0;
}

Torre::Torre() : Estructura(1, TORRE, 0, 0, 0) {
	alturaTorreY = 0;
	this->disparoFlecha = 0;
}

Torre::~Torre() {
}

void Torre::crearLinea(float x, float z) {
	if (z > 0) {
		GLfloat rojo[] = { 1,0,0,1.0 };
		glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
	}
	else {
		GLfloat azul[] = { 0,0,1,1.0 };
		glMaterialfv(GL_FRONT, GL_EMISSION, azul);
	}

	glBegin(GL_LINES);
	glVertex3f(aux[X] + x, alturaTorreY / 3 , 4);
	glVertex3f(aux[X] + x, alturaTorreY / 3 , -4);

	glEnd();
}

void Torre::crearFlecha() {
	glPushMatrix();
		glTranslated(0, 0, disparoFlecha + 0.5);

		crearCilindro(0.05, 1, true, true);
	
		glPushMatrix();
			glTranslated(0, 0, 1);
			glutSolidCone(0.1, 0.4, 4, 4);
		glPopMatrix();

		if ((disparoFlecha > 30) || (colision)){
			disparoFlecha = 0;
			interfaz.setTipoA(ENEMIGOS);
			interfaz.set_tDisparoFinEscena(clock());
			colision = false;

			interfaz.setColisionadoEscena(false);
		}
		
	glPopMatrix();

	if (!vuelta)
		posicionDisparo.set(getX(), getY(), getZ() + disparoFlecha / 20);
	else
		posicionDisparo.set(getX(), getY(), -getZ() + disparoFlecha / 20);
}

void Torre::crearTorre() {
	glPushMatrix();
		float x, z;

		if (aux[X] == -2.25 && aux[Z] == -1.25) { //terreno 1
			x = -0.45;  z = -1.25;

			glPushMatrix();
			crearLinea(x, z);
			glPopMatrix();

			posicion.set(aux[X] + x, alturaTorreY / 3, aux[Z]);

			glTranslated(aux[X] + x, 0.1, aux[Z] + z);
		}

		if (aux[X] == -0.75 && aux[Z] == -1.25) { //terreno 2
			x = -0.15;  z = -1.25;

			glPushMatrix();
			crearLinea(x, z);
			glPopMatrix();

			posicion.set(aux[X] + x, alturaTorreY / 3, aux[Z]);

			glTranslated(aux[X] + x, 0.1, aux[Z] + z);
		}

		if (aux[X] == 0.75 && aux[Z] == -1.25) { //terreno 3
			x = +0.15;  z = -1.25;

			glPushMatrix();
			crearLinea(x, z);
			glPopMatrix();

			posicion.set(aux[X] + x, alturaTorreY / 3, aux[Z]);

			glTranslated(aux[X] + x, 0.1, aux[Z] + z);
		}

		if (aux[X] == -1.5 && aux[Z] == 1.25) { //terreno 4
			x = -0.3;  z = +1.25;

			glPushMatrix();
			crearLinea(x, z);
			glPopMatrix();

			posicion.set(aux[X] + x, alturaTorreY / 3, aux[Z]);

			glTranslated(aux[X] + x, 0.1, aux[Z] + z);
		}

		if (aux[X] == 0 && aux[Z] == 1.25) { //terreno 5
			x = 0;  z = +1.25;

			glPushMatrix();
			crearLinea(x, z);
			glPopMatrix();

			posicion.set(aux[X] + x, alturaTorreY / 3, aux[Z]);

			glTranslated(aux[X] + x, 0.1, aux[Z] + z);
		}

		if (aux[X] == 1.5 && aux[Z] == 1.25) { //terreno 6
			x = +0.3;  z = +1.25;

			glPushMatrix();
			crearLinea(x, z);
			glPopMatrix();

			posicion.set(aux[X] + x, alturaTorreY / 3, aux[Z]);

			glTranslated(aux[X] + x, 0.1, aux[Z] + z);
		}

		glScaled(0.3, 0.3, 0.3);

		if (aux[Z] < 0)
			vuelta = false;
		else 
			vuelta = true;

		glMaterialfv(GL_FRONT, GL_EMISSION, color);
		glColor3fv(color);

		 glPushMatrix();
			crearBase();
		 glPopMatrix();
	glPopMatrix();
}

void Torre::crearBase(){
	//Palos Verticales 1-4
	if (alturaTorreY < 0.5)
		alturaTorreY = 0.5;
	else if (alturaTorreY > 10)
		alturaTorreY = 10;

	glPushMatrix();
	 glTranslated(0, alturaTorreY - alturaTorreY / 2, 0);
	 glScaled(0.35, alturaTorreY, 0.35);

	 //Palo Vertical 1
	 glPushMatrix();
	  glTranslated(-2, 0, 2);
	  glutSolidCube(1);
	 glPopMatrix();

	 //Palo Vertical 2
	 glPushMatrix();
	  glTranslated(2, 0, 2);
	  glutSolidCube(1);
	 glPopMatrix();

	 //Palo Vertical 3
	 glPushMatrix();
	  glTranslated(-2, 0, -2);
	  glutSolidCube(1);
	 glPopMatrix();

	 //Palo Vertical 4
   	 glPushMatrix();
	  glTranslated(2, 0, -2);
	  glutSolidCube(1);
	 glPopMatrix();

	 glPopMatrix();
	//Palos Horizontales 1-4
	glPushMatrix();
	 glScaled(0.35, 0.35, 1.7);

	 //Palo Horizontal 1
	 glPushMatrix();
	  glTranslated(2, alturaTorreY - alturaTorreY / 2, 0);
	  glutSolidCube(1);
	 glPopMatrix();
	
	 //Palo Horizontal 2
	 glPushMatrix();
	  glTranslated(2, alturaTorreY * 2 - 1.5, 0);
	  glutSolidCube(1);
	 glPopMatrix();

	 //Palo Horizontal 3
	 glPushMatrix();
	  glTranslated(-2, alturaTorreY * 2 - 1.5, 0);
	  glutSolidCube(1);
	 glPopMatrix();

	 //Palo Horizontal 4
	 glPushMatrix();
	  glTranslated(-2, alturaTorreY - alturaTorreY / 2, 0);
	  glutSolidCube(1);
	 glPopMatrix();
	
	glPopMatrix();

	//Palos Horizontales 5-8
	glPushMatrix();
	 glScaled(1.7, 0.35, 0.35);

	 //Palo Horizontal 5
	 glPushMatrix();
	  glTranslated(0, alturaTorreY - alturaTorreY / 2, 2);
	  glutSolidCube(1);
	 glPopMatrix();

	 //Palo Horizontal 6
	 glPushMatrix();
	  glTranslated(0, alturaTorreY * 2 - 1.5, 2);
	  glutSolidCube(1);
	 glPopMatrix();

	 //Palo Horizontal 7
	 glPushMatrix();
	  glTranslated(0, alturaTorreY - alturaTorreY / 2, -2);
	  glutSolidCube(1);
	 glPopMatrix();

	 //Palo Horizontal 8
	 glPushMatrix();
	  glTranslated(0, alturaTorreY * 2 - 1.5, -2);
	  glutSolidCube(1);
	 glPopMatrix();

	glPopMatrix();
	//Base Tope
	glPushMatrix();
	 glTranslated(0, alturaTorreY, 0);

	 glPushMatrix();
	  glScaled(2, 1, 2);
	  glutSolidCube(1);
	 glPopMatrix();

 	glPushMatrix();
		if (posicion.c[2] > 0) { //esta en el lado izquierdo
			glRotated(180, 0, 1, 0);
		}
		crearFlecha(); //flecha
	glPopMatrix();

	glPopMatrix();
}

