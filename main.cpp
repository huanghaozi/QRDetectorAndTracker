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
        for(int i=0;i<std::distance(qr.xyAvailable.begin(), qr.xyAvailable.end());i++)
        {
            if(qr.xyAvailable[i])
                std::cout << qr.qrChars[i] << ": " << qr.x[i] << ", " << qr.y[i] << std::endl;
        }
        //Delay for loop
        if(cv::waitKey(1)==27)
            break;
    }
    return 0;
}
