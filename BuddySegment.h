#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
struct BuddyPartition
{
    int size;
    struct BuddyPartition* left;
    struct BuddyPartition* right;
    int pid;
    int free;
    int start;
    int end;

} typedef BuddyPartition;


struct
{
    BuddyPartition* root;
} typedef BuddySegment;

BuddyPartition* newPartition(int ssize,int sstart, int eend){
    BuddyPartition* partition = (BuddyPartition*)malloc(sizeof(BuddyPartition));
    partition->start=sstart;
    partition->end=eend;
    partition->free=1;
    partition->pid=-1;
    partition->size=ssize;
    partition->left=NULL;
    partition->right=NULL;
    return partition;
}

BuddySegment* initializeMemory(){
    BuddySegment* mainSeg = (BuddySegment*) malloc(sizeof(BuddySegment));
    mainSeg->root = newPartition(1024,0,1023);
    return mainSeg;
}

void rec_freememory(BuddyPartition* partition){
    printf("freeing partition");
    if(partition->left !=NULL){
        rec_freememory(partition->left);
    }
    if(partition->right != NULL){
        rec_freememory(partition->right);
    }
    free(partition);
    printf("\ndone freeing memory\n");
}

void deleteSegment(BuddySegment* segment){
    rec_freememory(segment->root);
}

BuddyPartition* rec_allocatePartition(BuddyPartition* root,int size){
printf("\ncurrent partition is of size %d looking for size = %d, free = %d\n",root->size,size,root->free);
if(root==NULL) return NULL;
if(size>root->size||root->free==0)
    return NULL;
if(size==root->size&&root->left==NULL)
{
    root->free=0;
    return root;
}
if(root->size!=1){
    if(root->left==NULL){
        root->left = newPartition(root->size/2,root->start,root->start + root->size/2-1);
        root->right = newPartition(root->size/2,root->start + root->size/2,root->end);
        return rec_allocatePartition(root->left,size);
    }
    else {
        if(root->left->free)
        {
            BuddyPartition* lfound= rec_allocatePartition(root->left,size);
            if(lfound==NULL) {
                lfound = rec_allocatePartition(root->right,size);
                if(lfound){
                    if(root->left->free + root->right->free==0)
                        root->free=0;
                }
            }
                return lfound;
        }
        else {
            BuddyPartition* found =  rec_allocatePartition(root->right,size);
            root->free = root->right->free;
            return found;
        }
    }
}
    return NULL;
}
int rec_deallocatePartition(BuddyPartition* root, int size, int prcss_id) {
    if(root==NULL) return 0;
    if(size>root->size)
        return 0;
    if(root->pid==prcss_id)
    {
        root->free=1;
        root->pid=-1;
        return 1;
    }
    else if(root->left !=NULL)
    {
        if(rec_deallocatePartition(root->left,size,prcss_id)==1||rec_deallocatePartition(root->right,size,prcss_id)==1){
            if(root->left->free==1 && root->right->free==1&& root->left->left==NULL &&root->right->left==NULL)
            {
                free(root->left);
                free(root->right);
                root->free=1;
                root->left = NULL;
                root->right = NULL;
            }
            return 1;
        }
        else
            return 0;
    }
    else
        return 0;

}
void deallocatePartition(BuddySegment* segment, int processSize, int pid) {
    
    int found = rec_deallocatePartition(segment->root, processSize, pid);
    if(found==0)
        printf("___\n___\n___\n___\nYALAHWAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY\n___\n___\n___\n___");
}
BuddyPartition* allocatePartition(BuddySegment* segment,int processSize,int pid) {
    processSize = pow(2,ceil(log(processSize)/log(2)));
    BuddyPartition* newpart=rec_allocatePartition(segment->root,processSize);
    if(newpart)
    {
        newpart->pid=pid;
        printf("\n special kalam allocated memory of %d starting from %d to %d\n",newpart->size,newpart->start,newpart->end);
        return newpart;
    }
    return NULL;
}
