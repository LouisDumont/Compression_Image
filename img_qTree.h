/*************************************************************
 * Fuctions for the Image Compression project (ENPC)
 * Author: Louis Dumont
 * Last Modified: 07/12/2017
*************************************************************/

#include <iostream>
#include "quadtree.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>

QuadTree<int>* buildQTree
        (Imagine::Image<byte> img, int xMin, int xMax, int yMin, int yMax);

QuadTree<int>* imgToQTree(Imagine::Image<byte> img);

void fillTab
        (QuadTree<int>* tree, byte* tab, int xMin, int xMax, int yMin, int yMax, int size);

int getSize(QuadTree<int>* tree);

Imagine::Image<byte> qTreeToImg(QuadTree<int>* tree);

void afficheImgFromTree(QuadTree<int>* tree);