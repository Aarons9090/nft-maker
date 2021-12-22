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

    for(QString &image : filenames__){
        new QListWidgetItem(image, widget);
    }

}

bool MainWindow::combineImages()
{
    QImage resultImage;
    resultImage = QImage(QSize(100,100), QImage::Format_ARGB32_Premultiplied);
    QImage img1(filenames__[0]);
    QImage img2(filenames__[1]);

    QPainter::CompositionMode mode = QPainter::CompositionMode::CompositionMode_SourceOver;

    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, img1);
    painter.setCompositionMode(mode);
    painter.drawImage(0, 0, img2);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.fillRect(resultImage.rect(), Qt::white);
    painter.end();

    ui->imageLabel->setPixmap(QPixmap::fromImage(resultImage));
    ui->imageLabel->setScaledContents( true );

    ui->imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    return true;
}


void MainWindow::on_fileselectButton_clicked()
{


    filenames__ = QFileDialog::getOpenFileNames(this,"Select image(s)");

    if(!filenames__.empty()){
        updateListWidget();
    }
}


void MainWindow::on_createImagesButton_clicked()
{
    combineImages();
}

