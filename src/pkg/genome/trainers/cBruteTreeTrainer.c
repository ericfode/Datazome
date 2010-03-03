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

//used in tree
struct genePart{
	short DataValue;
	uint HitNumber;
	GeneNode Next;
};

//used in list
struct geneData{
	short* DataValue;
	uint len;
	uint HitNumber;
};

struct geneNode{
	short DataValue;
	short Depth;
	uint HitNumber;
	GenePart GeneParts[256];
};

struct geneDataSlice{
        uint len;
	GeneData* Data;
};

struct bruteTreeTrainer{
	GeneNode root;
	GeneDataSlice curHighest;
};

int cmpGeneData(GeneData right, GeneData left){
     int index= 0;
     if (right->len != left->len){  return 0;  }
     for( index=0; index < right->len; index++){
	  if(right->DataValue[index] != left->DataValue[index]){ return 0; }
     }
     return 1;
}

GeneDataSlice newGDS(int size){
     GeneDataSlice gds = malloc(sizeof(GeneDataSlice*));
     gds->Data = malloc(sizeof(GeneData)*size);
     return gds;
}

GeneData newGD(int size){
     GeneData GD = malloc(sizeof(GeneData*));
     GD->len = size;
     return GD;
}

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

int numChildren(GeneNode gn) {
     int num = 0;
     for(int i = 0; i < 256; i++){
	  if( gn->GeneParts[i] != 0) {
	       num++;
	  }
     }
     return num;
}

GeneDataSlice mergeGeneData(GeneDataSlice right, GeneDataSlice left) {
 
     int dups = 0;
     GeneData curGDl = 0;
     GeneData curGDr = 0;
     //first pass to determine the size of the new list
     //for ever item on the right side
     int indexl = 0;
     int indexr = 0;
     
     for(indexr = 0; indexr < right->len; curGDr = right->Data[indexr], indexr++ ){
	  //for every item on the left side
	  for(indexl = 0; indexl < left->len; curGDl = left->Data[indexl], indexl++ ){
	       //if the byte slices are equal
	       if(1 == cmpGeneData(curGDr,curGDl)){
		    //add anouther tally to the dups list
		    dups++;
		    break;
	       }
	  }
     }
     
     GeneDataSlice GDS = newGDS(right->len+left->len -dups);
     //copy the right side over
     for(indexr = 0; indexr < right->len; curGDr = right->Data[indexr], indexr++ ){
	  GDS->Data[indexr] = curGDr;
     }
     
     //current offset of left side
     int tally = 0;
     //for every item on the left side check to see if there is already an equal in the list
     for(indexl = 0; indexl < left->len; curGDl = left->Data[indexl]){
	  indexl++;
	  int matched = 0;
          for(indexr = 0; indexr < right->len; curGDr = right->Data[indexr] ){
	       indexr++;
	       if(1 == cmpGeneData(curGDr,curGDl)){
		    GDS->Data[indexr]->HitNumber += curGDl->HitNumber;
		    matched = 1;
		    break;
	       }
	  }
	  if (matched != 1) {
	       //tally starts at zero because the len of the right will be one index of the
	       //end of the list
	       GDS->Data[right->len+tally] = curGDl;
	       tally++;
	  }
     }
     return GDS;
}

GeneDataSlice collapseLeafs(GeneNode gn) {
     //Create a geneDataSlice to hold the results
     GeneDataSlice GDS;
     
     //if this is a leaf
     if(numChildren(gn) == 0) {
	  //then make a new geneDataSlice 
	  GDS = newGDS(1);
	  GDS->Data[0]->HitNumber = gn->HitNumber;
	  GDS->Data[0] = newGD(gn->Depth);
	  GDS->Data[0]->DataValue[gn->Depth-1] = gn->DataValue;
	  return GDS;
     }
     
     //if this is a brach
     //join all of the leafs together
     //add this data value to the depth place in all of the slices
     int index =0
     for(index =0; index < 256; index++){
	  //select the child
	  child = gn->GeneParts[index];
	  if(child->Next != 0) {
	       GDS = join(GDS,collapseLeafs(child->Next);
	  }
     }
     if gn->Depth >0 {
	  
     
     //
}