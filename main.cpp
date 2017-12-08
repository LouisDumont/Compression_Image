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
    //Protecting the leafs, that will be used as global variables
    QuadTree<int>::protect_leaves_from_destruction=true;


    //Loading and displaying the image
    Image<byte> I1;
    const char* fic1 = srcPath("running-horse-square.png");
    if(! load(I1, fic1)) {
        cout << "Probleme dans le chargement d'images" << endl;
        return 1;
    }

    QuadTree<int>* test = imgToQTree(I1);
    display(test);

    Window W1=openWindow(I1.height(),I1.width());


    //afficheImgFromTree(test);
    Image<byte> decoded = qTreeToImg(test);
    display(decoded);

    delete test;
    //delete whiteLeaf;
    //delete blackLeaf;


    endGraphics();
    return 0;
}