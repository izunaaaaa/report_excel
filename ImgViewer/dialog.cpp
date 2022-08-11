#include "dialog.h"
#include "ui_dialog.h"
#include <iostream>
#include <exception>

#define toKor(str) QString::fromLocal8Bit(str)

Dialog *g_pDlg = nullptr;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    g_pDlg = this;
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->label_2->setAlignment(Qt::AlignCenter);
    setWindowTitle("Image viewer");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    setAcceptDrops(true);
    ui->btn_excel->setEnabled(false);
    m_strInstallPath = QDir::currentPath();
    m_strImageFileName = "";
    img_date = "";
    img_time_hhmm = "";
    img_time_hhmmss = "";

}

Dialog::~Dialog()
{
    delete ui;
}
QStringList Dialog::get_file_list()  // 1분전 파일 + 현재 파일 + 1분 후 파일 리스트 반환
{
    QStringList dir_list;
    QStringList front_list;
    QStringList back_list;
    QStringList current_list;
    QDir front_dir;
    QDir back_dir;
    QDir current_dir;
    QFileInfoList fileinfolist;
    QFileInfo fileinfo;

    int front_file_dir = img_time_hhmm.toInt()-100;
    int back_file_dir = img_time_hhmm.toInt()+100;

    QDir dir (m_strInstallPath + "/../save/main/" + img_date + "/");
    fileinfolist = dir.entryInfoList();

    for( int i = 0; i < fileinfolist.size(); i++)
     {
             QFileInfo fileinfo = fileinfolist.at(i);
             dir_list.push_back(fileinfo.fileName());
     }
    if (dir_list.contains(QString::number(front_file_dir)))
    {
        front_dir = m_strInstallPath + "/../save/main/" + img_date + "/" +(QString::number(front_file_dir));
            fileinfolist = front_dir.entryInfoList();
            for( int i = 2; i < fileinfolist.size(); i++)
            {
                 QFileInfo fileinfo = fileinfolist.at(i);
                 front_list.push_back(fileinfo.absoluteFilePath());
            }
    }
    if (dir_list.contains(QString::number(back_file_dir)))
    {
        back_dir = m_strInstallPath + "/../save/main/" + img_date + "/" +(QString::number(back_file_dir));
            fileinfolist = back_dir.entryInfoList();
            for( int i = 2; i < fileinfolist.size(); i++)
            {
                 QFileInfo fileinfo = fileinfolist.at(i);
                 back_list.push_back(fileinfo.absoluteFilePath());
            }
    }
        current_dir = m_strInstallPath + "/../save/main/" + img_date + "/" + img_time_hhmm;
        fileinfolist = current_dir.entryInfoList();
        for( int i = 2; i < fileinfolist.size(); i++)
        {
             fileinfo = fileinfolist.at(i);
             current_list.push_back(fileinfo.absoluteFilePath());
        }
        all_list = front_list + current_list + back_list;
        idx = 0;
        for(int i = 0; i < all_list.size(); i++)
        {
            if (all_list.at(i).contains(m_strImageFileName))
            {
                idx = i;
                break;
            }
        }

    return all_list;
}

void Dialog::dragEnterEvent(QDragEnterEvent *event) // 이미지 드래그
{
    if (event->mimeData()->hasUrls())
    {
        const QMimeData* mimeData = event->mimeData();

        QList<QUrl> paths = mimeData->urls();
        foreach(QUrl path, paths)
        {
            QString strPath = path.toLocalFile().right(4).toLower();
            if (strPath == ".jpg")
            {
                event->acceptProposedAction();
            }
            break;
        }
    }
}

