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

QuadTree<byte>* imgToQTree(Image<byte> Img){
    //Getting the Image dimensions
    int height = Img.height();
    int width  = Img.width();
    //Calling the recursive function on the right region
    return QTreeBuilding(Img, 0, height-1, 0, width-1, 1);
}

QuadTree<byte>* QTreeBuilding
        (Image<byte> Img, int xMin, int xMax, int yMin, int yMax, int prof){

    //Case of a single pixel
    if ((xMin==xMax) && (yMin==yMax)) {
        cout<<"pixel"<<endl;
        return new QuadLeaf<byte>(Img[xMin, yMin]);
    }
    cout<<prof<<endl;
    //Case of a wider region
    int xMid = (xMin + xMax) / 2;
    int yMid = (yMin + yMax) / 2;
    QuadTree<byte>* sonNW =  QTreeBuilding(Img, xMin, xMid, yMin, yMid, prof+1);
    QuadTree<byte>* sonNE =  QTreeBuilding(Img, xMin, xMid, yMid+1, yMax, prof+1);
    QuadTree<byte>* sonSW =  QTreeBuilding(Img, xMid+1, xMax, yMin, yMid, prof+1);
    QuadTree<byte>* sonSE =  QTreeBuilding(Img, xMid+1, xMax, yMid+1, yMax, prof+1);
    return new QuadNode<byte>(sonNW, sonNE, sonSE, sonSW);
}