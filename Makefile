# Mockingbird Makefile
#
# contact: jeffsp@gmail.com

BUILD=debug

.SUFFIXES:
.PHONY: waf check clean run

default:
	$(MAKE) -C mbc
	$(MAKE) -C apps

check:
	$(MAKE) -C tests check

clean:
	$(MAKE) -C tests clean

run:
	$(MAKE) BUILD=$(BUILD) -C mbc run
