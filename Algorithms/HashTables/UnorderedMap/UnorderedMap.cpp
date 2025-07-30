#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

int main() {
    unordered_map<int, int> hashTable;

    hashTable.insert({ 10, 100 });
    hashTable[20] = 200;
    hashTable[30] = 300;
    hashTable[40] = 400;

    auto it = hashTable.find(20);
    if (it != hashTable.end()) {
        cout << "Found: " << it->first << " -> " << it->second << endl;
    }
    else {
        cout << "Key 20 not found" << endl;
    }

    hashTable.erase(20);

    if (hashTable.count(20) > 0) {
        cout << "Key 20 exists" << endl;
    }
    else {
        cout << "Key 20 doesn't exist" << endl;
    }

    cout << "\nAll elements:" << endl;
    for (const auto& pair : hashTable) {
        cout << pair.first << " -> " << pair.second << endl;
    }

    return 0;
}