LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)
PREFIX = /usr/local

all: sway_status

sway_status: sway_status.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o sway_status sway_status.o $(LDLIBS)

clean:
	$(RM) *.o

distclean: clean
	$(RM) sway_status

install:
	install -m 755 sway_status $(DESTDIR)$(PREFIX)/bin
