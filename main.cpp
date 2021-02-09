#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>
#include <sys/time.h>
#include <omp.h>
#include "irradiance.h"

using namespace cv;

int main(int argc, char** argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " YOUR_VIDEO.EXT NUMBER_OF_THREADS" << std::endl;
        return 1;
    }
    int frameCount = 0;
    double irradianceCalcTime = 0.0;

    double totalTime = 0.0;
    double start_time = omp_get_wtime();
    try {
        // Mat frame;
        VideoCapture cap;
        if (!cap.open(argv[1])) {
            cout << "ERROR on load video..." << endl;
            return 0;
        }

        setNumThreads(atoi(argv[2]));
        omp_set_num_threads(atoi(argv[2]));

        namedWindow( "depth", WINDOW_AUTOSIZE );
        namedWindow( "irradiance", WINDOW_AUTOSIZE );

        Mat frame;
        Mat irradiance, inverseDepth, gray, aux;
        cap >> frame;
        Irradiance i = Irradiance(frame);
        while (1) {
            imshow("depth", frame);
            cvtColor(frame, gray, cv::COLOR_RGB2GRAY);
            aux = i.computeIrradiance(gray);
            aux.convertTo(irradiance, CV_8U);
            imshow("irradiance", irradiance);
            
            int key = waitKey(1);
            if (key == 27) {
                break;
            } else if (key == 's') {
                imwrite("depth.png", gray);
                imwrite("irradiance.png", irradiance);
            }
            irradianceCalcTime = i.elapsedTime;
            frameCount = i.frameCount;
            cap >> frame;
        }
        
    } catch(const Exception& ex) 	{
        cout << "Error: " << ex.what() << endl;
    }
    totalTime += omp_get_wtime() - start_time;
    cout << "Results: " << endl;
    cout << " - total time: " << totalTime << endl;
    cout << " - irradiance calc time: " << irradianceCalcTime << endl;
    cout << " - frames number: " << frameCount << endl;
    cout << "--> Frames/s = " << frameCount/irradianceCalcTime << endl;
    return 0;
    
}

