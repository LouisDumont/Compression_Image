/****************************************************************************
 * Image compression project for the Advanced Programmation course at ENPC
 * Author: Louis Dumont
 * Last Modified: 04/12/2017
 ***************************************************************************/

#include <iostream>
#include "quadtree.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>

using namespace std;
using namespace Imagine;

QuadTree<int>* buildQTree(Image<byte> img, int xMin, int xMax, int yMin, int yMax){
    if (xMin==xMax && yMin==yMax) {
        return new QuadLeaf<int>(int(img[xMin*img.width()+yMin]));
    }
    int xMid = (xMin + xMax) / 2;
    int yMid = (yMin + yMax) / 2;
    QuadTree<int>* sonNW = buildQTree(img, xMin, xMid, yMin, yMid);
    QuadTree<int>* sonNE = buildQTree(img, xMin, xMid, yMid+1, yMax);
    QuadTree<int>* sonSE = buildQTree(img, xMid+1, xMax, yMid+1, yMax);
    QuadTree<int>* sonSW = buildQTree(img, xMid+1, xMax, yMin, yMid);
    bool condNorth = ((sonNW->isLeaf() && sonNE->isLeaf()) && (sonNW->value()==sonNE->value()));
    bool condSouth = ((sonSW->isLeaf() && sonSE->isLeaf()) && (sonSW->value()==sonSE->value()));
    if ((condNorth && condSouth) && (sonNW->value()==sonSW->value())){
        int val = sonNE->value();
        delete sonNW;
        delete sonNE;
        delete sonSW;
        delete sonSE;
        return new QuadLeaf<int>(val);
    }
    return new QuadNode<int>(sonNW, sonNE, sonSE, sonSW);
}

QuadTree<int>* imgToQTree(Image<byte> img){
    int width  = img.width();
    int height = img.height();
    return buildQTree(img, 0 ,width-1, 0, height-1);
}

void fillTab(QuadTree<int>* tree, byte* tab, int xMin, int xMax, int yMin, int yMax, int size){
    if (tree->isLeaf()){
        for (int i=xMin; i<=xMax; i++){
            for (int j =yMin; j<=yMax; j++){
                tab[i*size+j]=byte(tree->value());
            }
        }
        return;
    }
    int xMid = (xMin + xMax) / 2;
    int yMid = (yMin + yMax) / 2;
    fillTab(tree->son(0), tab, xMin, xMid, yMin, yMid, size);
    fillTab(tree->son(1), tab, xMin, xMid, yMid+1, yMax, size);
    fillTab(tree->son(2), tab, xMid+1, xMax, yMid+1, yMax, size);
    fillTab(tree->son(3), tab, xMid+1, xMax, yMin, yMid, size);
}

int getSize(QuadTree<int>* tree){
    if (tree->isLeaf()){return 1;}
    int s0 = getSize(tree->son(0));
    int s1 = getSize(tree->son(1));
    int s2 = getSize(tree->son(2));
    int s3 = getSize(tree->son(3));
    return max(max(2*s0,2*s1),max(2*s2,2*s3));
}

// NE FONCTIONNE PAS CAR LE CONSTRUCTEUR DE IMAGINE++ NE MARCHE PAS COMME PREVU
Image<byte> qTreeToImg(QuadTree<int>* tree){
    int size = getSize(tree);
    byte* tab = new byte[size*size];

    fillTab(tree,tab, 0, size-1, 0, size-1, size);
    Image<byte> res(tab,size,size);
    delete [] tab;
    return res;
}

void afficheImgFromTree(QuadTree<int>* tree){
    int size = getSize(tree);
    byte* tab = new byte[size*size];
    fillTab(tree, tab, 0, size-1, 0, size-1, size);
    putGreyImage(IntPoint2(0,0),tab,size,size);
}



int main() {

    //Loading and displaying the image
    Image<byte> I1;
    const char* fic1 = srcPath("running-horse-square.png");
    if(! load(I1, fic1)) {
        cout << "Probleme dans le chargement d'images" << endl;
        return 1;
    }
    cout<<"pixel blanc "<<int(I1[0,0])<<endl;
    for (int i=0;i<I1.width();i++){
        for (int j=0; j<I1.width(); j++){
            int val=int(I1[i*I1.width()+j]);
            if (val != 255){cout<<i<<" "<<j<<" "<<val<<endl;}
        }
    }
    cout<<"pixel noir "<<int(I1[230,180])<<endl;
    QuadTree<int>* test = imgToQTree(I1);
    //Image<byte> test2=qTreeToImg(test);
    display(test);
    cout<<getSize(test)<<endl;

    Window W1=openWindow(I1.height(),I1.width());
    //display(test2);
    int size=I1.width();
    byte* tab = new byte[size*size];
    //for(int i=0; i<size;i++){
    //    for(int j=0; j<size;j++){
    //        tab[i*size+j]=byte(0);
    //    }
    //}
    fillTab(test, tab, 0, size-1, 0, size-1, size);
    //putGreyImage(IntPoint2(0,0),tab,512,512);
    afficheImgFromTree(test);
    //int width, height;
    //byte* image;
    //loadGreyImage(fic1, image, width,height);
    //putGreyImage(IntPoint2(0,0),image,width,height);
    //delete image;
    delete [] tab;
    //display(res);
    delete test;


    endGraphics();
    return 0;
}