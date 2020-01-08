#include <iostream>
#include <cassert>

#include "doubly_linked_list.h"

int main(int, char *[]) {
    std::cout << "init data" << std::endl;
    DoublyLinkedList<int> doubly_linked_list {0, 1, 2, 3};
    for (auto i : doubly_linked_list) {
        std::cout << i << std::endl;
    }

    assert(doubly_linked_list.size() == 4);
    assert(doubly_linked_list.empty() == false);

    auto it = doubly_linked_list.find(1);
    doubly_linked_list.erase(it);
    for (auto i : doubly_linked_list) {
        std::cout << i << std::endl;
    }
    assert(doubly_linked_list.size() == 3);

    it = doubly_linked_list.find(2);
    doubly_linked_list.insert(it, 5);
    assert(doubly_linked_list.size() == 4);
    for (auto i : doubly_linked_list) {
        std::cout << i << std::endl;
    }

    doubly_linked_list.clear();
    assert(doubly_linked_list.size() == 0);
    for (auto i : doubly_linked_list) {
        std::cout << i << std::endl;
    }

    return 0;
}
