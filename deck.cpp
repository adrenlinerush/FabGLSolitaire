#include <algorithm>
#include <random>
#include <chrono>
#include "stack.cpp"

#define DECK_X       25
#define DECK_Y       5
#define TURN_OFFSET  25
#define TURN_X       75
#define TURN_Y       5
#define STACK_OFFSET 58
#define STACK_X      25
#define STACK_Y      60
#define COMPLETE_X   200
#define COMPLETE_Y   5
#define CARDS_TO_TURN 3

class Deck {
  public:
    Deck();
    bool deck_turn(int cards);
    bool init();
    void check_click(int x,int y);
  private:
    std::vector<Card> deck{};
    std::vector<Card> turn{};
    std::vector<Stack> in_play{};
    std::vector<Stack> complete{};
    bool init_deck();
    bool init_stacks();
    bool deal();
    bool shuffle();
    bool redraw_turn();
    bool reset_deck();
    bool reset_turn();
    bool stack_move(Stack * target_stack);
    Card pop();
    Card next();
    Card * s_card;
    Stack * s_stack;
    Stack * check_stack_click(int x, int y);
};

void Deck::check_click(int x,int y)
{
  if ((x >= DECK_X) && (x <= (DECK_X + CARD_WIDTH)) &&
      (y >= DECK_Y) && (y <= (DECK_Y + CARD_HEIGHT)))
  {
    this->deck_turn(CARDS_TO_TURN);
    if ((s_card) && (s_stack == NULL))
    {
      s_card->c_selected = false;
      s_card = NULL;
    }
    return;
  }
  if (turn.size() > 0)
  {
    Card * turn_card = &turn.back();
    if ((x >= turn_card->c_x) && (x <= (turn_card->c_x + CARD_WIDTH)) &&
        (y >= turn_card->c_y) && (y <= (turn_card->c_y + CARD_HEIGHT)))
    {
      if (s_card == NULL) {
        turn_card->c_selected = true;
        turn_card->draw();
        s_card = turn_card;
        s_stack = NULL;
	return;
      }
      else
      {
        if ((turn_card->c_value == s_card->c_value) && (turn_card->c_suit == s_card->c_suit)) 
	{
	  turn_card->c_selected = false;
	  turn_card->draw();
	  s_card = NULL;
	  s_stack = NULL;
	  return;
	}
      }
    }
  }
  Stack * click_stack = this->check_stack_click(x,y);
  if (s_card && click_stack)
  {
    Serial.println("going to attempt a move");
    this->stack_move(click_stack);
  }
}

Stack * Deck::check_stack_click(int x, int y)
{
  //in_play stacks
  for (int s = 0; s < 7; ++s)
  {
    Stack * stack = &in_play[s];
    if (stack->stack.size() == 0) 
    {
      if ((x >= stack->s_x) && (x <= (stack->s_x + CARD_WIDTH)) &&
          (y >= stack->s_y) && (y <= (stack->s_y + CARD_HEIGHT)))
      {
        return stack;
      }
    }
    Serial.println("looping through stack");
    Serial.println(s);
    for (int c = 0; c < stack->stack.size(); ++c)
    {
      Serial.println("*** Card ***");
      Card * card = &stack->stack[c];
      if (card->c_visible)
      {
        Serial.println("visible");
	Serial.println(card->c_x);
	Serial.println(card->c_y);
        if ((x >= card->c_x) && (x <= (card->c_x + CARD_WIDTH)) &&
            (y >= card->c_y) && (y <= (card->c_y + CARD_HEIGHT)))
        {
	  Serial.println("matches location");
	  Serial.println(card->c_value);
	  Serial.println(card->c_suit);
	  if (s_card == NULL) 
	  {
	    Serial.println("selecting card");
	    s_card = card;
	    s_stack = stack;
	    card->c_selected = true;
	    card->draw();
	  }
          else if ((card->c_value == s_card->c_value) && (card->c_suit == s_card->c_suit)) 
	  {
	    Serial.println("unselecting card");
            s_stack->redraw_stack();
	    s_card = NULL;
	    s_stack = NULL;
	    card->c_selected = false;
	    card->draw();
	  }
	  return stack;
	}
      }
    }
  }
  //complete stacks
  for (int s = 0; s < 4; ++s)
  {
    Serial.println("Checking stack..");
    Serial.println(s);
    Stack * stack = &complete[s];
      if ((x >= stack->s_x) && (x <= (stack->s_x + CARD_WIDTH)) &&
          (y >= stack->s_y) && (y <= (stack->s_y + CARD_HEIGHT)))
      {
        Serial.println("found complete stack");
        return stack;
      }
  }
  return NULL;
}

