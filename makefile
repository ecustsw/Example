OBJS=ftp.o CThreadBase.o CFtpWorker.o
CXX=g++
CXXFLAG=-c -g
LIBS=-lpthread

all:ftp
	@echo ftp is updated!

ftp:$(OBJS)
	$(CXX) $^ -o $@ $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAG) $< -o $@

clean:
	rm $(OBJS) ftp

