/*************************************************************
 * Fuctions for the Image Compression project (ENPC)
 * Author: Louis Dumont
 * Last Modified: 07/12/2017
*************************************************************/
#pragma once

#include <iostream>
#include "quadtree.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>

int nextPow2(int dim);

QuadTree<int>* buildQTree
        (Imagine::Image<byte> img, int xMin, int xMax, int yMin, int yMax);

QuadTree<int>* buildQTreev2
        (Imagine::Image<byte> img, int xMin, int xMax, int yMin, int yMax);

QuadTree<int>* imgToQTree(Imagine::Image<byte> img);

void fillTab
        (QuadTree<int>* tree, byte* tab, int xMin, int xMax, int yMin, int yMax, int size, bool isBlackAndWhite);

int getSize(QuadTree<int>* tree);

Imagine::Image<byte> qTreeToImg(QuadTree<int>* tree, bool isBlackAndWhite = true);

void afficheImgFromTree(QuadTree<int>* tree, bool isBlackAndWhite = true);