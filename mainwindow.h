#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <bits/stdc++.h>
#include <unordered_set>
#include <QPainter>

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

private slots:
    void on_fileselectButton_clicked();

    void on_createImagesButton_clicked();

private:
    Ui::MainWindow *ui;

    // current selected files
    QStringList filenames__;

};
#endif // MAINWINDOW_H
