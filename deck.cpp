#include <algorithm>
#include <random>
#include <chrono>
#include "stack.cpp"

#define DECK_X       25
#define DECK_Y       25
#define TURN_OFFSET  25
#define TURN_X       75
#define TURN_Y       25
#define STACK_OFFSET 59
#define STACK_X      25
#define STACK_Y      105
#define COMPLETE_X   200
#define COMPLETE_Y   25

class Deck {
  public:
    Deck();
    bool deck_turn(int cards);
    bool init();
    Card pop();
    Card next();
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
};

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
      turn.push_back(card);
      card.c_visible = true;
      card.move(TURN_X+(TURN_OFFSET*(c-1)),TURN_Y);
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
    Card card = turn[c];
    card.move(TURN_X,TURN_Y);
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
