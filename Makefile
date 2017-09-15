
# Time-stamp: <2017-09-14 17:40:26 dmendyke>

# Makefile used to build the "C" exception library and sample code

cc := /usr/bin/gcc
cflags := -I./
libs := -L./ -lexception

target := demo
objs := main.o libexception.a exception.o

all : $(target)

$(target) : $(objs)
	$(cc) -o $@ $< $(libs)

libexception.a : exception.o ; ar rcs $@ $<

exception.o : exception.c exception.h
	$(cc) $(cflags) -c -o $@ $<

main.o : main.c exception.h
	$(cc) $(cflags) -c -o $@ $<

.PHONY : clean
clean : ; rm --force $(target) $(objs)
