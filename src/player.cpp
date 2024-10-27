#include "player.h"
#include "linked_list.h"

#include <glm/ext/vector_float2.hpp>

Player::Player() {
    std::cout << "Player initialized." << std::endl;
}

bool Player::moveTo(DoublyLinkedList<glm::vec2> *movementOPList) {

    // Get linked list of movement operatiosn from map
    current_movement_path = movementOPList;
    current_movement_node = current_movement_path->head;
    return true;
}

void Player::update() {

    //std::cout << "update!" << std::endl;
    if (doMovement) {
        // Move to next postion in linked list on every update

        // If the end of the list is reached, reset the movement path and node
        if(current_movement_node != nullptr) {
            pos = current_movement_node->data;
            std::cout << pos.x << " " << pos.y << std::endl;
            if(current_movement_node->next != nullptr) {
                current_movement_node = current_movement_node->next;
                return;
            }
            doMovement = false;
            current_movement_path = nullptr;
            current_movement_node = nullptr;
            return;
        }


    }
}
