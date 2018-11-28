#include <stdio.h>
#include <stdlib.h>
#include "genericdfs.h"
#include <limits.h>

typedef struct node* pNode2;

typedef struct node {
    unsigned int value;
    pNode2 *children;
    int numberOfChildren;
} node;

void printTree(pNode2 head);

unsigned int getNodeVal(pNode pn){
    if (pn ==  NULL){
        return 0;
    }
    
    return ((pNode2)pn)->value;
}

pNode copyNode(pNode pn){
    pNode2 newPn = (pNode2) malloc(sizeof(node));
    newPn->value = getNodeVal(pn);
    if (((pNode2)pn)->children == NULL){
        newPn->children = NULL;
        newPn->numberOfChildren = 0;
        return newPn;
    }
    newPn->children = malloc (((pNode2)pn)->numberOfChildren * sizeof(pNode));
    newPn->numberOfChildren = ((pNode2)pn)->numberOfChildren;
    int i;
    for (i = 0; i < ((pNode2)pn)->numberOfChildren; i++){
        newPn->children[i] = copyNode(((pNode2)pn)->children[i]);
    }
    return newPn;
}

int getNodeChildren (pNode node, pNode **children){
    pNode2 *childArr;
    childArr = malloc (((pNode2)node)->numberOfChildren * sizeof(pNode2));
    
    int i;
    for (i = 0; i < ((pNode2)node)->numberOfChildren; i++){
        childArr[i] = copyNode(((pNode2)node)->children[i]);
    }
    
    *children = (pNode *)childArr;
    
    return ((pNode2)node)->numberOfChildren;
}

void freeNode(pNode pn){
    if (pn == NULL){
        return;
    }
    if(((pNode2)pn)->numberOfChildren == 0){
        free(pn);
        return;
    }
    
    int i;
    
    for (i = 0; i < ((pNode2)pn)->numberOfChildren; i++){
        freeNode(((pNode2)pn)->children[i]);
    }
    
    free(((pNode2)pn)->children);
    free(pn);
}


/** TESTING CODE **/

pNode2 newNodeWithVal (unsigned int val){
    pNode2 pn = (pNode) malloc(sizeof(node));
    pn->children = NULL;
    pn->numberOfChildren = 0;
    pn->value = val;
    
    return pn;
}

void addChildToNode(pNode2 parent, pNode2 child){
    
    pNode2 *pNewChildren = malloc ((parent->numberOfChildren + 1) * sizeof(pNode2));
    
    int i;
    for (i = 0; i < parent->numberOfChildren; i++){
        pNewChildren[i] = parent->children[i];
    }
    
    
    pNewChildren[parent->numberOfChildren] = child;
    free(parent->children);
    parent->children = pNewChildren;
    
    parent->numberOfChildren++;
}

int heightOfTree (pNode2 head){
    if (head->numberOfChildren == 0){
        return 0;
    }
    
    int max = 0;
    int i;
    for (i = 0; i < head->numberOfChildren; i++){
        int curHeight = heightOfTree(head->children[i]);
        if (curHeight > max){
            max = curHeight;
        }
    }
    
    return max + 1;
}

void printTreeLevel(pNode2 head, int level){
    if (level == 0){
        printf(" %d ", getNodeVal(head));
        return;
    }
    
    if (head->numberOfChildren == 0){
        return;
    }
    
    int i;
    for (i = 0; i < head->numberOfChildren; i++){
        printTreeLevel(head->children[i], level-1);
        printf("                   ");
    }
}

void printTree(pNode2 head){
    int i, height;
    height = heightOfTree(head);
    
    for (i = 0; i <= height; i++){
        // Space at the beginning of the line
        int j;
        for (j = 0; j < height-i; j++){
            printf("   ");
        }
        printTreeLevel(head, i);
        printf("\n");
    }
}


int deepCompareNodes(pNode2 pn1, pNode2 pn2){
    // If only one of them is null
    if ((pn1 == NULL) ^ (pn2 == NULL)){
        return 0;
    }
    
    // Check value
    if (getNodeVal(pn1) != getNodeVal(pn2)){
        return 0;
    }
    
    // Check height
    if (heightOfTree(pn1) != heightOfTree(pn2)){
        return 0;
    }
    
    // Check number of children
    if (pn1->numberOfChildren != pn2->numberOfChildren){
        return 0;
    }
    
    // Compare children
    int i;
    for (i = 0; i < pn1->numberOfChildren; i++){
        if (!deepCompareNodes(pn1->children[i], pn2->children[i])){
            return 0;
        }
    }
    
    // If all is well, return 1
    return 1;
}

// Compares pn1 to pn2 and return 1 if the nodes are equal, false otherwise.
// 2 nodes are considered equal if:
//  1. Their address is the same, or
//  2. Deep compare finds
int compareNodes(pNode pn1, pNode pn2){
    if (pn1 == pn2){
        return 1;
    }
    
    return deepCompareNodes(pn1, pn2);
}

int verifyVal(pNode2 tree, int best, pNode2 expectedNode, char *testName){
    pNode2 result = (pNode2)getBest(tree, getNodeChildren, getNodeVal, freeNode, copyNode, best);
    
    int passedTest = compareNodes(result, expectedNode);

    if (!passedTest){
        // printf("***Expected:\n");
        // printTree(expectedNode);
        // printf("***Received:\n");
        // printTree(tree);
    }
    
    // Dummy test to avoid 'testName not used' warning
    if (testName == NULL){
        // Do nothing
    }
    
    printf("%d\n", passedTest);
    
    return passedTest;
}


// TESTERS

