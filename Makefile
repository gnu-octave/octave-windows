sinclude ../../Makeconf

ifeq (,$(findstring -cygwin,$(canonical_host_type)))
  ifeq (,$(findstring -mingw,$(canonical_host_type)))
none: ; touch NOINSTALL
  endif
endif

LINKS= win32_MessageBox.oct win32_ReadRegistry.oct

all: grab.oct win32api.oct $(LINKS)

grab.oct: grab.o grab_win32part.o
	$(MKOCTFILE) -o $@ $^

win32api.oct: win32api.o win32api_win32part.o
	$(MKOCTFILE) -o $@ $^

$(LINKS): win32api.oct
	ln -s $< $@

