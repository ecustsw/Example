OBJS=ftp.o CThreadBase.o CFtpWorker.o CSocket.o
OBJS1=testftp.o CThreadBase.o CFtpWorker.o CSocket.o
CXX=g++
CXXFLAG=-c -g -std=c++11
LIBS=-lpthread

all:ftp testftp
	@echo ftp is updated!

ftp:$(OBJS)
	$(CXX) $^ -o $@ $(LIBS)

testftp:$(OBJS1)
	$(CXX) $^ -o $@ $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAG) $< -o $@

clean:
	rm $(OBJS) ftp

