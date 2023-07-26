#ifndef __IGVMATERIAL
#define __IGVMATERIAL

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "igvColor.h"

class igvMaterial {
	protected:
		// atributos
		igvColor Ka; // coeficiente reflexion luz ambiental
		igvColor Kd; // coeficiente reflexion difuso
		igvColor Ks; // coeficiente reflexion especular
		double   Ns; // exponente de Phong

	public:
		// Constructores por defecto y destructor
		igvMaterial();
		~igvMaterial();

		// Otros constructores
		igvMaterial(const igvMaterial& p);	//de copia
		igvMaterial(igvColor _Ka, igvColor _Kd, igvColor _Ks, double _Ns);

		// Metodos
		void aplicar(bool act);

		void set(igvColor _Ka, igvColor _Kd, igvColor _Ks, double _Ns);
};

#endif

