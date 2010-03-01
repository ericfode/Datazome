package main

import (
	. "io/ioutil"
	"os"
	"bytes"
	. "encoding/binary"
	"fmt"
	. "sort"
)


//function to get all of the dirs and then call send the results to a go rutine that reads them
func getAndRunFiles() (result []int, total int) {
	fmt.Println("reading files")
	result = make([]int, 65538)
	files, err := ReadDir("/usr/bin/")
	if err != nil {
		panic("ohh shit file read error")
	}
	for i := 0; i < 500; i++ {
		if files[i].IsRegular() {
			total += readBinFile(files[i], result)
			fmt.Print(".")
		}
	}
	return
}
//function to read all of the bytes in a file and send them to the function that adds the result in to an array
func readBinFile(file *os.Dir, results []int) int {
	fileName := "/usr/bin/"
	fileName = fileName + file.Name
	// fmt.Println("reading",fileName)
	contents, err := ReadFile(fileName)
	if err != nil {
		panic("ohh shit file read error", err.String())
	}
	contentBuf := bytes.NewBuffer(contents)
	size := len(contents)
	curDataI := make([]uint16, size/2)
	Read(contentBuf, LittleEndian, curDataI)
	//fmt.Println("size of", fileName, "is",size);
	//for all of the resuts in cur dataI add them to the array in their pos
	for _, val := range (curDataI) {
		results[val] = results[val] + 1
	}
	//fmt.Println("finished Reading", fileName);

	return size
}

type pair struct {
	Value  int
	Number int
}
//really pair slice
type pairArray []pair

func NewPair(value, number int) pair {
	newpair := pair{Value: value, Number: number}
	return newpair
}

//going to add in ability to statistical operations on the arrays
//this will including finding the median values and how many
//dataparts fall into each part

//the goal of this is to set up stat output for adelle to generate
//graphs using google code

func (p pairArray) Sort()    { Sort(p) }
func (p pairArray) Len() int { return len(p) }

func (p pairArray) Less(i, j int) bool { return p[i].Value < p[j].Value }

func (p pairArray) Swap(i, j int) {
	var tempV, tempN int
	tempV = p[i].Value
	tempN = p[i].Number
	p[i].Number = p[j].Number
	p[i].Value = p[j].Value
	p[j].Value = tempV
	p[j].Number = tempN
}

func MakePairArray(data []int) pairArray {
	pairs := make(pairArray, len(data))
	for key, value := range (data) {
		pairs[key] = NewPair(value, key)
	}
	return pairs
}

func (p pairArray) Max(isSorted bool) pair {
	if isSorted {
		return p[len(p)-1]
	}
	p.Sort()
	return p.Max(true)
}

func (p pairArray) Min(isSorted bool) pair {
	if isSorted {
		return p[0]
	}
	p.Sort()
	return p.Min(true)
}
func (p pairArray) Range(isSorted bool) int {
	if isSorted {
		return p[len(p)-1].Value - p[0].Value
	}
	p.Sort()
	return p.Range(true)
}
func (p pairArray) NumZeros(isSorted bool) int {
	i := 0
	for j := 0; j == 0; i++ {
		j = p[i].Value
	}
	return i
}
func fmtAndPrint(data []int, total int) {
	length := len(data)
	fmt.Println("stat update length and total:")
	fmt.Println(length)
	fmt.Println(total)
	pairs := MakePairArray(data)
	pairs.Sort()

	for key, value := range (data) {
		thisVal := (float64(value) / float64(total)) * 100
		if thisVal > .02 {
			fmt.Println(key, ": ", value)
		}
	}
	fmt.Println("Max", pairs[len(data)-2])
	fmt.Println("Min", pairs[2])
	fmt.Println("Num Zeros", pairs.NumZeros(true))
	fmt.Println("Range", pairs.Range(true))
}
func main() {
	fmt.Println("starting")
	fmt.Println("creating chans")
	result, size := getAndRunFiles()
	fmtAndPrint(result, size)
	fmt.Println("main waiting for quit Sig")
	fmt.Println("quitting")
	return
}
