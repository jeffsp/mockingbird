# Mockingbird Makefile
#
# contact: jeffsp@gmail.com

BUILD=debug

.SUFFIXES:
.PHONY: waf check clean run

waf:
	waf

INSTALL_DIR=/usr/local/bin

install: waf
	sudo install -s -v \
		./build/release/ahd_interpolate \
		./build/release/nef2bayer \
		./build/release/normalize \
		./build/release/srgb \
		$(INSTALL_DIR)
	sudo install -v \
		./mkchains.py \
		$(INSTALL_DIR)

uninstall:
	sudo rm -f \
		$(INSTALL_DIR)/ahd_interpolate \
		$(INSTALL_DIR)/nef2bayer \
		$(INSTALL_DIR)/normalize \
		$(INSTALL_DIR)/srgb \
		$(INSTALL_DIR)/mkchains.py

check: waf
	./mkchains.py --help
	./mkchains.py wc
	./mkchains.py -f "../images/*.nef" --ext .pgm nef2bayer
	./mkchains.py --dir /tmp --fs "../images/*.nef" --ext .ppm \
		./build/debug/nef2bayer \
		./build/debug/ahd_interpolate \
		./build/debug/srgb \
		./build/debug/normalize \
		./build/debug/srgb_gamma \
		| xargs -P 12 --verbose -I{} sh -c {}
	display /tmp/cps*.ppm
