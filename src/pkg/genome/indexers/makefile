all: ../../../../lib/genome/indexers.a

clean:
	-rm ../../../../lib/genome/indexers.a
	-rm *.8

../../../../lib/genome/indexers.a:indexers.8
	gopack gru ../../../../lib/genome/indexers.a indexers.8

indexers.8: smallIndexer.go
	8g -I ../../../../lib -o indexers.8 smallIndexer.go