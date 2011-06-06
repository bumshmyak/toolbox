SRCROOT = $(realpath ..)
include $(SRCROOT)/config.mak

TARGET = libtoolbox_storage.so

SOURCES = $(SRCROOT)/storage/*.cc
HEADERS = $(INCLUDES_PATH)/storage/*.h

CXXFLAGS += -shared -fPIC

.PHONY: clean check all 

all : $(INSTALL_LIB_PATH)/$(TARGET)

$(INSTALL_LIB_PATH)/$(TARGET) : $(SOURCES) $(HEADERS)
#	$(CPPLINT) $(SOURCES) $(HEADERS)
	$(CXX)  -o $(INSTALL_LIB_PATH)/$(TARGET) $(CXXFLAGS) $(SOURCES) 

clean : 
	rm -rf $(INSTALL_LIB_PATH)/$(TARGET)

check :
	$(MAKE) -C ut check
