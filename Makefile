# Mockingbird Makefile
#
# contact: jeffsp@gmail.com

BUILD=debug

.SUFFIXES:
.PHONY: waf check clean run

waf:
	waf

check: waf
	$(MAKE) -C tests check

clean:
	$(MAKE) -C tests clean

run:
	$(MAKE) BUILD=$(BUILD) -C mbc run
