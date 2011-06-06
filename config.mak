# default settings for all subprojects

INCLUDES_PATH = $(SRCROOT)/include
GTEST_INCLUDE_PATH = $(SRCROOT)/googletest/include

INSTALL_LIB_PATH = $(SRCROOT)/lib
GTEST_LIB_PATH = $(SRCROOT)/googletest/make

CPPLINT_OPTIONS="--filter=-legal/copyright,-runtime/threadsafe_f,\
-readability/stream,-runtime/references,-runtime/int"

CPPLINT = $(SRCROOT)/cpplint.py $(CPPLINT_OPTIONS)

CXXFLAGS = -g -Wall -Wno-sign-compare -Werror -pthread -I$(INCLUDES_PATH)
OUTPUT_BIN_DIR = bin