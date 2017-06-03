CFLAGS += -pipe -g -ggdb -ggdb3 -O0 -DDEBUG -finline-functions -Wall
CFLAGS += -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion
CFLAGS += -Winit-self -Wunreachable-code -Wformat-y2k -Wformat-nonliteral
CFLAGS += -Wformat-security -Wmissing-include-dirs -Wswitch-default
CFLAGS += -Wtrigraphs -Wstrict-overflow=5 -Wfloat-equal -Wundef -Wshadow
CFLAGS += -Wcast-qual -Wcast-align -Wwrite-strings -Winline
CFLAGS += -Wsuggest-attribute=const -Wsuggest-attribute=pure
CFLAGS += -Wsuggest-attribute=noreturn -Wsuggest-attribute=format
CFLAGS += -Wmissing-format-attribute -Wlogical-op

all: TBezierInterpolationCPP TBezierInterpolationC

TBezierInterpolationCPP: TBezierInterpolation.cpp
	$(CXX) $(CFLAGS) -std=c++11 -o TBezierInterpolationCPP TBezierInterpolation.cpp -lm

TBezierInterpolationC: TBezierInterpolation.c
	$(CC) $(CFLAGS) -std=c11 -Wbad-function-cast -o TBezierInterpolationC TBezierInterpolation.c -lm

clean:
	rm TBezierInterpolationCPP TBezierInterpolationC
