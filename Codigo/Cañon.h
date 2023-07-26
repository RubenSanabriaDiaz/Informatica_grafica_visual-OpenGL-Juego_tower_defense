#ifndef __CAÑON
#define __CAÑON

#include "Estructura.h"

class Cañon : public Estructura {
private:
	//Variables relacionadas con los grados de libertad
	float movBaseX;
	float movBaseZ;
	float giroRuedasZ;
	float giroCuerpoY;
	float alargCuerpoX;
	float lanzamietoBala;

	//Limites del cañon
	const float maxGiro = 30;
	const float minGiro = -5;
	const float maxAlarg = 1.4;
	const float minAlarg = 0.55;

public:
	//Constructor y Destructor
	Cañon(int _nivel, float _x, float _y, float _z);
	Cañon(const Cañon& orig);
	Cañon();
	virtual ~Cañon();

	void crearLinea(float x, float z);

	//Implementaciones de las partes del cañon
	void crearCañon();
	void crearBase();
	void crearEjesRueda();
	void crearRuedas();
	void crearCuerpo();
	void crearBala();

	//Metodos para modifcar la libertad de movimiento del cañon
	void set_movBaseX(float movBaseX, float giroRuedasZ) { this->movBaseX += movBaseX; this->giroRuedasZ += giroRuedasZ; };

	void set_movBaseZ(float movBaseZ) { this->movBaseZ += movBaseZ; };

	void set_giroRuedasZ(float giroRuedasZ) { this->giroRuedasZ += giroRuedasZ; };

	void set_giroCuerpoY(int giroCuerpoY) {
		if (this->giroCuerpoY <= maxGiro && this->giroCuerpoY >= minGiro)
			this->giroCuerpoY += giroCuerpoY;
		else if (this->giroCuerpoY > maxGiro)
			this->giroCuerpoY = maxGiro;
		else if (this->giroCuerpoY < minGiro)
			this->giroCuerpoY = minGiro;
	};

	void set_lanzamietoBala(float val) {
		this->lanzamietoBala = lanzamietoBala + val;
		crearBala();
	};

	float getLanzmientoBala() {
		return lanzamietoBala;
	}

	void set_alargCuerpoX(float alargCuerpoX) {
		if (this->alargCuerpoX >= minAlarg && this->alargCuerpoX <= maxAlarg)
			this->alargCuerpoX += alargCuerpoX;
		else if (this->alargCuerpoX <= minAlarg)
			this->alargCuerpoX = minAlarg;
		else if (this->alargCuerpoX >= maxAlarg)
			this->alargCuerpoX = maxAlarg;
	};
};

#endif