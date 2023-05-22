#include "fabgl.h"
#include "global.h"
#include "card.cpp"
#include "deck.cpp"
//#include "stack.cpp"
  


//Deck deck = NULL;
//auto deck;

void setup()
{
  Serial.begin(115200);
  Serial.println("Solitaire by Austin Mount");
  VGAController.begin();
  VGAController.setResolution(VGA_640x480_60Hz, 640, 480);
  canvas.selectFont(&fabgl::FONT_4x6);
  //Card testClub(CLUB, 1, 25, 25, true, true, false);
  //Card testHeart(HEART, 10, 50, 25, false, true, false);
  //Card testDiamond(DIAMOND, 11, 75, 25, false, false, false);
  //Card testSpade(SPADE, 12, 100, 25, false, true, true);
  //deck = new Deck;
}

void loop()
{
  delay(5000);
  Deck deck;
  deck.init();
  while (true)
  {
    deck.deck_turn(3);
    delay(1000);
  }
}

