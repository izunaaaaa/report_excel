#ifndef MAINDLG_H
#define MAINDLG_H

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QKeyEvent>
#include <QStringList>
#include <QImageReader>

namespace Ui {
class MainDlg;
}

class MainDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MainDlg(QWidget *parent = nullptr);
    ~MainDlg();
    QString  m_strInstallPath;
    QImage mImage;
    QImage back_pole;
    QImage front_pole;
    QPixmap pixmap;
    QImageReader reader;
    QStringList pole_list;
    int front_pole_idx;
    int back_pole_idx;
    int count;



private slots:

    void on_f_pole_list_currentRowChanged(int currentRow);

    void on_b_pole_list_currentRowChanged(int currentRow);

    void on_pushButton_clicked();

    void on_number_valueChanged(int arg1);

private:
    Ui::MainDlg *ui;

};

#endif // MAINDLG_H
