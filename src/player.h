#ifndef PLAYER_H
#define PLAYER_H

#include <glm/ext/vector_float2.hpp>
#include <iostream>

#include "linked_list.h"
#include "map.h"

class Player {

public:
    bool doMovement = false;
    glm::vec2 pos;

    Player();

    // Move to location on map, true if able to, false if non-reachable
    bool moveTo(DoublyLinkedList<glm::vec2> *movementOPList);

    void update();

private:
    DoublyLinkedList<glm::vec2>::DoublyLinkedListNode *current_movement_node = nullptr;
    DoublyLinkedList<glm::vec2> *current_movement_path = nullptr;
};

#endif
