SRCROOT = $(realpath ..)
include $(SRCROOT)/config.mak

TARGET = libtoolbox_math.so

SOURCES = $(SRCROOT)/math/*.cc
HEADERS = $(INCLUDES_PATH)/math/*.h

CXXFLAGS += -shared -fPIC

.PHONY: clean check all

all : $(INSTALL_LIB_PATH)/$(TARGET)

$(INSTALL_LIB_PATH)/$(TARGET) : $(SOURCES) $(HEADERS)
	$(CPPLINT) $(SOURCES) $(HEADERS)
	$(CXX)  -o $(INSTALL_LIB_PATH)/$(TARGET) $(CXXFLAGS) $(SOURCES) 

clean : 
	rm -rf $(INSTALL_LIB_PATH)/$(TARGET)

check :
	$(MAKE) -C ut check
