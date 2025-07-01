#include "mochikorowidget.h"
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QMessageBox>
#include <Qdir>

// Наша самодельная очередь (замена QQueue)
class SimpleQueue {
private:
    struct Node {
        int value;
        Node* next;
    };
    Node* head;
    Node* tail;

public:
    SimpleQueue() : head(nullptr), tail(nullptr) {}

    ~SimpleQueue() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void enqueue(int value) {
        Node* newNode = new Node{value, nullptr};
        if (tail) {
            tail->next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
    }

    int dequeue() {
        if (!head) return -1;
        int result = head->value;
        Node* temp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
        return result;
    }

    bool isEmpty() const {
        return head == nullptr;
    }
};

// Наш самодельный аналог QSet (для хранения уникальных значений)
class SimpleSet {
private:
    struct Node {
        int value;
        Node* next;
    };
    Node* head;

public:
    SimpleSet() : head(nullptr) {}

    ~SimpleSet() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insert(int value) {
        if (contains(value)) return;
        head = new Node{value, head};
    }

    bool contains(int value) const {
        Node* current = head;
        while (current) {
            if (current->value == value) return true;
            current = current->next;
        }
        return false;
    }

    int size() const {
        int count = 0;
        Node* current = head;
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }
};

// Наш самодельный аналог QMap<int, SimpleSet> (для графа)
class IslandGraph {
private:
    struct GraphNode {
        int island;
        SimpleSet* connections;
        GraphNode* next;
    };
    GraphNode* head;

public:
    IslandGraph() : head(nullptr) {}

    ~IslandGraph() {
        while (head) {
            GraphNode* temp = head;
            head = head->next;
            delete temp->connections;
            delete temp;
        }
    }

    void addConnection(int from, int to) {
        GraphNode* node = getOrCreateNode(from);
        node->connections->insert(to);

        node = getOrCreateNode(to);
        node->connections->insert(from);
    }

    SimpleSet* getConnections(int island) {
        GraphNode* node = findNode(island);
        return node ? node->connections : nullptr;
    }

private:
    GraphNode* findNode(int island) {
        GraphNode* current = head;
        while (current) {
            if (current->island == island) return current;
            current = current->next;
        }
        return nullptr;
    }

