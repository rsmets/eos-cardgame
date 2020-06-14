#include "cardgame.hpp"
#include "gameplay.cpp"

ACTION cardgame::login(name username) {
  require_auth(username);
  
  auto user_iterator = _users.find(username.value);
  if (user_iterator == _users.end()) {
    // create a new record for the user (_users)
    user_iterator = _users.emplace
      (username, [&](auto& new_user) {
        new_user.username = username;
      });
  }
}
