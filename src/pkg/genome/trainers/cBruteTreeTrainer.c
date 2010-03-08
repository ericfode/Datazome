#include "stdlib.h"
#include "stdio.h"
#include "malloc.h"
typedef struct genePart* GenePart;
typedef struct geneData* GeneData;
typedef struct geneNode* GeneNode;
typedef u_char byte;
typedef struct bruteTreeTrainer* BruteTreeTrainer;
typedef struct geneDataSlice* GeneDataSlice;
typedef struct trainingData* TrainingData;
typedef unsigned int uint;

#define NextNodeBound 2
#define MinLen 6
#define MaxDepth 50

//used in tree
struct genePart{
	byte DataValue;
	uint HitNumber;
	GeneNode Next;
};

//used in list
struct geneData{
	byte* DataValue;
	uint len;
	uint HitNumber;
};

struct geneNode{
	byte DataValue;
	byte Depth;
	uint HitNumber;
	GenePart* GeneParts;
};

struct geneDataSlice{
        uint len;
	GeneData* Data;
};

struct bruteTreeTrainer{
	GeneNode root;
	GeneDataSlice curHighest;
};

struct trainingData{
        byte* Data;
	uint len;
};
     
unsigned long long totalMem;

void* CountedMalloc(uint size) { 
     totalMem += size;
    // printf("mem allocted: %d total mem: %lld\n",size,totalMem);
    void* newptr = malloc(size);
    if (newptr == 0){
	 printf("out of memory\n");
	 exit(1);
    }
     return newptr;
}

void CountedFree(void* ptr) {
     totalMem -= sizeof(ptr);
     free(ptr);
}


int cmpGeneData(GeneData right, GeneData left){
     int index= 0;
     if (right->len != left->len){  return 0;  }
     for( index=0; index < right->len; index++){
	  if(right->DataValue[index] != left->DataValue[index]){ return 0; }
     }
     return 1;
}



GeneData newGD(int size){
     GeneData GD = CountedMalloc(sizeof(GeneData*));
     GD->DataValue = CountedMalloc(sizeof(byte)*size);
     GD->len = size;
     return GD;
}

//returns an array that can contain pointers YOU NEED TO INI THE GD YOUR SELF
GeneDataSlice newGDS(int size){
     GeneDataSlice gds = CountedMalloc(sizeof(GeneDataSlice*));
     gds->Data = CountedMalloc(sizeof(GeneData)*size);
     int index = 0;
     gds->len=size;
     return gds;
}
GeneNode newGN(){
     GeneNode GN = CountedMalloc(sizeof(GeneNode*));
     GN->HitNumber = 0;
     return GN;
}
TrainingData newTD(){
     TrainingData TD = malloc(sizeof(TrainingData*));
     TD->len = 0;
     return TD;
}

GeneNode newTree() {
       // printf("creating tree\n");
	GeneNode root = CountedMalloc(sizeof(GeneNode*));
	root->Depth = 0;
	root->DataValue = 0;
	root->HitNumber = 0;
	uint index =0;
	GenePart curGenePart=0;
	root->GeneParts = CountedMalloc(sizeof(GenePart)*256);
	for(index =0; index < 256; index++){
	       
		root->GeneParts[index]= CountedMalloc(sizeof(GenePart*));
	  //      printf("creating node\n");
		root->GeneParts[index]->DataValue = index;
	//	printf("indexing node\n");
		root->GeneParts[index]->HitNumber = 0;
	//	printf("hitNumbering node\n");
		root->GeneParts[index]->Next =0;
	//	printf("treeIndex : %d, TreeHitnumber: %d\n",index,root->GeneParts[index]->HitNumber);
	}
	
// 	for(index =0; index < 256; index++){
// 	//	printf("treeIndex : %d, TreeHitnumber: %d\n",index,root->GeneParts[index]->HitNumber);
// 	}
	//printf("tree ready\n");
	
	return root;
}
void testTree(GeneNode gn){
	  int index= 1;
     	for(index =0; index < 256; index++){
	       
		GenePart curGenePart = gn->GeneParts[index];
		printf("treeIndex : %d, TreeHitnumber: %d\n",index,curGenePart->HitNumber);
	}
	return;
}
BruteTreeTrainer newBTT(){
     printf("stage1 tree creation\n");
     BruteTreeTrainer btt = CountedMalloc(sizeof(BruteTreeTrainer*));
     printf("tree alllocated\n");
     btt->root = newTree();
  //   testTree(btt->root);
     btt->curHighest = newGDS(0);
     return btt;
}

int numLeafs(GeneNode gn) {
	int tally, index, total;
	GenePart curGP;
	for(index =0; index < 256; curGP = gn->GeneParts[index], index++ ){
		if (curGP->Next != 0){
			total += numLeafs(curGP->Next);
		}
	}
	
	if (numChildren(gn) == 0) {
	     //because this is a leaf
		total = 1;
	}
	return total;
}

