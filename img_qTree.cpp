/*************************************************************
 * Fuctions for the Image Compression project (ENPC)
 * Author: Louis Dumont
 * Last Modified: 07/12/2017
*************************************************************/

#include <iostream>
#include "quadtree.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include "img_qTree.h"

using namespace std;
using namespace Imagine;

//---------------------------------------------------------------------//
// Building a QuadTree<int> from a byte image
//---------------------------------------------------------------------//

// Recursive function that returns a pointer to the QuadTree representing a sub-image
// Is not aimed at being called by the user
QuadTree<int>* buildQTree
        (Image<byte> img, int xMin, int xMax, int yMin, int yMax){
    //Case of a pixel
    if (xMin==xMax && yMin==yMax) {
        return new QuadLeaf<int>(int(img[xMin*img.width()+yMin]));
    }

    //Case of a wider region
    int xMid = (xMin + xMax) / 2;
    int yMid = (yMin + yMax) / 2;
    //evaluating the sub-regions
    QuadTree<int>* sonNW = buildQTree(img, xMin, xMid, yMin, yMid);
    QuadTree<int>* sonNE = buildQTree(img, xMin, xMid, yMid+1, yMax);
    QuadTree<int>* sonSE = buildQTree(img, xMid+1, xMax, yMid+1, yMax);
    QuadTree<int>* sonSW = buildQTree(img, xMid+1, xMax, yMin, yMid);
    bool condNorth = ((sonNW->isLeaf() && sonNE->isLeaf()) && (sonNW->value()==sonNE->value()));
    bool condSouth = ((sonSW->isLeaf() && sonSE->isLeaf()) && (sonSW->value()==sonSE->value()));
    if ((condNorth && condSouth) && (sonNW->value()==sonSW->value())){
        //If all the sons are same-colored leafs
        //Fusion of the leafs
        int val = sonNE->value();
        delete sonNW;
        delete sonNE;
        delete sonSW;
        delete sonSE;
        return new QuadLeaf<int>(val);
    }
    //general case
    return new QuadNode<int>(sonNW, sonNE, sonSE, sonSW);
}


// Fancy function for the user
// Return a pointer to the QuadTree representing an image
QuadTree<int>* imgToQTree(Image<byte> img) {
    int width = img.width();
    int height = img.height();
    return buildQTree(img, 0, width - 1, 0, height - 1);
}

//------------------------------------------------------------//
// Building back a byte Image from an int QuadTree
//------------------------------------------------------------//

void fillTab
        (QuadTree<int>* tree, byte* tab, int xMin, int xMax, int yMin, int yMax, int size){
    if (tree->isLeaf()){
        for (int i=xMin; i<=xMax; i++){
            for (int j =yMin; j<=yMax; j++){
                tab[i*size+j]=byte(tree->value());
            }
        }
        return;
    }
    int xMid = (xMin + xMax) / 2;
    int yMid = (yMin + yMax) / 2;
    fillTab(tree->son(0), tab, xMin, xMid, yMin, yMid, size);
    fillTab(tree->son(1), tab, xMin, xMid, yMid+1, yMax, size);
    fillTab(tree->son(2), tab, xMid+1, xMax, yMid+1, yMax, size);
    fillTab(tree->son(3), tab, xMid+1, xMax, yMin, yMid, size);
}

int getSize(QuadTree<int>* tree){
    if (tree->isLeaf()){return 1;}
    int s0 = getSize(tree->son(0));
    int s1 = getSize(tree->son(1));
    int s2 = getSize(tree->son(2));
    int s3 = getSize(tree->son(3));
    return max(max(2*s0,2*s1),max(2*s2,2*s3));
}


Image<byte> qTreeToImg(QuadTree<int>* tree){
    int size = getSize(tree);
    byte* tab = new byte[size*size];
    fillTab(tree,tab, 0, size-1, 0, size-1, size);
    Image<byte> res(tab,size,size, true);
    return res;
}

void afficheImgFromTree(QuadTree<int>* tree){
    int size = getSize(tree);
    byte* tab = new byte[size*size];
    fillTab(tree, tab, 0, size-1, 0, size-1, size);
    putGreyImage(IntPoint2(0,0),tab,size,size);
}