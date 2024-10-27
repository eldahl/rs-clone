#ifndef PLAYER_H
#define PLAYER_H

#include <glm/ext/vector_float2.hpp>
#include <iostream>

#include "map.h"

class Player {

public:
    bool doMovement = false;

    Player();

    // Move to location on map, true if able to, false if non-reachable
    bool moveTo(Map *map, glm::vec2 targetLocation);
    
    void update();

private:
    glm::vec2 pos;

};

#endif