void Dialog::dropEvent(QDropEvent *event) // 이미지 드롭
{
    const QMimeData* mimeData = event->mimeData();
    ui->label->clear();

    if (mimeData->hasUrls())
    {
        QList<QUrl> paths = mimeData->urls();
        foreach(QUrl path, paths)
        {
            QString strPath = path.toLocalFile().right(4).toLower();
            if (strPath == ".jpg")
            {
                QPixmap pixmap;
                QImageReader reader;
                reader.setFileName(path.toLocalFile());
                mImage = reader.read();
                pixmap = QPixmap::fromImage(mImage.scaled(ui->label->width(), ui->label->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

                ui->label->setPixmap(pixmap);

                setWindowTitle(path.toLocalFile());
                QFileInfo fileInfo(path.toLocalFile());

                m_strImageFileName = fileInfo.fileName();
               img_date = fileInfo.path().right(17).left(10);
               img_time_hhmm = m_strImageFileName.left(4)+"00";
               img_time_hhmmss = m_strImageFileName.left(6);

            }

            break;
        }
    }
}

void Dialog::dragMoveEvent(QDragEnterEvent *event) // 이미지 드래그
{
    event->acceptProposedAction();
}

void Dialog::on_btn_open_clicked() // 버튼 클릭 이벤트
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Image Open"),m_strInstallPath + "/../save/main", tr("Image Files (*.jpg;*.bmp)"));

    if (fileName.isEmpty())
        return;

    QPixmap pixmap;
    QImageReader reader;

    reader.setFileName(fileName);
    mImage = reader.read();
    pixmap = QPixmap::fromImage(mImage.scaled(ui->label->width(), ui->label->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->label->setPixmap(pixmap);

    QFileInfo fileInfo(fileName);
    img_date = fileInfo.path().right(17).left(10);
    m_strImageFileName = fileInfo.fileName();
    img_time_hhmm = m_strImageFileName.left(4) + "00";
    img_time_hhmmss = m_strImageFileName.left(6);


}

void Dialog::on_rd_normal_clicked()  // 라디오 버튼 Normal
{
    ui->btn_excel->setEnabled(true);
}

void Dialog::on_rd_error_clicked()  // 라디오 버튼 Error
{
    ui->btn_excel->setEnabled(true);
}
bool Dialog::navi_cam() // 같은 시간대의 네비캠 이미지 ( 1장 )
{
    QStringList navi_Filelist;
    QFileInfoList fileinfolist;
    QImageReader navi_reader;

    QDir dir (m_strInstallPath + "/../save/navi/" + img_date);
    if (mImage.isNull())
    {
        QMessageBox::information(this,"Error", toKor("Select Image."));
        return false;
    }

    int navi_idx = 0;
    int check = 0;
    fileinfolist = dir.entryInfoList();
    for( int i = 2; i < fileinfolist.size(); i++)
    {
         QFileInfo fileinfo = fileinfolist.at(i);
         navi_Filelist.push_back(fileinfo.filePath());
    }
    for ( int j = 0; j < navi_Filelist.size(); j++)
    {
        if(navi_Filelist.at(j).contains( front_img_time))
        {
            navi_idx = j;
            navi_reader.setFileName(navi_Filelist.at(navi_idx-15));
            navi_img = navi_reader.read();
            check = 1;
        }
        else if(check == 1 && navi_Filelist.at(j).contains( back_img_time) )
        {
            navi_idx = j;
            navi_reader.setFileName(navi_Filelist.at(navi_idx-15));
            navi_img_2 = navi_reader.read();
            return true;
        }
        else continue;
    }
    QMessageBox::information(this,"Error", toKor("같은 시간대 촬영된 네비캠 사진이 없습니다."));
    return false;
}


QImage Dialog::front_pole() // 앞 전주 이미지
{
    QStringList Filelist;
    QImageReader f_reader;

    Filelist = get_file_list();

    if  (idx-50 > 0){
    f_reader.setFileName(Filelist[idx-50]);
    }
    else f_reader.setFileName(Filelist[0]);
    QImage front_pole= f_reader.read();
    return front_pole;
}

QImage Dialog::back_pole() // 뒷 전주 사진
{
    QStringList Filelist;
    QImageReader b_reader;

    Filelist = get_file_list();
    if  (idx+50 > Filelist.size()){
    b_reader.setFileName(Filelist[Filelist.size()-1]);
    }
    else b_reader.setFileName(Filelist[idx+50]);
    QImage back_pole= b_reader.read();
    return back_pole;
}

void Dialog::error_report() // 불량 보고서 작성
{
       if (!mImage.isNull())
{
       // 앞 전주 이미지
       QImage f_pole = front_pole();

       // 뒷 전주 이미지
       QImage b_pole = back_pole();

       QMessageBox reply;
       reply.setWindowTitle(toKor("사진을 확인하고 전주를 선택하세요"));
       reply.setText(toKor("기본 선택 사진은 선택한 사진의 50번째 앞 / 뒤 사진입니다."));
       reply.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
       reply.setDefaultButton(QMessageBox::Ok);
       int ret = reply.exec();
       switch (ret)
       {
       case QMessageBox::Ok:
       {
           //전주 선택 ui
           MainDlg mainDlg;
           mainDlg.exec();

           ui->label_2->setText(toKor("불량 전선 보고서 작성"));
           ui->progressBar->show();

           //open excel
              QXlsx::Document xlsx(m_strInstallPath + "/../Report.xlsx");

              //select sheet
              xlsx.selectSheet(QString::fromLocal8Bit("불량 전선 보고서"));

              //write text
              QXlsx::Format format;
              format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
              format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
              format.setFontSize(24);
              format.setBorderStyle(QXlsx::Format::BorderThin);
              int computerized_num = 111111;
              QString line_name = toKor("선");
              int pole_num = 222222;
              QString date;

              xlsx.write("E2", computerized_num, format);
              xlsx.write("N2", line_name, format);
              xlsx.write("S2", pole_num , format);
              xlsx.write("E3", computerized_num, format);
              xlsx.write("N3", line_name, format);
              xlsx.write("S3", pole_num , format);
              xlsx.write("N4", img_date, format);

              ui->label_2->setText(toKor("전주 정보 삽입"));
              ui->progressBar->setValue(5);


           //insert image
           QImage img = mImage.scaled(ui->label->width()+945, ui->label->height()+360, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
           xlsx.insertImage(103, 1, img);

           ui->label_2->setText(toKor("불량 이미지 삽입"));
           ui->progressBar->setValue(8);

           // insert pole
           f_pole = mainDlg.front_pole.scaled(890,820,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
           b_pole = mainDlg.back_pole.scaled(890,820,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
           xlsx.insertImage(25,1,f_pole);
           xlsx.insertImage(25,11,b_pole);

           ui->label_2->setText(toKor("전주 이미지 삽입"));
           ui->progressBar->setValue(15);

           // insert line
           QImageReader line_reader;
           QImage line;

           int space = (mainDlg.back_pole_idx - mainDlg.front_pole_idx) / (mainDlg.count+1);
           ui->label_2->setText(toKor("전선 삽입"));
           for ( int i =0; i < mainDlg.count; i++)
           {
               line_reader.setFileName(all_list.at(mainDlg.front_pole_idx + space*(i+1)));
               line = line_reader.read();
               line = line.scaled(295,205,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
               ui->progressBar->setValue(ui->progressBar->value()+(i+1)*85/mainDlg.count);

               if ( mainDlg.front_pole_idx + space*(i+1) < idx && mainDlg.front_pole_idx + space*(i+2) >= idx )
               {
                 ui->label_2->setText(toKor("불량 전선 위치 표시"));

                 QPainter painter(&line);
                 painter.setPen(QPen(Qt::red,10));
                 painter.drawRect(0,0,line.width()-1,line.height()-1);

               }

               if (i % 6 == 0)
               {
                  xlsx.insertImage(93 + i/6*5 , 1, line);
               }
               else if (i % 6 == 1)
               {
                   xlsx.insertImage(93 + i/6*5 , 4, line);
               }
               else if (i % 6 == 2)
               {
                   xlsx.insertImage(93 + i/6*5, 7, line);
               }
               else if (i % 6 == 3)
               {
                   xlsx.insertImage(93 + i/6*5 , 11, line);
               }
               else if (i % 6 == 4)
               {
                   xlsx.insertImage(93 + i/6*5 , 14, line);
               }
               else if (i % 6 == 5)
               {
                   xlsx.insertImage(93 + i/6*5 , 17, line);
               }



           }

           //inser navi_img

           front_img_time = all_list.at(mainDlg.front_pole_idx).split('/').back().left(6);
           back_img_time = all_list.at(mainDlg.back_pole_idx).split('/').back().left(6);

           ui->label_2->setText(toKor("전방 사진 삽입"));
           if(navi_cam()==true)
           {
//           navi_cam();
           navi_img = navi_img.scaled(ui->label->width()+20, ui->label->height()+180, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
           navi_img_2 = navi_img_2.scaled(ui->label->width()+20, ui->label->height()+180, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

           QPainter navi_line(&navi_img);
           QPainter navi_line_2(&navi_img_2);
           QPen pen(Qt::red,2,Qt::PenStyle::DotLine);

           navi_line.setPen(pen);

           navi_line.drawLine(250, 0, 250, navi_img.height());
           navi_line.drawLine(navi_img.width()-250, 0, navi_img.width()-250, navi_img.height());

           navi_line.drawLine(0, 200, navi_img.width(), 200);
           navi_line.drawLine(0, navi_img.height()-200, navi_img.width(), navi_img.height()-200);

           navi_line_2.setPen(pen);

           navi_line_2.drawLine(250, 0, 250, navi_img_2.height());
           navi_line_2.drawLine(navi_img_2.width()-250, 0, navi_img_2.width()-250, navi_img_2.height());

           navi_line_2.drawLine(0, 200, navi_img.width(), 200);
           navi_line_2.drawLine(0, navi_img_2.height()-200, navi_img.width(), navi_img_2.height()-200);

           xlsx.insertImage(7,1,navi_img);
           xlsx.insertImage(7,11,navi_img_2);

           ui->progressBar->setValue(95);
           }

           //save excel
           xlsx.saveAs(m_strInstallPath + "/../report/" + "E_Report.xlsx");
           ui->progressBar->setValue(100);
           ui->label_2->setText(toKor("불량 전선 보고서 작성 완료"));
           ui->progressBar->hide();
           ui->progressBar->setValue(0);
           break;
       }
       case QMessageBox::Cancel:
           qDebug() <<QMessageBox::Cancel;
           break;
       }


       }
       else QMessageBox::warning(this,"Error","Select Image");
}
void Dialog::normal_report() // 정상 보고서 작성
{
    if (!mImage.isNull())
{
       // 앞 전주 이미지
       QImage f_pole = front_pole();

       // 뒷 전주 이미지
       QImage b_pole = back_pole();

       QMessageBox reply;
       reply.setWindowTitle(toKor("사진을 확인하고 전주를 선택하세요"));
       reply.setText(toKor("기본 선택 사진은 선택한 사진의 50번째 앞 / 뒤 사진입니다."));
       reply.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
       reply.setDefaultButton(QMessageBox::Ok);
       int ret = reply.exec();
       switch (ret)
       {
       case QMessageBox::Ok:
       {
           //전주 선택 ui
           MainDlg mainDlg;
           mainDlg.exec();

           ui->label_2->setText(toKor("정상 전선 보고서 작성"));
           ui->progressBar->show();

           //open excel
              QXlsx::Document xlsx(m_strInstallPath + "/../Report.xlsx");

              //select sheet
              xlsx.selectSheet(QString::fromLocal8Bit("정상 전선 보고서"));

              //write text
              QXlsx::Format format;
              format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
              format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
              format.setFontSize(24);
              format.setBorderStyle(QXlsx::Format::BorderThin);
              int computerized_num = 111111;
              QString line_name = toKor("선");
              int pole_num = 222222;
              QString date;

              xlsx.write("E2", computerized_num, format);
              xlsx.write("N2", line_name, format);
              xlsx.write("S2", pole_num , format);
              xlsx.write("E3", computerized_num, format);
              xlsx.write("N3", line_name, format);
              xlsx.write("S3", pole_num , format);
              xlsx.write("N4", img_date, format);

              ui->label_2->setText(toKor("전주 정보 삽입"));
              ui->progressBar->setValue(5);


           f_pole = mainDlg.front_pole.scaled(810,820,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
           b_pole = mainDlg.back_pole.scaled(810,820,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
           xlsx.insertImage(25,1,f_pole);
           xlsx.insertImage(25,11,b_pole);

           ui->label_2->setText( toKor("전주 이미지 삽입"));
           ui->progressBar->setValue(15);

           QImageReader line_reader;
           QImage line;
           int space = (mainDlg.back_pole_idx - mainDlg.front_pole_idx) / (mainDlg.count+1);
           ui->label_2->setText(toKor("전선 삽입"));
           for ( int i =0; i < mainDlg.count; i++)
           {
               line_reader.setFileName(all_list.at(mainDlg.front_pole_idx + space*(i+1)));
               line = line_reader.read();
               ui->progressBar->setValue(ui->progressBar->value()+(i+1)*90/mainDlg.count);

               if (i % 3 == 0)
               {
                  line = line.scaled(600,400,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                  xlsx.insertImage(94 + i/3*10 , 1, line);
               }
               else if (i % 3 == 1)
               {
                   line = line.scaled(500,400,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                   xlsx.insertImage(94 + i/3*10 , 8, line);
               }
               else if (i % 3 == 2)
               {
                   line = line.scaled(500,400,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                   xlsx.insertImage(94 + i/3*10 , 15, line);
               }       
           }

           //inser navi_img
           front_img_time = all_list.at(mainDlg.front_pole_idx).split('/').back().left(6);
           back_img_time = all_list.at(mainDlg.back_pole_idx).split('/').back().left(6);
           if(navi_cam()==true)
           {
//           navi_cam();
           navi_img = navi_img.scaled(ui->label->width()-60, ui->label->height()+180, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
           navi_img_2 = navi_img_2.scaled(ui->label->width()-60, ui->label->height()+180, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

           QPainter navi_line(&navi_img);
           QPainter navi_line_2(&navi_img_2);
           QPen pen(Qt::red,2,Qt::PenStyle::DotLine);

           navi_line.setPen(pen);

           navi_line.drawLine(250, 0, 250, navi_img.height());
           navi_line.drawLine(navi_img.width()-250, 0, navi_img.width()-250, navi_img.height());

           navi_line.drawLine(0, 200, navi_img.width(), 200);
           navi_line.drawLine(0, navi_img.height()-200, navi_img.width(), navi_img.height()-200);

           navi_line_2.setPen(pen);

           navi_line_2.drawLine(250, 0, 250, navi_img_2.height());
           navi_line_2.drawLine(navi_img_2.width()-250, 0, navi_img_2.width()-250, navi_img_2.height());

           navi_line_2.drawLine(0, 200, navi_img.width(), 200);
           navi_line_2.drawLine(0, navi_img_2.height()-200, navi_img.width(), navi_img_2.height()-200);

           xlsx.insertImage(7,1,navi_img);
           xlsx.insertImage(7,11,navi_img_2);

           ui->label_2->setText(toKor("전방 사진 삽입"));
           ui->progressBar->setValue(95);
           }
           //save excel
           xlsx.saveAs(m_strInstallPath + "/../report/" + "N_Report.xlsx");

           ui->progressBar->setValue(100);
           ui->label_2->setText(toKor("정상 전선 보고서 작성 완료"));
           ui->progressBar->hide();
           ui->progressBar->setValue(0);


           break;
       }
       case QMessageBox::Cancel:
           qDebug() <<QMessageBox::Cancel;
           break;
       }

}
       else QMessageBox::warning(this,"Error","Select Image");
}
void Dialog::on_btn_excel_clicked()   // 보고서 작성 버튼 클릭
{
    if(ui->rd_error->isChecked())
   {
       check = false; // 불량 false
       error_report();
   }
   if(ui->rd_normal->isChecked())
   {
       check = true; // 정상 true
       normal_report();
   }
}

