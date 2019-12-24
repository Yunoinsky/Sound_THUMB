#include <iostream>
#include <SDL2/SDL.h>
#include "./Audio/miniaudio/dr_wav.h"
#include "./Audio/miniaudio/dr_mp3.h"
#include "./Audio/miniaudio/miniaudio.h"
#include "utils.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT  480
#define TILE_SIZE 40

static SDL_Window *win;
static SDL_Renderer *ren;
static std::string resPath;

// SDL system init
inline int init_SDL() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    logSDLError(std::cout, "SDL_Init");
    return 1;
  }
  win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (win == nullptr) {
    logSDLError(std::cout, "SDL_CreateWindow");
    SDL_Quit();
    return 1;
  }
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr) {
    logSDLError(std::cout, "SDL_CreateRenderer");
    cleanup(win);
    SDL_Quit();
    return 1;
  }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
    logSDLError(std::cout, "IMG_Init");
    SDL_Quit();
    return 1;
  }
  if (TTF_Init() != 0) {
    logSDLError(std::cout, "TTF_Init");
    SDL_Quit();
    return 1;
  }
  resPath = getResourcePath();
  SDL_ShowCursor(0);
  return 0;
}
inline void term_SDL() {
  cleanup(ren, win);
  IMG_Quit();
  SDL_Quit();
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
  ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
  if (pDecoder == NULL) {
    return;
  }
  ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);
  
  (void)pInput;
}

SDL_Thread *thread;
static int TestThread(void *ptr) {
  ma_decoder decoder;
  ma_result mresult;
  ma_device_config devconf;
  ma_device dev;
  mresult = ma_decoder_init_file((resPath+"music_1.mp3").c_str(), NULL, &decoder);
  if (mresult != MA_SUCCESS) {
    std::cout << "Audio Wrong!" << std::endl;
    return 1;
  }
  devconf = ma_device_config_init(ma_device_type_playback);
  devconf.playback.format = decoder.outputFormat;
  devconf.playback.channels = decoder.outputChannels;
  devconf.sampleRate = decoder.outputSampleRate;
  devconf.dataCallback = data_callback;
  devconf.pUserData = &decoder;
  if (ma_device_init(NULL, &devconf, &dev) != MA_SUCCESS) {
    std::cout << "Failed to open playback device!" << std::endl;
    ma_decoder_uninit(&decoder);
    return 1;
  }

  if (ma_device_start(&dev) != MA_SUCCESS) {
    std::cout << "Failed to start playback device!" << std::endl;
    ma_device_uninit(&dev);
    ma_decoder_uninit(&decoder);
    return 1;   
  }
  
  printf("Press Enter to quit ...");
  getchar();
  ma_device_uninit(&dev);
  ma_decoder_uninit(&decoder);
  return 0;
}
inline void game_init() {
  thread = SDL_CreateThread(TestThread, "TestTHread", (void *)NULL);
  return;
}

inline int game_update() {
  static int result_quit = 1;
  static SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) result_quit = 0;
    else if (e.type == SDL_KEYDOWN) result_quit = 0;
  }
  return result_quit;
}

inline void game_draw() {
  
}

int main(int argc, char ** argv) {
  if(init_SDL()) return 1;
  game_init();
  while(game_update()){
    game_draw();
    SDL_Delay(20);
  }
  return 0;
}
