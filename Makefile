# User's Guide (VitePress, see doc/src/Makefile)
.PHONY: docs docs-serve

docs:
	$(MAKE) -C doc/src dist

docs-serve:
	$(MAKE) -C doc/src serve
