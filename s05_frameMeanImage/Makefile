TARGETS = camFM  # コンパイルしたいプログラム名
# VPATH = ../
SRCS = camFrMean.cpp
CC = g++ # 使うコンパイラ
LIB = 
CFLAGS = -Wall -O2 -std=c++11 # c++11のオプション追加(3to4)
OBJS := $(SRCS:.cpp=.o)
OBJS := $(addprefix objs/,$(OBJS))
OPENCVINC = `pkg-config --cflags opencv4`
OPENCVLIB = `pkg-config --libs opencv4`

all: $(TARGETS)

$(TARGETS): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(OPENCVLIB) $(LIB)

objs/%.o: ./%.cpp
	-mkdir objs
	$(CC) $(CFLAGS) -c $< -o $@ $(OPENCVINC) $(OPENCVLIB) $(LIB)

clean:
	rm -rf $(TARGETS)
	rm -rf objs/*.o
	rm -rf objs

