
all: inverse_single inverse_double epsilon loop sum_numbers

inverse_single: inverse_single.c
	gcc -o $@ $^

inverse_double: inverse_double.c
	gcc -o $@ $^

epsilon: epsilon.c
	gcc -o $@ $^

loop: loop.c
	gcc -o $@ $^

sum_numbers: sum_numbers.f90
	gfortran -o $@ $^

clean:
	rm -f *~ inverse_single inverse_double epsilon loop sum_numbers
