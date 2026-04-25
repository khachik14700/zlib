CC = g++
CFLAGS = -std=c++17 -Wall

TARGET = compressor

SRCS = Compressor/main.cpp \
	Compressor/src/fileio.cpp \
	Deflate/src/deflate.cpp \
	Huffman/src/huffman.cpp \
	LZ77/src/lz77.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re