    GraphNode* getOrCreateNode(int island) {
        GraphNode* node = findNode(island);
        if (!node) {
            node = new GraphNode{island, new SimpleSet(), head};
            head = node;
        }
        return node;
    }
};


MochikoroWidget::MochikoroWidget(QWidget *parent) : QWidget(parent), currentLevel(0), size(10), allocatedSize(0), currentLevelMatrix(nullptr)
{
    grid = nullptr; // Инициализируем grid как nullptr
    initializeGrid(size);

    QPushButton *nextButton = new QPushButton("Next Level", this);
    QPushButton *prevButton = new QPushButton("Previous Level", this);
    QPushButton *checkButton = new QPushButton("Check Level", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *gridWidget = new QWidget(this);
    gridWidget->setFixedSize(size * 50, size * 50);
    layout->addWidget(gridWidget);
    layout->addWidget(prevButton);
    layout->addWidget(nextButton);
    layout->addWidget(checkButton);

    connect(nextButton, &QPushButton::clicked, this, &MochikoroWidget::nextLevel);
    connect(prevButton, &QPushButton::clicked, this, &MochikoroWidget::prevLevel);
    connect(checkButton, &QPushButton::clicked, this, &MochikoroWidget::checkLevel);

    loadLevel(currentLevel);
}

MochikoroWidget::~MochikoroWidget()
{
    cleanupGrid();
}

void MochikoroWidget::initializeGrid(int s)
{
    cleanupGrid();
    size = s;
    if (size <= 0) {
        qDebug() << "Недопустимый размер сетки:" << size;
        size = 6;
    }
    qDebug() << "Инициализация сетки размером:" << size;
    grid = new int*[size];
    for (int i = 0; i < size; i++) {
        grid[i] = new int[size]();
    }
    allocatedSize = size;
}

void MochikoroWidget::cleanupGrid()
{
    if (grid) {
        qDebug() << "Очистка сетки, размер:" << allocatedSize;
        for (int i = 0; i < allocatedSize; i++) {
            if (grid[i]) {
                delete[] grid[i];
                grid[i] = nullptr;
            }
        }
        delete[] grid;
        grid = nullptr;
        allocatedSize = 0;
    }
}

void MochikoroWidget::loadLevel(int level) {
    // Очищаем предыдущие данные
    cleanupGrid();
    delete currentLevelMatrix;
    currentLevelMatrix = new SparseMatrix();
    bool isLevelNumber = false;

    QFile file("levels.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Ошибка открытия файла уровней";
        initializeGrid(6); // Инициализируем сетку по умолчанию
        return;
    }

    QTextStream in(&file);
    int foundLevel = -1;
    int foundSize = 0;
    QString line;

    // Ищем нужный уровень
    while (in.readLineInto(&line)) {
        if (line.startsWith("SIZE")) {
            foundSize = line.section(' ', 1, 1).toInt();
            initializeGrid(foundSize);
        }
        if (line.startsWith("LEVEL")) {
            foundLevel = line.section(' ', 1, 1).toInt();
            if (foundLevel == level + 1) { // Уровни нумеруются с 1 в файле
                if (!currentLevelMatrix->loadFromStream(in)) {
                    qDebug() << "Ошибка загрузки уровня" << level;

                    file.close();
                    return;
                }
                break;
            }
        }
    }

    file.close();

    if (foundLevel != level + 1) {
        qDebug() << "Уровень" << level << "не найден";
        initializeGrid(6);
        return;
    }


    // Инициализируем grid на основе sparse матрицы
    size = currentLevelMatrix->getSize();
    initializeGrid(size);
    currentLevelMatrix->toDense(grid);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == 1) {
                grid[i][j] = -2; // 2 обозначает начальную единичку
            }
        }
    }

    update();
    parentWidget()->resize(size * 50 + 20, size * 50 + 100);
}

void MochikoroWidget::nextLevel()
{
    QFile file("levels.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл уровней";
        return;
    }

    QTextStream in(&file);
    int maxLevel = -1;
    QString line;

    // Считаем количество уровней в файле
    while (in.readLineInto(&line)) {
        if (line.startsWith("LEVEL")) {
            maxLevel = qMax(maxLevel, line.section(' ', 1, 1).toInt());
        }
    }
    file.close();

    if (currentLevel < maxLevel - 1) { // Уровни нумеруются с 0
        currentLevel++;
        loadLevel(currentLevel);
    } else {
        QMessageBox::information(this, "Info", "Это последний уровень");
    }
}

void MochikoroWidget::prevLevel()
{
    if (currentLevel > 0) {
        currentLevel--;
        loadLevel(currentLevel);
    } else {
        QMessageBox::information(this, "Info", "Это первый уровень");
    }
}

void MochikoroWidget::checkLevel()
{
    if (validateLevel()) {
        QMessageBox::information(this, "Успех", "Уровень пройден!");
    } else {
        QMessageBox::warning(this, "Ошибка", "Уровень не пройден. Проверьте правила.");
    }
}

void MochikoroWidget::mousePressEvent(QMouseEvent *event)
{
    int cellSize = 50;
    int row = event->y() / cellSize;
    int col = event->x() / cellSize;

    if (row >= 0 && row < size && col >= 0 && col < size && grid[row][col] <= 1) {
        if (grid[row][col] == 0) grid[row][col] = 1;
        else if (grid[row][col] == 1) grid[row][col] = -1;
        else if (grid[row][col] == -1) grid[row][col] = 0;
        update();
    }
}

bool MochikoroWidget::canPlaceWhite(int row, int col)
{
    if (row < 0 || row >= size || col < 0 || col >= size) return false;
    if (grid[row][col] == -1) return false;
    if (grid[row][col] == 1 || grid[row][col] > 1) return true;

    if (row > 0 && grid[row-1][col] == 1) return false;
    if (row < size-1 && grid[row+1][col] == 1) return false;
    if (col > 0 && grid[row][col-1] == 1) return false;
    if (col < size-1 && grid[row][col+1] == 1) return false;

    return true;
}

