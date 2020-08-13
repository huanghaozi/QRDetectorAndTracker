#include "QRDetectorAndTracker.h"

QRDetectorAndTracker::QRDetectorAndTracker()
{
    if(!capture->isOpened())
        return;
    param.detect_thresh =  0.2f;
    tracker.push_back(cv::TrackerKCF::create(param));
    tracker.push_back(cv::TrackerKCF::create(param));
    tracker.push_back(cv::TrackerKCF::create(param));
    tracker.push_back(cv::TrackerKCF::create(param));
    tracker.push_back(cv::TrackerKCF::create(param));
    scanner->set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_POSITION, 1);
    scanner->set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_NUM, 1);
    width = capture->get(cv::CAP_PROP_FRAME_WIDTH);
    height = capture->get(cv::CAP_PROP_FRAME_HEIGHT);
}

void QRDetectorAndTracker::detect()
{
    for(int i=0;i<std::distance(qrChars.begin(), qrChars.end());i++)
    {
        xyAvailable[i] = false;
        mode[i] = 0;
    }
    *capture>>pic;
    cvtColor(pic, picGray, CV_BGR2GRAY);
    equalizeHist(picGray, picGray);
    if(imageZbar != NULL)
        delete imageZbar;
    imageZbar = new zbar::Image(width, height, "Y800", picGray.data, width * height);
    int n = scanner->scan(*imageZbar);
    if(n > 0)
    {
        for(symbol=imageZbar->symbol_begin(); symbol!=imageZbar->symbol_end(); ++symbol)
        {
            int pos = 0;
            auto data = symbol->get_data();
            std::vector<std::string>::iterator itemPosIte = std::find(qrChars.begin(), qrChars.end(), data);
            if(itemPosIte != qrChars.end())
            {
                pos = std::distance(qrChars.begin(), itemPosIte);
            }
            else
            {
                qrChars.push_back(data);
                x.push_back(0);
                y.push_back(0);
                bbox.push_back(cv::Rect2d(0,0,0,0));
                newBboxAvailabel.push_back(true);
                xyAvailable.push_back(true);
                mode.push_back(1);
                inited.push_back(false);
                releaseFlag.push_back(0);
                rect.push_back(std::vector<cv::Point>());
                pos = std::distance(qrChars.begin(), qrChars.end())-1;
            }
            int minX, maxX, minY, maxY;
            minX = symbol->get_location_x(0);
            maxX = symbol->get_location_x(0);
            minY = symbol->get_location_y(0);
            maxY = symbol->get_location_y(0);
            for(int i=1;i<4;i++)
            {
                if(symbol->get_location_x(i)<minX) minX=symbol->get_location_x(i);
                if(symbol->get_location_x(i)>maxX) maxX=symbol->get_location_x(i);
                if(symbol->get_location_y(i)<minY) minY=symbol->get_location_y(i);
                if(symbol->get_location_y(i)>maxY) maxY=symbol->get_location_y(i);
            }
            x[pos] = (symbol->get_location_x(0) + symbol->get_location_x(1) +
                    symbol->get_location_x(2) + symbol->get_location_x(3))/4;
            y[pos] = (symbol->get_location_y(0) + symbol->get_location_y(1) +
                    symbol->get_location_y(2) + symbol->get_location_y(3))/4;
            bbox[pos] = cv::Rect2d(cv::Point(minX, minY),
                                   cv::Point(maxX, maxY));
            newBboxAvailabel[pos] = true;
            xyAvailable[pos] = true;
            mode[pos] = 1;
        }
    }
    else
    {
        for(int i=0;i<std::distance(qrChars.begin(), qrChars.end());i++)
        {
            if(newBboxAvailabel[i])
            {
                if(int(tracker.size())-1<i)
                {
                    tracker.push_back(cv::TrackerKCF::create(param));
                }
                else
                    tracker[i] = cv::TrackerKCF::create(param);
                tracker[i]->init(pic, bbox[i]);
                inited[i] = true;
            }
            if(inited[i])
            {
                if(tracker[i]->update(pic,bbox[i]))
                {
                    x[i] = bbox[i].x + bbox[i].size().width/2;
                    y[i] = bbox[i].y + bbox[i].size().height/2;
                    xyAvailable[i] = true;
                    mode[i] = 2;
                }
                else
                {
                    releaseFlag[i]++;
                    if(releaseFlag[i] == 20)
                    {
                        releaseFlag[i] = 0;
                        tracker[i].release();
                        inited[i] = false;
                    }
                }
            }
            newBboxAvailabel[i] = false;
        }
    }
}

void QRDetectorAndTracker::drawBound()
{
    symbol=imageZbar->symbol_begin();
    for(int i=0;i<std::distance(qrChars.begin(), qrChars.end());i++)
    {
        if(mode[i] == 0)
            continue;
        if(mode[i] == 1)
        {
            cv::rectangle(pic, bbox[i], cv::Scalar(255,0,0),2);
            continue;
        }
        if(mode[i] == 2)
        {
            cv::rectangle(pic, bbox[i], cv::Scalar(0,255,0),2);
            continue;
        }
        ++symbol;
    }
    return;
}

void QRDetectorAndTracker::showPic(cv::String s)
{
    cv::imshow(s.c_str(), pic);
}
