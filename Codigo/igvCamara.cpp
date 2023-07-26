#include <math.h>

#include "igvCamara.h"

// Metodos constructores

igvCamara::igvCamara() {}

igvCamara::~igvCamara() {}

igvCamara::igvCamara(tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V) {
	P0 = _P0;
	r = _r;
	V = _V;

	tipo = _tipo;
	vista = ORIGINAL;
	porcentajeZoom = 1.0;

	camera_angle_x = 0;
	camera_angle_y = 0;
}

// Metodos publicos 
void igvCamara::set(igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V) {
	P0 = _P0;
	r = _r;
	V = _V;

	camera_angle_x = 0;
	camera_angle_y = 0;
}

void igvCamara::set(tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V,
	double _xwmin, double _xwmax, double _ywmin, double _ywmax, double _znear, double _zfar) {
	tipo = _tipo;

	P0 = _P0;
	r = _r;
	V = _V;

	xwmin = _xwmin;
	xwmax = _xwmax;
	ywmin = _ywmin;
	ywmax = _ywmax;
	znear = _znear;
	zfar = _zfar;

	camera_angle_x = 0;
	camera_angle_y = 0;
}

void igvCamara::set(tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V,
	double _angulo, double _raspecto, double _znear, double _zfar) {
	tipo = _tipo;

	P0 = _P0;
	r = _r;
	V = _V;

	angulo = _angulo;
	raspecto = _raspecto;
	znear = _znear;
	zfar = _zfar;

	camera_angle_x = 0;
	camera_angle_y = 0;
}

void igvCamara::orbital(float valX, float valY) {
	this->camera_angle_x = camera_angle_x + valX;
	this->camera_angle_y = camera_angle_y + valY;

	float _x = 5.0f * sin(camera_angle_x * 3.1415 / 180.0f);
	float _y = 5.0f * sin(camera_angle_y * 3.1415 / 180.0f);
	float _z = 5.0f * cos(camera_angle_x * 3.1415 / 180.0f);

	igvPunto3D _p0 = igvPunto3D(_x, _y, _z);

	this->P0 = _p0;
}

void igvCamara::aplicar(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (tipo == IGV_PARALELA) {
		glOrtho(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	}

	if (tipo == IGV_FRUSTUM) {
		glFrustum(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	}

	if (tipo == IGV_PERSPECTIVA) {
		if (znear <= 0) {
			gluPerspective(angulo * this->porcentajeZoom, raspecto, 1, zfar);
		}
		else {
			gluPerspective(angulo * this->porcentajeZoom, raspecto, znear, zfar);
		}
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(P0[X], P0[Y], P0[Z], r[X], r[Y], r[Z], V[X], V[Y], V[Z]);
}

void igvCamara::zoom(double factor) {
	if (tipo != IGV_PERSPECTIVA) {
		set(tipo, P0, r, V,
			-porcentajeZoom * 3, porcentajeZoom * 3,
			-porcentajeZoom * 3, porcentajeZoom * 3,
			znear, zfar);
	}

	aplicar();
}