/******************************************************************
 * Header file for the Image Compression projet
 * Author: Louis Dumont
 * Last Modified: 05/12/2017
******************************************************************/

#pragma once

#include <iostream>
#include "quadtree.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>

QuadTree<byte>* imgToQTree(Imagine::Image<byte> Img);

QuadTree<byte>* QTreeBuilding
        (Imagine::Image<byte> Img, int xMin, int xMax, int yMin, int yMax, int prof);

int getSize(QuadTree<byte>* tree);

Imagine::Image<byte> qTreeToImg(QuadTree<byte>* tree);

void tabFilling(QuadTree<byte>* tree, byte* tab, int size);
