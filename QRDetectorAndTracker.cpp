#include "QRDetectorAndTracker.h"

QRDetectorAndTracker::QRDetectorAndTracker()
{
    if(!capture->isOpened())
        return;
    param.detect_thresh =  0.15f;
    tracker = cv::TrackerKCF::create(param);
    scanner->set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_POSITION, 1);
    width = capture->get(cv::CAP_PROP_FRAME_WIDTH);
    height = capture->get(cv::CAP_PROP_FRAME_HEIGHT);
}

void QRDetectorAndTracker::detect()
{
    xyAvailable = false;
    mode = 0;
    *capture>>pic;
    cvtColor(pic, picGray, CV_BGR2GRAY);
    equalizeHist(picGray, picGray);
    if(imageZbar != NULL)
        delete imageZbar;
    imageZbar = new zbar::Image(width, height, "Y800", picGray.data, width * height);
    if(scanner->scan(*imageZbar) > 0)
    {
        symbol = imageZbar->symbol_begin();
        x = (symbol->get_location_x(0) + symbol->get_location_x(1) +
                symbol->get_location_x(2) + symbol->get_location_x(3))/4;
        y = (symbol->get_location_y(0) + symbol->get_location_y(1) +
                symbol->get_location_y(2) + symbol->get_location_y(3))/4;
        bbox = cv::Rect2d(cv::Point(symbol->get_location_x(0), symbol->get_location_y(0)),
                      cv::Point(symbol->get_location_x(2), symbol->get_location_y(2)));
        newBboxAvailabel = true;
        xyAvailable = true;
        mode = 1;
    }
    else
    {
        if(newBboxAvailabel)
        {
            tracker = cv::TrackerKCF::create(param);
            tracker->init(pic, bbox);
            inited = true;
        }
        if(inited)
        {
            if(tracker->update(pic,bbox))
            {
                x = bbox.x + bbox.size().width/2;
                y = bbox.y + bbox.size().height/2;
                xyAvailable = true;
                mode = 2;
            }
            else
            {
                tracker.release();
                inited = false;
            }
        }
        newBboxAvailabel = false;
    }
}

void QRDetectorAndTracker::drawBound()
{
    if(mode == 0)
        return;
    if(mode == 1)
    {
        (*rect)[0] = cv::Point(symbol->get_location_x(0), symbol->get_location_y(0));
        (*rect)[1] = cv::Point(symbol->get_location_x(1), symbol->get_location_y(1));
        (*rect)[2] = cv::Point(symbol->get_location_x(2), symbol->get_location_y(2));
        (*rect)[3] = cv::Point(symbol->get_location_x(3), symbol->get_location_y(3));
        polylines(pic, *rect, true, cv::Scalar(255,0,0), 2);
        return;
    }
    if(mode == 2)
    {
        rectangle(pic, bbox, cv::Scalar(0,255,0),2);
        return;
    }
    return;
}

void QRDetectorAndTracker::showPic(cv::String s)
{
    cv::imshow(s.c_str(), pic);
}
