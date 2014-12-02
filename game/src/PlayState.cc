#include <iostream>
#include <sstream>
#include <iomanip>
#include "PlayState.h"
#include "Level.h"
#include "Renderer.h"
#include "Platform.h"
#include "Player.h"
#include "Object.h"
#include "Game.h"
#include "Enemy.h"
#include <iostream>
#include "Highscores.h"

PlayState::PlayState()
{
    mTestBkgd = nullptr;
    mPlayer = nullptr;
    mPauseMenu = nullptr;;
    boostSeconds = SPEEDBOOSTSEC;
    boostEnable = false;
    mPaused = false;
	mTimeOnScreen = nullptr;
	mKilledCreaturesScreen = nullptr;
	PowerupTimer = nullptr;
	FPSTimer = nullptr;
    mTimer.start();
    mHighscores = new Highscores("highscores.txt");
}

PlayState::~PlayState()
{
    for (unsigned int i = 0; i < buttonList.size(); i++)
		delete buttonList.at(i);

    mHighscores->save();
    
    delete mLevel;
    delete mTestBkgd;
    delete mPauseMenu;
    delete mHighscores;
	delete FPSTimer;
}

void PlayState::init(string initData) // initData will be the filename of the level
{
    mLevel = new Level();
    mLevel->loadLevel(initData, 1); // 1 för att det är playstate
    mPlayer = mLevel->findPlayer();
	mTimeOnScreen = new ButtonText(Vec2(20, 20), 80, 40, " ", 250,250,250);
	mKilledCreaturesScreen = new ButtonText(Vec2(20, 55), 70, 30, " ", 250,250,250);
	PowerupTimer = new ButtonText(Vec2((SCREEN_WIDTH/2)-75, 10), 150, 75, " ", 174,0,0);
	FPSTimer = new ButtonText(Vec2(SCREEN_WIDTH - 90, 10), 70, 25, " ", 250,250,250);
	mTimer.reset();
	buttonList = {
		new ButtonImg(Vec2(0, 0), 1024, 768, PAUSE_BACKGROUND),
		new ButtonImg(Vec2((SCREEN_WIDTH/2)-100, 310), 200, 40, CONTINUE_BUTTON),
		new ButtonImg(Vec2((SCREEN_WIDTH/2)-60, 370), 120, 40, TOMENU_BUTTON),
		new ButtonImg(Vec2((SCREEN_WIDTH/2)-24, 430), 48, 48, VOLUME_BUTTON),
		new ButtonImg(Vec2((SCREEN_WIDTH/2)-24, 430), 48, 48, MUTE_BUTTON)
		
	};
	R = L = false;
}

void PlayState::cleanup()
{
    ;
}

void PlayState::update(float dt)
{	
    if(!mPaused) {
		mLevel->update(dt);
		
		if (boostEnable) // Enables when collision with powerup
			speedUp(); // speedUp varar i SPEEDBOOSTSEC sekunder

		
		if ( mLevel->getLevelFinish() ) { // Om spelaren klarat banan;
			if (getLvlUnlocks() == 0 && mLevel->getCurrentLevel() == FILEPATH_LVL1)
				incLvl();
			else if (getLvlUnlocks() == 1 && mLevel->getCurrentLevel() == FILEPATH_LVL2)
				incLvl();
			else if (getLvlUnlocks() == 2 && mLevel->getCurrentLevel() == FILEPATH_LVL3)
				incLvl();
			
			if (mLevel->getCurrentLevel() == FILEPATH_LVL4)
				setNextState(BaseState::CREDIT);
			else
				setNextState(BaseState::MENU_STATE);

			mHighscores->updateHighscore(mLevel->getCurrentLevel(), mTimer.getSeconds(), mPlayer->getScore());
			mHighscores->save();		       
		} 
		else if (mPlayer->getPosition().y + mPlayer->getHeight() > mLevel->getLevelSize().y || mPlayer->getDead()) // Trillar spelaren ned, ladda om banan
		    init(mLevel->getCurrentLevel()); // (Kan behövas ändras)
	}

    //cout << "timer: " << mTimer.getTimer() << endl;
}