//   10
//  7   13
pNode simpleTree(){
    pNode head = newNodeWithVal(10);
    pNode child1 = newNodeWithVal(7);
    pNode child2 = newNodeWithVal(13);
    
    addChildToNode(head, child1);
    addChildToNode(head, child2);
    
    return head;
}

pNode wideTree(){
    pNode2 head = newNodeWithVal(1);
    
    int i;
    for (i = 3; i < 13; i++){
        pNode2 child = newNodeWithVal(i);
        addChildToNode(head, child);
    }
    
    pNode2 grandChild = newNodeWithVal(17);
    addChildToNode(head->children[7], grandChild);
    
    return head;
}

pNode deepTree(){
    pNode head = newNodeWithVal(5);
    pNode child = head;
    
    // Add first deep branch
    int i;
    for (i = 100; i < 110; i++){
        pNode newChild = newNodeWithVal(i);
        addChildToNode(child, newChild);
        child = newChild;
    }
    
    // Add second deep branch
    child = head;
    for (i = 300; i < 310; i++){
        pNode newChild = newNodeWithVal(i);
        addChildToNode(child, newChild);
        child = newChild;
    }
    
    return head;
}

// A big tree where all the nodes have the value zero
pNode zeroTree(){
    pNode head = newNodeWithVal(0);
    int i, j;
    
    for (i = 0; i < 5; i ++){
        pNode child = newNodeWithVal(0);
        addChildToNode(head, child);
        for (j = 0; j < 3; j++){
            pNode grandChild = newNodeWithVal(0);
            addChildToNode(child, grandChild);
        }
    }
    
    return head;
}

// Tree with height 1
int simpleTest(){
    pNode2 tree = simpleTree();
    return verifyVal(tree, 13, tree->children[1], "simpleTest");
}

// Find the value in a wide tree
int wideTreeTest(){
    pNode2 tree = wideTree();
    return verifyVal(tree, 17, tree->children[7]->children[0], "wideTreeTest");
}

// Find the value in a deep tree
int deepTreeTest(){
    pNode2 tree = deepTree();
    
    pNode2 cur = tree->children[1];  // The current search node

    while (getNodeVal(cur) != 307 && cur != NULL){
        cur = cur->children[0];
    }
    return verifyVal(tree, 307, cur, "deepTreeTest");
}

// A tree where the best value appears multiple times.
// Should return the left-most, top-most value
int multipleBestTest(){
    pNode2 tree = simpleTree();
    pNode2 tree2 = copyNode(tree);
    
    addChildToNode(tree->children[0], tree2);
    
    return verifyVal(tree, 13, tree->children[1], "multipleBestTest");
}

// A tree where the best value is in the head
int headHasBestTest(){
    pNode2 tree = simpleTree();
    tree->value = 101;
    
    return verifyVal(tree, 101, tree, "headHasBestTest");
}


// A search with UINT_MAX as the best
int noBestTest(){
    pNode2 tree = simpleTree();
    return verifyVal(tree, UINT_MAX, tree->children[1], "noBestTest");
}

// A search where the best value cannot be found
// Should return the highest value in the tree
int bestNotFoundTest(){
    pNode2 tree = simpleTree();
    return verifyVal(tree, 5, tree->children[1], "bestNotFoundTest");
}

// A tree where all the nodes have the value 0.
// Should return NULL
int zeroTreeTest(){
    pNode2 tree = zeroTree();
    
    return verifyVal(tree, 5, NULL, "zeroTreeTest");
}

// A tree with no children
int noChildrenTreeTest(){
    pNode tree = newNodeWithVal(5);
    return verifyVal(tree, 5, tree, "noChildrenTree");
}

/** END OF TESTING CODE **/

void doTests(){
   simpleTest();
   wideTreeTest();
   deepTreeTest();
   multipleBestTest();
   headHasBestTest();
   noBestTest();
   bestNotFoundTest();
   zeroTreeTest();
   noChildrenTreeTest();
}

void runTest(int testNum){
    switch (testNum) {
        case 0:
            simpleTest();
            break;
        case 1:
            wideTreeTest();
            break;
        case 2:
            deepTreeTest();
            break;
        case 3:
            multipleBestTest();
            break;
        case 4:
            headHasBestTest();
            break;
        case 5:
            noBestTest();
            break;
        // case 6:
        //     bestNotFoundTest();
        //     break;
        case 6:
            zeroTreeTest();
            break;
        case 7:
            noChildrenTreeTest();
            break;
        default:
            break;
    }
}


int main(int argc, char *argv[])
{
    if (argc > 1){
        runTest(atoi(argv[1]));
    } else {
        doTests();
    }

    
    return 1;
    pNode pn, pn2, pn3, pn4, pn5;
    pNode *ppn;
    pn = newNodeWithVal(10);
    pn2 = newNodeWithVal(7);
    pn3 = newNodeWithVal(3);
    
    addChildToNode(pn, pn2);
    addChildToNode(pn, pn3);
    

    
    pn4 = copyNode(pn);
    // printf("pn4->value %d, height - %d\n", getNodeVal(pn4), heightOfTree(pn4));
    addChildToNode(pn3, pn4);
    
    printTree(pn);
    
    // printf("pn->value %d, height - %d\n", getNodeVal(pn), heightOfTree(pn));
    // printf("pn2->value %d, height - %d\n", getNodeVal(pn2), heightOfTree(pn2));
    
    printf("Got %d children\n", getNodeChildren(pn, &ppn));
//    printTree(ppn[1]);
    pn5 = copyNode(pn);
    
    printf("Comparing pn and pn - %d\n", compareNodes(pn, pn));
    printf("Comparing pn5 and pn - %d\n", compareNodes(pn5, pn));
    
//    freeNode(ppn[0]);
//    freeNode(ppn[1]);
    
    return 0;
}
