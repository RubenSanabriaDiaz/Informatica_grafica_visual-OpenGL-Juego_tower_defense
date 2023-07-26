/************************************************************************************************/
/* La clase igvPunto3D implementa la funcionalidad de los objetos tanto Punto como Vector en 3D */
/************************************************************************************************/

#ifndef __IGVPUNTO3D
#define __IGVPUNTO3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#define IGV_EPSILON 0.000001 // para comparaciones con 0

#ifndef __ENUM_XYZ
#define __ENUM_XYZ

typedef enum {
	X,
	Y,
	Z
};
#endif

class igvPunto3D {
public:
	double c[3]; // componentes x, y, z del punto o vector

public:
	// Constructores
	igvPunto3D();
	igvPunto3D(const double& x, const double& y, const double& z);

	// Constructor de copia
	igvPunto3D(const igvPunto3D& p);

	// Operador de asignación
	igvPunto3D& operator = (const igvPunto3D& p);

	// Destructor
	~igvPunto3D();

	// Operadores
	inline double& operator[] (const unsigned char idx) { return c[idx]; };
	inline double operator[] (const unsigned char idx) const { return c[idx]; };

	int operator == (const igvPunto3D& p);
	int operator != (const igvPunto3D& p);

	void set(const double& x, const double& y, const double& z);
};
#endif

