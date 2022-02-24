.PHONY: check clear

lint:
	$(MAKE) -C src lint

check:
	$(MAKE) -C src check

clear:
	$(MAKE) -C src clear

all:
	$(MAKE) -C src all
