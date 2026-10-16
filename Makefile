PLUGIN=heapless.so
SOURCES=\
		heapless/* \
        heapless.cpp \
		$(END)

# Common makefile to be included from all other makefiles

# Where we installed gcc and its headers
# INSTALLDIR=<<INSTALLDIR>>

# CC=$(INSTALLDIR)/bin/gcc
# CXX=$(INSTALLDIR)/bin/g++
CC=$(shell which gcc)
CXX=$(shell which g++)
PLUGINDIR=$(shell $(CC) -print-file-name=plugin)

CFLAGS= -Wall -Wextra -g -fno-rtti -I$(PLUGINDIR)/include
CXXFLAGS= -std=c++23 -Wall -Wextra -g -fno-rtti -I$(PLUGINDIR)/include
LDFLAGS=
LDADD=

END=
OBJECTS=$(patsubst %.cc,%.o,$(SOURCES))

all: $(PLUGIN)

$(PLUGIN): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ -shared $+ $(LDADD) $(CXXFLAGS)

%.o: %.cc
	$(CXX) -c -o $@ $(CFLAGS) $<

.PHONY: all clean test
clean:
	rm -f *.a *.so

test: $(PLUGIN)
	CC="$(CC)" PLUGIN="$(PLUGIN)" ./test/run_tests.sh
