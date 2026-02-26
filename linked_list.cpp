#include <iostream>
#include <cassert>

// =============================================================================
//                        SINGLY LINKED LIST
// =============================================================================

template<typename T>
class SinglyLinkedListImpl {
public:
    struct Node {
        T value;
        Node* next;
        Node(T val) : value(val), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t size;

    SinglyLinkedListImpl() : head(nullptr), tail(nullptr), size(0) {}

    // Insert at the beginning
    Node* insertFront(T value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        
        if (!tail)
            tail = newNode;
        
        size++;
        return newNode;
    }

    // Insert after a specific node
    Node* insertAfter(Node* node, T value) {
        if (!node) return insertFront(value);
        
        Node* newNode = new Node(value);
        newNode->next = node->next;
        node->next = newNode;
        
        if (node == tail)
            tail = newNode;
        
        size++;
        return newNode;
    }

    // Find node with value
    Node* find(T value) {
        Node* current = head;
        while (current) {
            if (current->value == value)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    // Remove a node
    void remove(Node* node) {
        if (!node || !head) return;
        
        if (head == node) {
            head = head->next;
            if (!head)
                tail = nullptr;
            delete node;
            size--;
            return;
        }
        
        Node* current = head;
        while (current && current->next != node)
            current = current->next;
        
        if (current) {
            current->next = node->next;
            if (tail == node)
                tail = current;
            delete node;
            size--;
        }
    }

    // Cleanup all nodes
    ~SinglyLinkedListImpl() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void printList() {
        Node* current = head;
        std::cout << "List: ";
        while (current) {
            std::cout << current->value << " -> ";
            current = current->next;
        }
        std::cout << "nullptr\n";
    }
};

// =============================================================================
//                        DOUBLY LINKED LIST
// =============================================================================

template<typename T>
class DoublyLinkedListImpl {
public:
    struct Node {
        T value;
        Node* prev;
        Node* next;
        Node(T val) : value(val), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t size;

    DoublyLinkedListImpl() : head(nullptr), tail(nullptr), size(0) {}

    // Insert at the beginning
    Node* insertFront(T value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        
        if (head)
            head->prev = newNode;
        
        head = newNode;
        
        if (!tail)
            tail = newNode;
        
        size++;
        return newNode;
    }

    // Insert at the end
    Node* insertBack(T value) {
        Node* newNode = new Node(value);
        newNode->prev = tail;
        
        if (tail)
            tail->next = newNode;
        
        tail = newNode;
        
        if (!head)
            head = newNode;
        
        size++;
        return newNode;
    }

    // Insert after a specific node
    Node* insertAfter(Node* node, T value) {
        if (!node) return insertFront(value);
        
        Node* newNode = new Node(value);
        newNode->next = node->next;
        newNode->prev = node;
        
        if (node->next)
            node->next->prev = newNode;
        
        node->next = newNode;
        
        if (!newNode->next)
            tail = newNode;
        
        size++;
        return newNode;
    }

    // Insert before a specific node
    Node* insertBefore(Node* node, T value) {
        if (!node) return insertBack(value);
        
        Node* newNode = new Node(value);
        newNode->prev = node->prev;
        newNode->next = node;
        
        if (node->prev)
            node->prev->next = newNode;
        
        node->prev = newNode;
        
        if (!newNode->prev)
            head = newNode;
        
        size++;
        return newNode;
    }

    // Find node with value
    Node* find(T value) {
        Node* current = head;
        while (current) {
            if (current->value == value)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    // Remove a node
    void remove(Node* node) {
        if (!node) return;
        
        if (node->prev)
            node->prev->next = node->next;
        else
            head = node->next;
        
        if (node->next)
            node->next->prev = node->prev;
        else
            tail = node->prev;
        
        delete node;
        size--;
    }

    // Cleanup all nodes
    ~DoublyLinkedListImpl() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void printList() {
        Node* current = head;
        std::cout << "List: ";
        while (current) {
            std::cout << current->value << " <-> ";
            current = current->next;
        }
        std::cout << "nullptr\n";
    }
};

// =============================================================================
//                            TESTS
// =============================================================================

void testSingly() {
    SinglyLinkedListImpl<int> list;

    auto n1 = list.insertFront(10);
    auto n2 = list.insertAfter(n1, 20);
    auto n3 = list.insertAfter(n2, 30);

    assert(list.size == 3);

    auto found = list.find(20);
    assert(found == n2);

    list.remove(n2);
    assert(list.size == 2);

    found = list.find(20);
    assert(!found);

    std::cout << "Singly linked list tests PASSED\n";
}

void testDoubly() {
    DoublyLinkedListImpl<int> list;

    auto n1 = list.insertFront(1);
    auto n2 = list.insertBack(2);
    auto n3 = list.insertBefore(n2, 5);

    assert(list.size == 3);

    auto found = list.find(5);
    assert(found == n3);

    list.remove(n2);
    assert(list.size == 2);

    found = list.find(2);
    assert(!found);

    std::cout << "Doubly linked list tests PASSED\n";
}

int main() {
    testSingly();
    testDoubly();

    std::cout << "All tests passed successfully!\n";
    return 0;
}
