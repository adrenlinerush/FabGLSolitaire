#include "fabgl.h"

extern fabgl::VGAController VGAController;
extern fabgl::Canvas canvas;

#define STACK_INPLAY 0
#define STACK_COMPLETE 1
#define Y_OFFSET 20

class Stack 
{
  public:
    Stack(int x, int y);
    Stack(int x, int y, int suit);
    bool add_card(Card card);
    //Card get_card(int position);
    bool rm_card(int position);
    bool add_card_deal(Card card);
    bool redraw_stack();
    bool flip_card();
    int size();
  private:
    std::vector<Card> stack{};
    bool can_add_card(Card card);
    bool can_rm_card(Card card);
    int s_type;
    int s_suit;
    int s_x;
    int s_y;
};

Stack::Stack(int x, int y)
{
  s_type = STACK_INPLAY;
  s_x = x;
  s_y = y;
}

Stack::Stack(int x, int y, int suit)
{
  s_type = STACK_COMPLETE;
  s_x = x;
  s_y = y;
  s_suit = suit;

  canvas.setBrushColor(COLOR_WHITE);
  canvas.fillRectangle(s_x,s_y,s_x+CARD_WIDTH,s_y+CARD_HEIGHT);
  if (s_suit == SPADE || s_suit == CLUB)
  {
    canvas.setPenColor(COLOR_BLACK);
    canvas.drawRectangle(s_x,s_y,s_x+CARD_WIDTH,s_y+CARD_HEIGHT);
    if(s_suit == SPADE)
    {
      canvas.drawBitmap(s_x+((CARD_WIDTH/2)-4),s_y+((CARD_HEIGHT/2)-4),&suite_spade);
    }
    else if (s_suit == CLUB)
    {
      canvas.drawBitmap(s_x+((CARD_WIDTH/2)-4),s_y+((CARD_HEIGHT/2)-4),&suite_club);
    }
  }
  else
  {
    canvas.setPenColor(COLOR_RED);
    canvas.drawRectangle(s_x,s_y,s_x+CARD_WIDTH,s_y+CARD_HEIGHT);
    if (s_suit == HEART)
    {
      canvas.drawBitmap(s_x+((CARD_WIDTH/2)-4),s_y+((CARD_HEIGHT/2)-4),&suite_heart);
    }
    else if (s_suit == DIAMOND)
    {
      canvas.drawBitmap(s_x+((CARD_WIDTH/2)-4),s_y+((CARD_HEIGHT/2)-4),&suite_diamond);
    }
  }
}

bool Stack::add_card_deal(Card card)
{
  Serial.println(this->stack.size());
  Serial.println("Adding card.");
  this->stack.push_back(card);
  Serial.println(this->stack.size());
  //this->redraw_stack();
  return true;
}

bool Stack::add_card(Card card)
{
  if (this->can_add_card(card))
  {
    this->stack.push_back(card);
    this->redraw_stack();
  }
  else
  {
    return false;
  }
  return true;
}

bool Stack::rm_card(int position)
{
  if (stack.size() > 0)
  {
    Card card = stack.at(position);
    if (this->can_rm_card(card)) {
      //stack.erase(stack.at(position));
      this->redraw_stack();
      return true;
    }
  }
  return false;
}

bool Stack::flip_card()
{
  //Serial.println(this->stack.size());
  if (this->stack.size() > 0)
  {
    Card * card = &this->stack.back();
    //Serial.println(card->c_visible);
    if (card->c_visible == false){
      Serial.println("***CARD VALUE AND SUIT***");
      Serial.println(card->c_value);
      Serial.println(card->c_suit);
      card->c_visible = true;
      card->draw();
      return true;
    }
  }
  return false;
}

bool Stack::can_add_card(Card card)
{
  if (s_type == STACK_INPLAY)
  {
    Card last_card = stack.back();
    if ((last_card.c_suit == CLUB && (card.c_suit == HEART || card.c_suit == DIAMOND)) ||
       (last_card.c_suit == SPADE && (card.c_suit == HEART || card.c_suit == DIAMOND)) ||
       (last_card.c_suit == HEART && (card.c_suit == CLUB || card.c_suit == SPADE)) ||
       (last_card.c_suit == DIAMOND && (card.c_suit == CLUB || card.c_suit == SPADE)))

     {
       if (card.c_value == (last_card.c_value - 1)) { return true; }
     }
  }
  else if (s_type = STACK_COMPLETE)
  {
    Card last_card = stack.back();
    if (s_suit == card.c_suit)
    {
      if (card.c_value == (last_card.c_value + 1)) { return true; }
    }
  }
  return false;
}

bool Stack::can_rm_card(Card card)
{
  if (s_type == STACK_INPLAY)
  {
    if (card.c_visible)
    {
      return true;
    }
  }
  else if (s_type = STACK_COMPLETE)
  {
    Card s_card = stack.back();
    if ((card.c_value  == s_card.c_value) && (card.c_suit == s_card.c_suit)) 
    {
      return true;
    }
  }
  return false;
}

bool Stack::redraw_stack()
{
  Serial.println("Redrawing stack.");
  Serial.println(stack.size());
  for (int c=0; c < stack.size(); c++)
  {
    Card card = stack.at(c);
    Serial.println("card");
    Serial.println(c);
    Serial.println(s_x);
    Serial.println(s_y + (c*Y_OFFSET));
    card.move(s_x, s_y + (c*Y_OFFSET));
  }
  return true;
}
