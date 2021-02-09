CPP=g++
CPPFLAGS=$(shell pkg-config --cflags opencv2)
LIBS=$(shell pkg-config --libs opencv2)

irradiance: main.cpp irradiance.cpp
	$(CPP) -o irradiance main.cpp irradiance.cpp $(CPPFLAGS) $(LIBS) -fopenmp -O3 -march=native -lgomp -ltbb

irr: main.cpp irradiance.cpp
	$(CPP) -I /usr/include/opencv -I /usr/include/opencv4 -L /usr/local/lib/ -g -o binary_multi main.cpp irradiance.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -lopencv_imgcodecs -lopencv_videoio -fopenmp -O3 -march=native -lgomp -ltbb