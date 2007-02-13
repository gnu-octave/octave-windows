sinclude ./Makeconf

PROGS=grab.oct win32api.oct __COM__.oct # ../inst/BROWER.m ../inst/image.m

ifeq (,$(findstring -cygwin,$(canonical_host_type)))
  ifeq (,$(findstring -mingw,$(canonical_host_type)))
    ifeq (,$(findstring -msdosmsvc,$(canonical_host_type)))
all:
	@echo "Not building Windows Specific functions"
    else
all: $(PROGS)
    endif
  else
all: $(PROGS)
  endif
else
all: $(PROGS)
endif

grab.oct: grab.o grab_win32part.o
	$(MKOCTFILE) -o $@ $^ -luser32

win32api.oct: win32api.o win32api_win32part.o
	$(MKOCTFILE) -o $@ $^ -luser32 -ladvapi32

__COM__.oct: __COM__.o
	$(MKOCTFILE) -o $@ $^ -lole32 -loleaut32 -luser32

../inst/%.m : %.m.in
	if [ ! -d ../inst ]; then mkdir ../inst; fi
	cp $< $@

clean: ; -rm *.o core octave-core *.oct *~
