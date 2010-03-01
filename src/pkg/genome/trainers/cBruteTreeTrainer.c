#include "stdlib.h"

typedef struct genePart* GenePart;
typedef struct geneData* GeneData;
typedef struct geneNode* GeneNode;
typedef struct bruteTreeTrainer* BruteTreeTrainer;
typedef struct geneDataSlice* GeneDataSlice;
typedef unsigned int uint;

#define NextNodeBound 2
#define MinLen 6
#define MaxDepth 6

struct genePart{
	short DataValue;
	uint HitNumber;
	GeneNode Next;
};

struct geneData{
	short* DataValue;
	uint HitNumber;
};

struct geneNode{
	short DataValue;
	short Depth;
	uint HitNumber;
	GenePart GeneParts[256];
};

struct bruteTreeTrainer{
	GeneNode root;
	GeneDataSlice curHighest;
};


GeneNode newTree() {
	GeneNode root = malloc(sizeof(GeneNode));
	root->Depth = 0;
	root->DataValue = 0;
	root->HitNumber = 0;
	int index =0;
	GenePart curGenePart=0;
	for(index =0; index < 256; curGenePart = root->GeneParts[index], index++){
		curGenePart->DataValue = index;
		curGenePart->HitNumber = 1;
	}
	return root;
}

int numLeafs(GeneNode gn) {
	int tally, index, total;
	GenePart curGP =0;
	for(index =0; index < 256; curGP = gn->GeneParts[index], index++ ){
		if (curGP->Next != 0){
			total += numLeafs(curGP->Next);
			tally++;
		}
	}
	
	if (tally == 0) {
		total = 1;
	}
	return total;
}

	
