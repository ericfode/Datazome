//gg:{genome.a: geneAddress.go gene.go tGene.go}
package genome
//needs get methods
import(
 "io"
 "gob"
 )

type TGene struct {
	KnownGenes   []GeneAddress //needs to be changed to hash instead of GeneAddress
	UnknownGenes []byte
	Ordering     []bool
	DataSize     int64
}

func NewTGene(knownGenes []GeneAddress, unknownGenes []byte, ordering []bool, dataSize int64) (tGene TGene) {
	tGene = *(new(TGene))
	tGene.KnownGenes = knownGenes
	tGene.UnknownGenes = unknownGenes
	tGene.Ordering = ordering
	tGene.DataSize = dataSize
	return
}

func (Tgene *TGene) Save( file io.Writer){
	enc := gob.NewEncoder(file)
	err := enc.Encode(Tgene)
	if err != nil {
		panic(err.String())
	}
	return
}

func LoadTGene( file io.Reader)(tGene *TGene){
	tGene = new(TGene)
	dec := gob.NewDecoder(file)
	err := dec.Decode(tGene)
	if err != nil {
		panic(err.String())
	}
	return
}
