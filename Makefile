CX = g++
CXFLAGS = -g -Wall 

DXLFLAGS = -I/usr/local/include/dynamixel_sdk_cpp
DXLFLAGS += -ldxl_x64_cpp              #/usr/local/lib/libdxl_x64_cpp.so               
DXLFLAGS += -lrt

CVFLAGS = `pkg-config opencv4 --cflags --libs`

BUILDFLAGS = $(DXLFLAGS)
BUILDFLAGS += $(CVFLAGS)

TARGET = handdetect
OBJS = main.o dxl.o yolo.o
$(TARGET) :  $(OBJS)
	$(CX) $(CXFLAGS) -o $(TARGET) $(OBJS) $(BUILDFLAGS) 
main.o : main.cpp
	$(CX) $(CXFLAGS) -c main.cpp $(BUILDFLAGS) 
dxl.o : dxl.hpp dxl.cpp
	$(CX) $(CXFLAGS) -c dxl.cpp $(DXLFLAGS)
yolo.o : yolo.hpp yolo.cpp
	$(CX) $(CXFLAGS) -c yolo.cpp $(CVFLAGS)

.PHONY: all clean
all: $(TARGET)

clean:
	rm -rf $(TARGET) $(OBJS)


