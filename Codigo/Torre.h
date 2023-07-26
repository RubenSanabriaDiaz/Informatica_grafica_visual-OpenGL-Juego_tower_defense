#ifndef __TORRE
#define __TORRE

#include "Estructura.h"

class Torre : public Estructura {
private:
	//Variables relacionadas con los grados de libertad
	double alturaTorreY;
	double disparoFlecha;

public:
	//Constructor y Destructor
	Torre(int _nivel, float _x, float _y, float _z);
	Torre(const Torre& orig);
	Torre();
	virtual ~Torre();

	void crearLinea(float x, float z);

	//Implementaciones de las partes de la torre
	void crearTorre();
	void crearBase();
	void crearFlecha();

	double getDisparoFlecha() {
		return disparoFlecha;
	}

	void set_dispararFlecha(float val) {
		this->disparoFlecha += val;
		crearFlecha();
	};

	void setAlturaTorre(double var) {
		alturaTorreY += var;
	}

	double getAlturaTorre() {
		return  alturaTorreY;
	}
};

#endif