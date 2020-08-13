#ifndef QRDETECTORANDTRACKER_H
#define QRDETECTORANDTRACKER_H

#include <vector>
#include <iterator>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/imgproc/types_c.h>
#include <zbar.h>
#include <zbar/Symbol.h>

class QRDetectorAndTracker
{
public:
    QRDetectorAndTracker();
    void detect();
    void drawBound();
    void showPic(cv::String s);
    std::vector<int> x, y;
    cv::Mat pic;
    std::vector<bool> xyAvailable;
    std::vector<std::string> qrChars;

private:
    cv::Mat picGray;

    //Tracker
    cv::TrackerKCF::Params param;
    std::vector<cv::Ptr<cv::Tracker>> tracker;
    std::vector<int> releaseFlag;

    //QRCode detector
    zbar::Image *imageZbar = NULL;
    zbar::ImageScanner *scanner = new zbar::ImageScanner();
    zbar::Image::SymbolIterator symbol;

    //Camera
    cv::VideoCapture *capture = new cv::VideoCapture(0);
    int width, height;

    //Mark Bound
    std::vector<std::vector<cv::Point>> rect;
    std::vector<cv::Rect2d> bbox;

    //Decision tree
    std::vector<bool> newBboxAvailabel, inited;
    std::vector<int> mode;
};

#endif // QRDETECTORANDTRACKER_H
