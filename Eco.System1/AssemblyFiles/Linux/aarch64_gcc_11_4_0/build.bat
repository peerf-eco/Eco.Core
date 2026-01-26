rem call make clean -f Makefile TARGET=0 DEBUG=0
rem call make -f Makefile TARGET=0 DEBUG=0
rem call make clean -f Makefile TARGET=0 DEBUG=1
rem call make -f Makefile TARGET=0 DEBUG=1
call make clean -f Makefile TARGET=1 DEBUG=0
call make -f Makefile TARGET=1 DEBUG=0
call make clean -f Makefile TARGET=1 DEBUG=1
call make -f Makefile TARGET=1 DEBUG=1
