#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>

template <typename T>
class SinglyLinkedList {
public:
    struct SinglyLinkedListNode {
        SinglyLinkedListNode *next;
        T data;
    };

    SinglyLinkedListNode *head;
    SinglyLinkedListNode *end;

    SinglyLinkedList<T>() { head = nullptr; end = nullptr; };

    void insertAtBeginning(T data);
    void insertAtEnd(T data);
    // Zero-indexed - Inserts node as given position
    void insertAtPosition(unsigned int pos, T data);

    void deleteFromBeginning();
    void deleteFromEnd();
    // Zero-indexed - Deletes node at given position
    void deleteFromPosition(unsigned int pos);
private:
    unsigned int count = 0;
};

template <typename T>
class DoublyLinkedList {
public:
    struct DoublyLinkedListNode {
        DoublyLinkedListNode *prev;
        DoublyLinkedListNode *next;
        T data;
    };

    DoublyLinkedListNode *head;
    DoublyLinkedListNode *end;

    DoublyLinkedList<T>() { head = nullptr; end = nullptr; };

    void insertAtBeginning(T data);
    void insertAtEnd(T data);
    // Zero-indexed - Inserts node as given position
    void insertAtPosition(unsigned int pos, T data);

    void deleteFromBeginning();
    void deleteFromEnd();
    // Zero-indexed - Deletes node at given postion
    void deleteFromPosition(unsigned int pos);
private:
    unsigned int count = 0;
};

template <typename T>
void DoublyLinkedList<T>::insertAtBeginning(T data) {

    // Create new node with parameter data
    DoublyLinkedListNode *newNode = new DoublyLinkedListNode();
    newNode->data = data;

    // No prev, as this is at the beginning of the linked list
    newNode->prev = nullptr;
    // Next is the previous head location
    newNode->next = head;

    // Set head to new node
    head = newNode;

    // Add end reference if linked list is empty
    if (count == 0) {
        end = newNode;
    }

    // Increment node quantity
    count++;
}

template <typename T>
void DoublyLinkedList<T>::insertAtEnd(T data) {

    // Create new node with parameter data
    DoublyLinkedListNode *newNode = new DoublyLinkedListNode();
    newNode->data = data;

    // Prev is the previous end location
    newNode->prev = end;
    // No next, as this is at the end of the linked list
    newNode->next = nullptr;
    
    // Set previous node to new node
    if(end != nullptr)
        end->next = newNode;

    // Set end to new node
    end = newNode;

    // Add head reference if linked list is empty
    if (count == 0) {
        head = newNode;
    }

    // Increment node quantity
    count++;
}

// Zero-indexed - Inserts node as given position
template <typename T>
void DoublyLinkedList<T>::insertAtPosition(unsigned int pos, T data) {

    // Boundary checking
    if (pos > count) {
        throw "Trying to insert at position that exceeds size of linked list!";
        return;
    }

    // If pos is 0 or count, then shortcut to insertAt-beginning/-end
    if (pos == 0) { insertAtBeginning(data); return; }
    if (pos == count) { insertAtEnd(data); return; }

    // Iterate through linked list, until we get to pos
    DoublyLinkedListNode* node = head;
    for(int i = 0; i < pos; i++) {
        node = node->next;
    }

    // Create new node with parameter data
    DoublyLinkedListNode *newNode = new DoublyLinkedListNode();
    newNode->data = data;

    // To insert new node at the position given, the new node is inserted
    // in front of the previous element at pos.
    newNode->prev = node->prev;
    newNode->next = node;
    node->prev->next = newNode;
    node->prev = newNode;

    // Increment node quantity
    count++;
}

template <typename T>
void DoublyLinkedList<T>::deleteFromBeginning() {

    // Check for empty linked list
    if(head == nullptr || end == nullptr || count == 0) {
        std::cout << "Attempt to delete beginning of linked list that is empty!" << std::endl;
        return;
    }
    
    // Get node to delete from head
    DoublyLinkedListNode *nodeToDelete = head;

    // Handle single element linked list
    if(count == 1) {
        head = nullptr;
        end = nullptr;
    }
    // More than 1 element
    else {
        // Shift head forward to next node
        head = head->next;
        // Set prev to nullptr to indicate beginning of list
        head->prev = nullptr;
    }

    // Delete node
    delete nodeToDelete;

    // Decrement node quantity
    count--;
}

template <typename T>
void DoublyLinkedList<T>::deleteFromEnd() {

    // Check for empty linked list
    if(head == nullptr || end == nullptr || count == 0) {
        std::cout << "Attempt to delete end of linked list that is empty!" << std::endl;
        return;
    }

    // Get node to delete from end
    DoublyLinkedListNode *nodeToDelete = end;

    // Handle single element linked list
    if(count == 1) {
        head = nullptr;
        end = nullptr;
    }
    else {
        // Shift end backwards to previous node
        end = end->prev;
        // Set next to nullptr to indicate end of list
        end->next = nullptr;
    }

    // Delete node
    delete nodeToDelete;

    // Decrement node quantity
    count--;
}

// Zero-indexed - Deletes node at given position
template <typename T>
void DoublyLinkedList<T>::deleteFromPosition(unsigned int pos) {

    // Boundary checking
    if (pos >= count) {
        throw "Trying to delete at position that exceeds size of linked list!";
        return;
    }

    // Handle linked lists with fewer than two elements
    if(pos == 0) {
        deleteFromBeginning();
        return;
    }
    else if (pos == count - 1) {
        deleteFromEnd();
        return;
    }

    // Check for empty linked list
    if(head == nullptr || end == nullptr || count == 0) {
        std::cout << "Attempt to delete node at position " << pos << " of linked list that is empty!" << std::endl;
        return;
    }

    // Iterate through linked list, until we get to pos of nodeToDelete
    DoublyLinkedListNode* nodeToDelete = head;
    for(int i = 0; i < pos; i++) {
        nodeToDelete = nodeToDelete->next;
    }

    // Set next on the preceding node of nodeToDelete to be the succeeding node of nodeToDelete
    //   & prev on the succeding node of nodeToDelete to be the preceding node of nodeToDelete
    nodeToDelete->prev->next = nodeToDelete->next;
    nodeToDelete->next->prev = nodeToDelete->prev;

    // Delete node
    delete nodeToDelete;

    // Decrement node quantity
    count--;
}

#endif
