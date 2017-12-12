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

QuadLeaf<int>* whiteLeaf = new QuadLeaf<int>(255);
QuadLeaf<int>* blackLeaf = new QuadLeaf<int>(0);
int seuil = 30;

int nextPow2(int dim){
    int res = 1;
    while (res<dim){
        res = res*2;
    }
    return res;
}


//---------------------------------------------------------------------//
// Building a QuadTree<int> from a byte image
//---------------------------------------------------------------------//

// Recursive function that returns a pointer to the QuadTree representing a square of the image
// Is not aimed at being called by the user
// Used if protect_Leaves_From_Destruction is false and isBlackAndWhite true
QuadTree<int>* buildQTree
        (Image<byte> img, int xMin, int xMax, int yMin, int yMax){
    //Case of a pixel
    if (xMin==xMax && yMin==yMax) {
        int val;
            // Cheking if the pixel is in the original image
        if (xMin<img.height() && yMin<img.width()){
            val = img[xMin*img.width()+yMin];
        }
            // Otherwise a white value is returned
        else {val= 255;}
        return new QuadLeaf<int>(val);
    }

    //Case of a wider square
    int xMid = (xMin + xMax) / 2;
    int yMid = (yMin + yMax) / 2;
    //evaluating the sub-regions
    QuadTree<int>* sonNW = buildQTree(img, xMin, xMid, yMin, yMid);
    QuadTree<int>* sonNE = buildQTree(img, xMin, xMid, yMid+1, yMax);
    QuadTree<int>* sonSE = buildQTree(img, xMid+1, xMax, yMid+1, yMax);
    QuadTree<int>* sonSW = buildQTree(img, xMid+1, xMax, yMin, yMid);
    bool condFeuilles = ((sonNW->isLeaf() && sonNE->isLeaf()) && (sonSW->isLeaf() && sonSE->isLeaf()));
    if (condFeuilles){
        //If all the sons are leafs
        int Max = max(max(sonNW->value(),sonNE->value()),max(sonSE->value(), sonSW->value()));
        int Min = min(min(sonNW->value(),sonNE->value()),min(sonSE->value(), sonSW->value()));
        if (Max-Min<seuil) {
            // If the color of the sons is similar, they merge
            int val = (sonNW->value() + sonNE->value() + sonSE->value() + sonSW->value()) / 4;
            delete sonNW;
            delete sonNE;
            delete sonSW;
            delete sonSE;
            return new QuadLeaf<int>(val);
        }
    }
    //general case
    return new QuadNode<int>(sonNW, sonNE, sonSE, sonSW);
}

// Second function using shared leaves in the case of a black and white image
// Is not aimed at being called directly by the user
QuadTree<int>* buildQTreev2
        (Image<byte> img, int xMin, int xMax, int yMin, int yMax){
    //Case of a pixel
    if (xMin==xMax && yMin==yMax) {
        // Cheking if the pixel is in the original image
        if (xMin<img.height() && yMin<img.width()){
            if (int(img[xMin*img.width()+yMin])==0){
                return blackLeaf;
            }
            return whiteLeaf;
        }
            // Otherwise a white value is returned
        else {return whiteLeaf;}
    }

    //Case of a wider region
    int xMid = (xMin + xMax) / 2;
    int yMid = (yMin + yMax) / 2;
    //evaluating the sub-regions
    QuadTree<int>* sonNW = buildQTreev2(img, xMin, xMid, yMin, yMid);
    QuadTree<int>* sonNE = buildQTreev2(img, xMin, xMid, yMid+1, yMax);
    QuadTree<int>* sonSE = buildQTreev2(img, xMid+1, xMax, yMid+1, yMax);
    QuadTree<int>* sonSW = buildQTreev2(img, xMid+1, xMax, yMin, yMid);
    bool condNorth = ((sonNW->isLeaf() && sonNE->isLeaf()) && (sonNW->value()==sonNE->value()));
    bool condSouth = ((sonSW->isLeaf() && sonSE->isLeaf()) && (sonSW->value()==sonSE->value()));
    if ((condNorth && condSouth) && (sonNW->value()==sonSW->value())){
        //If all the sons are same-colored leafs
        //Fusion of the leafs
        int val = sonNE->value();
        if (val==0){
            return blackLeaf;
        }
        return whiteLeaf;
    }
    //general case
    return new QuadNode<int>(sonNW, sonNE, sonSE, sonSW);
}


