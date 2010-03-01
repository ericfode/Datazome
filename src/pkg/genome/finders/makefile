all: ../../../../lib/genome/finders.a

finders.8: bruteFinder.go
	8g -I ../../../../lib -o finders.8 bruteFinder.go

../../../../lib/genome/finders.a: finders.8
	gopack gru ../../../../lib/genome/finders.a finders.8

clean:
	-rm *.8
	-rm ../../../../lib/genome/finders.a

