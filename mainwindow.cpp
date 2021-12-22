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

    int counter = 1;
    qDebug() << "combinations: " << combinations.size();




    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);





    for(vector<QString> &vec : combinations){
        QString saveFileName = fileName+"/"+QString::number(counter)+".png";

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
        counter += 1;
        qDebug() << "saved to " << saveFileName << Qt::endl;
    }







//    ui->imageLabel->setPixmap(QPixmap::fromImage(resultImage));
//    ui->imageLabel->setScaledContents( true );

//    ui->imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

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

