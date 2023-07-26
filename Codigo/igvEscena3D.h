#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include <sstream>
#include <deque>
#include <vector>
#include <list>

#include <ctime>

#include <iostream>
using namespace std;

#include "MenuEstructuras.h"
#include "Enemigo.h"
#include "Cañon.h"
#include "Torre.h"
#include "igvFuenteLuz.h"
#include "igvTextura.h"
#include "igvMaterial.h"
#include "TerrenoEstructuras.h"

class igvEscena3D { //https://es.wikibooks.org/wiki/Programaci%C3%B3n_en_C%2B%2B/Librer%C3%ADa_Est%C3%A1ndar_de_Plantillas/Listas
protected:
	// Atributos
	bool ejes;
	bool consola;
	int numEnemigos; //numero de enemigos por oleada

	std::deque<Enemigo*> enemigos;

	std::deque<TerrenoEstructuras*> terrenos;
	std::deque<TerrenoEstructuras*> terrenosOriginales;

	int numTerrenos;

	bool colisionado;

	igvFuenteLuz focoGeneral;
	double phong;
	double coefDifuso;
	double coefEspecular;

	double x;
	double radioFoco;
	double z;

	float tEjecucion, tInicio, tFin, tDisparoInicio, tDisparoFin;

	bool activarMaterial;
	bool autorizarMaterial;
	bool tipoSombreado;
	bool principio;
	bool finalizar;

	int sentido; //esta variable de tipo entera sirve para indicar el sentido en el que se realiza 
	//la animacion de saltar del muñeco, sentido = 2 (para arriba),  sentido = 1 (para abajo), 
	//sentido = 0 (para abajo (principio))

	double ang_rot_x = 0; //angulo de rotacion en el eje x
	double ang_rot_y = 0; //angulo de rotacion en el eje y
	double ang_rot_z = 0; //angulo de rotacion en el eje z

	double grLib1; //grado de libertad 1: mueve el sombrero de arriba a abajo pulsando 's' (arriba) o 'S' (abajo)

	double grLib2; //grado de libertad 2: mueve el brazo1 (izquierda) de arriba a abajo pulsando '2' (arriba) o '9' (abajo)
	double grLib3; //grado de libertad 3: mueve el brazo1 (izquierda) de izquierda a derecha pulsando 3 (izquierda) o 8 (derecha)

	double grLib4; //grado de libertad 4: mueve el brazo2 (derecha) de arriba a abajo pulsando '4' (arriba) o '7' (abajo)
	double grLib5; //grado de libertad 5: mueve el brazo2 (derecha) de izquierda a derecha pulsando '5' (izquierda) o '6' (derecha)

	double grLib6; //grado de libertad 6: mueve femur1 pulsando 'f' o 'F'
	double grLib7; //grado de libertad 7: mueve femur2 pulsando 'g' o 'G'

	double grLib8; //grado de libertad 8: el muñeco se agacha pulsando 'm' y se estira pulsando 'M'

public:
	// Constructores por defecto y destructor
	igvEscena3D();
	~igvEscena3D();

	void terreno();
	void castillo();
	void rey();
	void corazones();

	void visualizarVB();
	void cargarExternos();

	void crearCilindro(float tam_cilindro, float altura);
	void crearTapa(float tam_cilindro);
	void crearCorazones();
	void parteArriba();
	void costadoIzquierdo();
	void costadoDerecho();
	void inferiorIzquierda();
	void inferiorDerecha();

	std::deque<Enemigo*> getEnemigos() {
		return enemigos;
	}

	std::deque<TerrenoEstructuras*> getTerrenos() {
		return terrenos;
	}

	void anadirEnemigo(int nivel, tipoEnemigo _tipo, float _x, float _y, float _z);
	
	void anadirEstructura(int nivel, tipoEstructura _tipo, int nTerreno);
	void quitarEstructura(int nTerreno);
	
	bool realizarAccionesMenu();
	
	bool comprar(tipoEstructura tipo);
	bool vender(tipoEstructura tipo);
	bool mejorar(tipoEstructura tipo);

	void cambiarColorTorres();

	void controlColisiones();

	// Metodos
	// metodo con las llamadas OpenGL para visualizar la escena
	void visualizar();

	bool get_ejes() { return ejes; };
	void set_ejes(bool _ejes) { ejes = _ejes; };

