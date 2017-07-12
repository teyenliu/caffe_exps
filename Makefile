CAFFE_ROOT = /home/liudanny/git/caffe
#CAFFE_DPKG_INCLUDE = /home/liudanny/local_install/include
#CAFFE_DPKG_LIB = /home/liudanny/local_install/lib

CC = g++
CFLAGS = -I $(CAFFE_ROOT)/include/ -D CPU_ONLY -I $(CAFFE_ROOT)/build/src/
LDFLAGS =  -L $(CAFFE_ROOT)/build/lib/ 
LIBS = -lcaffe -lglog

all: app

app: blob_demo.o
	$(CC) blob_demo.o -o app $(LDFLAGS) $(LIBS)

blob_demo.o:
	$(CC) $(CFLAGS) -c blob_demo.cpp

clean:
	rm -f main.o app
