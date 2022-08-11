#include "maindlg.h"
#include "ui_maindlg.h"

#include "dialog.h"

#define toKor(str) QString::fromLocal8Bit(str)

MainDlg::MainDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDlg)
{
    ui->setupUi(this);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);


    m_strInstallPath =  GETDLG() ->m_strInstallPath;
    front_pole = GETDLG() -> front_pole();
    back_pole = GETDLG() -> back_pole();

    int idx = GETDLG() -> idx;

    if (idx < 50)
    {
       front_pole_idx = 0;
    }
    else front_pole_idx = idx - 50;

    if (idx + 50 > GETDLG()->all_list.size())
    {
        back_pole_idx = GETDLG() -> all_list.size()-1;
    }
    else back_pole_idx= idx + 50;

    if  (!GETDLG()->check)
    {
    ui->label_3->hide();
    ui->number->hide();
    count = 12;
    }
    else {
        count = 8;
        ui->number->setSuffix("장");
        ui->number->setWrapping(true);
        ui->number->setRange(8,15);
    }

    pixmap = QPixmap::fromImage(front_pole.scaled(476, 431, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->f_pole->setPixmap(pixmap);
    pixmap = QPixmap::fromImage(back_pole.scaled(476, 431, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->b_pole->setPixmap(pixmap);
    pole_list = GETDLG()->all_list;
    for (int i =0; i < idx; i++)
    {
        ui->f_pole_list->addItem(pole_list[idx-i]);
    }
    for (int i =0; idx+i < pole_list.size(); i++)
    {
        ui->b_pole_list->addItem(pole_list[idx+i]);
    }
    if (ui->f_pole_list->count()!=0)
    {
        ui->f_pole_list->item(0)->setBackgroundColor(QColor(222, 254, 233,220));
    }

    if (ui->b_pole_list->count()!=0)
    {
        ui->b_pole_list->item(0)->setBackgroundColor(QColor(222, 254, 233,220));
    }

}

MainDlg::~MainDlg()
{
    delete ui;
}

void MainDlg::on_f_pole_list_currentRowChanged(int currentRow)
{
    if(currentRow!=0){
        ui->f_pole_list->item(currentRow)->setBackgroundColor(QColor(252, 224, 233));
        reader.setFileName(ui->f_pole_list->item(currentRow)->text());
        front_pole = reader.read();
        pixmap = QPixmap::fromImage(front_pole.scaled(476, 431, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->f_pole->setPixmap(pixmap);
        front_pole_idx = GETDLG()->idx - ui->f_pole_list->currentRow();
    }
}


void MainDlg::on_b_pole_list_currentRowChanged(int currentRow)
{
    if(currentRow!=0)
    {
        QPixmap pixmap;
        QImageReader reader;
        ui->b_pole_list->item(currentRow)->setBackgroundColor(QColor(252, 254, 233));
        reader.setFileName(ui->b_pole_list->item(currentRow)->text());
        back_pole = reader.read();
        pixmap = QPixmap::fromImage(back_pole.scaled(476, 431, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->b_pole->setPixmap(pixmap);    
        back_pole_idx = GETDLG()->idx + ui->b_pole_list->currentRow();
    }
}

void MainDlg::on_pushButton_clicked()
{
    if (GETDLG()->check){
    QMessageBox::information(this,"Success", "Normal Report Create");
    this->close();
    }
    else
    {

        QMessageBox::information(this,"Success", "Error Report Create");

        this->close();
    }
}


void MainDlg::on_number_valueChanged(int arg1)
{
    count = arg1;
}