int numChildren(GeneNode gn) {
     int num = 0;
     int i =0;
     //printf("in numChildren\n");
     for(i = 0; i < 256; i++){
	  if( gn->GeneParts[i]->Next != 0) {
	       num++;
	  }
     }
     return num;
}
//destructive to both the left and right pointers
GeneDataSlice joinGeneDataSlice(GeneDataSlice right, GeneDataSlice left) {
     //printf("joining\n");
     GeneDataSlice GDS = newGDS(right->len+left->len);
     int index = 0; 
     for( index= 0 ; index < right->len; index++) {
	  GDS->Data[index] = right->Data[index];
     }
     for( index = 0; index < left ->len; index++) {
	  GDS->Data[index+right->len] = left->Data[index];
     }
     CountedFree(left);
     CountedFree(right);
     return GDS;
}
GeneDataSlice mergeGeneDataSlice(GeneDataSlice right, GeneDataSlice left) {
 
     int dups = 0;
     GeneData curGDl = 0;
     GeneData curGDr = 0;
     //first pass to determine the size of the new list
     //for ever item on the right side
     int indexl = 0;
     int indexr = 0;
     
     for(indexr = 0; indexr < right->len; indexr++ ){
	  curGDr = right->Data[indexr];
	  //for every item on the left side
	  for(indexl = 0; indexl < left->len; indexl++ ){
	        curGDl = left->Data[indexl];
	       //if the byte slices are equal
	       if(1 == cmpGeneData(curGDr,curGDl)){
		    //add anouther tally to the dups list
		    dups++;
		    break;
	       }
	  }
     }
    printf("%d Dups\n",dups);
    
    
     GeneDataSlice GDS = newGDS((right->len+left->len) -dups);
     //copy the right side over
     for(indexr = 0; indexr < right->len; indexr++){
	  curGDr = right->Data[indexr];
	  GDS->Data[indexr] = curGDr;
     }
     
     //verifier
//      for(indexr = 0; indexr < GDS->len; curGDr = GDS->Data[indexr]){
// 	  indexr++;
// 	//  GDS->Data[indexr] = right->Data[indexr];
// 	  printf("coping r %d\n",GDS->Data[indexr]->DataValue);
//      }
     
     printf("copying leftside\n");
     //current offset of left side
     int offset = 0;
     //for every item on the left side check to see if there is already an equal in the list
     for(indexl = 0; indexl < left->len; indexl++){
	  curGDl = left->Data[indexl];
	  int matched = 0;
          for(indexr = 0; indexr < right->len;indexr++ ){
	       curGDr = GDS->Data[indexr];
	       if(1 == cmpGeneData(curGDr,curGDl)){
		    GDS->Data[indexr]->HitNumber += curGDl->HitNumber;
		    CountedFree(curGDl->DataValue);
		    CountedFree(curGDl);
		    matched = 1;
		    break;
	       }
	  }
	  if (matched != 1) {
	       //tally starts at zero because the len of the right will be one index of the
	       //end of the list
	       GDS->Data[right->len+offset] = curGDl;
	//       printf("coping leftside len:%d data:%d index:%d\n",curGDl->len, curGDl->DataValue, right->len+offset);
	       offset++;
	  }	  
     }

  
     GDS->len = right->len+(offset-1);
     printf("merged len %d\n", GDS->len);
     CountedFree(right);
     CountedFree(left);
     return GDS;
}

GeneDataSlice collapseLeafs(GeneNode gn) {
     //Create a geneDataSlice to hold the results
     
    // printf("in collapse leafs\n");
     //if this is a leaf
     if(numChildren(gn) == 0) {
	  //create a GDS with room for one node
	  GeneDataSlice GDS = newGDS(1);
	  //printf("in leaf node \n");
	  //then make a new geneDataSlice 
	//  printf("created GDS hitnumber:%d depth:%d dataValue:%d\n",gn->HitNumber,gn->Depth, gn->DataValue);
	  GDS->Data[0] = newGD(gn->Depth);
	  (GDS->Data[0])->HitNumber = gn->HitNumber;
	 // printf("got hitnumber\n");
	  
	 // printf("created GeneData\n");
	  GDS->Data[0]->DataValue[gn->Depth-1] = gn->DataValue;
	  return GDS;
     }
     
     //if this is a brach
     //join all of the leafs together
     //add this data value to the depth place in all of the slices
     int index =0;
     //create a GDS just to have something to return to
     GeneDataSlice GDS = newGDS(0);
     //printf("in branch children:%d\n",numChildren(gn));
     for(index =0; index < 256; index++){
	  GenePart child = gn->GeneParts[index];
	  if(child->Next != 0) {
	   //    printf("on index:%d\n",index);
	       //GDS contains a running "total" of all of leafs
	       GDS = joinGeneDataSlice(GDS,collapseLeafs(child->Next));
	  }
     }
     //if this is not a leaf
     //printf("checking depth\n");
     if( gn->Depth > 0 ){
	 //printf("copyinging data GDSlen:%d depth:%d datavalue:%d\n",GDS->len,gn->Depth,gn->DataValue);
	  //for every item in the current GeneDataSlice, add this nodes data value to the datavalues that are stored
	  //???Somethign is wrong with last item of the GDS...????//
	  for (index = 0; index < GDS->len; index++){
	//       printf("on index %d len:%d\n",index,GDS->Data[index]->len);
	       GDS->Data[index]->DataValue[gn->Depth]= gn->DataValue;
	  }
     }
   //  printf("returning from collapseLeafs\n");
     return GDS;
}

