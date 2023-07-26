#include "igvMaterial.h"

// Metodos constructores 

igvMaterial::igvMaterial() {

}

igvMaterial::~igvMaterial() {

}

igvMaterial::igvMaterial(const igvMaterial& p) {	//de copia
	Ka = p.Ka;
	Kd = p.Kd;
	Ks = p.Ks;
	Ns = p.Ns;
}

igvMaterial::igvMaterial(igvColor _Ka, igvColor _Kd, igvColor _Ks, double _Ns) {
	Ka = _Ka;
	Kd = _Kd;
	Ks = _Ks;
	Ns = _Ns;
}

// Metodos publicos 
void igvMaterial::aplicar(bool act) {
	GLfloat color[] = { 0.0, 0.0, 0.0 }; //objeto no es una fuente de 

	if (act == true) {
		glMaterialfv(GL_FRONT, GL_EMISSION, color);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ka.cloneToFloatArray());
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Kd.cloneToFloatArray());
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks.cloneToFloatArray());

		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Ns);
	}
	else {
		Ka = igvColor(0.1, 0.1, 0.1, 1);
		Kd = igvColor(0.1, 0.1, 0.1, 1);
		Ks = igvColor(0.1, 0.1, 0.1, 1);

		glMaterialfv(GL_FRONT, GL_EMISSION, color);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ka.cloneToFloatArray());
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Kd.cloneToFloatArray());
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks.cloneToFloatArray());

		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Ns);
	}
}

void igvMaterial::set(igvColor _Ka, igvColor _Kd, igvColor _Ks, double _Ns) {
	Ka = _Ka;
	Kd = _Kd;
	Ks = _Ks;
	Ns = _Ns;
}