bool MochikoroWidget::checkNoBlackSquare()
{
    // Проверяем только клетки с grid[i][j] == -1 (черные)
    // Клетки с grid[i][j] == 0 (серые) не считаются черными
    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j < size-1; j++) {
            if (grid[i][j] == -1 && grid[i][j+1] == -1 &&
                grid[i+1][j] == -1 && grid[i+1][j+1] == -1) {
                qDebug() << "Найден квадрат 2x2 из черных клеток в (" << i << "," << j << ")";
                return false;
            }
        }
    }
    return true;
}

void MochikoroWidget::markConnected(int row, int col, bool **visited)
{
    if (row < 0 || row >= size || col < 0 || col >= size) return;
    if (visited[row][col]) return;
    if (grid[row][col] != 1 && grid[row][col] != -2 && grid[row][col] <= 1) return;

    visited[row][col] = true;

    markConnected(row - 1, col, visited); // вверх
    markConnected(row + 1, col, visited); // вниз
    markConnected(row, col - 1, visited);  // влево
    markConnected(row, col + 1, visited);  // вправо
}

// Переработанная функция проверки связности
bool MochikoroWidget::checkDiagonalConnectivity(int** islandMap, int currentIsland)
{
    // Поиск диагональных связей между островами
    IslandGraph graph;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (islandMap[i][j] == 0) continue;

            for (int di = -1; di <= 1; di += 2) {
                for (int dj = -1; dj <= 1; dj += 2) {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < size && nj >= 0 && nj < size &&
                        islandMap[ni][nj] > 0 && islandMap[ni][nj] != islandMap[i][j]) {
                        graph.addConnection(islandMap[i][j], islandMap[ni][nj]);
                    }
                }
            }
        }
    }

    // Проверка связности (обход в ширину)
    SimpleSet visited;
    SimpleQueue queue;
    queue.enqueue(1);
    visited.insert(1);

    while (!queue.isEmpty()) {
        int current = queue.dequeue();
        SimpleSet* connections = graph.getConnections(current);
        if (!connections) continue;
        for (int neighbor = 1; neighbor < currentIsland; neighbor++) {
            if (connections->contains(neighbor) && !visited.contains(neighbor)) {
                visited.insert(neighbor);
                queue.enqueue(neighbor);
            }
        }
    }

    bool allConnected = (visited.size() == currentIsland - 1);

    if (!allConnected) {
        qDebug() << "Несвязанные острова:";
        for (int island = 1; island < currentIsland; island++) {
            if (!visited.contains(island)) {
                qDebug() << "Остров" << island;
            }
        }
    }

    return allConnected;
}


void MochikoroWidget::fillIslandOrthogonal(int row, int col, int islandId, int** islandMap)
{
    if (row < 0 || row >= size || col < 0 || col >= size) return;
    if (islandMap[row][col] > 0) return;
    if (grid[row][col] != 1 && grid[row][col] != -2 && grid[row][col] <= 1) return;

    islandMap[row][col] = islandId;

    fillIslandOrthogonal(row-1, col, islandId, islandMap); // вверх
    fillIslandOrthogonal(row+1, col, islandId, islandMap); // вниз
    fillIslandOrthogonal(row, col-1, islandId, islandMap); // влево
    fillIslandOrthogonal(row, col+1, islandId, islandMap); // вправо
}

void MochikoroWidget::markDiagonallyConnected(int row, int col, bool **visited)
{
    if (row < 0 || row >= size || col < 0 || col >= size) return;
    if (visited[row][col]) return;
    if (grid[row][col] != 1 && grid[row][col] != -2 && grid[row][col] <= 1) return;

    visited[row][col] = true;

    markDiagonallyConnected(row-1, col-1, visited);
    markDiagonallyConnected(row-1, col+1, visited);
    markDiagonallyConnected(row+1, col-1, visited);
    markDiagonallyConnected(row+1, col+1, visited);
}

