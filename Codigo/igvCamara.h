#ifndef __IGVCAMARA
#define __IGVCAMARA

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "igvPunto3D.h"

#include <iostream>
using namespace std;

typedef enum {
	IGV_PARALELA,
	IGV_FRUSTUM,
	IGV_PERSPECTIVA
} tipoCamara;

typedef enum {
	ORIGINAL,
	ALZADO,
	PLANTA,
	PERFIL
} tipoVista;

class igvCamara {

public:

	// atributos
	tipoCamara tipo; // paralela o perspectiva
	tipoVista vista; //original, planta, perfil, alzado
	double porcentajeZoom; //porcentaje de zoom que se le da a la escena

	// ventana de visi�n: parametros proyecci�n paralela y frustum
	GLdouble xwmin, xwmax, ywmin, ywmax;

	// ventana de visi�n: parametros proyecci�n perspectiva
	GLdouble angulo, raspecto;

	// distancias de planos cercano y lejano
	GLdouble znear, zfar;

	// angulos de rotacion
	GLdouble camera_angle_x, camera_angle_y;

	// punto de visi�n
	igvPunto3D P0;

	// punto de referencia de visi�n
	igvPunto3D r;

	// vector arriba
	igvPunto3D V;

	// Metodos

public:
	// Constructores por defecto y destructor
	igvCamara();
	~igvCamara();

	// Otros constructores
	igvCamara(tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V);

	// Metodos
	// define la posici�n de la c�mara
	void set(igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V);

	void orbital(float valX, float valY);

	void mover(double _x, double _y, double _z) {
		P0.set(P0.c[0] + _x , P0.c[1] + _y , P0.c[2] + _z);
		 r.set(r.c[0] + _x  , r.c[1] + _y  , r.c[2] + _z);

		aplicar();
	}

	// define una c�mara de tipo paralela o frustum
	void set(tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V,
		double _xwmin, double _xwmax, double _ywmin, double _ywmax, double _znear, double _zfar);

	// define una c�mara de tipo perspectiva
	void set(tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V,
		double _angulo, double _raspecto, double _znear, double _zfar);

	void aplicar(void); // aplica a los objetos de la escena la transformaci�n de visi�n y la transformaci�n de proyecci�n
						// asociadas a los par�metros de la c�mara
	void zoom(double factor); // realiza un zoom sobre la c�mara
};

#endif

