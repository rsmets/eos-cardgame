#include <cardgame.hpp>
#include "gameplay.cpp"

ACTION cardgame::trymessage(name sender, name recipient) {
  //require_auth(get_self());
  require_auth(sender);
  print ("Sorry, you will have to say hello to, ", name{recipient}, " in person");
}

ACTION cardgame::hi(name user, string message) {
/*
  // Init the _message table
  //users_table _usersT(get_self(), get_self().value);

  // Find the record from _messages table
  auto msg_itr = _usersT.find(user.value);
  if (msg_itr == _usersT.end()) {
    // Create a message record if it does not exist
    _usersT.emplace(user, [&](auto& msg) {
      msg.username = user;
      msg.text = message;
    });
  } else {
    // Modify a message record if it exists
    _usersT.modify(msg_itr, user, [&](auto& msg) {
      msg.text = message;
    });
  }
*/
  require_auth(get_self()); // this should be uncommented for the tutorial to pick up!
  //require_auth(user);
  print ("hello, ", name{user});
}

EOSIO_DISPATCH(cardgame, (hi)(trymessage))
