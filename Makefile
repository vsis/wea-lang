.PHONY: check clear

check:
	$(MAKE) -C src check

clear:
	$(MAKE) -C src clear

all:
	$(MAKE) -C src all
