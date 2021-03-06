#ifndef BUTTON_H
#define BUTTON_H

#include "Vec2.h"
#include "Texture.h"
#include "Renderer.h"
#include <SDL2/SDL.h>
#include <string>

class Button
{
public:
  Button(Vec2 pos, int width, int height);
  //Button(Vec2 pos, int width, int height, std::string str);
  virtual ~Button();
  
  virtual void draw(Renderer* renderer, std::string newString = "");
  
  bool mouseOver(Vec2 mousePos);
  void setPosition( Vec2 NewPos ){ mPos = NewPos; }
  bool getHighlightOn(){ return mHighlightOn; }
  void setHighlightOn(){ mHighlightOn = !mHighlightOn; }
  virtual std::string getFilename() {return " ";}
  SDL_Rect getRect();
  
private:
    Vec2 mPos;
    int mWidth, mHeight;
    bool mHighlightOn = false;
};
  
class ButtonText : public Button
{
public:
    ButtonText(Vec2 pos, int width, int height, std::string str, const int color1, const int color2, const int color3);
    ~ButtonText();
    
    void draw(Renderer* renderer, std::string newString = "");
    std::string getFilename() {return mFilePath;}
    
private:
    const std::string mFilePath;
    const int mcolor1, mcolor2, mcolor3;
    Texture* mText = nullptr;
    Texture* mHighlight = nullptr;
};

class ButtonImg : public Button
{
public:
    ButtonImg(Vec2 pos, int width, int height, const std::string str);
    ~ButtonImg();
    
    void draw(Renderer* renderer, std::string newString = "");
    std::string getFilename(){  return mFilePath; } 
private:
    Texture* mTexture = nullptr;
    const std::string mFilePath;
};

#endif