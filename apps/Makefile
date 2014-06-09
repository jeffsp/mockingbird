# Mockingbird Makefile
#
# contact: jeffsp@gmail.com

BUILD=debug

.SUFFIXES:
.PHONY: waf check clean run

waf:
	waf

check:
	./mkchains.py --help
	./mkchains.py wc
	./mkchains.py -f "../images/*.nef" --ext .pgm nef2bayer
	./mkchains.py --dir /tmp --fs "../images/*.nef" --ext .ppm ./build/debug/nef2bayer ./build/debug/ahd_interpolate | xargs -P 12 --verbose -I{} sh -c {}
