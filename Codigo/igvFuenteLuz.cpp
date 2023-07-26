#include "igvFuenteLuz.h"

#include "igvInterfaz.h"

extern igvInterfaz interfaz; // los callbacks deben ser estaticos y se requiere este objeto para acceder desde
// ellos a las variables de la clase

// Metodos constructores 
igvFuenteLuz::igvFuenteLuz() {
}

igvFuenteLuz::igvFuenteLuz(const unsigned int _idLuz,
	const igvPunto3D& _posicion,
	const igvColor& cAmb, const igvColor& cDif, const igvColor& cEsp,
	const double a0, const double a1, const double a2) {

	idLuz = _idLuz;

	posicion = _posicion;

	colorAmbiente = cAmb;
	colorDifuso = cDif;
	colorEspecular = cEsp;

	aten_a0 = a0;
	aten_a1 = a1;
	aten_a2 = a2;

	// valores por defecto cuando la luz no es un foco
	direccion_foco = igvPunto3D(0, 0, 0);
	angulo_foco = 180; // de esta manera la luz se convierte en puntual	
	exponente_foco = 0;

	encendida = true;
}

igvFuenteLuz::igvFuenteLuz(const unsigned int _idLuz,
	const igvPunto3D& _posicion,
	const igvColor& cAmb, const igvColor& cDif, const igvColor& cEsp,
	const double a0, const double a1, const double a2,
	const igvPunto3D& dir_foco, const double ang_foco, const double exp_foco) {

	idLuz = _idLuz;

	posicion = _posicion;

	colorAmbiente = cAmb;
	colorDifuso = cDif;
	colorEspecular = cEsp;

	aten_a0 = a0;
	aten_a1 = a1;
	aten_a2 = a2;

	direccion_foco = dir_foco;
	angulo_foco = ang_foco;
	exponente_foco = exp_foco;

	encendida = true;
}

// Metodos publicos ----------------------------------------
igvPunto3D& igvFuenteLuz::getPosicion(void) {
	return posicion;
}

void igvFuenteLuz::setPosicion(igvPunto3D pos) {
	posicion = pos;
}

void igvFuenteLuz::set(const igvColor& cAmb, const igvColor& cDif, const igvColor& cEsp) {
	colorAmbiente = cAmb;
	colorDifuso = cDif;
	colorEspecular = cEsp;
}

void igvFuenteLuz::setAtenuacion(double a0, double a1, double a2) {
	aten_a0 = a0;
	aten_a1 = a1;
	aten_a2 = a2;
}

void igvFuenteLuz::getAtenuacion(double& a0, double& a1, double& a2) {
	a0 = aten_a0;
	a1 = aten_a1;
	a2 = aten_a2;
}

void igvFuenteLuz::encender(void) {
	encendida = true;
}

void igvFuenteLuz::apagar(void) {
	encendida = false;
}

unsigned int igvFuenteLuz::esta_encendida(void) {
	return encendida;
}

void igvFuenteLuz::aplicar(bool luzFocal) {
	if (this->encendida == true) {
		glEnable(GL_LIGHTING);

		float posLuz[] = { posicion[X], posicion[Y], posicion[Z], 1 };

		glLightfv(this->idLuz, GL_POSITION, posLuz);

		if (luzFocal) {
			glLightf(this->idLuz, GL_SPOT_CUTOFF, 10.0);

			float direccion[] = { direccion_foco[X], direccion_foco[Y], direccion_foco[Z], 0 };

			glLightfv(this->idLuz, GL_SPOT_DIRECTION, direccion);

			glLightfv(this->idLuz, GL_AMBIENT, colorAmbiente.cloneToFloatArray());
			glLightfv(this->idLuz, GL_DIFFUSE, colorDifuso.cloneToFloatArray());
			glLightfv(this->idLuz, GL_SPECULAR, colorEspecular.cloneToFloatArray());

			glLightf(this->idLuz, GL_CONSTANT_ATTENUATION, aten_a0);
			glLightf(this->idLuz, GL_LINEAR_ATTENUATION, aten_a1);
			glLightf(this->idLuz, GL_QUADRATIC_ATTENUATION, aten_a2);

			glEnable(this->idLuz);
		}
		else {
			glLightfv(this->idLuz, GL_AMBIENT, colorAmbiente.cloneToFloatArray());
			glLightfv(this->idLuz, GL_DIFFUSE, colorDifuso.cloneToFloatArray());
			glLightfv(this->idLuz, GL_SPECULAR, colorEspecular.cloneToFloatArray());

			glLightf(this->idLuz, GL_CONSTANT_ATTENUATION, aten_a0);
			glLightf(this->idLuz, GL_LINEAR_ATTENUATION, aten_a1);
			glLightf(this->idLuz, GL_QUADRATIC_ATTENUATION, aten_a2);

			glEnable(this->idLuz);
		}
	}
}