	void setColisionado(bool col) { colisionado = col; };
	void crearEnemigo();

	void incrementarX(float val) { x = x + val; }

	void incrementarRadioFoco(float val) { radioFoco = radioFoco + val; }

	void incrementarZ(float val) { z = z + val; }

	void incrementarPhong(float val) { phong = phong + val; }

	void incrementarcoefDifuso(float val) { coefDifuso = coefDifuso + val; }

	void incrementarcoefEspecular(float val) { coefEspecular = coefEspecular + val; }

	double get_phong() { return phong; };
	void set_phong(double _phong) { phong = _phong; };

	double get_coefDifuso() { return coefDifuso; };
	void set_coefDifuso(double _coefDifuso) { coefDifuso = _coefDifuso; };

	double get_coefEspecular() { return coefEspecular; };
	void set_coefEspecular(double _coefEspecular) { coefEspecular = _coefEspecular; };

	double get_x() { return x; };
	void set_x(double _x) { x = _x; };

	double get_radioFoco() { return radioFoco; };
	void set_radioFoco(double _radioFoco) { radioFoco = _radioFoco; };

	double get_z() { return z; };
	void set_z(double _z) { z = _z; };

	clock_t get_tInicio() { return tInicio; };
	void set_tInicio(clock_t time) { tInicio = time; };

	clock_t get_tFin() { return tFin; };
	void set_tFin(clock_t time) { tFin = time; };

	clock_t get_tDisparoInicio() { return tDisparoInicio; };
	void set_tDisparoInicio(clock_t time) { tDisparoInicio = time; };

	clock_t get_tDisparoFin() { return tDisparoFin; };
	void set_tDisparoFin(clock_t time) { tDisparoFin = time; };

	void incrementarAngRotX(double sum) {
		this->ang_rot_x = this->ang_rot_x + sum;
	}

	void incrementarAngRotY(double sum) {
		this->ang_rot_y = this->ang_rot_y + sum;
	}

	void incrementarAngRotZ(double sum) {
		this->ang_rot_z = this->ang_rot_z + sum;
	}

	void incrementarGrLib1(double sum) {
		this->grLib1 = this->grLib1 + sum;
	}

	void incrementarGrLib2(double sum) {
		this->grLib2 = this->grLib2 + sum;
	}

	void incrementarGrLib3(double sum) {
		this->grLib3 = this->grLib3 + sum;
	}

	void incrementarGrLib4(double sum) {
		this->grLib4 = this->grLib4 + sum;
	}

	void incrementarGrLib5(double sum) {
		this->grLib5 = this->grLib5 + sum;
	}

	void incrementarGrLib6(double sum) {
		this->grLib6 = this->grLib6 + sum;
	}

	void incrementarGrLib7(double sum) {
		this->grLib7 = this->grLib7 + sum;
	}

	void incrementarGrLib8(double sum) {
		this->grLib8 = this->grLib8 + sum;
	}

	double getAngRotX() {
		return this->ang_rot_x;
	}

	double getAngRotY() {
		return this->ang_rot_y;
	}

	double getAngRotZ() {
		return this->ang_rot_z;
	}

	double getGrLib1() {
		return this->grLib1;
	}

	double getGrLib2() {
		return this->grLib2;
	}

	double getGrLib3() {
		return this->grLib3;
	}

	double getGrLib4() {
		return this->grLib4;
	}

	double getGrLib5() {
		return this->grLib5;
	}

	double getGrLib6() {
		return this->grLib6;
	}

	double getGrLib7() {
		return this->grLib7;
	}

	double getGrLib8() {
		return this->grLib8;
	}

	int get_sentido() { return sentido; };
	void set_sentido(int _sentido) { sentido = _sentido; };

	igvFuenteLuz getFocoGeneral() {
		return focoGeneral;
	}

	double get_activarMaterial() { return activarMaterial; };
	void set_activarMaterial(bool act) { activarMaterial = act; };

	void cambiarTipoSombreado() {
		if (tipoSombreado == true) {
			tipoSombreado = false;
		}
		else {
			tipoSombreado = true;
		}
	};

	double get_autorizarMaterial() { return autorizarMaterial; };
	void set_autorizarMaterial(bool act) { autorizarMaterial = act; };
};

#endif
