/***************************************************************************
 * Cpp file containing the functions of the Image Compression project
 * Author: Louis Dumont
 * Last modified: 05/12/2017
***************************************************************************/

#include <iostream>
#include "quadtree.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include "image_quadtrees.h"

using namespace std;
using namespace Imagine;

/*------------------------------------------------------------*
 * Compressing an image into a QuadTree
 *------------------------------------------------------------*/

// Fancy function that will be called in the main
QuadTree<byte>* imgToQTree(Image<byte> Img){
    //Getting the Image dimensions
    int height = Img.height();
    int width  = Img.width();
    //Calling the recursive function on the right region
    return QTreeBuilding(Img, 0, height-1, 0, width-1, 1);
}

// Recursive function building the QuadTree of a region of the image
QuadTree<byte>* QTreeBuilding
        (Image<byte> Img, int xMin, int xMax, int yMin, int yMax, int prof){

    //Case of a single pixel
    if ((xMin==xMax) && (yMin==yMax)) {
        //cout<<"pixel"<<endl;
        return new QuadLeaf<byte>(Img[xMin, yMin]);
    }
    //cout<<prof<<endl;

    //Case of a wider region
    int xMid = (xMin + xMax) / 2;
    int yMid = (yMin + yMax) / 2;
    //Calculating the sub_trees
    QuadTree<byte>* sonNW =  QTreeBuilding(Img, xMin, xMid, yMin, yMid, prof+1);
    QuadTree<byte>* sonNE =  QTreeBuilding(Img, xMin, xMid, yMid+1, yMax, prof+1);
    QuadTree<byte>* sonSW =  QTreeBuilding(Img, xMid+1, xMax, yMin, yMid, prof+1);
    QuadTree<byte>* sonSE =  QTreeBuilding(Img, xMid+1, xMax, yMid+1, yMax, prof+1);
    bool condNorth = ((sonNW->isLeaf() && sonNE->isLeaf()) && (sonNW->value() == sonNE->value()));
    bool condSouth = ((sonSW->isLeaf() && sonSE->isLeaf()) && (sonSW->value() == sonSE->value()));
    if ((condNorth && condSouth) && (sonNE->value() == sonSE->value())) {
        byte value = sonNW->value();
        delete sonNW;
        delete sonNE;
        delete sonSE;
        delete sonSW;
        //cout<<"fusion"<<endl;
        return new QuadLeaf<byte>(value);
    }
    return new QuadNode<byte>(sonNW, sonNE, sonSE, sonSW);
}

/*--------------------------------------------------------------------------------------------------*
 * Getting back an image from a QuadTree
 *--------------------------------------------------------------------------------------------------*/

//Returnsthe size of the image to create, assimilated to 2^^ the depth of the QuadTree
int getSize(QuadTree<byte>* tree){
    if(tree->isLeaf()){return 1;}
    int x0 = getSize(tree->son(0));
    int x1 = getSize(tree->son(1));
    int x2 = getSize(tree->son(2));
    int x3 = getSize(tree->son(3));
    return max(max(2*x0,2*x1),max(2*x2,2*x3));
}

Image<byte> qTreeToImg(QuadTree<byte>* tree){
    int size = getSize(tree);
    byte* tab = new byte [size*size];
    tabFilling(tree, tab, size);
    Image<byte> I1(tab,512);
    delete [] tab;
    return I1;
}

void tabFilling(QuadTree<byte>* tree, byte* tab, int size){
    for(int i=0; i<size; i++){
        tab[i]=1;
    }
}