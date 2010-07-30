lib_LTLIBRARIES=libmta_mysql.la
libmta_mysql_la_SOURCES=handler.cpp module.cpp result.cpp
libmta_mysql_la_LIBADD=-L../lib -L../luaimports -lluaimports -l_mysqlclient -lm -lz
AM_CPPFLAGS=-I. -I.. -I../luaimports -I../include -I/usr/include/mysql -DHAVE_ERRNO_AS_DEFINE=1 -pipe
