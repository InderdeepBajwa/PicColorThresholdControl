
// Project includes
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include <QAction>

#include <QFont>
#include <QFileDialog>

#include "tusvir.h"
#include "ui_tusvir.h"

// Global variables
Mat img;

Mat imHist(Mat hist, float scaleX = 1, float scaleY = 1){
    double maxVal = 0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);
    int rows = 64; //default height size
    int cols = hist.rows; //get the width size from the histogram
    Mat histImg = Mat::zeros(rows*scaleX, cols*scaleY, CV_8UC3);
    //for each bin
    for (int i = 0; i<cols - 1; i++) {
        float histValue = hist.at<float>(i, 0);
        float nextValue = hist.at<float>(i + 1, 0);
        Point pt1 = Point(i*scaleX, rows*scaleY);
        Point pt2 = Point(i*scaleX + scaleX, rows*scaleY);
        Point pt3 = Point(i*scaleX + scaleX, (rows - nextValue*rows / maxVal)*scaleY);
        Point pt4 = Point(i*scaleX, (rows - nextValue*rows / maxVal)*scaleY);

        int numPts = 5;
        Point pts[] = { pt1, pt2, pt3, pt4, pt1 };

        fillConvexPoly(histImg, pts, numPts, Scalar(255, 255, 255));
    }
    return histImg;
}

Tusvir::Tusvir(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Tusvir)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    //this->setCentralWidget(ui->graphicsView);
}

Tusvir::~Tusvir()
{
    delete ui;
}

void Tusvir::on_exitButton_clicked() {
    QCoreApplication::quit();
}

void Tusvir::on_pushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Browse picture");
    QFile file(fileName);
    currentFile = fileName;

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Can't open file: " + file.errorString());
        return;
    }
    setWindowTitle("Tusvir - " + fileName);
    displayPicture();
}

void Tusvir::displayPicture(int value) {
    QGraphicsScene *scene = new QGraphicsScene;
    QPixmap pixmap;

    bool firstOpenImage = true;

    Mat mat = imread(fileName.toStdString(), CV_LOAD_IMAGE_COLOR);
    if ((value > -1)) {
        threshold(mat,mat, value, 198, THRESH_BINARY);
    }
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        pixmap = QPixmap::fromImage(img);
    }
    if(mat.type()==CV_8UC3)
        {
            // Copy input Mat
            const uchar *qImageBuffer = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            pixmap = QPixmap::fromImage(img.rgbSwapped());
        }
    else {
        qDebug() << "ERROR: Mat could not be converted to QImage or QPixmap.";
        return;
    }
    scene->clear();
    scene->addPixmap(pixmap);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    if (!(value > -1)) {
        ui->OriginalPictureDisp->setScene(scene);
        ui->OriginalPictureDisp->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        histogramDisplay(mat);
    }
}

void Tusvir::histogramDisplay(Mat img) {
    //Hold the histogram

    MatND hist;
    Mat histImg;
    int nbins = 256; // lets hold 256 levels
    int hsize[] = { nbins }; // just one dimension
    float range[] = { 0, 255 };
    const float *ranges[] = { range };
    int chnls[] = { 0 };

    // create colors channels
    vector<Mat> colors;
    split(img, colors);

    // compute for all colors
    calcHist(&colors[0], 1, chnls, Mat(), hist, 1, hsize, ranges);
    histImg = imHist(hist, 3, 3);

    QGraphicsScene *scene = new QGraphicsScene;
    QPixmap pixmap;

    if(histImg.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)histImg.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, histImg.cols, histImg.rows, histImg.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        pixmap = QPixmap::fromImage(img);
    }
    if(histImg.type()==CV_8UC3)
        {
            // Copy input Mat
            const uchar *qImageBuffer = (const uchar*)histImg.data;
            // Create QImage with same dimensions as input Mat
            QImage img(qImageBuffer, histImg.cols, histImg.rows, histImg.step, QImage::Format_RGB888);
            pixmap = QPixmap::fromImage(img.rgbSwapped());
        }
    else {
        qDebug() << "ERROR: Mat could not be converted to QImage or QPixmap.";
        return;
    }

    scene->addPixmap(pixmap);

    ui->HistogramDisp->setScene(scene);
    ui->HistogramDisp->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

}

void Tusvir::histogramSlider(Mat img, int value) {

}

void Tusvir::on_histogramScroller_rangeChanged(int a, int b) {
}

void Tusvir::on_histogramScroller_actionTriggered(int action) {
}

void Tusvir::on_histogramScroller_valueChanged(int value) {
    ui->progressBar->setValue(value/1.8);
    displayPicture(value);
}
