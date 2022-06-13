#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

int main()
{
    cv::Mat img = cv::imread("D:/json/nlohmann-json-tutorial/Release/af.jpg");
    std::cout << "channels: " << img.channels() << std::endl;
    std::cout << "hello world" << std::endl;
    return 0;
}