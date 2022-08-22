#include "error_select.h"
#include "ui_error_select.h"
#include "dialog.h"

error_select::error_select(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::error_select)
{

    ui->setupUi(this);
    mImage = GETDLG() ->mImage;
    QPixmap pixmap = QPixmap::fromImage(mImage.scaled(ui->label->width(), ui->label->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->label->setPixmap(pixmap);
    setWindowTitle("Defect Select");
        m_nPosX = 0;
        m_nPosY =0;
        m_nImgWidth=0;
        m_nImgHeight=0;

        m_fStepX=0;
        m_fStepY=0;
        m_fZoom=1;
        m_nMousePosX=0;
        m_nMousePosY=0;
        check = false;
}

void error_select::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bMouseDown == true)
    {
        if (!mImage.isNull())
        {
            float fStepX = event->x() - m_nMousePosX;
            float fStepY = event->y() - m_nMousePosY;
            fStepX *= m_fStepX;
            fStepY *= m_fStepY;

            m_nPosX -= fStepX;
            m_nPosY -= fStepY;

            OnDisplay();
        }

        m_nMousePosX = event->x();
        m_nMousePosY = event->y();
    }
}

void error_select::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bMouseDown = true;
        m_nMousePosX = event->x();
        m_nMousePosY = event->y();
    }
}

void error_select::mouseReleaseEvent(QMouseEvent *event)
{
    m_bMouseDown = false;
}

void error_select::wheelEvent(QWheelEvent *event)
{
    if (mImage.isNull() )
        return;

    if (event->x() >= ui->label->x() && event->x() <= ui->label->width() + ui->label->x()
            && event->y() >= ui->label->y() && event->y() <= ui->label->height() + ui->label->y())
    {
        if(event->delta() < 0)
        {
            int nDelta = event->delta() * -1;
            float fStep =float(nDelta / 120.0 * 0.01);
            m_fZoom += fStep;
            if (m_fZoom > 1.0)
                m_fZoom = 1.0;
        }
        else if(event->delta() > 0)
        {
            int nDelta = event->delta();
            float fStep = float(nDelta / 120.0 * 0.01);
            m_fZoom -= fStep;
            if (m_fZoom < 0.1)
                m_fZoom = 0.1;
        }
        else
            return;

        m_nImgWidth = int(mImage.width() * m_fZoom);
        m_nImgHeight = int(mImage.height() * m_fZoom);

        OnDisplay();
    }
}

void error_select::OnDisplay()
{
    if (m_nPosX < 0)
        m_nPosX = 0;

    if (m_nPosY < 0)
        m_nPosY = 0;

    if (m_nPosX + m_nImgWidth > mImage.width())
        m_nPosX = mImage.width() - m_nImgWidth;

    if (m_nPosY + m_nImgHeight > mImage.height())
        m_nPosY = mImage.height() - m_nImgHeight;

    m_fStepX = float(m_nImgWidth) / float(ui->label->width());
    m_fStepY = float(m_nImgHeight) / float(ui->label->height());
    mDisplayImage = mImage.copy(m_nPosX, m_nPosY, m_nImgWidth, m_nImgHeight);
    qDebug() << m_nPosX << m_nPosY << m_nImgWidth << m_nImgHeight;
    QPixmap pixmap = QPixmap::fromImage(mDisplayImage.scaled(ui->label->width(), ui->label->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->label->setPixmap(pixmap);

    ui->label_2->setAlignment(Qt::AlignCenter);
    ui->label_2->setStyleSheet("QLabel { background-color : white; color : black; }");
    ui->label_2->setText(QString::number(int((1.01-double(m_fZoom))* 100)));
}
void error_select::closeEvent(QCloseEvent *event)
{
    if (!check)
    {
    event->ignore(); // work
    }
}

void error_select::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        event->ignore(); // not work
    }
}



void error_select::on_pushButton_clicked()
{
    check = true;
    this->close();
}

void error_select::on_verticalSlider_valueChanged(int value)
{
    //    ui->label_2->setNum(double(value)/10);
    m_fZoom = (110 - float(value))/100 ;;
    m_nImgWidth = int(mImage.width() * m_fZoom);
    m_nImgHeight = int(mImage.height() * m_fZoom);
    OnDisplay();
}

error_select::~error_select()
{
    delete ui;
}
