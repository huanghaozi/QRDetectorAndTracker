**(2021-6-12更新： ~~这个是我<20岁时的项目，~~ 很拉，想要用更好的目标定位请搜索 ArUco)**

# QRDetectorAndTracker
一个能定位二维码并跟踪其位置的程序，可用于摄像头对机器的定位

# 使用方法
## 加入包含文件及链接库依赖
1. 首先需要将opencv和zlib文件夹复制到自己的项目目录下
2. 需要在项目的Include Path中加入opencv/include、zlib/include
3. 需要将opencv/bin、zlib/bin下的dll文件加入项目的动态链接库中，或直接将其复制到与exe相同的目录下

## 包含头文件
需要将该Repo中的QRDetectorAndTracker.cpp及QRDetectorAndTracker.h文件加入自己的项目中

在需要用到的地方添加
```c
#include "QRDetectorAndTracker.h"
```

## 类的结构
### 变量
```cpp
    //默认从摄像头获取图像
    std::vector<int> x, y; //二维码的坐标，考虑到存在多个二维码，所以用容器
    cv::Mat pic; //含有方框标记的输出图像，执行drawBound后此变量才有内容
    std::vector<bool> xyAvailable; //是否识别到二维码，即坐标是否可用
    std::vector<std::string> qrChars; //二维码的内容，考虑到存在多个二维码，所以用容器
```

### 函数
```cpp
    void detect(); //检测函数，将从摄像头读取一帧图像并检测，将检测的坐标、内容存入上述变量中
    void drawBound(); //在检测到二维码的位置画个方框，存入pic变量中
    void showPic(cv::String s); //显示图像窗口，其中参数s为窗口标题
```

## 示例
```cpp
    //初始化
    QRDetectorAndTracker qr;
    while(1) //主循环
    {
        //检测坐标
        qr.detect();
        //绘制方框并显示
        qr.drawBound();
        qr.showPic("Window");
        //输出坐标，遍历xyAvailable
        for(int i=0;i<std::distance(qr.xyAvailable.begin(), qr.xyAvailable.end());i++)
        {
            if(qr.xyAvailable[i]) //若坐标可用则输出
                std::cout << qr.qrChars[i] << ": " << qr.x[i] << ", " << qr.y[i] << std::endl;
        }
        //循环延时，不加会卡死
        if(cv::waitKey(1)==27)
            break;
    }
```
