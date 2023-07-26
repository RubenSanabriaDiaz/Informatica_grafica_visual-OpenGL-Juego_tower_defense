#ifndef __IGVFUENTELUZ
#define __IGVFUENTELUZ

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "igvPunto3D.h"
#include "igvColor.h"

class igvFuenteLuz {

protected:
	// Atributos
	unsigned int idLuz; // identificador de la luz (GL_LIGHT0 a GL_LIGHT7)

	igvPunto3D posicion; // posicion de la luz

	igvColor colorAmbiente; // color ambiental de la luz
	igvColor colorDifuso; // color difuso de la luz
	igvColor colorEspecular; // color especular de la luz

	double aten_a0; // coeficiente de atenuacion a0
	double aten_a1; // coeficiente de atenuacion a1
	double aten_a2; // coeficiente de atenuacion a2

	// parametros para definir un foco
	igvPunto3D direccion_foco;
	double angulo_foco;
	double exponente_foco;

	bool encendida; // Indica si la luz esta encendida o no

public:
	// Constructores por defecto y destructor
	igvFuenteLuz();

	// Otros constructores
	// luz puntual
	igvFuenteLuz(const unsigned int _idLuz,
		const igvPunto3D& _posicion,
		const igvColor& cAmb, const igvColor& cDif, const igvColor& cEsp,
		const double a0, const double a1, const double a2);

	// foco
	igvFuenteLuz(const unsigned int _idLuz,
		const igvPunto3D& _posicion,
		const igvColor& cAmb, const igvColor& cDif, const igvColor& cEsp,
		const double a0, const double a1, const double a2,
		const igvPunto3D& dir_foco, const double ang_foco, const double exp_foco);

	// Metodos
	igvPunto3D& getPosicion(void); // devuelve la posicion de la luz
	void setPosicion(igvPunto3D pos); // establece la posicion de la luz

	void set(const igvColor& cAmb, const igvColor& cDif, const igvColor& cEsp);

	void setId(int id) {
		this->idLuz = id;
	}

	int getId() {
		return this->idLuz;
	}

	void setAmbiental(const igvColor& a) {
		this->colorAmbiente = a;
	}

	void setDifuso(const igvColor& d) {
		this->colorDifuso = d;
	}

	void setEspecular(const igvColor& e) {
		this->colorEspecular = e;
	}

	void setDireccionFoco(igvPunto3D f) {
		this->direccion_foco = f;
	}

	igvColor& getAmbiental(void) {
		return this->colorAmbiente;
	}

	igvColor& getDifuso(void) {
		return this->colorDifuso;
	}

	igvColor& getEspecular(void) {
		return this->colorEspecular;
	}

	void setAtenuacion(double a0, double a1, double a2);
	void getAtenuacion(double& a0, double& a1, double& a2);

	void encender(void);
	void apagar(void);

	unsigned int esta_encendida(void);

	void aplicar(bool luzFocal);
};

#endif

