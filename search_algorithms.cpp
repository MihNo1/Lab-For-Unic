#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <functional>

using namespace std;
using namespace std::chrono;

// Количество повторов эксперимента для усреднения времени
const int EXPERIMENT_COUNT = 5;

// Размеры массивов для тестирования
const vector<int> DATA_SIZES = {1000, 5000, 10000};

// ===============================
// Алгоритмы поиска
// ===============================

// 1. Линейный поиск
// Сложность: O(n) — перебор каждого элемента массива
int linearSearch(const vector<int>& arr, int key)
{
    for (size_t i = 0; i < arr.size(); i++)
        if (arr[i] == key)
            return i; // элемент найден — возвращаем индекс
    return -1;      // элемент не найден
}

// 2. Бинарный поиск (для отсортированного массива)
// Сложность: O(log n) — делим массив пополам на каждом шаге
int binarySearchAlgo(const vector<int>& arr, int key)
{
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (arr[mid] == key)
            return mid;

        if (arr[mid] < key)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}

// 3. Интерполяционный поиск (для отсортированных массивов с равномерным распределением)
// Сложность: в лучшем случае O(log log n), в худшем O(n)
int interpolationSearch(const vector<int>& arr, int key)
{
    int low = 0;
    int high = arr.size() - 1;

    while (low <= high && key >= arr[low] && key <= arr[high])
    {
        if (low == high)
        {
            if (arr[low] == key) return low;
            return -1;
        }

        // Рассчитываем "предполагаемую" позицию на основе значения
        int pos = low + ((double)(high - low) / (arr[high] - arr[low])) * (key - arr[low]);

        if (arr[pos] == key)
            return pos;

        if (arr[pos] < key)
            low = pos + 1;
        else
            high = pos - 1;
    }

    return -1;
}

// ===============================
// Бинарное дерево поиска (BST)
// ===============================

// Узел дерева
struct Node
{
    int value;
    Node* left;
    Node* right;

    Node(int v) : value(v), left(nullptr), right(nullptr) {}
};

// Вставка в BST
// Сложность: O(log n) в среднем, O(n) в худшем случае (выраженное дерево)
Node* insert(Node* root, int value)
{
    if (!root)
        return new Node(value);

    if (value < root->value)
        root->left = insert(root->left, value);
    else
        root->right = insert(root->right, value);

    return root;
}

// Поиск в BST
// Сложность: O(log n) в среднем
bool searchBST(Node* root, int key)
{
    if (!root) return false;

    if (root->value == key)
        return true;

    if (key < root->value)
        return searchBST(root->left, key);

    return searchBST(root->right, key);
}

// Очистка дерева (рекурсивная)
void deleteTree(Node* root)
{
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// ===============================
// Генерация данных
// ===============================

// Отсортированный массив (0..size-1)
vector<int> generateSorted(int size)
{
    vector<int> arr(size);
    for (int i = 0; i < size; i++)
        arr[i] = i;
    return arr;
}

// Обратно отсортированный массив
vector<int> generateReverse(int size)
{
    vector<int> arr = generateSorted(size);
    reverse(arr.begin(), arr.end());
    return arr;
}

// Случайный порядок элементов
vector<int> generateRandom(int size)
{
    vector<int> arr = generateSorted(size);
    random_device rd;
    mt19937 g(rd());
    shuffle(arr.begin(), arr.end(), g);
    return arr;
}

// ===============================
// Тестирование алгоритмов
// ===============================

// Универсальная функция тестирования
// algorithm — это функция поиска, которую мы тестируем
void testAlgorithm(const string& name,
                   function<void(const vector<int>&)> algorithm)
{
    cout << "\n===== " << name << " =====\n";

    for (int size : DATA_SIZES)
    {
        cout << "\nSize: " << size << endl;

        // Генерируем три типа массивов
        vector<vector<int>> datasets =
        {
            generateSorted(size),
            generateReverse(size),
            generateRandom(size)
        };

        vector<string> types =
        {
            "Sorted",
            "Reverse",
            "Random"
        };

        // Для каждого типа массива измеряем среднее время
        for (size_t i = 0; i < datasets.size(); i++)
        {
            long long totalTime = 0;

            for (int exp = 0; exp < EXPERIMENT_COUNT; exp++)
            {
                auto start = high_resolution_clock::now();
                algorithm(datasets[i]);
                auto end = high_resolution_clock::now();

                totalTime += duration_cast<microseconds>(end - start).count();
            }

            cout << types[i] << " avg time: "
                 << totalTime / EXPERIMENT_COUNT
                 << " microseconds\n";
        }
    }
}

// ===============================
// main — запуск тестов
// ===============================

int main()
{
    // Линейный поиск
    testAlgorithm("Linear Search", [](const vector<int>& arr)
    {
        linearSearch(arr, arr.back()); // ищем последний элемент
    });

    // Бинарный поиск (требует сортировки)
    testAlgorithm("Binary Search", [](const vector<int>& arr)
    {
        vector<int> sorted = arr;
        sort(sorted.begin(), sorted.end());
        binarySearchAlgo(sorted, sorted.back());
    });

    // Интерполяционный поиск (для отсортированного массива)
    testAlgorithm("Interpolation Search", [](const vector<int>& arr)
    {
        vector<int> sorted = arr;
        sort(sorted.begin(), sorted.end());
        interpolationSearch(sorted, sorted.back());
    });

    // Бинарное дерево поиска
    testAlgorithm("Binary Search Tree", [](const vector<int>& arr)
    {
        Node* root = nullptr;
        for (int val : arr)
            root = insert(root, val);

        searchBST(root, arr.back());
        deleteTree(root);
    });

    return 0;
}