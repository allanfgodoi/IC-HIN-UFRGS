CC=$(shell root-config --cxx)

ROOTINC=$(shell root-config --cflags)

ROOTLIBS=$(shell root-config --glibs)

all: opendata_macro_azimuthal_correlation

opendata_macro_azimuthal_correlation: opendata_macro_azimuthal_correlation.cc
	$(CC) $(ROOTINC) -o opendata_macro_azimuthal_correlation opendata_macro_azimuthal_correlation.cc $(ROOTLIBS)
