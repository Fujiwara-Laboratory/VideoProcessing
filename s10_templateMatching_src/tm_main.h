#pragma once

#include <opencv2/opencv.hpp>

void GenMatchMap(cv::Mat srcImg, cv::Mat tempImg, cv::Mat &mapImg, int &x, int &y);
