SRCROOT = $(realpath ../..)
include $(SRCROOT)/config.mak

TARGET = basic_test 

SOURCES = $(SRCROOT)/basic/ut/*.cc
HEADERS = $(INCLUDES_PATH)/basic/*.h

LDFLAGS += -L$(INSTALL_LIB_PATH) -ltoolbox_basic
LDFLAGS += $(GTEST_LIB_PATH)/gtest_main.a 
LDFLAGS += -Wl,-rpath=$(INSTALL_LIB_PATH)

CXXFLAGS += -I$(GTEST_INCLUDE_PATH)

$(OUTPUT_BIN_DIR)/$(TARGET) : $(OUTPUT_BIN_DIR) $(SOURCES) $(HEADERS)
	$(CPPLINT) $(SOURCES) $(HEADERS)
	$(CXX) -o $(OUTPUT_BIN_DIR)/$(TARGET) $(LDFLAGS) $(CXXFLAGS) $(SOURCES)

$(OUTPUT_BIN_DIR) : 
	mkdir $(OUTPUT_BIN_DIR)

.PHONY: clean check
clean :
	rm -rf $(OUTPUT_BIN_DIR)

check : $(OUTPUT_BIN_DIR)/$(TARGET)
	./$(OUTPUT_BIN_DIR)/$(TARGET)

