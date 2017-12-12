/****************************************************************************
 * Image compression project for the Advanced Programmation course at ENPC
 * Author: Louis Dumont
 * Last Modified: 07/12/2017
 ***************************************************************************/

#include <iostream>
#include "quadtree.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include "img_qTree.h"

using namespace std;
using namespace Imagine;





int main() {
    //Protection the leafs, so that they can be used as global variables
    QuadTree<int>::protect_leaves_from_destruction=false;


    //Loading the image
    Image<byte> I1;
    const char* fic1 = srcPath("lena_r.png");
    //const char* fic1 = srcPath("lena_r.png");
    if(! load(I1, fic1)) {
        cout << "Probleme dans le chargement d'images" << endl;
        return 1;
    }
    Window W1=openWindow(I1.width(),I1.height());

    //Tests the construction of the QuadTree
    QuadTree<int>* test = imgToQTree(I1, false);
    display(test);

    //Tests the decoding of the QuadTree and the quality of the decoded image
    Image<byte> decoded = qTreeToImg(test, false);
    display(decoded);

    //Tests the efficiency of the compression
    int compressedSize = compt_MemorySize(test);
    int unCompressedSize = I1.height()*I1.width();
    cout<<"Compression ratio: "<<(float(compressedSize)/float(unCompressedSize))<<endl;

    delete test;
    //delete whiteLeaf;
    //delete blackLeaf;


    endGraphics();
    return 0;
}