# Mockingbird Makefile
#
# contact: jeffsp@gmail.com

BUILD=debug

.SUFFIXES:
.PHONY: waf clean run

waf:
	waf

clean:
	waf clean

run: waf
	./build/$(BUILD)/mbc --help
	./build/$(BUILD)/mbc

check:
	./build/$(BUILD)/mbc --help
	./build/$(BUILD)/mbc --file check.config
