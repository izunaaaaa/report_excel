#ifndef ERROR_SELECT_H
#define ERROR_SELECT_H

#include <QDialog>

namespace Ui {
class error_select;
}

class error_select : public QDialog
{
    Q_OBJECT

public:
    //?? ????? Zoom?? ?? ??
    int m_nPosX;
    int m_nPosY;
    int m_nImgWidth;
    int m_nImgHeight;
    QImage mImage;
    QImage mDisplayImage;
    //Zoom level ? pixel? ?? ??
    float m_fStepX;
    float m_fStepY;
    float m_fZoom;

    //Zoom ?? move??? ?? ??
    bool m_bMouseDown;
    int m_nMousePosX;
    int m_nMousePosY;

    //?? display
    void OnDisplay();

    bool check;

    //mouse event? ???? ?? api
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    explicit error_select(QWidget *parent = nullptr);
    ~error_select();

private slots:
    void on_pushButton_clicked();
    void on_verticalSlider_valueChanged(int value);

private:
    Ui::error_select *ui;
};

#endif // ERROR_SELECT_H