bool Deck::stack_move(Stack * target_stack)
{
  bool add_card = target_stack->add_card(s_card);
  if (add_card)
  {
    if (s_stack) 
    {
      Serial.println("Card successfully added to new stack... cleanup started");
      s_stack->rm_card(s_card);
      s_stack->flip_card();
      s_stack->redraw_stack();
      s_stack = NULL;
      s_card = NULL;
    }
    else
    {
      Serial.println("Cleanup turn...");
      turn.pop_back();
      this->redraw_turn();
      s_stack = NULL;
      s_card = NULL;
    }
  }
  return add_card;
}

Deck::Deck()
{
  Serial.println("deck initialized");
}

bool Deck::init()
{
  deck.clear();
  turn.clear();
  this->init_deck();
  this->shuffle();
  this->init_stacks();
  this->deal();
  return true;
}

bool Deck::init_stacks()
{
  for(int s=0; s < 7; ++s)
  {
    Stack stack(STACK_X + (STACK_OFFSET * s), STACK_Y);
    in_play.push_back(stack);
  }
  for(int s=0; s < 4; ++s)
  {
    Stack stack(COMPLETE_X + (STACK_OFFSET * s), COMPLETE_Y, s);
    complete.push_back(stack);
  }
  return true;
}

bool Deck::deal()
{
  Serial.println("Dealing...");
  for(int s=0; s < 7; ++s)
  {
    for(int n=s; n < 7; ++n)
    {
      Serial.println(n);
      Stack * stack = &in_play[n];
      Card card = deck.back();
      stack->add_card_deal(card);
      deck.pop_back();
    }
  }
  Serial.println("Flipping...");
  for(int s=0; s < 7; ++s)
  {
    //Serial.println(s);
    Stack * stack = &in_play[s];
    stack->flip_card();
    stack->redraw_stack();
  }
  Card card = deck.front();
  card.draw();
  return true;
}


bool Deck::shuffle()
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rng(seed);
  std::shuffle(std::begin(deck), std::end(deck), rng);
  //std::shuffle(std::begin(deck), std::end(deck), std::random_device());
  return true;
}

bool Deck::init_deck()
{
  Serial.println("Initializing deck..");
  for(int suit = 0; suit <= 3; suit++) 
  {
    for(int value = 1; value <= 13; value++)
    {
      Serial.println("Suit");
      Serial.println(suit);
      Serial.println("Value");
      Serial.println(value);
      Card card(suit, value, DECK_X, DECK_Y, false, false, false);
      Serial.println("Card initialized...");
      deck.push_back(card);
      Serial.println("Card added to deck...");
    }
    Serial.println("Suit completed");
    Serial.println(suit);
  }
  return true;
}

Card Deck::pop()
{
  Card card = turn.back();
  turn.pop_back();
  this->redraw_turn();
  return card;
}


Card Deck::next()
{
  Card card = turn.back();
  return card;
}

bool Deck::deck_turn(int cards)
{
  if (deck.empty()) 
  {
    this->reset_deck();
  }
  this->reset_turn();
  for(int c=1; c <= cards; ++c){
    if (deck.size() > 0)
    {
      Card card = deck.front();
      card.c_visible = true;
      card.move(TURN_X+(TURN_OFFSET*(c-1)),TURN_Y);
      turn.push_back(card);
      deck.erase(deck.begin());
    }
  }
  if (deck.size() > 0) 
  {
    Card card = deck.front();
    card.draw();
  }
  return true;
}

bool Deck::reset_deck()
{
  for(int c=0; c < turn.size(); ++c){
    Card card = turn[c];
    card.c_visible=false;
    card.move(DECK_X,DECK_Y);
  }
  deck = turn;
  turn.clear();
  return true;
}

bool Deck::reset_turn()
{
  for (int c=0; c < turn.size(); ++c){
    Card * card = &turn[c];
    card->move(TURN_X,TURN_Y);
  }
  return true;
}

bool Deck::redraw_turn()
{
  for (int c=0; c < turn.size(); ++c){
    Card card = turn[c];
    card.draw();
  }
  return true;
}
