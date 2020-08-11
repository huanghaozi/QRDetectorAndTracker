#ifndef QRDETECTORANDTRACKER_H
#define QRDETECTORANDTRACKER_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/imgproc/types_c.h>
#include <zbar.h>

class QRDetectorAndTracker
{
public:
    QRDetectorAndTracker();
    void detect();
    void drawBound();
    void showPic(cv::String s);
    int x, y;
    cv::Mat pic;
    bool xyAvailable = false;

private:
    cv::Mat picGray;

    //Tracker
    cv::TrackerKCF::Params param;
    cv::Ptr<cv::Tracker> tracker;

    //QRCode detector
    zbar::Image *imageZbar = NULL;
    zbar::ImageScanner *scanner = new zbar::ImageScanner();
    zbar::Image::SymbolIterator symbol;

    //Camera
    cv::VideoCapture *capture = new cv::VideoCapture(0);
    int width, height;

    //Mark Bound
    std::vector<cv::Point> *rect = new std::vector<cv::Point>(4);
    cv::Rect2d bbox;

    //Decision tree
    bool newBboxAvailabel = false, inited = false;
    int mode = 0;
};

#endif // QRDETECTORANDTRACKER_H
