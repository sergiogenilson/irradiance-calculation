#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;

float radius = 1.0;
float lenght = 30.0;
float outputPower = 30.0;

float L = lenght/radius;
float pi = 2 * acos(0.0);
float denominator = 2 * pi * radius * lenght;

float calculateIrradiance(float x) {
    float H = x / radius;
    float X = pow((1 + H), 2) + pow(L, 2);
    float Y = pow((1 - H), 2) + pow(L, 2);
    float M = sqrt((H - 1)/(H + 1));
    float F = (L / (pi * H)) * ((1 / L) * atan(L / sqrt(pow(H, 2) -1) - atan(M) + ((X - 2 * H) / sqrt(X * Y)) * atan(M * sqrt(X / Y)))); 
    return (outputPower / denominator) * F;
}

float normalizeIrradianceToDisplay(float I) {
    return I * 2000000;
}

Mat computeIrradiance(Mat frame) {
    int height = frame.rows;
    int width = frame.cols;
    float irr;
    Mat irradiance = Mat(height, width, CV_32F);
    Mat floatFrame = Mat(height, width, CV_32F);
    frame.convertTo(floatFrame, CV_32F);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            irr = calculateIrradiance(floatFrame.at<float>(i,j));
            irradiance.at<float>(i,j) = normalizeIrradianceToDisplay(irr);
        }
    }
    return irradiance;
}


int main(int argc, char** argv) {
    // Mat frame;
    VideoCapture cap;
    if (!cap.open("videos/depth_video.avi")) {
        return 0;
    }
    Mat frame;
    Mat irradiance, inverseDepth, gray, aux;
    for (;;) {
        cap >> frame;
        if (frame.empty())
            break;
        imshow("depth", frame);
        cvtColor(frame, gray, cv::COLOR_RGB2GRAY);
        aux = computeIrradiance(gray);
        aux.convertTo(irradiance, CV_8U);
        imshow("irradiance", irradiance);
        if (waitKey(10) == 27)
            break;
    }
    return 0;
}