#include <stdio.h>
#include <math.h>

#include "igvPunto3D.h"

// Constructores
igvPunto3D::igvPunto3D() {
	c[X] = c[Y] = c[Z] = 0.0;
}

igvPunto3D::igvPunto3D(const double& x, const double& y, const double& z) {
	c[X] = x;
	c[Y] = y;
	c[Z] = z;
}

// Constructor de copia
igvPunto3D::igvPunto3D(const igvPunto3D& p) {
	c[X] = p.c[X];
	c[Y] = p.c[Y];
	c[Z] = p.c[Z];
}

// Operador de asignación
igvPunto3D& igvPunto3D::operator = (const igvPunto3D& p) {
	c[X] = p.c[X];
	c[Y] = p.c[Y];
	c[Z] = p.c[Z];
	return(*this);
}

// Operadores
int igvPunto3D::operator == (const igvPunto3D& p) {
	return ((fabs(c[X] - p[X]) < IGV_EPSILON) && (fabs(c[Y] - p[Y]) < IGV_EPSILON) && (fabs(c[Z] - p[Z]) < IGV_EPSILON));
}

int igvPunto3D::operator != (const igvPunto3D& p) {
	return ((fabs(c[X] - p[X]) >= IGV_EPSILON) || (fabs(c[Y] - p[Y]) >= IGV_EPSILON) || (fabs(c[Z] - p[Z]) >= IGV_EPSILON));
}

// Destructor
igvPunto3D::~igvPunto3D() {
}

void igvPunto3D::set(const double& x, const double& y, const double& z) {
	c[X] = x;
	c[Y] = y;
	c[Z] = z;
}
