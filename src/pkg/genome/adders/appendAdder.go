package adders

import (
	"os"
	"io"
	"genome"
	"bytes"
)

func AppendAdder(genome io.ReadWriteSeeker, gene genome.Gene, geneMap genome.GeneMap) (pos int64, err os.Error) {
	//go to the end of the file
	pos, err = genome.Seek(0, 2)
	length, err := genome.Write(gene)
	if err != nil {panic(err.String())}
	
	genome.Seek(pos,0)
	test := make([]byte, length)
	genome.Read(test)
	if !bytes.Equal(gene.GetData(), test) {
		panic("apperder broken")
	}
	return

}
