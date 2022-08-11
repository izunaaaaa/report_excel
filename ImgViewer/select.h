#ifndef SELECT_H
#define SELECT_H

#include <QMainWindow>

namespace Ui {
class Select;
}

class Select : public QMainWindow
{
    Q_OBJECT

public:
    explicit Select(QWidget *parent = nullptr);
    ~Select();



private:
    Ui::Select *ui;
};

#endif // SELECT_H
