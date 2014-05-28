# Mockingbird Makefile
#
# contact: jeffsp@gmail.com

BUILD=debug

.SUFFIXES:
.PHONY: waf run check clean debug release

waf:
	waf

run: debug

check: waf
	$(MAKE) -C tests check

clean:
	$(MAKE) -C tests clean

debug: waf
	./build/debug/mbc --help
	./build/debug/mbc

release:
	$(MAKE) -C tests $(BUILD)
