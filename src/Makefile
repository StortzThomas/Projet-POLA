#
# Makefile
#

all:	libs polash

libs:	libpolalib.so

libpolalib.so:	polalib.c
	rm -f libpolalib.so*
	gcc -fPIC -shared -Wl,-soname,libpolalib.so.1 -ldl -o libpolalib.so.1.0  polalib.c
	ln -s libpolalib.so.1.0 libpolalib.so.1
	ln -s libpolalib.so.1 libpolalib.so

clean:	rm -f libpolalib.so* polash
