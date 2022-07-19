CC=$(shell root-config --cxx)

ROOTINC=$(shell root-config --cflags)

ROOTLIBS=$(shell root-config --glibs)

all: macro_azimuthal_correlation

macro_azimuthal_correlation: macro_azimuthal_correlation.cc
	$(CC) $(ROOTINC) -o macro_azimuthal_correlation macro_azimuthal_correlation.cc $(ROOTLIBS)
