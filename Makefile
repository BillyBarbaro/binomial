CC=gcc

default: compile

compile: binomial.c
	gcc -o binomial binomial.c

run: binomial
	./binomial

clean:
	$(RM) binomial
