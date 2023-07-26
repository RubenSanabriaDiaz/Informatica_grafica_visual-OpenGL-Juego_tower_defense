#include "igvTextura.h"

// Metodos constructores y destructor
igvTextura::igvTextura(char *name) {
	ancho = 0;
	alto = 0;

	SDL_Surface* imagen;

	glEnable(GL_TEXTURE_2D);

	if (!glIsTexture(idTextura)) {
		imagen = IMG_Load(name);

		if (!imagen){
			printf("Error al cargar la imagen: %s\n", IMG_GetError());
		}
		else {
			idTextura = 1;

			glBindTexture(GL_TEXTURE_2D, idTextura);

			SDL_Surface* iRGBA = SDL_ConvertSurfaceFormat(imagen, SDL_PIXELFORMAT_RGBA32, 0);

			if (!iRGBA)
			{
				printf("Error al convertir la imagen a formato RGBA: %s\n", SDL_GetError());
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iRGBA->w, iRGBA->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, iRGBA->pixels);

				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				SDL_FreeSurface(iRGBA);
				SDL_FreeSurface(imagen);
			}
		}
	}
}

void igvTextura::aplicar(void) {
  glBindTexture(GL_TEXTURE_2D, idTextura);
}

igvTextura::~igvTextura() {
  glDeleteTextures(1, &idTextura); 
}
