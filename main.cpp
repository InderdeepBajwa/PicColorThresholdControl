#include "tusvir.h"
#include <QApplication>

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tusvir w;


    // load the image, load the image in grayscale
    Mat img = imread("circle.jpeg", CV_LOAD_IMAGE_COLOR);

    // always check
    if (img.data == NULL) {
            cout << "Cannot load file " << argv[1] << endl;
            return 1;
    }

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

    // show image
    //imshow("Image", img);
    w.setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    w.size(),
                    qApp->desktop()->screenGeometry()
                )
    );
    w.show();
    return a.exec();
}
