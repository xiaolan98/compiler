#include"tree.h"
using namespace std;
multitree::multitree(){
    mroot = NULL;
    pos_pointor = mroot;
}
void multitree::addNode(string& key){
    if(mroot==NULL){
        mroot = new node(key);
        pos_pointor = mroot;
    }
    else{
        node* childNode = new node(key);
        pos_pointor->children.push_back(childNode);
        pos_pointor = childNode;
    }
}
