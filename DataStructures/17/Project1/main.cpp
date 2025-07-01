#include <iostream>
#include <fstream>
using namespace std;

struct Visitor {
    int arrival_h;   // ���� �������
    int arrival_m;   // ������ �������
    int departure_h; // ���� �����
    int departure_m; // ������ �����
    int id;         // ���������� ID
};

struct Event {
    int time;       // ����� � �������
    int type;       // 1 - ����, -1 - �����
    int personID;   // ID ����������
};

// ������� ��� ����������� ������� � ������
int timeToMinutes(int hours, int minutes) {
    return hours * 60 + minutes;
}

// ������� ���������� �������
void quickSort(Event* arr, int low, int high) {
    if (low >= high) return;

    int i = low, j = high;
    Event pivot = arr[(low + high) / 2];

    while (i <= j) {
        while (arr[i].time < pivot.time ||
            (arr[i].time == pivot.time && arr[i].type < pivot.type)) i++;
        while (arr[j].time > pivot.time ||
            (arr[j].time == pivot.time && arr[j].type > pivot.type)) j--;

        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    if (low < j) quickSort(arr, low, j);
    if (i < high) quickSort(arr, i, high);
}

// ������� ��� �������� ������������ ����� �������
bool isValidTime(int h, int m) {
    return h >= 0 && h < 24 && m >= 0 && m < 60;
}

int main() {
    setlocale(LC_ALL, "Russian");
    ifstream input("file.txt");

    // �������� �������� �����
    if (!input.is_open()) {
        cout << "������ �������� �����!" << endl;
        return 1;
    }

    int N;
    input >> N;
    if (input.fail() || N <= 0) {
        cout << "������������ ���������� �����������!" << endl;
        return 1;
    }

    Visitor* visitors = new Visitor[N];
    int valid_visitors = 0;

    for (int i = 0; i < N; ++i) {
        char colon;
        input >> visitors[i].arrival_h >> colon >> visitors[i].arrival_m;
        input >> visitors[i].departure_h >> colon >> visitors[i].departure_m;

        // �������� ������������ �������
        if (input.fail() || colon != ':' ||
            !isValidTime(visitors[i].arrival_h, visitors[i].arrival_m) ||
            !isValidTime(visitors[i].departure_h, visitors[i].departure_m) ||
            timeToMinutes(visitors[i].arrival_h, visitors[i].arrival_m) >=
            timeToMinutes(visitors[i].departure_h, visitors[i].departure_m)) {
            cout << "������������ ������ ��� ����������! " << endl;
            exit(1);
        }

        visitors[i].id = ++valid_visitors;
    }

    // ���� ��� ���������� ���� �����������
    if (valid_visitors == 0) {
        cout << "��� ���������� ������ � �����������!" << endl;
        delete[] visitors;
        return 1;
    }

    // ������ ������ ������� ������ ��� �������� �����������
    Event* events = new Event[2 * valid_visitors];
    for (int i = 0, j = 0; i < N; ++i) {
        if (visitors[i].id == 0) continue; // ���������� ������������ �����������

        events[2 * j] = { timeToMinutes(visitors[i].arrival_h, visitors[i].arrival_m),
                      1, visitors[i].id };
        events[2 * j + 1] = { timeToMinutes(visitors[i].departure_h, visitors[i].departure_m),
                          -1, visitors[i].id };
        j++;
    }

    // ��������� �������
    quickSort(events, 0, 2 * valid_visitors - 1);

    // ������� ��������������� ������ �������
    cout << "��������������� ������ �������:" << endl;
    for (int i = 0; i < 2 * valid_visitors; ++i) {
        int hours = events[i].time / 60;
        int minutes = events[i].time % 60;
        cout << "�����: " << hours << ":" << (minutes < 10 ? "0" : "") << minutes << " | ";
        if (events[i].type == 1) {
            cout << "���� ���������� " << events[i].personID << endl;
        }
        else {
            cout << "����� ���������� " << events[i].personID << endl;
        }
    }
    cout << endl;

    int currentVisitors = 0;
    int maxVisitors = 0;
    int startTime = 0;
    int endTime = 0;
    bool inMaxPeriod = false;

    // �������� �� �������� � ������� ������ � ������������ ������ �����������
    for (int i = 0; i < 2 * valid_visitors; ++i) {
        if (events[i].type == 1) {
            currentVisitors++;
        }
        else {
            currentVisitors--;
        }

        if (currentVisitors > maxVisitors) {
            maxVisitors = currentVisitors;
            startTime = events[i].time;
            inMaxPeriod = true;
        }
        else if (inMaxPeriod && currentVisitors < maxVisitors) {
            endTime = events[i].time;
            inMaxPeriod = false;
        }
    }

    // ������� ����������
    cout << "������������ ���������� �����������: " << maxVisitors << endl;

    int start_h = startTime / 60;
    int start_m = startTime % 60;
    int end_h = endTime / 60;
    int end_m = endTime % 60;

    cout << "��������� ����������: ["
        << start_h << ":" << (start_m < 10 ? "0" : "") << start_m
        << ", "
        << end_h << ":" << (end_m < 10 ? "0" : "") << end_m
        << "]" << endl;

    cout << "���������� � ����� � ���� ������: ";
    for (int i = 0; i < valid_visitors; ++i) {
        int arrival = timeToMinutes(visitors[i].arrival_h, visitors[i].arrival_m);
        int departure = timeToMinutes(visitors[i].departure_h, visitors[i].departure_m);
        if (arrival <= startTime && departure >= endTime) {
            cout << visitors[i].id << " ";
        }
    }
    cout << endl;

    // ����������� ������
    delete[] visitors;
    delete[] events;

    return 0;
}