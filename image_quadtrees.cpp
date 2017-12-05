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
    QuadTree<byte>* res = QTreeBuilding(Img, 0,height-1, 0, width-1);
    return res;
}

// Recursive function building the QuadTree of a region of the image
QuadTree<byte>* QTreeBuilding
        (Image<byte> Img, int xMin, int xMax, int yMin, int yMax){

    //Case of a single pixel
    if ((xMin==xMax) && (yMin==yMax)) {
        byte val = Img[xMin,yMin];
        //cout<<xMin<<"  "<<yMin<<endl;
        //cout<<int(val)<<endl;
        return new QuadLeaf<byte>(val);
    }
    //cout<<prof<<endl;

    //Case of a wider region
    int xMid = (xMin + xMax) / 2;
    int yMid = (yMin + yMax) / 2;
    //Calculating the sub_trees
    QuadTree<byte>* sonNW =  QTreeBuilding(Img, xMin, xMid, yMin, yMid);
    QuadTree<byte>* sonNE =  QTreeBuilding(Img, xMin, xMid, yMid+1, yMax);
    QuadTree<byte>* sonSW =  QTreeBuilding(Img, xMid+1, xMax, yMin, yMid);
    QuadTree<byte>* sonSE =  QTreeBuilding(Img, xMid+1, xMax, yMid+1, yMax);
    //bool condNorth = ((sonNW->isLeaf() && sonNE->isLeaf()) && (sonNW->value() == sonNE->value()));
    //bool condSouth = ((sonSW->isLeaf() && sonSE->isLeaf()) && (sonSW->value() == sonSE->value()));
    //if ((condNorth && condSouth) && (sonNE->value() == sonSE->value())) {
    //    byte value = sonNW->value();
    //    delete sonNW;
    //    delete sonNE;
    //    delete sonSE;
    //    delete sonSW;
    //    cout<<"fusion"<<endl;
    //    return new QuadLeaf<byte>(value);
    //}
    //cout<<"node"<<endl;
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
    byte* tab = new byte[size*size];
    tabFilling(tree, tab, 0, size-1, 0, size-1, size);
    Image<byte> Ires(tab,size,size);
    delete [] tab;
    //display(I1);
    return Ires;
}

void tabFilling(QuadTree<byte>* tree, byte* tab, int xMin, int xMax, int yMin, int yMax, int size){

    if (tree->isLeaf()){
        byte val = tree->value();
        for (int i=xMin; i<=xMax; i++){
            for (int j=yMin; j<=yMax; j++){
                tab[i*size+j]=val;
            }
        }
        return;
    }
    QuadTree<byte>* sonNW = tree->son(0);
    QuadTree<byte>* sonNE = tree->son(1);
    QuadTree<byte>* sonSE = tree->son(2);
    QuadTree<byte>* sonSW = tree->son(3);
    int xMid = (xMin + xMax) / 2;
    int yMid = (yMin + yMax) / 2;
    tabFilling(sonNW, tab, xMin, xMid, yMin, yMid, size);
    tabFilling(sonNE, tab, xMin, xMid, yMid+1, yMax, size);
    tabFilling(sonSW, tab, xMid+1, xMax, yMin, yMid, size);
    tabFilling(sonSE, tab, xMid+1, xMax, yMid+1, yMax, size);
}