CFLAGS=-w

all: init end prodn prodp prodc prodkons kons result

result:
	gcc result.c queue.c -o result

kons:
	gcc kons.c queue.c -o kons $(CFLAGS)

prodc:
	gcc prodc.c queue.c -o prodc $(CFLAGS)

prodkons:
	gcc prodkons.c queue.c -o prodkons $(CFLAGS)

prodn:
	gcc prodn.c queue.c -o prodn $(CFLAGS)

prodp:
	gcc prodp.c queue.c -o prodp $(CFLAGS)

q_test:
	gcc queue_unit_test.c queue.c -o qt $(CFLAGS)

init:
	gcc initiate.c queue.c -o init $(CFLAGS)

end:
	gcc end.c -o end $(CFLAGS)

clean:
	rm end init qt prodn prodp prodc kons result prodkons
