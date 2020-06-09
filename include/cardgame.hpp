#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace std;
using namespace eosio;

CONTRACT cardgame : public contract {
  public:
    using contract::contract;
    cardgame(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds),
    _usersT(receiver, receiver.value),
    _games(receiver, receiver.value),
    _seed(receiver, receiver.value)
    {}

    ACTION trymessage(name sender, name recipient);
    ACTION hi(name from, string text);

    //ACTION login(name user, string message);
    ACTION login(name user);
    ACTION startgame(name user);

  private:

    int random(const int range);
    void drawCard(vector<uint8_t>& deck, vector<uint8_t>& hand) ;

    enum game_status: int8_t {
      ONGOING = 0,
      PLAYER_WON = 1,
      PLAYER_LOST = -1
    };
    
    enum card_type: uint8_t {
      EMPTY = 0, // for empty slot in player's hand
      FIRE = 1,
      WOOD = 2,
      WATER = 3,
      NEUTRAL = 4,
      VOID = 5
    };

    struct card {
      uint8_t type;
      uint8_t attack_point;
    };

    const map<uint8_t, card> card_dict = {
      {0, {EMPTY, 1} },
      {1, {FIRE, 1} },
      {2, {FIRE, 1} },
      {3, {FIRE, 2} },
      {4, {FIRE, 2} },
      {5, {FIRE, 3} },
      {6, {WOOD, 1} },
      {7, {WOOD, 1} },
      {8, {WOOD, 2} },
      {9, {WOOD, 2} },
      {10, {WOOD, 3} },
      {11, {WATER, 1} },
      {12, {WATER, 1} },
      {13, {WATER, 2} },
      {14, {WATER, 2} },
      {15, {WATER, 3} },
      {16, {NEUTRAL, 3} },
      {17, {VOID, 0} }
    };

    struct game {
      int8_t life_player = 5;
      int8_t life_ai = 5;
      vector<uint8_t> deck_player = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17}; // vector is just an unbounded (size) array
      vector<uint8_t> deck_ai = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17}; // vector is just an unbounded (size) array
      vector<uint8_t> hand_player = {0,0,0,0};
      vector<uint8_t> hand_ai = {0,0,0,0};
      uint8_t selected_card_player = 0;
      uint8_t selected_card_ai = 0;
      int8_t status = ONGOING;
      uint8_t life_lost_player = 0;
      uint8_t life_lost_ai = 0;
    };

    TABLE user_info {
      name    username;
      //string  text;
      uint64_t win_count = 0;
      uint64_t lost_count = 0;
      game game_data;
      
      auto primary_key() const { return username.value; };
    };
    typedef multi_index<name("users"), user_info> users_table;
    // users is what this table will be called on chain
    // user_info is referring to what pattern this will follow
    // users_table is the name of the type we just created

    TABLE game_info {
      name    gameId;
      string  game_name;
      bool isenabled;
      
      auto primary_key() const { return gameId.value; };
    };
    typedef multi_index<name("games"), game_info> games_table;
    // users is what this table will be called on chain
    // user_info is referring to what pattern this will follow
    // users_table is the name of the type we just created

    TABLE seed {
      uint64_t    key = 1;
      uint64_t value =1;
      
      auto primary_key() const { return key; };
    };
    typedef multi_index<name("seed"), seed> seed_table;


    users_table _usersT;
    games_table _games;
    seed_table _seed;
};
