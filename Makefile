OBJS=class/*.cpp \
class/*h \
main.cpp \

all: system-info.out

system-info.out: $(OBJS)
	g++ ./class/sys-info.h ./class/sys-info.cpp main.cpp ./class/ui.cpp -o system-info.out -lncurses

clear: 
	rm -rf *.out *.o