#ifndef __ENEMIGO
#define __ENEMIGO

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "igvPunto3D.h"

#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

typedef enum {
	VOLADOR,
	TERRESTRE
} tipoEnemigo;

class Enemigo {
protected:
	int nivel;
	GLfloat color[3];

	float velocidad;
	float salud;
	float dineroOtorgado;
	tipoEnemigo tipo;

	igvPunto3D posicion;
	
public:
	Enemigo(int nivel, tipoEnemigo _tipo, float _x, float _y, float _z);
	~Enemigo();

	void mover();
	void crearEnemigo();
	void decrementarSalud(float dano);

	double getX() {
		return posicion.c[0];
	}

	double getY() {
		return posicion.c[1];
	}

	double getZ() {
		return posicion.c[2];
	}

	int getNivel(){
		return nivel;
	}

	int getDineroOtorgado() {
		return dineroOtorgado;
	}

	float getSalud() {
		return salud;
	}
};

#endif