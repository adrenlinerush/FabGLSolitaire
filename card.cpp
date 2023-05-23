#include "fabgl.h"
#include <string> 
#include "suites.cpp"

extern fabgl::VGAController VGAController;
extern fabgl::Canvas canvas;

#define CLUB 0
#define HEART 1
#define DIAMOND 2
#define SPADE 3

#define COLOR_RED          RGB888(0xff, 0x00, 0x00)
#define COLOR_LIGHT_RED    RGB888(0xf0, 0x80, 0x80)
#define COLOR_BLACK        RGB888(0x00, 0x00, 0x00)
#define COLOR_GRAY         RGB888(0x80, 0x80, 0x80)
#define COLOR_BLUE         RGB888(0x00, 0x00, 0xff)
#define COLOR_WHITE        RGB888(0xff, 0xff, 0xff)
#define COLOR_BACKGROUND   RGB888(0x00, 0x00, 0x00) //BLACK

#define CARD_HEIGHT        45
#define CARD_WIDTH         30

class Card {
  public:
    Card(int suit, int value, int x, int  y, bool highlight,  bool visible, bool selected);
    bool move(int x, int y);
    bool c_highlight;
    bool c_visible;
    bool c_selected;
    bool draw();
    int c_suit;
    int c_value;
    int c_x;
    int c_y;
  private:
    bool erase();
    std::__cxx11::string getText();
    bool setColors();
};

bool Card::move(int x, int y) 
{
  this->erase();
  c_x = x;
  c_y = y;
  this->draw();
  return 1;
}

Card::Card(int suit, int value, int x, int y, bool highlight, bool visible, bool selected)
{
  c_suit = suit;
  c_value = value;
  c_x = x;
  c_y = y;
  c_highlight = highlight;
  c_visible = visible;
  c_selected = selected;
  this->draw();
}

bool Card::erase() {
  canvas.setBrushColor(COLOR_BACKGROUND);
  canvas.fillRectangle(c_x,c_y,c_x+CARD_WIDTH,c_y+CARD_HEIGHT);
  return 1;
}

bool Card::draw() {
  this->setColors();
  canvas.fillRectangle(c_x,c_y,c_x+CARD_WIDTH,c_y+CARD_HEIGHT);
  canvas.drawRectangle(c_x,c_y,c_x+CARD_WIDTH,c_y+CARD_HEIGHT);
  if (c_visible)
  {
    std::__cxx11::string card_text = this->getText();
    canvas.drawText(c_x+5, c_y+5, card_text.data());
    canvas.drawText(c_x+(CARD_WIDTH-20), c_y+(CARD_HEIGHT-11), card_text.data());
    if (c_suit == CLUB){
      canvas.drawBitmap(c_x+13,c_y+5,&suite_club);
      canvas.drawBitmap(c_x+(CARD_WIDTH-10),c_y+(CARD_HEIGHT-11),&suite_club);
    }
    if (c_suit == HEART){
      canvas.drawBitmap(c_x+15,c_y+4,&suite_heart);
      canvas.drawBitmap(c_x+(CARD_WIDTH-10),c_y+(CARD_HEIGHT-11),&suite_heart);
    }
    if (c_suit == DIAMOND){
      canvas.drawBitmap(c_x+12,c_y+5,&suite_diamond); 
      canvas.drawBitmap(c_x+(CARD_WIDTH-10),c_y+(CARD_HEIGHT-11),&suite_diamond);
    }
    if (c_suit == SPADE){
      canvas.drawBitmap(c_x+13,c_y+5,&suite_spade);
      canvas.drawBitmap(c_x+(CARD_WIDTH-10),c_y+(CARD_HEIGHT-11),&suite_spade);
    }
  }
  return 1;
}

bool Card::setColors() {
  canvas.selectFont(&fabgl::FONT_4x6);
  if (c_visible) 
  {
    if (c_suit == SPADE or c_suit == CLUB)
    {
      if (c_highlight)
      {
        canvas.setPenColor(COLOR_WHITE);
        canvas.setBrushColor(COLOR_BLACK);
      }
      else if (c_selected)
      {
        canvas.setPenColor(COLOR_WHITE);
        canvas.setBrushColor(COLOR_GRAY);
      }
      else
      {
        canvas.setPenColor(COLOR_BLACK);
        canvas.setBrushColor(COLOR_WHITE);
      }
    }
    else
    {
      if (c_highlight)
      {
        canvas.setPenColor(COLOR_WHITE);
        canvas.setBrushColor(COLOR_RED);
      }
      else if (c_selected)
      {
        canvas.setPenColor(COLOR_WHITE);
        canvas.setBrushColor(COLOR_LIGHT_RED);
      }
      else
      {
        canvas.setPenColor(COLOR_RED);
        canvas.setBrushColor(COLOR_WHITE);
      }
    }
  }
  else 
  {
    canvas.setPenColor(COLOR_WHITE);
    canvas.setBrushColor(COLOR_BLUE);
  }
  return 1;
}

std::__cxx11::string Card::getText(){
  std::__cxx11::string textValue;
  if (c_value == 1) // Ace
  {
    textValue = "A";
  }
  else if (c_value == 11) // Jack
  {
    textValue = "J";
  }
  else if (c_value == 12) // Queen
  {
    textValue = "Q";
  }
  else if (c_value == 13) // King
  {
    textValue = "K";
  }
  else
  {
    textValue  = std::to_string(c_value);
  }
  return textValue;
}

