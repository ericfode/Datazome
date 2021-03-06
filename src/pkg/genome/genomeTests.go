//gg:{exe: genomeTests.go, genome.a: genome.go, finders.a: ./finders/bruteFinder.go }
package main

import (
	"os"
	"io"
	"genome"
	"genome/finders"
	"genome/indexers"
	"genome/trainers"
	"genome/adders"
	//"runtime"
)


func NewGenomeTest() {
	//runtime.GOMAXPROCS(2)
	genomeFile, _ := os.Open("test.genome", os.O_CREATE^os.O_RDWR, 0666)
	indexFile, _ := os.Open("testIndex.genome", os.O_CREATE^os.O_RDWR, 0666)

	//genomeFile, _ := os.Open("test.genome", os.O_RDWR, 0666)
	//indexFile, _ := os.Open("testIndex.genome",os.O_RDWR, 0666)
	trainer := trainers.NewBruteTreeTrainer()
	genomeTrainer := genome.Trainer(trainer)

	genomeP, _ := genome.NewGenome(io.ReadWriteSeeker(genomeFile),
		io.ReadWriteSeeker(indexFile),
		genome.IndexerFunc(indexers.SmallIndexer),
		genomeTrainer,
		genome.FinderFunc(finders.HashFinder),
		genome.AdderFunc(adders.AppendAdder))
	err := genomeP.TrainOnDir("/usr/bin/")
	if err != nil {
	     print(err.String())
	}
	genomeP.SaveGenome()
	//blender, _ := os.Open("/home/eric/win/PandoraFox/MP3/Strangefinger/Into The Blue/Strangefinger - Sleep (Radio Edit).mp3", os.O_RDONLY, 0666)
	//blenderTest, _ := os.Open("test.mp3", os.O_CREATE^os.O_RDWR, 0666)
	//tegeneFile, _ := os.Open("testTgene", os.O_CREATE^os.O_RDWR, 0666)
	//tGene, _ := genomeP.NewTGene(blender, "blender")
	//tGene.Save(tegeneFile)
	//tGene := genome.LoadTGene(tegeneFile)
	//data, _ := genomeP.RecreateData(*tGene)
	//blenderTest.Write(data)
	//blenderTest.Close()
}

func main() { NewGenomeTest() }
