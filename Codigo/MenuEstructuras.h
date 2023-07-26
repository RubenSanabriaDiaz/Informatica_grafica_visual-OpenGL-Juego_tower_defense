#ifndef __MENUESTRUCTURAS
#define __MENUESTRUCTURAS

#include "Cañon.h"
#include "Torre.h"
#include "TerrenoEstructuras.h"

#include <deque>

class TerrenoEstructuras;

class MenuEstructuras {
private:
	igvPunto3D posicion;
	Cañon* c;
	Torre* t;
	TerrenoEstructuras* myTerreno;

public:
	//Constructor y Destructor
	MenuEstructuras();
	MenuEstructuras(Cañon* _c, Torre* _t);
	~MenuEstructuras();

	void mostrarMenu();

	Cañon* getCañon() {
		return c;
	}

	Torre* getTorre() {
		return t;
	}

	void setCañon(Cañon* _c) {
		c = _c;
	}

	void setTorre(Torre* _t) {
		t = _t;
	}

	void setTerreno(TerrenoEstructuras* _myTerreno) {
		myTerreno = _myTerreno;
	}

	TerrenoEstructuras* getTerreno() {
		return myTerreno;
	}
};

#endif
