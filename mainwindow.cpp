#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    ui->textBrowser->append("Application started");
    ui->textBrowser->append("Start by selecting folders with images");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateListWidget()
{
    QListWidget *widget = ui->imageList;

    widget->clear();

    for(vector<QString> &vec: fileDirectories__){
        for(QString const &str : vec){
            new QListWidgetItem(str, widget);
        }
    }

}

bool MainWindow::combineImages()
{
    if(saveFilePath__.size() == 0){
        ui->textBrowser->append("Select save file before creating images!");
        return false;
    }

    // number of arrays
    vector<vector<QString>> combinations;
    int n = fileDirectories__.size();

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
            vec.push_back(fileDirectories__[i][indices[i]]);
        }

        combinations.push_back(vec);



        // find the rightmost array that has more
        // elements left after the current element
        // in that array
        int next = n - 1;
        while (next >= 0 &&
              (indices[next] + 1 >= int(fileDirectories__[next].size())))
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

    int counter = 1;


    ui->textBrowser->append(QString::number(combinations.size()) + " combinations created");







    for(vector<QString> &vec : combinations){
        QString saveFileName = saveFilePath__+"/"+QString::number(counter)+".png";

        QImage resultImage;

        resultImage = QImage(QSize(100,100), QImage::Format_ARGB32_Premultiplied);

        QPainter painter(&resultImage);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        for(QString &path : vec){
            qDebug() << path << Qt::endl;
            QImage img(path);
            painter.drawImage(0, 0, img);
        }

        painter.end();
        resultImage.save(saveFileName);
        allImages__.push_back(resultImage);

        ui->textBrowser->append("#"+QString::number(counter) + " image saved");
        counter += 1;
    }

    ui->textBrowser->append("All images created");
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
    for(QString& str : image_list){
        vec.push_back(dir.absolutePath()+"/"+str);
    }
    if(!vec.empty()){
        fileDirectories__.push_back(vec);
        ui->textBrowser->append(QString::number(vec.size()) + " images added from " + dir.path());
    }
    if(!fileDirectories__.empty()){
        updateListWidget();
    }
}

void MainWindow::initImageViewer()
{
    if(allImages__.size() == 0){
        return;
    }
    QImage firstImage = allImages__[0];


    ui->imageLabel->setPixmap(QPixmap::fromImage(firstImage));
    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    updateImageIndexLabel();

}

void MainWindow::on_createImagesButton_clicked()
{
    if(combineImages()){
        initImageViewer();
    }
}


void MainWindow::on_nextImageButton_clicked()
{
    if(imageViewerIndex < int(allImages__.size() - 1)){
        imageViewerIndex += 1;
        ui->imageLabel->setPixmap(QPixmap::fromImage(allImages__[imageViewerIndex]));
        updateImageIndexLabel();
    }
}


void MainWindow::on_prevImageButton_clicked()
{
    if(imageViewerIndex > 0){
        imageViewerIndex -= 1;
        ui->imageLabel->setPixmap(QPixmap::fromImage(allImages__[imageViewerIndex]));
        updateImageIndexLabel();
    }
}

void MainWindow::updateImageIndexLabel()
{
    QString labelText = QString::number(imageViewerIndex+1) + " of " + QString::number(allImages__.size());
    ui->imageIndexLabel->setText(labelText);
}


void MainWindow::on_saveFileButton_clicked()
{
    saveFilePath__ = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);


}

