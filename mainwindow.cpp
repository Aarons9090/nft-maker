#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateListWidget()
{
    QListWidget *widget = ui->imageList;

    widget->clear();

    for(vector<QString> &vec: file_directories__){
        for(QString const &str : vec){
            new QListWidgetItem(str, widget);
        }
    }

}

bool MainWindow::combineImages()
{
    // number of arrays
        vector<vector<QString>> combinations;
        int n = file_directories__.size();

        // to keep track of next element in each of
        // the n arrays
        int* indices = new int[n];

        // initialize with first element's index
        for (int i = 0; i < n; i++)
            indices[i] = 0;

        while (1) {
            vector<QString> vec;
            // print current combination

            for (int i = 0; i < n; i++){
                vec.push_back(file_directories__[i][indices[i]]);
            }

            combinations.push_back(vec);



            // find the rightmost array that has more
            // elements left after the current element
            // in that array
            int next = n - 1;
            while (next >= 0 &&
                  (indices[next] + 1 >= int(file_directories__[next].size())))
                next--;

            // no such array is found so no more
            // combinations left
            if (next < 0)
                break;
            // if found move to next element in that
            // array
            indices[next]++;

            // for all arrays to the right of this
            // array current index again points to
            // first element
            for (int i = next + 1; i < n; i++)
                indices[i] = 0;

        }
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
                                                        QString(),
                                                        tr("Images (*.png)"));

        qDebug() << "combinations: " << combinations.size();


        QImage resultImage;
        resultImage = QImage(QSize(100,100), QImage::Format_ARGB32_Premultiplied);

        QImage img1(combinations[0][0]);
        QImage img2(combinations[0][1]);
        QImage img3(combinations[0][2]);

        QPainter painter(&resultImage);

        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(resultImage.rect(), Qt::transparent);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        painter.drawImage(0, 0, img1);
        painter.drawImage(0, 0, img2);
        painter.drawImage(0, 0, img3);
//        painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
//        painter.fillRect(resultImage.rect(), Qt::white);
        painter.end();

        resultImage.save(fileName);

        ui->imageLabel->setPixmap(QPixmap::fromImage(resultImage));
        ui->imageLabel->setScaledContents( true );

        ui->imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

//        for(vector<QString> &vec: combinations){
//            QImage resultImage;
//            for(QString const &image_name : vec){

//                resultImage = QImage(QSize(100,100), QImage::Format_ARGB32_Premultiplied);
//                QImage img1 = resultImage;
//                QImage img2(image_name);

//                QPainter::CompositionMode mode = QPainter::CompositionMode::CompositionMode_SourceOver;

//                QPainter painter(&resultImage);
//                painter.setCompositionMode(QPainter::CompositionMode_Source);
//                painter.fillRect(resultImage.rect(), Qt::transparent);
//                painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
//                painter.drawImage(0, 0, img1);
//                painter.setCompositionMode(mode);
//                painter.drawImage(0, 0, img2);
//                painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
//                painter.fillRect(resultImage.rect(), Qt::white);
//                painter.end();

////                ui->imageLabel->setPixmap(QPixmap::fromImage(resultImage));
////                ui->imageLabel->setScaledContents( true );

////                ui->imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
//            }
//            //resultImage.save(fileName);
//        }





    return true;
}


void MainWindow::on_fileselectButton_clicked()
{

    QDir dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    QStringList image_list = dir.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png", QDir::Files);

    vector<QString> vec;
    for(QString str : image_list){
        vec.push_back(dir.absolutePath()+"/"+str);
    }
    if(!vec.empty()){
        file_directories__.push_back(vec);
    }
    if(!file_directories__.empty()){
        updateListWidget();
    }
}


void MainWindow::on_createImagesButton_clicked()
{
    combineImages();
}

