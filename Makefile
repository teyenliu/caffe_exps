CAFFE_ROOT = /home/liudanny/git/caffe
#CAFFE_DPKG_INCLUDE = /home/liudanny/local_install/include
#CAFFE_DPKG_LIB = /home/liudanny/local_install/lib

CC = g++ -g
CFLAGS = -I $(CAFFE_ROOT)/include/ -D CPU_ONLY -I $(CAFFE_ROOT)/build/src/
LDFLAGS =  -L $(CAFFE_ROOT)/build/lib/ 
LIBS = -lcaffe -lglog

all: app opencv

app: blob_demo.o
	$(CC) blob_demo.o -o app $(LDFLAGS) $(LIBS)

blob_demo.o:
	$(CC) $(CFLAGS) -c blob_demo.cpp

opencv:
	$(CC) -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o test  opencv_test.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching

clean:
	rm -f *.o app test
