#ifndef TUSVIR_H
#define TUSVIR_H

#include <QMainWindow>
#include <QtWidgets>

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
using namespace cv;

namespace Ui {
class Tusvir;
}

class Tusvir : public QMainWindow
{
    Q_OBJECT

public:
    explicit Tusvir(QWidget *parent = nullptr);
    ~Tusvir();
    QString fileName;

private slots:

    void displayPicture(int value=-2);

    void histogramDisplay(Mat img);

    void histogramSlider(Mat img, int value);

    //void save();

    void on_exitButton_clicked();

    void on_pushButton_clicked();

    void on_histogramScroller_rangeChanged(int min, int max);

    void on_histogramScroller_actionTriggered(int action);

    void on_histogramScroller_valueChanged(int value);

private:
    Ui::Tusvir *ui;
    QString currentFile;
};

#endif // TUSVIR_H
