#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "BaseState.h"
#include "Renderer.h"
//...
#include "BaseState.h"

class Game
{
public:
    Game();
    ~Game();

    void changeState(BaseState* state);

    bool initWindow();
    void run();
    void cleanup();
    
    void handleEvent(SDL_Event e, bool& exit);

private:
    BaseState* mActiveState;
    Renderer* mRenderer;
    Mix_Music *mMusic;
};

#endif
