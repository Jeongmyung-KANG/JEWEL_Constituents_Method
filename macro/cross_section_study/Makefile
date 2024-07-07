SHELL = /bin/sh
MYPYTHIA=/Users/gangjeongmyung/pythia8310
-include $(MYPYTHIA)/Makefile.inc
# PYTHIA variables
PYTHIA_INCDIR=$(MYPYTHIA)/include
PYTHIA_LIBDIR=$(MYPYTHIA)/lib

# ROOT variables (ROOTCFLAGS also includes include path)
ROOTCFLAGS	= $(shell root-config --cflags)
ROOTLIBS	= $(shell root-config --glibs)

# There is no default behaviour, so remind user.
all:
	@echo "Usage: make XXX, where XXX.cc is your program"

# Create an executable for one of the normal test programs
%:	%.cc /Users/gangjeongmyeong/pythia8310/lib/libpythia8.so #dependencies
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) -I/Users/gangjeongmyeong/pythia8310/include \
	$@.cc -o $@.exe \
	-L/Users/gangjeongmyeong/pythia8310/lib -Wl,-rpath,/Users/gangjeongmyeong/pythia8310/lib -lpythia8 -ldl \
	$(ROOTLIBS) -lEG


# Clean up: remove executables and outdated files.
.PHONY: clean
clean:
	rm -f *.exe
	rm -f *~; rm -f \#*; rm -f core*
