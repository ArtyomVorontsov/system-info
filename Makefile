OBJS=class/*.cpp \
class/*.h \
main.cpp \

all: system-info.out

system-info.out: $(OBJS)
	g++ ./class/config.h ./class/sys-info.h ./class/ui.h ./class/config.cpp  ./class/sys-info.cpp main.cpp ./class/ui.cpp -o system-info.out -lncurses

clear: 
	rm -rf *.out *.o