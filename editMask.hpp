#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void set_pixel(SDL_Surface *surface, int s, int x, int y, Uint32 pixel)
{
  SDL_Rect Dest;
  Dest.x = x-s/2;
  Dest.y = y-s/2;
  Dest.w = s;
  Dest.h = s;
  if(SDL_FillRect(surface, &Dest, pixel) != 0){
	  std::cout<<"Failed to fill rectangle in surface"<<std::endl;
  }
  
  //Uint32 *target_pixel = (Uint32*)(((Uint8 *) surface->pixels) + y * surface->pitch + x * sizeof *target_pixel);
  //*target_pixel = pixel;
  //std::cout<<"Filled with color: 0x";
  //std::cout<<std::hex<<pixel<<std::endl;
  
}


void set_pixel(SDL_Surface* surface, int s, int x, int y, int r, int g, int b, int a){
	set_pixel(surface, s, x, y, SDL_MapRGBA(surface->format, r, g, b, a));
}


