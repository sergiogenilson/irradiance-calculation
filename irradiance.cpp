#include "irradiance.h"
#include <opencv2/opencv.hpp>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

Irradiance::Irradiance(Mat firstFrame) {
    height = firstFrame.rows;
    width = firstFrame.cols;
    radius = 1.0;
    lenght = 30.0;
    outputPower = 30.0;

    L = lenght/radius;
    pi = 2 * acos(0.0);
    denominator = 2 * pi * radius * lenght;
    frameCount = 0;
    elapsedTime = 0.0;
}

Mat Irradiance::computeIrradiance(Mat frame) {
    Mat irradiance = Mat(height, width, CV_32F);
    Mat floatFrame = Mat(height, width, CV_32F);
    Mat aux = Mat(height, width, CV_32F);
    frame.convertTo(floatFrame, CV_32F);
    double start_time = omp_get_wtime();
    int i,j;
#pragma omp parallel for private(i)
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            aux.at<float>(i,j) = calculateIrradiance(floatFrame.at<float>(i,j));
            irradiance.at<float>(i,j) = normalizeIrradianceToDisplay(aux.at<float>(i,j));
        }
    }
    elapsedTime += omp_get_wtime() - start_time;
    frameCount++;
    return irradiance;
}

float Irradiance::calculateIrradiance(float x) {
    float H = x / radius;
    float X = pow((1 + H), 2) + pow(L, 2);
    float Y = pow((1 - H), 2) + pow(L, 2);
    float M = sqrt((H - 1)/(H + 1));
    float F = (L / (pi * H)) * ((1 / L) * atan(L / sqrt(pow(H, 2) -1) - atan(M) + ((X - 2 * H) / sqrt(X * Y)) * atan(M * sqrt(X / Y)))); 
    return (outputPower / denominator) * F;
}

float Irradiance::normalizeIrradianceToDisplay(float I) {
    return I * 2000000;
}