void deleteLeafs(GeneNode node){
     int index = 0;
     
     for(index = 0; index < 256; index++) {
	  GenePart child = node->GeneParts[index];
	  if (child->Next != 0){
	       deleteLeafs(child->Next);
	       CountedFree(child->Next);
	       child->Next = 0;
	  }
	  // many need to add an extra free here
     }
     return;
}
//byte is named b because in go a byte is also a byte and this byte is being used as a byte
void addNode(GeneNode gn,byte b){
     gn->GeneParts[b]->Next = newTree();
     gn->GeneParts[b]->Next->Depth = gn->Depth + 1;
     gn->GeneParts[b]->Next->DataValue = b;
     int index = 0;
     //initilize the new gene GeneParts
     for(index = 0; index < 256; index++) {
	  gn->GeneParts[b]->Next->GeneParts[index]->DataValue = index;
	  gn->GeneParts[b]->Next->GeneParts[index]->HitNumber = 0;
     }
     return;
}

void addByteSlice(GeneNode gn, byte* b,byte* end) {
     GeneNode curGN = gn;
     //printf("adding byte:%d\n",b[0]);
     while(1){
	  curGN->GeneParts[b[0]]->HitNumber++;
	  curGN->HitNumber++;
	  //printf("added byte.. hitnumber:%d\n",curGN->GeneParts[b[0]]->HitNumber);
	  if( curGN->GeneParts[b[0]]->HitNumber == NextNodeBound){
	   //    printf("adding node\n");
	       addNode(curGN,b[0]);
	  }
	  if( curGN->GeneParts[b[0]]->HitNumber > NextNodeBound) {
	       if (b != end && curGN->Depth < MaxDepth) {
		    curGN = curGN->GeneParts[b[0]]->Next;
		    b++;
		    continue;
	       }
	  }
	 // printf("finished addByteSlice\n");
	  return;
     }
}

void PrintStats(GeneDataSlice gd){
     printf("working on stats\n");
     float avarageLen = 0;
     int maxLen = 0;
     int minLen = 500000;
     int maxO = 0;
     int minO = 500000;
     float avarageO = 0;
     
     int index = 0;
     for(index = 0; index < gd->len; index++){
	 // printf("on index:%d addresss:%d \n", index,gd->Data[index]);
	  GeneData cur= gd->Data[index];
	  if (cur->len > maxLen) {
	       maxLen = cur->len;
	  }
	  if (cur->len < minLen) {
	       minLen = cur->len;
	  }
	  avarageLen += cur->len;
	  
	  if (cur->HitNumber > maxO) {
	       maxO = cur->HitNumber;
	  }
	  if (cur->HitNumber < minO) {
	       minO = cur->HitNumber;
	  }
	  avarageO += cur->HitNumber;
     }
     avarageLen /= gd->len;
     avarageO /= gd->len;
     printf("\n");
     printf("min Length: %d max Length: %d a Length: %f min Hits: %d max Hits: %d a Hits: %f total: %d\n"
     , minLen, maxLen,avarageLen,minO,maxO,avarageO,gd->len);
     return;
}

void BTTrain(BruteTreeTrainer bt, TrainingData td){
     printf("starting Train \n");
     int index;
     for( index = 0 ; index < td->len; index++){
	 // printf("%d\n",counter);
	  addByteSlice(bt->root,&(td->Data[index]),td->Data+td->len);
	  if(index % 50000 == 0 && index != 0){
	       printf("collapsing\n");
	       GeneDataSlice temp = collapseLeafs(bt->root);
	       printf("trying to merge\n");
	       bt->curHighest = mergeGeneDataSlice(temp,bt->curHighest );
	       printf("curHighest len:%d\n",bt->curHighest->len);
	       PrintStats(bt->curHighest);
	       deleteLeafs(bt->root);
	       CountedFree(bt->root);
	       bt->root= newTree();
	       printf("adding byte\n");
	       
	  }
     }
     return;
}

int main(){
     printf("starting\n");
     BruteTreeTrainer btt = newBTT();
     TrainingData TD = newTD();
     printf("created btt an td\n");
     TD->Data = malloc(sizeof(byte)*60 *1000000);
     TD->len = 60 *1000000;
     int index =0;
     for(index =0; index < TD->len; index++){
	  TD->Data[index] = (byte)rand();
     }
     printf("made really big td\n");
     BTTrain(btt, TD);
     free(TD->Data);
     free(TD);
     return 1;
}