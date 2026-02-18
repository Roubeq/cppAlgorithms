#include <iostream>
#include <string>

using namespace std;

const int MAX_CHARS = 256;
const int MAX_NODES = 256;

int main()
{
    string s;
    getline(cin, s);

    int freq[MAX_CHARS] = { 0 };

    for (int i = 0; i < s.size(); i++) {
        freq[s[i]]++;
    }

    char letters[MAX_CHARS];
    int freqLetter[MAX_CHARS];
    int k = 0;

    for (int i = 0; i < MAX_CHARS; i++) {
        if (freq[i] > 0) {
            letters[k] = (char)i;
            freqLetter[k] = freq[i];
            k++;
        }
    }


    int parent[MAX_NODES];
    int left[MAX_NODES];
    int right[MAX_NODES];
    int weight[MAX_NODES];
    bool used[MAX_NODES];

    for (int i = 0; i < MAX_NODES; i++) {
        parent[i] = -1;
        left[i] = -1;
        right[i] = -1;
        weight[i] = 0;
        used[i] = false;
    }

    for (int i = 0; i < k; i++) {
        weight[i] = freqLetter[i];
    }

    int countNodes = k;
    while (countNodes < 2 * k - 1) {
        int minFirst = -1;
        int minSecond = -1;

        for (int i = 0; i < countNodes; i++) {
            if (!used[i]) {
                if (minFirst == -1 || weight[i] < weight[minFirst]) {
                    minSecond = minFirst;
                    minFirst = i;
                }
                else if (minSecond == -1 || weight[i] < weight[minSecond]) {
                    minSecond = i;
                }
            }
        }

        weight[countNodes] = weight[minFirst] + weight[minSecond];
        left[countNodes] = minFirst;
        right[countNodes] = minSecond;
        parent[minFirst] = countNodes;
        parent[minSecond] = countNodes;
        used[minFirst] = true;
        used[minSecond] = true;

        countNodes++;
    }

    string codes[MAX_CHARS];

    for (int i = 0; i < k; i++) {
        string code = "";
        int current = i;

        while (parent[current] != -1) {
            int p = parent[current];
            if (left[p] == current) {
                code = "0" + code;
            }
            else {
                code = "1" + code;
            }
            current = p;
        }
        codes[letters[i]] = code;
    }

    string encoded = "";
    for (int i = 0; i < s.length(); i++) {
        encoded += codes[s[i]];
    }

    cout << k << " " << encoded.length() << endl;

    for (int i = 0; i < MAX_CHARS; i++) {
        if (freq[i] > 0) {
            cout << (char)i << ": " << codes[i] << endl;
        }
    }

    cout << encoded << endl;
}
