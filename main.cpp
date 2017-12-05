/****************************************************************************
 * Image compression project for the Advanced Programmation course at ENPC
 * Author: Louis Dumont
 * Last Modified: 04/12/2017
 ***************************************************************************/

#include <iostream>
#include "quadtree.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include "image_quadtrees.h"

using namespace std;
using namespace Imagine;



int main() {

    //Loading and displaying the image
    Image<byte> I1;
    const char* fic1 = srcPath("running-horse-square.png");
    if(! load(I1, fic1)) {
        cout << "Probleme dans le chargement d'images" << endl;
        return 1;
    }
    I1[256,256]=0;
    Window W1 = openWindow(I1.width(), I1.height());
    //display(I1);

    // Tests on the functions introduced
    QuadTree<byte>* test = imgToQTree(I1);
    //cout<<"taille "<<getSize(test)<<endl;
    //Image<byte> I2 = qTreeToImg(test);
    //cout<<I2.height()<<endl;
    //cout<<I2.width()<<endl;
    display(qTreeToImg(test));
    delete test;

    byte* auxtab = new byte[512*512];
    for (int i=0; i<512; i++){
        for (int j=0; j<512; j++) {
            auxtab[i*512+j] = 255;
        }
        auxtab[i*512+10]=0;
    }
    //Image<byte> I3(auxtab,512,512);
    delete [] auxtab;
    //display(I3);

    QuadNode<byte>* newTest1 = new QuadNode<byte>(new QuadLeaf<byte>(I1[0,0]),new QuadLeaf<byte>(I1[0,0]), new QuadLeaf<byte>(I1[256,256]), new QuadLeaf<byte>(I1[256,256]));
    QuadNode<byte>* newTest1b = new QuadNode<byte>(new QuadLeaf<byte>(I1[0,0]),new QuadLeaf<byte>(I1[0,0]), new QuadLeaf<byte>(I1[256,256]), new QuadLeaf<byte>(I1[256,256]));
    QuadNode<byte>* newTest2 = new QuadNode<byte>(new QuadLeaf<byte>(I1[256,256]),new QuadLeaf<byte>(I1[0,0]), new QuadLeaf<byte>(I1[0,0]), new QuadLeaf<byte>(I1[256,256]));
    QuadNode<byte>* newTest2b = new QuadNode<byte>(new QuadLeaf<byte>(I1[256,256]),new QuadLeaf<byte>(I1[0,0]), new QuadLeaf<byte>(I1[0,0]), new QuadLeaf<byte>(I1[256,256]));
    QuadNode<byte>* newTestT = new QuadNode<byte>(newTest1, newTest1b, newTest2, newTest2b);
    QuadNode<byte>* newTestF = new QuadNode<byte>(newTestT, new QuadLeaf<byte>(I1[0,0]), new QuadLeaf<byte>(I1[256,256]), new QuadLeaf<byte>(I1[0,0]));
    //display(qTreeToImg(newTestF));
    Image<byte> Itest=qTreeToImg(newTestF);
    delete newTestF;
    //Window W1 = openWindow(Itest.height(),Itest.width());
    //display(Itest);
    for (int i=0; i<Itest.height(); i++){
        for (int j=0; j<Itest.width(); j++){
            //cout<<i<<"  "<<j<<endl;
            cout<<int(Itest[i,j])<<endl;
        }
     }
    //QuadTree<byte>* resTest= imgToQTree(Itest);
    cout<<int(I1[256,256])<<endl;
    //display(qTreeToImg(resTest));
    //delete resTest;


    endGraphics();
    return 0;
}