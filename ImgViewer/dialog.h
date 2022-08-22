#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QtWidgets>
#include <QString>
#include <vector>
#include <iostream>
#include <qimage.h>
#include "xlsxdocument.h"
#include "xlsxdatavalidation.h"
#include "xlsxformat.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
#include <algorithm>
#include "maindlg.h"
#include "error_select.h"
#include <QPainter>
#include <QPen>

using namespace std;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    QString m_strInstallPath;
    QString m_strImageFileName;

    QImage mImage;
    QPixmap mPixmap;
    QString img_time_hhmmss;
    QString img_time_hhmm;
    QString img_date;
    QStringList get_file_list();
    QStringList all_list;
    int idx;
    QImage front_pole();
    QImage back_pole();
    QString front_img_time;
    QString back_img_time;
    QPixmap pixmap;
    MainDlg *m_pMainDlg;
    error_select *m_error;
    bool check;  

private slots:
    void on_btn_open_clicked();
    void on_btn_excel_clicked();
    void on_rd_normal_clicked();
    void on_rd_error_clicked();

    void on_verticalSlider_valueChanged(int value);

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    void dragMoveEvent(QDragEnterEvent* event);
    void error_report();
    void normal_report();
    bool navi_cam();
    QImage navi_img;
    QImage navi_img_2;
    int zoom_level;

    QImage mDisplayImage;

    int m_nPosX;
    int m_nPosY;
    int m_nImgWidth;
    int m_nImgHeight;

    float m_fStepX;
    float m_fStepY;
    float m_fZoom;


    bool m_bMouseDown;
    int m_nMousePosX;
    int m_nMousePosY;


    void OnDisplay();


    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H

extern Dialog       *g_pDlg;
#define GETDLG()    g_pDlg
#define toKor(str) QString::fromLocal8Bit(str)
