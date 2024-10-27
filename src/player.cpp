#include "player.h"
#include "map.h"

#include <glm/ext/vector_float2.hpp>

Player::Player() {
    std::cout << "Player initialized." << std::endl;
}

bool Player::moveTo(Map *map, glm::vec2 targetLocation) {

    // Get linked list of movement operatiosn from map
    

    return true;
}

void Player::update() {

    if (doMovement) {
        // Move to next postion in linked list on every update

    }
}
