#ifndef __ESTRUCTURA
#define __ESTRUCTURA

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
	NINGUNA,
	CAÑON,
	TORRE
} tipoEstructura;

class Estructura {
protected:
	int nivel;
	GLfloat* color;

	float dano;
	float precioConstruccion;
	float precioMejora;
	float precioVenta;
	bool colision;
	bool vuelta;

	tipoEstructura tipo;

	igvPunto3D aux;
	igvPunto3D posicion;
	igvPunto3D posicionDisparo;

public:
	Estructura(int nivel, tipoEstructura _tipo, float _x, float _y, float _z);
	virtual ~Estructura();
	void crearCilindro(float tam_cilindro, float altura, bool tapaExt, bool tapaInt);
	void crearTapa(float tam_cilindro);

	double getX() {
		return posicion.c[0];
	}

	double getY() {
		return posicion.c[1];
	}

	double getZ() {
		return posicion.c[2];
	}

	igvPunto3D getPosicionDisparo() {
		return posicionDisparo;
	}

	tipoEstructura getTipo() {
		return tipo;
	}

	float getDano() {
		return dano;
	}

	float getPrecioMejora() {
		return precioMejora;
	}

	float getPrecioVenta() {
		return precioVenta;
	}

	float getPrecioConstruccion() {
		return precioConstruccion;
	}

	int getNivel() {
		return nivel;
	}

	void setPosicion(float _x, float _y, float _z) {
		posicion.set(_x, _y, _z);
	}

	void setAux(float _x, float _y, float _z) {
		aux.set(_x, _y, _z);
	}

	GLfloat* getColor() {
		return color;
	}

	void setColor(float r, float g, float b) {
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}

	void setColision(bool _colision) {
		colision = _colision;
	}
};

#endif