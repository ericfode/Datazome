libs: clean ../libs/genome.a ../libs/genome/trainers.a ../libs/genome/finders.a ../libs/genome/indexers.a ../libs/genome/adders.a 

test: clean libs
	make -C ./pkg/genome/ test
	cp ./pkg/genome/genomeTester ./
all: clean test


../libs/genome.a:
	make -C ./pkg/genome all

../libs/genome/trainers.a:
	make -C ./pkg/genome/trainers/ all

../libs/genome/finders.a:
	make -C ./pkg/genome/finders/ all

../libs/genome/indexers.a:
	make -C ./pkg/genome/indexers/ all

../libs/genome/adders.a:
	make -C ./pkg/genome/adders/ all


clean:
	make -C ./pkg/genome/adders/ clean
	make -C ./pkg/genome/indexers/ clean
	make -C ./pkg/genome/finders/ clean
	make -C ./pkg/genome/trainers/ clean
	make -C ./pkg/genome/ clean
	
