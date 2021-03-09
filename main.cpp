#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "openFile.hpp"
#include "editMask.hpp"
#include "reportError.hpp"

int main()
{
	std::string imageName = openFile();
	
	if(imageName == ""){
		std::cout<<"Please select an image and try again."<<std::endl;
		tinyfd_messageBox("Warning", "Please select an image and try again.", "ok", "warning" , 1);
		return(-1);
	}
	
	SDL_Event event;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library"<<std::endl;
	reportError("Failed to initialize the SDL2 library");
        return -1;
    }
    IMG_Init(IMG_INIT_PNG);

	SDL_Window *window1 = SDL_CreateWindow("SDL2 Window1",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          680, 480,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	 SDL_Window *window2 = SDL_CreateWindow("SDL2 Window2",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          680, 480,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if(!window1 || !window2)
    {
        std::cout << "Failed to create window\n";
	reportError("Failed to create window");
        return -1;
    }

    SDL_Renderer* renderer1 = SDL_CreateRenderer(window1, -1 ,0);
    SDL_Renderer* renderer2 = SDL_CreateRenderer(window2, -1 ,0);
	
    if(!renderer1 || !renderer2){
	    std::cout<< "Failed to create renderer" <<std::endl;
	    reportError("Failed to create renderer");
	    return(-1);
    }
	SDL_SetRenderDrawColor(renderer1, 0, 0, 0, 0);
	SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 0);
    SDL_Surface* Image = IMG_Load(imageName.c_str());

    if(!Image){
	    std::cout<<"Failed to load image"<<std::endl;
	    reportError("Failed to load image");
	    return(-1);
    }



#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  int rmask = 0xff000000 >> shift;
  int gmask = 0x00ff0000 >> shift;
  int bmask = 0x0000ff00 >> shift;
  int amask = 0x000000ff >> shift;
#else // little endian, like x86
  int rmask = 0x000000ff;
  int gmask = 0x0000ff00;
  int bmask = 0x00ff0000;
  int amask = 0xff000000;
#endif

    SDL_Surface* Mask = SDL_CreateRGBSurface(SDL_PIXELFORMAT_RGBA8888, Image->w, Image->h, 32, rmask , gmask, bmask, amask);
   if(!Mask){
	   std::cout<<"Failed to create image mask"<<std::endl;
	   reportError("failed to create image mask");
	   return(-1);
   }

   SDL_FillRect(Mask, NULL, SDL_MapRGBA(Mask->format, 0, 0, 0, 255));

    SDL_Surface* Selection = SDL_CreateRGBSurface(SDL_PIXELFORMAT_RGBA8888, Image->w, Image->h, 32, rmask , gmask, bmask, amask);
   if(!Selection){
	   std::cout<<"Failed to create image selection"<<std::endl;
	   reportError("Failed to create image selection");
	   return(-1);
   }

   SDL_FillRect(Selection, NULL, SDL_MapRGBA(Mask->format, 0, 0, 0, 0));

   SDL_SetSurfaceBlendMode(Image, SDL_BLENDMODE_BLEND);
   SDL_SetSurfaceBlendMode(Mask, SDL_BLENDMODE_NONE);



    bool quit = false;

    int fillAmount = 30;

    while(!quit){
	while (SDL_PollEvent(&event))
	{
	    if (event.type == SDL_WINDOWEVENT
	        && event.window.event == SDL_WINDOWEVENT_CLOSE)
	    {
	        quit = true;
	    }else if(event.type == SDL_MOUSEWHEEL){
		if(event.wheel.y > 0){
			fillAmount+=event.wheel.y*3;
		}else if(event.wheel.y < 0){
			fillAmount+=event.wheel.y*3;
			if(fillAmount < 0){
				fillAmount = 0;
			}
		}
	    }
}
	
	int mouseX;
	int mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);

	SDL_Window* focusWindow = SDL_GetMouseFocus();
	int focusWindowX;
	int focusWindowY;
	SDL_GetWindowSize(focusWindow, &focusWindowX, &focusWindowY);
	
	int ajustedMouseX = (int)(((float)mouseX/(float)focusWindowX)*(float)Image->w);
	int ajustedMouseY = (int)(((float)mouseY/(float)focusWindowY)*(float)Image->h);

	if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
		std::cout<<"Found mouse button 1 is pressed at: (";
		std::cout<<mouseX;
		std::cout<<", ";
		std::cout<<mouseY;
		std::cout<<")"<<std::endl;

		set_pixel(Mask, fillAmount, ajustedMouseX, ajustedMouseY, 255, 255, 255, 0);
	}

	if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)){
		std::cout<<"Found mouse button 1 is pressed at: (";
		std::cout<<mouseX;
		std::cout<<", ";
		std::cout<<mouseY;
		std::cout<<")"<<std::endl;
	
		set_pixel(Mask, fillAmount, ajustedMouseX, ajustedMouseY, 0, 0, 0, 255);
	}
	SDL_FillRect(Selection, NULL, SDL_MapRGBA(Mask->format, 0, 0, 0, 0));

	set_pixel(Selection, fillAmount, ajustedMouseX, ajustedMouseY, 255, 0, 0, 100);

	SDL_RenderClear(renderer1);
	SDL_RenderClear(renderer2);


	SDL_Texture* ImageTex1 = SDL_CreateTextureFromSurface(renderer1, Image);
	SDL_Texture* ImageTex2 = SDL_CreateTextureFromSurface(renderer2, Image);

	SDL_Texture* SelectionTex = SDL_CreateTextureFromSurface(renderer1, Selection);

	SDL_Texture* MaskTex1 = SDL_CreateTextureFromSurface(renderer1, Mask);
	SDL_Texture* MaskTex2 = SDL_CreateTextureFromSurface(renderer2, Mask);
	
	SDL_SetTextureBlendMode(ImageTex1, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(ImageTex2, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(MaskTex1, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(MaskTex2, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(SelectionTex, SDL_BLENDMODE_BLEND);

	if(SDL_SetTextureAlphaMod(MaskTex1, 100) != 0){
		std::cout<<"Failed to set Alpha value of mask"<<std::endl;
	}

	SDL_RenderCopy(renderer1, ImageTex1, NULL, NULL);
	SDL_RenderCopy(renderer2, ImageTex2, NULL, NULL);

	SDL_RenderCopy(renderer1, MaskTex1, NULL, NULL);
	SDL_RenderCopy(renderer2, MaskTex2, NULL, NULL);

	SDL_RenderCopy(renderer1, SelectionTex, NULL, NULL);

	SDL_RenderPresent(renderer1);
	SDL_RenderPresent(renderer2);

	SDL_DestroyTexture(ImageTex1);
	SDL_DestroyTexture(ImageTex2);

	SDL_DestroyTexture(MaskTex1);
	SDL_DestroyTexture(MaskTex2);

	SDL_DestroyTexture(SelectionTex);

    }
    SDL_FreeSurface(Image);
    SDL_FreeSurface(Mask);
    SDL_FreeSurface(Selection);

    SDL_DestroyRenderer(renderer1);
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyWindow(window1);
    SDL_DestroyWindow(window2);

    IMG_Quit();
    SDL_Quit();

    return(0);
}
