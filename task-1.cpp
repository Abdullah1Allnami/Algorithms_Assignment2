#include <iostream>
#include <vector>
#include <list>
using namespace std;

// Hashing Techniques
int divisionMethod(int key, int tableSize) {
    return key % tableSize;
}

int multiplicationMethod(int key, int tableSize) {
    const float A = 0.618033;
    float hashValue = key * A;
    hashValue = hashValue - (int)hashValue;
    return (int)(hashValue * tableSize);
}

int midSquareMethod(int key, int tableSize) {
    int square = key * key;
    int mid = (square / 100) % 100;
    return mid % tableSize;
}

int foldingMethod(int key, int tableSize) {
    int part1 = key / 100;
    int part2 = key % 100;
    int sum = part1 + part2;
    return sum % tableSize;
}

// Collision Resolution Techniques
class HashTableChaining {
    vector<list<int>> table;
    int size;

public:
    HashTableChaining(int tableSize) : size(tableSize) {
        table.resize(tableSize);
    }

    void insert(int key) {
        int hash = key % size;
        table[hash].push_back(key);
    }

    void display() {
        for (int i = 0; i < size; i++) {
            cout << "Index " << i << ": ";
            for (int key : table[i]) {
                cout << key << " -> ";
            }
            cout << "NULL\n";
        }
    }
};

class HashTableOpenAddressing {
    vector<int> table;
    int size;

public:
    HashTableOpenAddressing(int tableSize) : size(tableSize) {
        table.resize(tableSize, -1);
    }

    void insert(int key) {
        int hash = key % size;
        while (table[hash] != -1) { // Linear probing
            hash = (hash + 1) % size; // Move to the next index
        }
        table[hash] = key;
    }

    void display() {
        for (int i = 0; i < size; i++) {
            if (table[i] == -1) {
                cout << "Index " << i << ": EMPTY\n";
            } else {
                cout << "Index " << i << ": " << table[i] << "\n";
            }
        }
    }
};

class HashTableDoubleHashing {
    vector<int> table;
    int size;

public:
    HashTableDoubleHashing(int tableSize) : size(tableSize) {
        table.resize(tableSize, -1);
    }

    int hash1(int key) {
        return key % size;
    }

    int hash2(int key) {
        return 7 - (key % 7);
    }

    void insert(int key) {
        int hash = hash1(key);
        int step = hash2(key);

        while (table[hash] != -1) {
            hash = (hash + step) % size;
        }
        table[hash] = key;
    }

    void display() {
        for (int i = 0; i < size; i++) {
            if (table[i] == -1) {
                cout << "Index " << i << ": EMPTY\n";
            } else {
                cout << "Index " << i << ": " << table[i] << "\n";
            }
        }
    }
};

int main() {
    int tableSize = 10;


    cout << "Division Method: " << divisionMethod(25, tableSize) << "\n";
    cout << "Multiplication Method: " << multiplicationMethod(25, tableSize) << "\n";
    cout << "Mid-Square Method: " << midSquareMethod(12, tableSize) << "\n";
    cout << "Folding Method: " << foldingMethod(1234, tableSize) << "\n";

    HashTableChaining chainingTable(tableSize);
    chainingTable.insert(15);
    chainingTable.insert(25);
    chainingTable.insert(35);
    cout << "\nChaining Table:\n";
    chainingTable.display();

    HashTableOpenAddressing openAddressingTable(tableSize);
    openAddressingTable.insert(15);
    openAddressingTable.insert(25);
    openAddressingTable.insert(35);
    cout << "\nOpen Addressing Table:\n";
    openAddressingTable.display();

    HashTableDoubleHashing doubleHashingTable(tableSize);
    doubleHashingTable.insert(15);
    doubleHashingTable.insert(25);
    doubleHashingTable.insert(35);
    cout << "\nDouble Hashing Table:\n";
    doubleHashingTable.display();

    return 0;
}
