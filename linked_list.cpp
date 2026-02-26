#include <iostream>   // Для вывода в консоль
#include <cassert>    // Для использования assert (проверки условий)

// =============================================================================
//                        SINGLY LINKED LIST
// =============================================================================

// Шаблонный класс односвязного списка
// T — тип данных, который будет храниться в списке
template<typename T>
class SinglyLinkedListImpl {
public:

    // Внутренняя структура узла списка
    struct Node {
        T value;        // Значение узла
        Node* next;     // Указатель на следующий элемент

        // Конструктор узла
        Node(T val) : value(val), next(nullptr) {}
    };

    Node* head;   // Указатель на первый элемент списка
    Node* tail;   // Указатель на последний элемент списка
    size_t size;  // Количество элементов в списке

    // Конструктор списка (создает пустой список)
    SinglyLinkedListImpl() : head(nullptr), tail(nullptr), size(0) {}

    // Вставка элемента в начало списка
    Node* insertFront(T value) {
        Node* newNode = new Node(value);  // Создаем новый узел
        newNode->next = head;             // Новый узел указывает на старый head
        head = newNode;                   // Новый узел становится head
        
        // Если список был пуст, tail тоже должен указывать на этот узел
        if (!tail)
            tail = newNode;
        
        size++;  // Увеличиваем размер списка
        return newNode;
    }

    // Вставка после определенного узла
    Node* insertAfter(Node* node, T value) {

        // Если переданный узел null — вставляем в начало
        if (!node) return insertFront(value);
        
        Node* newNode = new Node(value);
        newNode->next = node->next;  // Новый узел указывает туда же, куда указывал node
        node->next = newNode;        // node теперь указывает на новый узел
        
        // Если вставили после tail — обновляем tail
        if (node == tail)
            tail = newNode;
        
        size++;
        return newNode;
    }

    // Поиск узла по значению
    Node* find(T value) {
        Node* current = head;

        // Проходим по списку
        while (current) {
            if (current->value == value)
                return current;  // Возвращаем найденный узел
            current = current->next;
        }

        return nullptr;  // Если не найден
    }

    // Удаление узла
    void remove(Node* node) {

        if (!node || !head) return;  // Проверка на пустоту
        
        // Если удаляем первый элемент
        if (head == node) {
            head = head->next;

            // Если список стал пустым — обнуляем tail
            if (!head)
                tail = nullptr;

            delete node;
            size--;
            return;
        }
        
        // Ищем предыдущий узел
        Node* current = head;
        while (current && current->next != node)
            current = current->next;
        
        // Если нашли
        if (current) {
            current->next = node->next;

            // Если удаляли tail — обновляем его
            if (tail == node)
                tail = current;

            delete node;
            size--;
        }
    }

    // Деструктор — освобождает память
    ~SinglyLinkedListImpl() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;  // Удаляем каждый узел
        }
    }

    // Печать списка
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

// Шаблонный класс двусвязного списка
template<typename T>
class DoublyLinkedListImpl {
public:

    // Узел двусвязного списка
    struct Node {
        T value;     // Значение
        Node* prev;  // Указатель на предыдущий элемент
        Node* next;  // Указатель на следующий элемент

        Node(T val) : value(val), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t size;

    // Конструктор
    DoublyLinkedListImpl() : head(nullptr), tail(nullptr), size(0) {}

    // Вставка в начало
    Node* insertFront(T value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        
        // Если список не пуст — старый head должен знать про нового соседа
        if (head)
            head->prev = newNode;
        
        head = newNode;
        
        if (!tail)
            tail = newNode;
        
        size++;
        return newNode;
    }

    // Вставка в конец
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

    // Вставка после узла
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

    // Вставка перед узлом
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

    // Поиск элемента
    Node* find(T value) {
        Node* current = head;

        while (current) {
            if (current->value == value)
                return current;
            current = current->next;
        }

        return nullptr;
    }

    // Удаление узла
    void remove(Node* node) {

        if (!node) return;
        
        // Если есть предыдущий — соединяем его со следующим
        if (node->prev)
            node->prev->next = node->next;
        else
            head = node->next;
        
        // Если есть следующий — соединяем его с предыдущим
        if (node->next)
            node->next->prev = node->prev;
        else
            tail = node->prev;
        
        delete node;
        size--;
    }

    // Деструктор
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
