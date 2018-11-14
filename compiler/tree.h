#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
using namespace std;
struct node{
    string element;
    vector<node*> children;
    node(string key){
        element = key;
    }
};
class multitree{
private:
    node* mroot;
    node* pos_pointor;
    int getHeight();
    void destroy(node* node);
public:
    multitree();
    void addNode(string& key);
};

#endif // TREE_H_INCLUDED
