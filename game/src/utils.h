#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <utility>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>



#define PATH_SEP '/'
std::string getResourcePath(const std::string &subDir = "");

template<typename T, typename... Args>
void cleanup(T *t, Args&&... args){
	//Cleanup the first item in the list
	cleanup(t);
	//Clean up the remaining arguments
	cleanup(std::forward<Args>(args)...);
}
template<>
inline void cleanup<SDL_Window>(SDL_Window *win){
	if (!win){
		return;
	}
	SDL_DestroyWindow(win);
}
template<>
inline void cleanup<SDL_Renderer>(SDL_Renderer *ren){
	if (!ren){
		return;
	}
	SDL_DestroyRenderer(ren);
}
template<>
inline void cleanup<SDL_Texture>(SDL_Texture *tex){
	if (!tex){
		return;
	}
	SDL_DestroyTexture(tex);
}
template<>
inline void cleanup<SDL_Surface>(SDL_Surface *surf){
	if (!surf){
		return;
	}
	SDL_FreeSurface(surf);
}

inline void logSDLError(std::ostream &os, const std::string &msg) {
  os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *ren);

SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *ren); 

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h); 

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr);

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr);


#endif