// Fancy function for the user
// Return a pointer to the QuadTree representing an image
QuadTree<int>* imgToQTree(Image<byte> img, bool isBlackAndWhite) {
    int width = img.width();
    int height = img.height();
    // Adapting the size to deal with a virtualy square image
    int size = nextPow2(max(width,height));
    // Case of a black and white image with use of shared leaves
    if (QuadTree<int>::protect_leaves_from_destruction && isBlackAndWhite ){
             return buildQTreev2(img, 0, size-1, 0, size-1);
    }
    // General case
    return buildQTree(img, 0, size - 1, 0, size - 1);
}

//------------------------------------------------------------//
// Building back a byte Image from an int QuadTree
//------------------------------------------------------------//

// Fills the tab with the data of the square delimited by xMin, xMax, yMin, yMax
// Is not aimed at being called directly by the user
void fillTab
        (QuadTree<int>* tree, byte* tab, int xMin, int xMax, int yMin, int yMax, int size, bool isBlackAndWhite){

    // Case of a leaf
    if (tree->isLeaf()){
        if (isBlackAndWhite) {
            // Indicating by a grey square where the leaf is
            for (int j = yMin; j <= yMax - 1; j++) {
                tab[xMin * size + j] = byte(100);
                tab[xMax * size + j] = byte(100);
            }
            for (int i = xMin; i <= xMax; i++) {
                tab[i * size + yMin] = byte(100);
                tab[i * size + yMax] = byte(100);
            }
            // Filling the leaf by its color
            for (int i = xMin + 1; i < xMax; i++) {
                for (int j = yMin + 1; j < yMax; j++) {
                    tab[i * size + j] = byte(tree->value());
                }
            }
        }
            // In the general case we do not want to indicate the squares
        else {
            for (int i = xMin; i <= xMax; i++) {
                for (int j = yMin; j <= yMax; j++) {
                    tab[i * size + j] = byte(tree->value());
                }
            }
        }
        return;
    }

    // Case of a node
    int xMid = (xMin + xMax) / 2;
    int yMid = (yMin + yMax) / 2;
    fillTab(tree->son(0), tab, xMin, xMid, yMin, yMid, size, isBlackAndWhite);
    fillTab(tree->son(1), tab, xMin, xMid, yMid+1, yMax, size, isBlackAndWhite);
    fillTab(tree->son(2), tab, xMid+1, xMax, yMid+1, yMax, size, isBlackAndWhite);
    fillTab(tree->son(3), tab, xMid+1, xMax, yMin, yMid, size, isBlackAndWhite);
}

int getSize(QuadTree<int>* tree){
    if (tree->isLeaf()){return 1;}
    int s0 = getSize(tree->son(0));
    int s1 = getSize(tree->son(1));
    int s2 = getSize(tree->son(2));
    int s3 = getSize(tree->son(3));
    return max(max(2*s0,2*s1),max(2*s2,2*s3));
}

// General function returning an image from a QuadTree
// The size of the image is determined by the depth of the deepest nod
Image<byte> qTreeToImg(QuadTree<int>* tree, bool isBlackAndWhite){
    int size = getSize(tree);
    byte* tab = new byte[size*size];
    fillTab(tree,tab, 0, size-1, 0, size-1, size, isBlackAndWhite);
    Image<byte> res(tab,size,size, true);
    return res;
}

// General function to display an image from a QuadTree
void afficheImgFromTree(QuadTree<int>* tree, bool isBlackAndWhite) {
    int size = getSize(tree);
    byte *tab = new byte[size * size];
    fillTab(tree, tab, 0, size - 1, 0, size - 1, size, isBlackAndWhite);
    putGreyImage(IntPoint2(0, 0), tab, size, size);
}

// Returns the numbers of nodes and leaves of a tree to evaluate its size
int compt_MemorySize(QuadTree<int>* tree){
    if (tree->isLeaf()){return 1;}
    int size0 = compt_MemorySize(tree->son(0));
    int size1 = compt_MemorySize(tree->son(1));
    int size2 = compt_MemorySize(tree->son(2));
    int size3 = compt_MemorySize(tree->son(3));
    return (size0+size1+size2+size3+1);
}