

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"


void quit (int rc) {
  SDL_Quit();
  exit(rc);
}

int pattern[256][256];

int get_color_rgb(int u, int v) {
  return ((2 * u) & 255)<<16 | ((3 * v) & 255)<<8 | ((u+v) & 255);
}

void make_table(){
  int i,j;
  for (i = 0; i < 256; i++) {
    for (j = 0; j < 256; j++) {
      pattern[i][j] = get_color_rgb(i, j);
    }
  }
}


void draw_screen(int* scr, int pitch, int w, int h, int half_width, int half_height) {
  // Get the number of milliseconds since program start, then scale it back
  // to appropiate values to give neither too fast nor too slow movement
  float ticks = SDL_GetTicks();
  float t1 = ticks * 0.00010f;
  float t2 = ticks * 0.00030f;
  //printf("\nt1=%f,t2=%f; ", t1, t2);
  // Generate movement parameters:
  // - Zooming level can vary from 2x to 6x
  // - Rotation angle follows a sinusoidal movement with about -720 to +720
  //   degrees
  float zoom = 4.0f + 2.0f * sinf(t2);
  float angle = 2.0 * 3.1415f * sinf(t1);
  float zoom_cosf = zoom * cosf(angle);
  float zoom_sinf = zoom * sinf(angle);
  float half_w_zoom_cosf = half_width * zoom_cosf;
  float half_w_zoom_sinf = half_width * zoom_sinf;
  float half_h_zoom_cosf = half_height * zoom_cosf;
  float half_h_zoom_sinf = half_height * zoom_sinf;
  float u_offset = half_w_zoom_cosf + half_h_zoom_sinf;
  float v_offset = half_w_zoom_sinf + half_h_zoom_cosf;

  int y;
  int x;
  float u, v;
  float temp_u = 0 - zoom_sinf - u_offset;
  float temp_v = 0 - zoom_cosf - v_offset; //first loop y * zoom_cosf need be 0
  int one_fourth_pitch = pitch / 4;
  int y_pitch = 0 - one_fourth_pitch;
  for ( y = 0 ; y < h ; y++ ) {
    y_pitch += one_fourth_pitch;
    temp_u += zoom_sinf;
    temp_v += zoom_cosf;
    u = temp_u - zoom_cosf;
    v = temp_v + zoom_sinf;
    for ( x = 0 ; x < w ; x++ ) {
      u += zoom_cosf;
      v -= zoom_sinf;
      scr[x + y_pitch] = pattern[(int)u & 255][(int)v & 255];
    }
  }
}


void handle_screen(SDL_Surface* screen, int half_width, int half_height) {
  draw_screen((int*)(screen->pixels), screen->pitch, screen->w, screen->h, half_width, half_height);
  SDL_Flip(screen);
}


int main (int argc, char *argv[]) {
  SDL_Surface* screen;
  Uint32 videoflags;
  int i;
  int done;

  if (SDL_Init (SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  videoflags = SDL_SWSURFACE;
  while (argc > 1) {
    --argc;
    if (argv[argc] && !strcmp (argv[argc], "-fullscreen")) {
      videoflags |= SDL_FULLSCREEN;
    }
    else {
      fprintf(stderr, "Usage: %s [-fullscreen]\n", argv[0]);
      quit(1);
    }
  }

  make_table();

  const int width  = 160;
  const int height = 128;
  const int half_width = width / 2;
  const int half_height = height / 2;
  screen = SDL_SetVideoMode(width, height, 0, videoflags);
  if (!screen) {
    fprintf(stderr, "Couldn't set %dx%d video mode: %s\n", width, height, SDL_GetError());
    quit(2);
  }

  /* Enable UNICODE translation for keyboard input */
  SDL_EnableUNICODE(1);

  /* Enable auto repeat for keyboard input */
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  // Event loop
  for( i = 0, done = 0 ; !done && i < 1000; i++) {
    static int last_tick;
    // Rate limit the loops to 30fps. Every loop updates the screen,
    // handles the keyboard event and triggers the step sound if needed.
    last_tick = SDL_GetTicks();
    int curr_tick = SDL_GetTicks();
    int tick_difference = last_tick + (1000 / 30) - curr_tick;
    if ( tick_difference > 0 ) {
      SDL_Delay(tick_difference);
    }
    handle_screen(screen, half_width, half_height);
    SDL_Event event;
    while ( SDL_PollEvent (&event) ) {
      switch (event.type) {
      case SDL_KEYDOWN: {
        int sym = event.key.keysym.sym;
        if ( sym == SDLK_ESCAPE ) {
          done = 1;
        } else if ( event.key.keysym.sym == SDLK_UP ) {
        } else if ( event.key.keysym.sym == SDLK_DOWN ) {
        } else if ( sym == '0' ) {
        } else if ( sym >= '1' && sym <= '9') {
        } else {
        }
        break;
      }
      case SDL_KEYUP:
        break;
      case SDL_QUIT:
        done = 1;
        break;
      default:
        break;
      }
    }
  }

  SDL_Quit ();
  return (0);
}
