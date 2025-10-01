#include <iostream>

class Node {
private:
    int data;
    Node* link;
public:
    Node() : data(0), link(nullptr) {}
    Node(int d) : data(d), link(nullptr) {}
    
    int getData() {
        return data;
    }
    
    void setData(int d) {
        data = d;
    }
    
    Node* getLink() {
        return link;
    }
    
    void setLink(Node* l) {
        link = l;
    }
};
