int cardgame::ai_best_card_wins_strategy (const int ai_attack_point, const int player_attack_point) { // 0
  print("best card ai");
  if (ai_attack_point > player_attack_point) return 3;
  if (ai_attack_point < player_attack_point) return -2;
  return -1; //draw
}

int cardgame::ai_min_loss_strategy (const int ai_attack_point, const int player_attack_point) { // 1
  print("min loss ai");
  if (ai_attack_point > player_attack_point) return 1;
  if (ai_attack_point < player_attack_point) return -4;
  return -1; //draw
}

int cardgame::ai_points_tally_strategy (const int ai_attack_point, const int player_attack_point) { // 2
  print("tall points ai");
  return (ai_attack_point - player_attack_point);
}

int cardgame::ai_loss_prevention_strategy (const int8_t life_ai, const int ai_attack_point, const int player_attack_point){ //3
  print("ai loss prevent");
  if (life_ai + ai_attack_point - player_attack_point > 0) return 1;
  return 0;
}

int cardgame::ai_choose_card(const game& game_data) {
    int available_strategies = 4;

    if ( game_data.life_ai > 2) available_strategies --;
    int strategy_idx = random(available_strategies);

    //calc the score of each card of each card in ai hand
    int choosen_card_idx = -1;
    int choosen_card_score = std::numeric_limits<int>::min();

    for (int i = 0; i < game_data.hand_ai.size(); i++) {
        const auto ai_card_id = game_data.hand_ai[i];
        const auto ai_card = card_dict.at(ai_card_id);

        // ignore empty slot
        if (ai_card.type == EMPTY) continue;

        // calc total score for this cad relative to the player's hand
        int card_score = calculate_ai_card_score(strategy_idx, game_data.life_ai, ai_card, game_data.hand_player);

        // keep track of the card with the highest value
        if (choosen_card_score < card_score) {
            choosen_card_score = card_score;
            choosen_card_idx = i;
        }
    }

    return choosen_card_idx;
}

int cardgame::calculate_ai_card_score(const int strategy_idx, const uint8_t life_ai, const card& ai_card, vector<uint8_t> hand_player) {
    int total = 0;
    //int ai_card_attack_points = ai_card.attack_point;

    if (strategy_idx == 0) {
        for (int i = 0; i < hand_player.size(); i++) {
            auto player_card = card_dict.at(hand_player[i]);
            int player_card_attack_points = calculate_attack_point(player_card, ai_card);
            int ai_card_attack_points = calculate_attack_point(ai_card, player_card);

            total += ai_best_card_wins_strategy(ai_card_attack_points, player_card_attack_points);
        }
    }

    if (strategy_idx == 1) {
        for (int i = 0; i < hand_player.size(); i++) {
            auto player_card = card_dict.at(hand_player[i]);
            int player_card_attack_points = calculate_attack_point(player_card, ai_card);
            int ai_card_attack_points = calculate_attack_point(ai_card, player_card);

            total += ai_min_loss_strategy(ai_card_attack_points, player_card_attack_points);
        }
    }

    if (strategy_idx == 2) {
        for (int i = 0; i < hand_player.size(); i++) {
            auto player_card = card_dict.at(hand_player[i]);
            int player_card_attack_points = calculate_attack_point(player_card, ai_card);
            int ai_card_attack_points = calculate_attack_point(ai_card, player_card);

            total += ai_points_tally_strategy(ai_card_attack_points, player_card_attack_points);
        }
    }

    if (strategy_idx == 3) {
        for (int i = 0; i < hand_player.size(); i++) {
            auto player_card = card_dict.at(hand_player[i]);
            int player_card_attack_points = calculate_attack_point(player_card, ai_card);
            int ai_card_attack_points = calculate_attack_point(ai_card, player_card);

            total += ai_loss_prevention_strategy(life_ai, ai_card_attack_points, player_card_attack_points);
        }
    }

    return total;
}

int cardgame::calculate_attack_point(const card& card1, const card& card2) {
    int result = card1.attack_point;

    //elemental compat handling bonus of 1?
    if ((card1.type == FIRE && card2.type == WOOD) || 
        (card1.type == WOOD && card2.type == WATER) || 
        (card1.type == WATER && card2.type == FIRE)) {
            result ++;
        }

    return result;
}

// resolve selected cards aka update the damage dealt
void cardgame::resolve_selected_cards(game& game_data) {

    const auto player_card = card_dict.at(game_data.selected_card_player);
    const auto ai_card = card_dict.at(game_data.selected_card_ai);

    // VOID cards skipp all damage calculation
    if (player_card.type == VOID || ai_card.type == VOID) return;

    int player_attack_point = calculate_attack_point(player_card, ai_card);
    int ai_attack_point = calculate_attack_point(ai_card, player_card);

    // damage calculation
    // deal damage ot ai if the player's card attack points are higher
    // deal damage to player if the ai's AP are higher

    if (player_attack_point > ai_attack_point) {
        int diff = player_attack_point - ai_attack_point;
        game_data.life_lost_ai = diff;
        game_data.life_ai -= diff;

    } else if (player_attack_point < ai_attack_point) {
        int diff = ai_attack_point - player_attack_point;
        game_data.life_lost_player = diff;
        game_data.life_player -= diff;
    }

    
}