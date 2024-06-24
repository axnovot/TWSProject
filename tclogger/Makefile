CC := g++

#MN# #$@ is the .o file
#MN# #$< is the .c file
%.o:	%.cpp
	${CC} -o $@ -c $<

TWSClientOBJS := TWSclient.o tclogger.o

TWSClient:	${TWSClientOBJS}
		${CC} -o TWSClient ${TWSClientOBJS}

all:		TWSClient

clean:	
	rm -rf TWSClient
	rm -rf *.o
