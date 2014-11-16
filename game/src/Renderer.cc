#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Renderer.h"
#include "Texture.h"
#include "Vec2.h"
using namespace std;

Renderer::Renderer()
{
    mRenderer = nullptr;
    mWindow = nullptr;
}

Renderer::~Renderer()
{
    // cleanup SDL
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
	TTF_CloseFont(mFont);
    mWindow = NULL;
    mRenderer = NULL;
	TTF_Quit();
    SDL_Quit();
}

bool Renderer::initSDL()
{
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0)
    {
	cerr << "ERROR: Kunde inte initiera SDL\n";
        success = false;
    }
    else
    {

	 // create window
        mWindow = SDL_CreateWindow("Rockblock II", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN/*|SDL_WINDOW_RESIZABLE*/);
        if(mWindow == NULL)
        {
            cerr << "ERROR: Kunde inte skapa fönster\n";
            success = false;;
        }else
        {
            mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED );
            if(mRenderer == NULL)
            {
                cerr << "ERROR: Kunde inte skapa renderer\n" << SDL_GetError();
				success = false;
            }else
            {
                // initialize renderer color
                SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                
				// initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) 
				{ 
					cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl; 
					success = false; 
				}
				
				mFont = TTF_OpenFont("../fonts/arial.ttf", 40);
				if(mFont == NULL)
				{
					cerr << "ERROR: Kunde inte skapa mFont\n" << SDL_GetError();
					success = false;
				}
			
                // initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

void Renderer::beginScene()
{
    SDL_RenderClear(mRenderer);
}

void Renderer::endScene()
{
     SDL_RenderPresent(mRenderer);
     SDL_Delay(1000/60); // 60 FPS
}

void Renderer::drawTexture(Vec2 pos, int width, int height, Texture* texture, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect SDLRect{pos.x, pos.y, width, height}; // mPosX - camX, mPosY - camY
    SDL_RenderCopy(mRenderer, texture->getData(), nullptr, &SDLRect);
	
	//if( clip != NULL )
	//{
		//SDLRect.x -= camera.x;
		//SDLRect.y -= camera.y;
	//}
	//SDL_RenderCopyEx( mRenderer, texture->getData(), clip, &SDLRect, angle, center, flip );
}

Texture* Renderer::loadTexture(std::string filename)
{
    //The final texture
    SDL_Texture* newTexture = NULL;
    
    SDL_Surface* tmpSurface = IMG_Load(filename.c_str() );
    if(tmpSurface == NULL)
    {
        cerr << "ERROR: Kunde inte öppna fil: " << filename << IMG_GetError() << endl;
    }else
    {
        //Convertera surface till screen format
        newTexture = SDL_CreateTextureFromSurface(mRenderer, tmpSurface);
        if(newTexture == NULL)
        {
            cerr << "ERROR: Misslyckades konvertera fil: " << filename << SDL_GetError() << endl;
        }
        SDL_FreeSurface(tmpSurface);
    }

    return new Texture(newTexture);
}

Texture* Renderer::loadTexture(const char text[], unsigned int color1, unsigned int color2, unsigned int color3)
{
   
	SDL_Texture* newTexture = NULL;
    SDL_Color color = { (unsigned char)color1, (unsigned char)color2, (unsigned char)color3 };
    
    SDL_Surface* tmpSurface = TTF_RenderText_Solid(mFont, text, color);
    if(tmpSurface == NULL)
    {
        cerr << "ERROR: Kunde inte ladda text: " << text << endl;
    }else
    {
        //Convertera surface till screen format
        newTexture = SDL_CreateTextureFromSurface(mRenderer, tmpSurface);
        if(newTexture == NULL)
        {
            cerr << "ERROR: Misslyckades konvertera text: " << text << SDL_GetError() << endl;
        }
        SDL_FreeSurface(tmpSurface);
    }

    return new Texture(newTexture);
}


void Renderer::updateCamera(int x, int y, int width, int height)
{
	//Center the camera over the dot
	camera.x = ( x + width / 2 ) - SCREEN_WIDTH / 2;
	camera.y = ( y + height / 2 ) - SCREEN_HEIGHT / 2;
	
	//Keep the camera in bounds
	if( camera.x < 0 )
		camera.x = 0;
	if( camera.y < 0 )
		camera.y = 0;
	if( camera.x > LEVEL_WIDTH - camera.w )
		camera.x = LEVEL_WIDTH - camera.w;
	if( camera.y > LEVEL_HEIGHT - camera.h )
		camera.y = LEVEL_HEIGHT - camera.h;
}
