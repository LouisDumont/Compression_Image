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
    QuadTree<int>::protect_leaves_from_destruction=false;


    //Loading and displaying the image
    Image<byte> I1;
    const char* fic1 = srcPath("lena_r.png");
    //const char* fic1 = srcPath("lena_r.png");
    if(! load(I1, fic1)) {
        cout << "Probleme dans le chargement d'images" << endl;
        return 1;
    }

    QuadTree<int>* test = imgToQTree(I1, false);
    display(test);

    Window W1=openWindow(I1.width(),I1.height());


    //afficheImgFromTree(test);
    Image<byte> decoded = qTreeToImg(test, false);
    display(decoded);
    cout<<nextPow2(256)<<endl;

    delete test;
    //delete whiteLeaf;
    //delete blackLeaf;


    endGraphics();
    return 0;
}