#ifndef CAMP_H
#define CAMP_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Camp;
}
QT_END_NAMESPACE

class Camp : public QWidget
{
    Q_OBJECT

public:
    Camp(QWidget *parent = nullptr);
    ~Camp();

private:
    Ui::Camp *ui;
};
#endif // CAMP_H
