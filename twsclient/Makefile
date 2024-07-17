# $@ evaluates to the .o file
# $< evaluates to the .c file
# $^ evaluates to all prerequisites separated by a space

CPPFLAGS := -Werror -Wall -Wextra

%.o:	%.cpp
	g++ $(CPPFLAGS) -o $@ -c $<

# list of object files, corresponding to *.cpp files
TWSClientOBJS := $(patsubst %.cpp,%.o, $(wildcard *.cpp))

TWSClient:	${TWSClientOBJS}
		g++ $(LDFLAGS) -o TWSClient $^

all:		clean TWSClient

clean:	
	rm -rf TWSClient
	rm -rf *.o
