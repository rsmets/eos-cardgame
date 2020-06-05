int cardgame::random(const int range) {
  // find the existing seed 
  auto seed_iterator = _seed.begin();
  // initialize the seed if not found (if not used)
  if (seed_iterator = _seed.end()) {
    seed_iterator = _seed.emplace(_self, [&](auto& seed) {
      // defaults are good enough
    })
  }
  
  int prime = 65337;
  auto new_seed_value = (seed_iterator->value + now()) % prime;
  
  _seed.modify(seed_iterator,_self,[&](auto& seed){
    seed.value = new_seed_value;
  })
  
  int random_result = new_seed_value % range;
  return random_result;
}