bool MochikoroWidget::validateLevel()
{
    if (!grid) return false;

    bool isValid = true;
    QStringList errorMessages;

    // Шаг 1: Разметка островов и проверка серых клеток за один проход
    int** islandMap = new int*[size];
    for (int i = 0; i < size; i++) {
        islandMap[i] = new int[size]();
    }

    bool hasGrayCells = false;
    int currentIsland = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == 0) {
                hasGrayCells = true;
            }
            if ((grid[i][j] == 1 || grid[i][j] == -2 || grid[i][j] > 1) && islandMap[i][j] == 0) {
                fillIsland(i, j, currentIsland, islandMap);
                currentIsland++;
            }
        }
    }

    if (hasGrayCells) {
        errorMessages << "В сетке остались незакрашенные (серые) клетки. Все клетки должны быть белыми или черными.";
        isValid = false;
    }

    // Шаг 2: Подсчитаем размеры островов
    const int MAX_ISLANDS = 100; // Ограничение на количество островов
    int* islandSizes = new int[MAX_ISLANDS]();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (islandMap[i][j] > 0) {
                islandSizes[islandMap[i][j]]++;
            }
        }
    }

    // Шаг 3: Проверка размеров островов
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] > 1 || grid[i][j] == -2) {
                int expectedSize = (grid[i][j] == -2) ? 1 : grid[i][j];
                int islandId = islandMap[i][j];
                int filled = islandSizes[islandId];
                qDebug() << "Проверка числа" << expectedSize << "в позиции (" << i << "," << j << "): подсчитано" << filled << "клеток";
                if (filled != expectedSize) {
                    errorMessages << QString("Число %1 в (%2,%3) ожидает %4 клеток, найдено %5")
                                         .arg(grid[i][j] == -2 ? 1 : grid[i][j]).arg(i).arg(j).arg(expectedSize).arg(filled);
                    isValid = false;
                }
            }
        }
    }

    // Проверка отсутствия черных квадратов 2x2
    if (!checkNoBlackSquare()) {
        errorMessages << "Найден запрещенный 2x2 квадрат из черных клеток";
        isValid = false;
    }

    // Проверка, что белые регионы не соприкасаются сторонами
    if (!checkWhiteRegionsNotTouching(islandMap)) {
        errorMessages << "Белые регионы соприкасаются сторонами (см. отладочный вывод)";
        isValid = false;
    }

    // Проверка диагональной связности всех белых регионов
    if (!checkDiagonalConnectivity(islandMap, currentIsland)) {
        errorMessages << "Белые регионы не связаны диагонально";
        isValid = false;
    }

    // Очистка памяти
    for (int i = 0; i < size; i++) delete[] islandMap[i];
    delete[] islandMap;
    delete[] islandSizes;

    if (!isValid) {
        qDebug() << "Найдены ошибки:";
        foreach (const QString &msg, errorMessages) {
            qDebug() << " - " << msg;
        }
        QMessageBox::warning(this, "Ошибки", "Уровень содержит ошибки:\n" + errorMessages.join("\n"));
    } else {
        qDebug() << "Все проверки пройдены успешно!";
        QMessageBox::information(this, "Успех", "Уровень пройден!");
    }

    return isValid;
}

// Новая функция для проверки связности всех белых клеток
bool MochikoroWidget::checkWhiteConnectivity()
{
    bool **visited = new bool*[size];
    for (int i = 0; i < size; i++) {
        visited[i] = new bool[size]();
    }

    int startX = -1, startY = -1;
    for (int i = 0; i < size && startX == -1; i++) {
        for (int j = 0; j < size && startX == -1; j++) {
            if (grid[i][j] == 1 || grid[i][j] == -2 || grid[i][j] > 1) {
                startX = i;
                startY = j;
            }
        }
    }

    if (startX == -1) {
        for (int i = 0; i < size; i++) delete[] visited[i];
        delete[] visited;
        return true;
    }

    markConnected(startX, startY, visited);

    bool allConnected = true;
    for (int i = 0; i < size && allConnected; i++) {
        for (int j = 0; j < size && allConnected; j++) {
            if ((grid[i][j] == 1 || grid[i][j] == -2 || grid[i][j] > 1) && !visited[i][j]) {
                qDebug() << "Несвязанная белая клетка или число в (" << i << "," << j << ")";
                allConnected = false;
            }
        }
    }

    for (int i = 0; i < size; i++) delete[] visited[i];
    delete[] visited;

    return allConnected;
}

