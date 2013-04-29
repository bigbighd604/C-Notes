all: inetd echoserver tar

inetd: inetd.o readconf.o common.o
	$(CC) -o simple_inetd inetd.o readconf.o common.o

inetd.o: inetd.c readconf.h
	$(CC) -c inetd.c

readconf.o: readconf.c readconf.h
	$(CC) -c readconf.c

common.o: common.c common.h
	$(CC) -c common.c

echoserver: inet_echo.o
	$(CC) -o echoserver inet_echo.o

inet_echo.o: inet_echo.c
	$(CC) -c inet_echo.c

clean:
	rm -fr inetd.o readconf.o inet_echo.o simple_inetd echoserver simple_inetd.tar.gz

tar:
	tar -jcf simple_inetd.tar.gz common.c common.h readconf.c readconf.h inetd.c inet_echo.c inetd.sh config Makefile

install:
	install -m 755 -d /etc/simple_inetd
	install -m 755 config /etc/simple_inetd/simple_inetd.conf
	install -m 755 simple_inetd /usr/bin/simple_inetd
	install -m 755 echoserver /usr/bin/echoserver
