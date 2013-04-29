

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"


static void quit (int rc) {
  SDL_Quit();
  exit(rc);
}

static int get_color_r(int u, int v) {
  return 2 * u;
}

static int get_color_g(int u, int v) {
  return 3 * v;
}

static int get_color_b(int u, int v) {
  return u + v;
}

static void draw_screen_pixel(int* scr, int pitch, int x, int y, int color) {
  // pitch is defined in term of bytes, but the scr[] array is defined in term
  // of int == 4 bytes. Therefore we need to divide the pitch by 4 before using
  // it to address a pixel.
  scr[x + (y * pitch / 4)] = color;
}

static void draw_pixel(int* scr, int pitch, int w, int h, float zoom, float angle, int x, int y) {
  int u = 0 + (x - (w / 2)) * zoom * cosf(angle) + (y - (h / 2)) * zoom * sinf(angle);
  int v = 0 - (x - (w / 2)) * zoom * sinf(angle) + (y - (h / 2)) * zoom * cosf(angle);
  // keep only lower 8 bits of each color value
  int color_r = get_color_r(u,v) & 255;
  int color_g = get_color_g(u,v) & 255;
  int color_b = get_color_b(u,v) & 255;
  int color_rgb = color_r << 16 | color_g << 8 | color_b;
  draw_screen_pixel(scr, pitch, x, y, color_rgb);
}


static void draw_line(int* scr, int pitch, int w, int h, float zoom, float angle, int y) {
  int x;
  for ( x = 0 ; x < w ; x++ ) {
    draw_pixel(scr, pitch, w, h, zoom, angle, x, y);
  }
}


static void draw_screen(int* scr, int pitch, int w, int h) {
  // Get the number of milliseconds since program start, then scale it back
  // to appropiate values to give neither too fast nor too slow movement
  float ticks = SDL_GetTicks();
  float t1 = ticks * 0.00010f;
  float t2 = ticks * 0.00030f;
  // Generate movement parameters:
  // - Zooming level can vary from 2x to 6x
  // - Rotation angle follows a sinusoidal movement with about -720 to +720
  //   degrees
  float zoom = 4.0f + 2.0f * sinf(t2);
  float angle = 2.0 * 3.1415f * sinf(t1);
  int y;
  for ( y = 0 ; y < h ; y++ ) {
    draw_line(scr, pitch, w, h, zoom, angle, y);
  }
}


static void handle_screen(SDL_Surface* screen) {
  draw_screen((int*)(screen->pixels), screen->pitch, screen->w, screen->h);
  SDL_Flip(screen);
}


int main (int argc, char *argv[]) {
  SDL_Surface* screen;
  Uint32 videoflags;
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

  const int width  = 160;
  const int height = 128;
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
  for( done = 0 ; !done ; ) {
    static int last_tick;
    // Rate limit the loops to 30fps. Every loop updates the screen,
    // handles the keyboard event and triggers the step sound if needed.
    last_tick = SDL_GetTicks();
    int curr_tick = SDL_GetTicks();
    int tick_difference = last_tick + (1000 / 30) - curr_tick;
    if ( tick_difference > 0 ) {
      SDL_Delay(tick_difference);
    }
    last_tick = SDL_GetTicks();
    handle_screen(screen);
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
