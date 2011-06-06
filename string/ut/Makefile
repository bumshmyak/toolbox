SRCROOT = ../..
include $(SRCROOT)/config.mak

TARGET = string_test 

SOURCES = *.cc

#LDFLAGS += -L$(INSTALL_LIB_PATH) -ltoolbox_basic
LDFLAGS += $(GTEST_LIB_PATH)/gtest_main.a 
#LDFLAGS += -Wl,-rpath=$(INSTALL_LIB_PATH)

CXXFLAGS += -I$(GTEST_INCLUDE_PATH)

$(OUTPUT_BIN_DIR)/$(TARGET) : $(OUTPUT_BIN_DIR) $(SOURCES)
	$(CXX) -o $(OUTPUT_BIN_DIR)/$(TARGET) $(LDFLAGS) $(CXXFLAGS) $(SOURCES)

$(OUTPUT_BIN_DIR) : 
	mkdir $(OUTPUT_BIN_DIR)

.PHONY: clean check
clean :
	rm -rf $(OUTPUT_BIN_DIR)

check : $(OUTPUT_BIN_DIR)/$(TARGET)
	./$(OUTPUT_BIN_DIR)/$(TARGET)

