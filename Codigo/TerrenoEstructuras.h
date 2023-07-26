#ifndef __TERRENOESTRUCTURAS
#define __TERRENOESTRUCTURAS

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "MenuEstructuras.h"
#include "igvPunto3D.h"
#include "Cañon.h"
#include "Torre.h"

class MenuEstructuras;

class TerrenoEstructuras {
private:
	igvPunto3D posicion;
	GLfloat* color;

	bool seleccionado;

	Cañon* defensaCañon;
	Torre* defensaTorre;

	MenuEstructuras* myMenu;
	tipoEstructura tipo;

	int numeroLuz;

public:
	TerrenoEstructuras();
	TerrenoEstructuras(float r, float g, float b, igvPunto3D pos);
	TerrenoEstructuras(const TerrenoEstructuras& orig);
	~TerrenoEstructuras();

	void visualizar();

	GLfloat* getColor();
	GLubyte* getColorByte(); //Para comparar con el pixel obtenido en la seleccion

	void anadirEstructura(Cañon* _cañon);
	void anadirEstructura(Torre* _torre);
	void quitarEstructura();

	//Getters y Setters
	Cañon* getDefensaCañon() {
		if (tipo == CAÑON)
			return defensaCañon;
	}

	Torre* getDefensaTorre() {
		if (tipo == TORRE)
			return defensaTorre;
	}

	igvPunto3D getPosicion() {
		return posicion;
	}

	int getNumeroLuz() {
		return numeroLuz;
	}

	void setNumeroLuz(int n) {
		numeroLuz = n;
	}

	void setSeleccionados(bool _seleccionado) {
		seleccionado = _seleccionado;
	}

	MenuEstructuras* getMenu() {
		return myMenu;
	}

	tipoEstructura getTipo() {
		return tipo;
	}
};

#endif