bool MochikoroWidget::checkWhiteRegionsNotTouching(int** islandMap)
{
    bool valid = true;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (islandMap[i][j] > 0) {
                if (i > 0 && islandMap[i-1][j] > 0 && islandMap[i-1][j] != islandMap[i][j]) {
                    qDebug() << "Острова" << islandMap[i][j] << "и" << islandMap[i-1][j]
                             << "соприкасаются сторонами в (" << i << "," << j << ") и (" << i-1 << "," << j << ")";
                    valid = false;
                }
                if (j > 0 && islandMap[i][j-1] > 0 && islandMap[i][j-1] != islandMap[i][j]) {
                    qDebug() << "Острова" << islandMap[i][j] << "и" << islandMap[i][j-1]
                             << "соприкасаются сторонами в (" << i << "," << j << ") и (" << i << "," << j-1 << ")";
                    valid = false;
                }
            }
        }
    }

    if (valid) {
        qDebug() << "Проверка на соприкосновение островов: OK";
    } else {
        qDebug() << "Найдены разные острова, соприкасающиеся сторонами!";
    }

    return valid;
}
// Вспомогательная функция для заливки острова
void MochikoroWidget::fillIsland(int row, int col, int islandId, int** islandMap)
{
    if (row < 0 || row >= size || col < 0 || col >= size) return;
    if (islandMap[row][col] > 0) return;
    if (grid[row][col] != 1 && grid[row][col] != -2 && grid[row][col] <= 1) return;

    SimpleQueue queue;
    queue.enqueue(row * size + col);
    islandMap[row][col] = islandId;

    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // вверх, вниз, влево, вправо

    while (!queue.isEmpty()) {
        int pos = queue.dequeue();
        int r = pos / size;
        int c = pos % size;

        for (int d = 0; d < 4; d++) {
            int nr = r + directions[d][0];
            int nc = c + directions[d][1];
            if (nr < 0 || nr >= size || nc < 0 || nc >= size) continue;
            if (islandMap[nr][nc] > 0) continue;
            if (grid[nr][nc] != 1 && grid[nr][nc] != -2 && grid[nr][nc] <= 1) continue;

            islandMap[nr][nc] = islandId;
            queue.enqueue(nr * size + nc);
        }
    }
}

void MochikoroWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int cellSize = 50;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int x = j * cellSize;
            int y = i * cellSize;

            if (grid[i][j] == 1 || grid[i][j] == -2) {
                painter.setBrush(Qt::white);
            } else if (grid[i][j] == -1) {
                painter.setBrush(Qt::black);
            } else if (grid[i][j] > 1) {
                painter.setBrush(Qt::white);
            } else {
                painter.setBrush(Qt::gray);
            }

            painter.drawRect(x, y, cellSize, cellSize);

            if (grid[i][j] > 1) {
                painter.setPen(Qt::black);
                QFont font = painter.font();
                font.setPointSize(12);
                painter.setFont(font);
                QRect textRect(x, y, cellSize, cellSize);
                painter.drawText(textRect, Qt::AlignCenter, QString::number(grid[i][j]));
            }
            if (grid[i][j] == -2) {
                painter.setPen(Qt::black);
                QFont font = painter.font();
                font.setPointSize(12);
                painter.setFont(font);
                QRect textRect(x, y, cellSize, cellSize);
                painter.drawText(textRect, Qt::AlignCenter, QString::number(1));
            }

            painter.setBrush(Qt::NoBrush);
            painter.setPen(Qt::black);
            painter.drawRect(x, y, cellSize, cellSize);
        }
    }
}
