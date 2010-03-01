package indexers

import (
	"hash/crc32"
	"genome"
)

func SmallIndexer(gene genome.Gene) (index []byte) {
	hash := crc32.NewIEEE()
	// 	//TODO: fix me
	hash.Write(gene)
	index = hash.Sum()
	return
}
