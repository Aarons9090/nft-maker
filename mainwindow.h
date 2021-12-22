#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <bits/stdc++.h>
#include <algorithm>
#include <unordered_set>
#include <QPainter>
#include <QDebug>

using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Updates the list of files selected
    void updateListWidget();

    bool combineImages();

    void initImageViewer();

private slots:
    void on_fileselectButton_clicked();

    void on_createImagesButton_clicked();

    void on_nextImageButton_clicked();

    void on_prevImageButton_clicked();

    void updateImageIndexLabel();

private:
    Ui::MainWindow *ui;

    // selected folders with image paths
    vector<vector<QString>> file_directories__;

    // All created images
    vector<QImage> all_images__;

    // Index of current image on viewer
    int imageViewerIndex = 0;

};
#endif // MAINWINDOW_H
