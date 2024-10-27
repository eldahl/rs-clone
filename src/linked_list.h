#ifndef LINKED_LIST_H
#define LINKED_LIST_H

template <typename T>
class SinglyLinkedList {
public:
    struct SinglyLinkedListNode {
        SinglyLinkedListNode *next;
        T data;
    };

    SinglyLinkedListNode *head;
    SinglyLinkedListNode *end;

    SinglyLinkedList() { head = nullptr; end = nullptr; };

    void insertAtBeginning(T data);
    void insertAtEnd(T data);
    // Zero-indexed - Inserts node as given position
    void insertAtPosition(unsigned int pos, T data);

    void deleteFromBeginning();
    void deleteFromEnd();
    // Zero-indexed - Deletes node at given position
    void deleteFromPostion(unsigned int pos);
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

    DoublyLinkedList() { head = nullptr; end = nullptr; };

    void insertAtBeginning(T data);
    void insertAtEnd(T data);
    // Zero-indexed - Inserts node as given position
    void insertAtPosition(unsigned int pos, T data);

    void deleteFromBeginning();
    void deleteFromEnd();
    // Zero-indexed - Deletes node at given postion
    void deleteFromPostion(unsigned int pos);
private:
    unsigned int count = 0;
};
#endif
