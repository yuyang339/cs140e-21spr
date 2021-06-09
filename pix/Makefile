MAKEFLAGS += --no-print-directory

# change these so you can do a "make test" and it verifies what you already
# have in there.  (probably need a pi in there).   also you will need to 
# do a better design.
# ah: for test, doesn't work for us b/c we don't have the code...
#
# also it is tricky if they swap with our code and their code.
SUBDIRS= pix-user pix-kernel$(STAFF) pitag-linker tests

.PHONY: all check clean run
all check clean: $(SUBDIRS)

all: TARGET=all
clean: TARGET=clean

# No, you can't do TARGET=$@, or at least I don't know how to.

# recursive call to make
$(SUBDIRS): force
	$(MAKE) -s -C $@ $(TARGET)


check: 
	$(MAKE) -C tests check
run: 
	$(MAKE) -C tests run

clean:
	rm -f *~ *.bak mk/*~

.PHONY: force
	force :;