void PlayState::draw(Renderer* renderer)
{
    /* draw test bkgd
    if(mTestBkgd != nullptr)
		renderer->drawTexture(Vec2(0, 0), mLevel->getLevelSize().x, mLevel->getLevelSize().y, mTestBkgd);
    else
	mTestBkgd = renderer->loadTexture("../imgs/backgrounds/skygrad.jpg"); */

    mLevel->draw(renderer);
    
    if (mPaused) {
		for (unsigned int i = 0; i < buttonList.size()-2; i++){
			if (  buttonList.at(i)->getFilename() != VOLUME_BUTTON || buttonList.at(i)->getFilename() != MUTE_BUTTON )
				buttonList.at(i)->draw(renderer);
		}
		if (!mSoundMuted)
				buttonList.at(buttonList.size()-2)->draw(renderer);
		else if ( mSoundMuted )
				buttonList.at(buttonList.size()-1)->draw(renderer);
    }

    // draw timer progress
    ostringstream currentTime, currentKilledCreatures, currentPowerupTime, currentFPS;
	currentTime << fixed << setw(7) << std::setprecision(1) << left<< mTimer.getSeconds();
    mTimeOnScreen->draw( renderer, currentTime.str() );
	
	currentKilledCreatures << setw(10) << left << "Score:" <<  left << mPlayer->getScore();
	mKilledCreaturesScreen->draw( renderer, currentKilledCreatures.str() );
	
	if (mPlayer->getPowerUp())
	{
		currentPowerupTime << fixed << setw(7) << std::setprecision(1) << left  << -1*(mPlayer->getTimer()-mLevel->getPowerupTime());
		PowerupTimer->draw( renderer, currentPowerupTime.str() );
	}
	
	currentFPS << fixed << setw(7) << std::setprecision(1) << left << "FPS:" <<  left << renderer->getAvgFPS();
	FPSTimer->draw( renderer, currentFPS.str() );
	
}

void PlayState::handleEvent(SDL_Event e, bool& exit, bool& muteSound)
{
	mSoundMuted = muteSound;
    if(!mPaused)
    {
		//If a key was pressed
		if( e.type == SDL_KEYDOWN && e.key.repeat == 0 && !mPaused) 
		{
			//Adjust the velocity
			switch( e.key.keysym.sym )
			{
			case SDLK_SPACE:
			    if(!mPlayer->getjump() && !mPlayer->getfall())
			    {
					mPlayer->setjump(1);
			    }
			break;
			case SDLK_LEFT:
					mPlayer->move(1);
					L = true;
			break;
			case SDLK_RIGHT:
					mPlayer->move(2);
					R = true;
			break;
			default: break;
			}

			// ENTER PAUSE MENU
			if(e.key.keysym.sym == SDLK_ESCAPE)
			{
			    mPaused = !mPaused;
			    mTimer.pause();
				if (L)
					{
						mPlayer->move(3);
					}
				if (R)
					{
						mPlayer->move(4);
					}
			}
		}
		//If a key was released
		else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
		{
				//Adjust the velocity
				switch( e.key.keysym.sym )
				{
				case SDLK_LEFT:
					if (L)
					{
						mPlayer->move(3);
						L = false;
					}
				break;
				case SDLK_RIGHT:
					if (R)
					{
						mPlayer->move(4);
						R = false;
					}
				break;
				default: break;
				}
		}
    }
    else
    {
		if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
			if(e.key.keysym.sym == SDLK_ESCAPE) // GÅ TILLBAKA TILL SPELET VIA ESCAPE
			{
			    mPaused = !mPaused;
			    mTimer.start();
				L = R = false;
			}
		
		if(e.type == SDL_MOUSEBUTTONDOWN) // Kolla musknappnedtryck
		{
			SDL_GetMouseState(&mousePos.x, &mousePos.y);
			for (unsigned int i = 0; i < buttonList.size(); i++) {
			
				if (buttonList.at(i)->mouseOver(mousePos)) {
					switch (i) {
					case 1:
						mPaused = !mPaused;
						mTimer.start();
						L = R = false;
						break;
					case 2:
						setNextState(BaseState::MENU_STATE);
						break;
					case 3:
						muteSound = !muteSound;
					case 4:
						muteSound = !muteSound;
						break;
					default:
						break;
					}

				}

			}

		}

	}
    
}


void PlayState::setSpeed(float s)
{
	speed = s;
}

float PlayState::getSpeed()
{
	return speed;
}

void PlayState::setTimer(Uint32 t)
{
    //mTimer = t;
}

void PlayState::speedUp()
{
/*	if (boostSeconds > 0) {
		setSpeed(BOOSTUPSPEED);
		mTimer = SDL_GetTicks();
		if (mTimer > mLastTime + 1000) {
			boostSeconds--;
			mLastTime = mTimer;
		}
			
	} else {
		// Enough with the fun
		boostEnable = false;
		speed = DEFAULTSPEED;
		boostSeconds = SPEEDBOOSTSEC;
		}*/
}
