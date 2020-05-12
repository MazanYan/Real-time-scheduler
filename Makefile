# g++ *.cpp -o main -I/usr/include/python3.8 -I/usr/include/python3.8 -I/home/yan/.local/lib/python3.8/site-packages/numpy/core/include -std=c++11 -Wno-conversion -lpython3.8 -lcrypt -lpthread -ldl  -lutil -lm -lm
# Use C++11, dont warn on long-to-float conversion
CXXFLAGS += -std=c++11 -Wno-conversion

# Default to using system's default version of python
PYTHON_BIN     ?= python3
PYTHON_CONFIG  := $(PYTHON_BIN)-config
PYTHON_INCLUDE ?= $(shell $(PYTHON_CONFIG) --includes)
EXTRA_FLAGS    := $(PYTHON_INCLUDE)

LDFLAGS        += $(shell if $(PYTHON_CONFIG) --libs --embed >/dev/null; then $(PYTHON_CONFIG) --libs --embed; else $(PYTHON_CONFIG) --libs; fi)

EXTRA_FLAGS     += $(shell $(PYTHON_BIN) $(CURDIR)/matplotlib-cpp/numpy_flags.py)
WITHOUT_NUMPY   := $(findstring $(EXTRA_FLAGS), WITHOUT_NUMPY)

SRC		:= main.cpp input_task.cpp random_distributions.cpp modelling.cpp processor.cpp

TARGET		:= main

$(TARGET): $(SRC) matplotlib-cpp/matplotlibcpp.h
	$(CXX) $(SRC) -o $(TARGET) $(EXTRA_FLAGS) $(CXXFLAGS) $(LDFLAGS)


clean:
	rm main
