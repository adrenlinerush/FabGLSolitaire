#include "fabgl.h"
#include "global.h"
#include "card.cpp"
#include "deck.cpp"
//#include "stack.cpp"
  



Deck deck;

void title()
{
  canvas.selectFont(&fabgl::FONT_8x8);
  canvas.setPenColor(COLOR_WHITE);
  canvas.drawText(450,25,"Solitaire");
  canvas.drawText(450,50,"By: Austin Mount");
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Solitaire by Austin Mount");
  VGAController.begin();
  //VGAController.setResolution(VGA_640x480_60HzAlt1, 640, 480);
  //VGAController.setResolution(VGA_640x480_60HzAlt1);
  VGAController.setResolution(VGA_640x350_70Hz);
  PS2Controller.begin();
  PS2Controller.mouse()->setupAbsolutePositioner(canvas.getWidth(), canvas.getHeight(), true, &VGAController);
  VGAController.setMouseCursor((CursorName)0);
  title();
  //canvas.selectFont(&fabgl::FONT_4x6);
  deck.init();
}

void loop()
{
  MouseStatus status = PS2Controller.mouse()->getNextStatus(-1);
  canvas.selectFont(&fabgl::FONT_8x8);
  canvas.setPenColor(COLOR_WHITE);
  canvas.setBrushColor(COLOR_BLACK);
  canvas.fillRectangle(450,75,600,85);
  canvas.drawTextFmt(450, 75, " %d %d ", status.X, status.Y);
  if (status.buttons.left) {
    //canvas.drawText(450,100,"Left Click");
    deck.check_click(status.X, status.Y);
  }
  else if (status.buttons.right) {
    ESP.restart();
  }
  // check if won here...
  bool won = deck.check_win();
  if (won)
  {
    canvas.selectFont(&fabgl::FONT_8x8);
    canvas.setPenColor(COLOR_WHITE);
    canvas.drawText(250,150,"YOU WON!!!!");
  }
  //delay(1000);
}


