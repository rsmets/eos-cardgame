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

//ACTION cardgame::login(name user, string words) {
ACTION cardgame::login(name user) {
  require_auth(user);

  // look through the users table to find the user
  auto user_iterator = _usersT.find(user.value);
  // users_table::const_iterator ~ but really using "auto" instead
  // auto iterator_name = table.find(primary_key);
  if (user_iterator == _usersT.end()) {
    // does not exist so create a new record
    user_iterator = _usersT.emplace(user, [&](auto& new_user) { // [&] denotes a lamda function by reference. Allows for accessing variables outside of the lamda function (access to the "user" var).
        new_user.username = user;
        // new_user.text = words;
        
      });  // forcing the user to pay the RAM cost of creating an entry in our grame
    //user_iterator = _usersT.emplace(payer, function) //  
  } else {
      // Modify a message record if it exists
      _usersT.modify(user_iterator, user, [&](auto& old_user) {
      ///old_user.text = words;
      old_user.lost_count = 0;
    });
    
  }

  // if the user does not exist then we want to create a new record for the user
}

EOSIO_DISPATCH(cardgame, (hi)(trymessage)(login))
