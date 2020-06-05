#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

CONTRACT cardgame : public contract {
  public:
    using contract::contract;
    cardgame(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds),
    _usersT(receiver, receiver.value),
    _games(receiver, receiver.value)
    {}

    ACTION trymessage(name sender, name recipient);
    ACTION hi(name from, string text);

    //ACTION login(name user, string message);
    ACTION login(name user);

  private:
    TABLE user_info {
      name    username;
      //string  text;
      uint64_t win_count = 1;
      uint64_t lost_count = 2;
      
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


    users_table _usersT;
    games_table _games;
};
