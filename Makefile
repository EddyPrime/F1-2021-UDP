.phony: clean all

all:
	make -C src

debug:
	make debug -C src

clean:
	make -C src clean
