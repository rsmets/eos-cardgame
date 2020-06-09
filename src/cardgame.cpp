#include <cardgame.hpp>
#include "gameplay.cpp"


ACTION cardgame::startgame(name username) {
  require_auth(username);

  auto& user = _usersT.get(username.value, "User doesn't exist");

  _usersT.modify(user, username, [&](auto& modified_user) {
    game game_data;

    // draw 4 cards for the player and the ai from their decks
    for (uint8_t cards_drawn = 0; cards_drawn < 4; cards_drawn++){
      drawCard(game_data.deck_player, game_data.hand_player);
      drawCard(game_data.deck_ai, game_data.hand_ai);
    }

    // asing this game we just created to the user
    modified_user.game_data = game_data;
  });
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

// Simple Pseudo Random Number Algorithm: randomly pick a number within 0 to n-1
int cardgame::random(const int range) {
  // Find the existing seed. begin() gets an iterator pointing to the first record in a table 
  auto seed_iterator = _seed.begin();
 
  // Initialize the seed with default value if it is not found
  // _seed.end() is a position after the last record, so if begin() = end(), there are no records
  if (seed_iterator == _seed.end()) {
    seed_iterator = _seed.emplace( _self, [&]( auto& seed ) { });
  } // _self is paying the RAM & the lamba (aka anonymous) function to write the new record
 
  // Generate a new seed value using the existing seed value
  // % is the modulus operator, which returns the *remainder* after division.
  // The modulus operator is very useful when you don't want to exceed a certain number
  int prime = 65537;
  auto new_seed_value = (seed_iterator->value + current_time_point().sec_since_epoch()) % prime;
  
  // Store the updated seed value in the table
  _seed.modify( seed_iterator, _self, [&]( auto& s ) {
    s.value = new_seed_value;
  }); // seed_iterator is being modified, _self is paying the RAM & the lamba (aka anonymous) function to write the new record
  
  // Get the random result in desired range
  // Notice the modulus % operator lets us be sure that the
  // result is less than "range"
  int random_result = new_seed_value % range;
  return random_result;
}

void cardgame::drawCard(vector<uint8_t>& deck, vector<uint8_t>& hand) {
  //pick random card from deck
  int deck_card_index = random(deck.size());

  //find the first empyt slot int the player's hand
  int first_empty_slot = -1;
  for (int current_slot = 0; current_slot < hand.size(); current_slot++) { // <= ?
    auto id = hand[current_slot];
    //if (id == 0) { //< also works
    if (card_dict.at(id).type == EMPTY) {
      // bingo
      //place the card in that slot
      first_empty_slot = current_slot;
      break;
    }
  }

  //check(condition, "error message.");
  check(first_empty_slot != -1, "No empty slot in hand.");

  // asign the card we randomly picked to the emtpy slot
  hand[first_empty_slot] = deck[deck_card_index];

  // remove the card from the deck 
  deck.erase(deck.begin() + deck_card_index); // need to get the iterate at the begining of the vector then just add the index to get where we want to be

}

EOSIO_DISPATCH(cardgame, (hi)(trymessage)(login)(startgame))
