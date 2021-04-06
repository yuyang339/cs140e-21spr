SUBDIRS= 00-hello 01-fake-pi 02-tracing-pi 02-tracing-unix 1-fake-pi 2-trace

.PHONY: all clean
all check clean: $(SUBDIRS)

all: TARGET=all
check: TARGET=check
clean: TARGET=clean

# No, you can't do TARGET=$@, or at least I don't know how to.

# recursive call to make
$(SUBDIRS): force
	$(MAKE) -s -C $@ $(TARGET)

clean:
	rm -f *~ *.bak 

.PHONY: force
	force :;
