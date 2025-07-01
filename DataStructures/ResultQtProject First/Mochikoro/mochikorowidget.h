#ifndef MOCHIKOROWIDGET_H
#define MOCHIKOROWIDGET_H

#include <QWidget>



class SparseMatrix {
private:
    int* A;  // Значения
    int* LI; // Номера строк
    int* LJ; // Номера столбцов
    int NZ;  // Количество ненулевых элементов
    int size; // Размер матрицы (size x size)

public:
    SparseMatrix() : A(nullptr), LI(nullptr), LJ(nullptr), NZ(0), size(0) {}

    ~SparseMatrix() {
        cleanup();
    }

    void cleanup() {
        delete[] A; A = nullptr;
        delete[] LI; LI = nullptr;
        delete[] LJ; LJ = nullptr;
        NZ = 0;
        size = 0;
    }

    bool loadFromStream(QTextStream& in) {
        cleanup();

        QString line;
        while (in.readLineInto(&line)) {
            if (line.startsWith("SIZE")) {
                size = line.section(' ', 1, 1).toInt();
            }
            else if (line.startsWith("NZ")) {
                NZ = line.section(' ', 1, 1).toInt();
                A = new int[NZ];
                LI = new int[NZ];
                LJ = new int[NZ];
            }
            else if (line.startsWith("A")) {
                QStringList values = line.split(' ', Qt::SkipEmptyParts);
                for (int i = 1; i < values.size() && i-1 < NZ; i++) {
                    A[i-1] = values[i].toInt();
                }
            }
            else if (line.startsWith("LI")) {
                QStringList values = line.split(' ', Qt::SkipEmptyParts);
                for (int i = 1; i < values.size() && i-1 < NZ; i++) {
                    LI[i-1] = values[i].toInt();
                }
            }
            else if (line.startsWith("LJ")) {
                QStringList values = line.split(' ', Qt::SkipEmptyParts);
                for (int i = 1; i < values.size() && i-1 < NZ; i++) {
                    LJ[i-1] = values[i].toInt();
                }
                return true; // Успешно загружено
            }
        }
        return false;
    }

    void toDense(int** dense) const {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                dense[i][j] = 0;
            }
        }

        for (int k = 0; k < NZ; k++) {
            dense[LI[k]][LJ[k]] = A[k];
        }
    }

    int getSize() {
        return this->size;
    }
};

class Cell {
public:
    int value;  // 0 - пусто, >1 - число из уровня, -1 - черная
    bool isFilled; // true - закрашена белым (часть острова)

    Cell() : value(0), isFilled(false) {}
};
class MochikoroWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MochikoroWidget(QWidget *parent = nullptr);
    ~MochikoroWidget();

    void loadLevel(int level);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void nextLevel();
    void prevLevel();
    void checkLevel();

private:
    SparseMatrix* currentLevelMatrix;
    int **grid;
    int currentLevel;
    int size; // Текущий размер квадратной матрицы (X)
    int allocatedSize; // Размер, для которого выделена память

    void initializeGrid(int s);
    void cleanupGrid();
    bool canPlaceWhite(int row, int col);
    void fillIsland(int row, int col, int islandId, int** islandMap);
    bool checkNoBlackSquare();
    void markConnected(int row, int col, bool **visited);
    bool checkDiagonalConnectivity(int** islandMap, int currentIsland);
    bool validateLevel();
    bool checkWhiteConnectivity();
    bool checkWhiteRegionsNotTouching(int** islandMap);
    void markDiagonallyConnected(int row, int col, bool **visited);
    void fillIslandOrthogonal(int row, int col, int islandId, int** islandMap);
};

#endif // MOCHIKOROWIDGET_H
