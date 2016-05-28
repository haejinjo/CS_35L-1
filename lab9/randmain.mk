# randmain.mk


# builds program randmain using three types of linking
	# 1. static linking to combine randmain.o and randcpuid.o into
	#    single executable programs

	# 2. dynamic linking to link C library and any necessary 
	#    system-supplied files before its main function is called

	# 3. after main is called, use dynamic linking via dlsym

randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) randlibhw.c -shared -fPIC -o randlibhw.so
randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) randlibsw.c -shared -fPIC -o randlibsw.so
randmain: randmain.o randcpuid.o
	$(CC) $(CFLAGS) randmain.o randcpuid.o -ldl -Wl, -rpath=$(PWD) -o randmain
