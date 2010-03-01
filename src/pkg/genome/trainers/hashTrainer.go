package trainers

import


func HashFinder(data io.ReadSeeker, genomeRW io.ReadSeeker, geneMap genome.GeneMap, indexer genome.Indexer) (geneAddressSlice []genome.GeneAddress, unfound []byte, ordering []int, size int64) {
	//find the longest item in geneMap
	maxSize := 0
	for _, item := range (geneMap) {
		if item.Length > int64(maxSize) {
			maxSize = int(item.Length)
		}
	}
	fmt.Println("max Gene size:",maxSize)
	//make an array of byte arrays to hold all of the hashes that are generated that is as long as the longest
	strings := make([]string, maxSize+1)
	fileSize, _ := data.Seek(0, 2)
	fileTest, _ := data.Seek(50,0)
	println(fileTest)
	fileTest, _ = data.Seek(0,0)
	fmt.Println(data)
	println(fileTest)
	fmt.Println("file Size:", fileSize)
	geneAddressSlice = make([]genome.GeneAddress, fileSize/4)
	unfound = make([]byte, fileSize)
	ordering = make([]int, fileSize)
	//the starting position
	startPos := 0
	foundGenes := 0
	unfoundGenes := 0
	fmt.Println(data)
	//number of times the loop has run through
	i := 0
	//grab n data  to make room for the zero case 
	dataSlice := make([]byte, maxSize)
	for {/*
		if (i %1000) ==0 {
			fmt.Print("f")
		}*/
		//fmt.Println(data)
		//start at the begging of this data segment
		_, err := data.Seek(int64(startPos), 0 )
		if err != nil && err != os.EOF {
			panic(err.String())
		}
			
		//get the data remebering how much of the data is valid
		validData, err := data.Read(dataSlice)
		//fmt.Println("read :", validData)
		if err != nil && err != os.EOF {
			panic(err.String())
		}
		//for each pos in the data array
		//generate hashes from minSize (aka 5) size of an int to n length
		for index := 1; index < validData && index <= maxSize; index++ {
			tempBytes := indexer.Index(genome.Gene(dataSlice[0:index]))
			buf := bytes.NewBuffer(tempBytes)
			strings[index] = buf.String()
		}

		biggestHashSize := 0
		biggestHashIndex := ""
		//for each item in the genome cmp the strings
		for key, address := range (geneMap) {
			if int(address.Length) > biggestHashSize && key == strings[address.Length] {
				testData := make([]byte, int(address.Length))
				genomeRW.Seek(0, int(address.Addr))
				genomeRW.Read(testData)
				//if these are not infact the same skip over this one
				println(address.Length)
				if !bytes.Equal(testData, dataSlice[0:address.Length]) {
					fmt.Println(testData)
					fmt.Println(dataSlice[0:address.Length])
					println("false Positive")
					continue
				}
				biggestHashSize = int(address.Length)
				biggestHashIndex = key

			}
		}
		//now after the item has been found (or not) put the new infromation in the right list
		if biggestHashSize != 0 {
			//if something has been found assign it to the found genes
			geneAddressSlice[foundGenes] = genome.GeneAddress{0, 0, biggestHashIndex}
			//geneAddressSlice[foundGenes].Hash = biggestHashIndex
			ordering[i] = foundGenes
			foundGenes++
			startPos += biggestHashSize
		} else {
			ordering[i] = -1
			unfound[unfoundGenes] = dataSlice[0]
			unfoundGenes++
			startPos += 1
		}
		i++
		if 1 == validData {
			println("end of file")
			break
		}
	}
	//now finish up the ordering slice
	println("ordering size", i)
	newOrdering := make([]int, i)
	newAddressSlice := make([]genome.GeneAddress, foundGenes)
	newUnfound := make([]byte, unfoundGenes)
	//then copy both the unfound and geneAddressSlice over to well sized ones
	for place := 0; place < len(newOrdering); place++ {
		newOrdering[place] = ordering[place]
	}
	for j := 0; j < len(newAddressSlice); j++ {
		newAddressSlice[j] = geneAddressSlice[j]
	}
	for j := 0; j < len(newUnfound); j++ {
		newUnfound[j] = unfound[j]
	}
	ordering = newOrdering
	geneAddressSlice = newAddressSlice
	unfound = newUnfound
	counter := 0
	for j := 0; j < len(ordering); j++ {
		if ordering[j] == -1 {
			ordering[j] = foundGenes + counter
			counter++
		}
	}
	size = int64(maxSize)
	fmt.Print("\n")
	fmt.Println("found", foundGenes)
	fmt.Println("unfound", unfoundGenes)
	return

}
