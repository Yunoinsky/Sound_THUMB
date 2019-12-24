#include "utils.h"

std::string getResourcePath(const std::string &subDir) {
  static std::string baseRes;
  if (baseRes.empty()) {
    char* basePath = SDL_GetBasePath();
    if (basePath) {
      baseRes = basePath;
      SDL_free(basePath);
    } else {
      std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
      return "";
    }
    size_t pos = baseRes.rfind("build");
    baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
  }
  return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren) {
  SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
  if (texture == nullptr) {
    logSDLError(std::cout, "LoadTexture");
  }
  return texture;
}

SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *ren) {
  TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
  if (font == nullptr) {
    logSDLError(std::cout, "TTF_OpenFont");
    return nullptr;
  }
  SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
  if (surf == nullptr) {
    TTF_CloseFont(font);
    logSDLError(std::cout, "TTF_RenderText");
    return nullptr;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
  if (texture == nullptr) {
    logSDLError(std::cout, "CreateTexture");
  }
  SDL_FreeSurface(surf);
  TTF_CloseFont(font);
  return texture;
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopy(ren, tex, NULL, &dst);
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip) {
  SDL_RenderCopy(ren, tex, clip, &dst);
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip) {
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  if (clip != nullptr) {
    dst.w = clip->w;
    dst.h = clip->h;
  } else {
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
  }
  renderTexture(tex, ren, dst, clip);
}
  
