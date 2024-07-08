# define default target (first target = default)
# it depends on ¡¦hello.o¡¦ (which will be created if necessary)
# and hello_func.o (same as hello.o)
hello: hello.o hello_func.o
	gcc -Wall hello.o hello_func.o -o hello

# define hello.o target
# it depends on hello.c (and is created from it)
# also depends on hello_api.h which would mean that
# changing the hello.h api would force make to rebuild
# this target (hello.o).
# gcc -c: compile only , do not link
hello.o: hello.c hello_api.h
	gcc -Wall -c hello.c -o hello.o

# define hello_func.o target
# it depends on hello_func.c (and is created from)
# and hello_api.h (since that ¡¦s its declaration)
hello_func.o: hello_func.c hello_api.h
	gcc -Wall -c hello_func.c -o hello_func.o
