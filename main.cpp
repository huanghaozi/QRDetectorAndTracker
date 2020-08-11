#include <iostream>
#include "QRDetectorAndTracker.h"

int main()
{
    //Auto Initialize
    QRDetectorAndTracker qr;
    while(1)
    {
        //Detect QR Code
        qr.detect();
        //Paint on Pic & Show
        qr.drawBound();
        qr.showPic("Window");
        //Output Position
        if(qr.xyAvailable)
            std::cout << qr.x << ", " << qr.y << std::endl;
        //Delay for loop
        if(cv::waitKey(1)==27)
            break;
    }
    return 0;
}
