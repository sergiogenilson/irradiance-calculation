#ifndef IRRADIANCE_H
#define IRRADIANCE_H

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

using namespace cv;

class Irradiance {
public:
    Irradiance(Mat firstFrame);
    Mat computeIrradiance(Mat frame);
    double elapsedTime;
    int frameCount;

private: 
    int height, width;
    float radius, lenght, outputPower, L, pi, denominator;
    float calculateIrradiance(float x);
    float normalizeIrradianceToDisplay(float I);
};
#endif