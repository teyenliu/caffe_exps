CAFFE_ROOT = /home/liudanny/git/caffe
#CAFFE_DPKG_INCLUDE = /home/liudanny/local_install/include
#CAFFE_DPKG_LIB = /home/liudanny/local_install/lib

CC = g++ -O3 -g -flto -fuse-linker-plugin -fprofile-generate=./prof_data
CFLAGS = -std=c++11 -I $(CAFFE_ROOT)/include/ -I $(CAFFE_ROOT)/include/caffe -D CPU_ONLY -I $(CAFFE_ROOT)/build/include/ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -I /usr/include/boost 
LDFLAGS =  -L $(CAFFE_ROOT)/build/lib/ -L/usr/local/lib/
LIBS = -lcaffe -lglog -lgflags -lcaffe -lboost_filesystem -lboost_system -lboost_thread -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml \
-lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching
SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))


all: app csvset foreach work_queue blocking_queue opencv_test

work_queue: work_queue_demo.o
	$(CC) work_queue_demo.o -o $@ $(LDFLAGS) $(LIBS) 

blocking_queue: blocking_queue_demo.o
	$(CC) blocking_queue_demo.o -o $@ $(LDFLAGS) $(LIBS) 

app: blob_demo.o
	$(CC) blob_demo.o -o $@ $(LDFLAGS) $(LIBS)

csvset: convert_csvset.o
	$(CC) convert_csvset.o -o $@ $(LDFLAGS) $(LIBS)

foreach: opencv_foreach.o 
	$(CC) opencv_foreach.o -o $@ $(LDFLAGS) $(LIBS)

opencv_test: opencv_test.o 
	$(CC) opencv_test.o -o $@ $(LDFLAGS) $(LIBS)

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES) $(LIBS)

clean:
	rm -f *.o app csvset foreach work_queue blocking_queue opencv_test
