#include<bits/stdc++.h>
using namespace std;
#ifndef REDBLACK_H_INCLUDED
#define REDBLACK_H_INCLUDED

class Node{
public:
    int key;
    Node *left,*right,*parent;
    int color;
    Node(){
        parent = NULL;
    }
    Node(int d){
        key = d;
        left = right = parent = NULL;
    }
    bool isOnLeft(){
        return this == parent->left;
    }
    Node *sibling() {
        if (parent == NULL)
            return NULL;
        if (isOnLeft())
            return parent->right;
        return parent->left;
    }
    Node *uncle() {
        if (parent == NULL or parent->parent == NULL)
            return NULL;
        if (parent->isOnLeft())
            return parent->parent->right;
        else
            return parent->parent->left;
    }
    Node* GetUncle(Node* n){
        Node* p = GetParent(n);
        return GetSibling(p);
    }
    Node* GetParent(Node* n) {
        if(n==NULL){
            return NULL;
        }
        else{
            return n->parent;
        }
    }
    Node* GetGrandParent(Node* n){
        return GetParent(GetParent(n));
    }
    Node* GetSibling(Node* n) {
        Node* p = GetParent(n);
        if(p==NULL){
            return NULL;
        }
        if(n == p->left){
            return p->right;
        }
        else{
            return p->left;
        }
    }
    void moveDown(Node *nParent) {
        if (parent != NULL) {
            if (isOnLeft()) {
                parent->left = nParent;
            }
            else {
                parent->right = nParent;
            }
        }
        nParent->parent = parent;
        parent = nParent;
    }
    bool hasRedChild() {
        return (left != NULL and left->color == 1) or
           (right != NULL and right->color == 1);
    }
};

class RedBlack : public Node{
public:
    Node *root;
    RedBlack(){
        root = NULL;
    }
    void leftRotate(Node *x) {
        Node *nParent = x->right;
        if (x == root)
            root = nParent;
        x->moveDown(nParent);
        x->right = nParent->left;
        if (nParent->left != NULL)
            nParent->left->parent = x;
        nParent->left = x;
    }
    void rightRotate(Node *x) {
        Node *nParent = x->left;
        if (x == root)
            root = nParent;
        x->moveDown(nParent);
        x->left = nParent->right;
        if (nParent->right != NULL)
            nParent->right->parent = x;
        nParent->right = x;
    }
    void swapColors(Node *x1, Node *x2) {
        int temp;
        temp = x1->color;
        x1->color = x2->color;
        x2->color = temp;
    }
    void swapValues(Node *u, Node *v) {
        int temp;
        temp = u->key;
        u->key = v->key;
        v->key = temp;
    }
    void fixRedRed(Node *x) {
        if (x == root) {
            x->color = 0;
            return;
        }
        Node *parent = x->parent, *grandparent = parent->parent,
         *uncle = x->uncle();
        if (parent->color != 0) {
            if (uncle != NULL && uncle->color == 1) {
                parent->color = 0;
                uncle->color = 0;
                grandparent->color = 1;
                fixRedRed(grandparent);
            }
            else {
                if (parent->isOnLeft()) {
                    if (x->isOnLeft()) {
                        swapColors(parent, grandparent);
                    }
                    else {
                        leftRotate(parent);
                        swapColors(x, grandparent);
                    }
                    rightRotate(grandparent);
                }
                else {
                    if (x->isOnLeft()) {
                        rightRotate(parent);
                        swapColors(x, grandparent);
                    }
                    else {
                        swapColors(parent, grandparent);
                    }
                    leftRotate(grandparent);
                }
            }
        }
    }
    Node *successor(Node *x) {
        Node *temp = x;
        while (temp->left != NULL)
            temp = temp->left;
        return temp;
    }
    Node *BSTreplace(Node *x) {
        if (x->left != NULL and x->right != NULL)
            return successor(x->right);
        if (x->left == NULL and x->right == NULL)
            return NULL;
        if (x->left != NULL)
            return x->left;
        else
            return x->right;
    }
    void deleteNode(Node *v) {
        Node *u = BSTreplace(v);
        Node *parent = v->parent;
        if (u == NULL) {
            if (v == root) {
                root == NULL;
            }
        else {
            if ((u == NULL || u->color == 0) && (v->color == 0)) {
                fixDoubleBlack(v);
            }
            else {
                if (v->sibling() != NULL)
                     v->sibling()->color = 1;
            }
            if (v->isOnLeft()) {
                parent->left = NULL;
            }
            else {
                parent->right = NULL;
            }
        }
        delete v;
        return;
        }
        if (v->left == NULL || v->right == NULL) {
            if (v == root) {
                v->key = u->key;
                v->left = v->right = NULL;
                delete u;
            }
            else {
                if (v->isOnLeft()) {
                    parent->left = u;
                }
                else {
                    parent->right = u;
                }
                delete v;
                u->parent = parent;
                if ((u == NULL || u->color == 0) && (v->color == 0)) {
                    fixDoubleBlack(u);
                }
                else {
                    u->color = 0;
                }
            }
            return;
        }
        swapValues(u, v);
        deleteNode(u);
    }
    void fixDoubleBlack(Node *x) {
        if (x == root)
            return;
        Node *sibling = x->sibling(), *parent = x->parent;
        if (sibling == NULL) {
            fixDoubleBlack(parent);
        }
        else {
            if (sibling->color == 1) {
                parent->color = 1;
                sibling->color = 0;
                if (sibling->isOnLeft()) {
                    rightRotate(parent);
                }
                else {
                    leftRotate(parent);
                }
                fixDoubleBlack(x);
            }
            else {
                if (sibling->hasRedChild()) {
                    if (sibling->left != NULL and sibling->left->color == 1) {
                        if (sibling->isOnLeft()) {
                            sibling->left->color = sibling->color;
                            sibling->color = parent->color;
                            rightRotate(parent);
                        }
                        else {
                            sibling->left->color = parent->color;
                            rightRotate(sibling);
                            leftRotate(parent);
                        }
                    }
                    else {
                        if (sibling->isOnLeft()) {
                            sibling->right->color = parent->color;
                            leftRotate(sibling);
                            rightRotate(parent);
                        }
                        else {
                            sibling->right->color = sibling->color;
                            sibling->color = parent->color;
                            leftRotate(parent);
                        }
                    }
                    parent->color = 0;
                }
                else {
                    sibling->color = 1;
                    if (parent->color == 0)
                        fixDoubleBlack(parent);
                    else
                        parent->color = 0;
                }
            }
        }
    }
    Node *getRoot() { return root; }

    void RotateLeft(Node* n){
        Node* new1 = n->right;
        Node* p = GetParent(n);
        assert(new1 != NULL);
        n->right = new1->left;
        new1->left = n;
        n->parent = new1;
        if (n->right != NULL) {
            n->right->parent = n;
        }
        if (p != NULL){
            if (n == p->left){
                p->left = new1;
            }
            else if (n == p->right){
                p->right = new1;
            }
        }
        new1->parent = p;
    }
    void RotateRight(Node* n){
        Node* new1 = n->left;
        Node* p = GetParent(n);
        assert(new1 != NULL);
        n->left = new1->right;
        new1->right = n;
        n->parent = new1;
        if (n->left != NULL){
            n->left->parent = n;
        }
        if (p != NULL){
            if (n == p->left){
                p->left = new1;
            }
            else if (n == p->right){
                p->right = new1;
            }
        }
        new1->parent = p;
    }

    Node* Insert(Node* root, Node* n){
        InsertRecurse(root, n);
        InsertRepairTree(n);
        root = n;
        while (GetParent(root) != NULL){
            root = GetParent(root);
        }
        return root;
    }

    void InsertRecurse(Node* root, Node* n){
        if (root != NULL){
            if (n->key < root->key){
                if (root->left != NULL){
                    InsertRecurse(root->left, n);
                    return;
                }
                else{
                    root->left = n;
                }
            }
            else    {
                if (root->right != NULL){
                    InsertRecurse(root->right, n);
                    return;
                }
                else{
                    root->right = n;
                }
            }
        }
        n->parent = root;
        n->left = NULL;
        n->right = NULL;
        n->color = 1;
    }

    void InsertRepairTree(Node* n){
        Node *p = GetParent(n);
        if (p == NULL){
            InsertCase1(n);
        }
        else if (p->color == 0){
            InsertCase2(n);
        }
        else if (GetUncle(n) != NULL && GetUncle(n)->color == 1){
            InsertCase3(n);
        }
        else{
            InsertCase4(n);
        }
    }

    void InsertCase1(Node* n){
        n->color = 0;
    }
    void InsertCase2(Node* n){
        return;
    }
    void InsertCase3(Node* n){
        GetParent(n)->color = 0;
        GetUncle(n)->color = 0;
        GetGrandParent(n)->color = 1;
        InsertRepairTree(GetGrandParent(n));
    }
    void InsertCase4(Node* n){
        Node* p = GetParent(n);
        Node* g = GetGrandParent(n);
        if (n == p->right && p == g->left){
            RotateLeft(p);
            n = n->left;
        }
        else if (n == p->left && p == g->right){
            RotateRight(p);
            n = n->right;
        }
        InsertCase4Step2(n);
    }
    void InsertCase4Step2(Node* n){
        Node* p = GetParent(n);
        Node* g = GetGrandParent(n);
        if (n == p->left){
            RotateRight(g);
        }
        else{
            RotateLeft(g);
        }
        p->color = 0;
        g->color = 1;
    }

    void InOrder(Node *root){
        if(root!=NULL){
            InOrder(root->left);
            cout << root->key << " ";
            InOrder(root->right);
        }
    }
    void PreOrder(Node *root){
        if(root!=NULL){
            cout<< root->key << " ";
            PreOrder(root->left);
            PreOrder(root->right);

        }
    }
    void PostOrder(Node *root){
        if(root!=NULL){
            PreOrder(root->left);
            PreOrder(root->right);
            cout<< root->key << " ";
        }
    }
    bool Search(Node *root,int d){
        if(root==NULL){
            cout << "Tree empty!" << endl;
        }
        while (root != NULL) {
            if (d > root->key){
                root = root->right;
            }
            else if (d < root->key){
                root = root->left;
            }
            else{
                return true;
            }
        }
        return false;
     }
    void Minimum(Node *root){
        if(root==NULL){
            cout << "Tree empty!" << endl;
        }
        if(root->left==NULL){
            cout<<root->key;
            return ;
        }
        Minimum(root->left);
    }
    void Maximum(Node *root){
        if(root==NULL){
            cout << "Tree empty!" << endl;
        }
        if(root->right==NULL){
            cout<<root->key;
            return ;
        }
        Maximum(root->right);
    }
    Node *findminadd(Node *root){
        if(root->left==NULL)
            return root;
        else
            Minimum(root->left);
    }
    int Height(Node *root){
        if(root == NULL){
            return 0;
        }
        else{
            int lheight = Height(root->left);
            int rheight = Height(root->right);
            if(lheight > rheight){
                return lheight+1;
            }
            else{
                return rheight+1;
            }
        }
    }
    void Color(Node *root,int cl){
        if(root==NULL){
            cout << "Tree empty!" << endl;
        }
        Node* ptr=root;
        while(ptr->key!=cl){
            if(cl<ptr->key){
                ptr=ptr->left;
            }
            if(cl>ptr->key){
                ptr=ptr->right;
            }
        }
        cout<<"Color is: " ;
        if(ptr->color == 0){
            cout << "BLACK" << endl;
        }
        else{
            cout << "RED" << endl;
        }
    }
    //Least Common Ancestor
    Node* LCA(Node *root,int x,int y){
        if(root==NULL){
            return NULL;
        }
        if(root->key>x && root->key>y){
            return LCA(root->left,x,y);
        }
        if(root->key<x && root->key<y){
            return LCA(root->right,x,y);
        }
        return root;
    }

    void ReplaceNode(Node* n, Node* child){
        child->parent = n->parent;
        if (n == n->parent->left){
            n->parent->left = child;
        }
        else {
            n->parent->right = child;
        }
     }
     void DeleteOneChild(Node* n) {
         Node* child = (n->right == NULL) ? n->left : n->right;
         assert(child);
         ReplaceNode(n, child);
         if (n->color == 0){
            if (child->color == 1){
                child->color = 0;
            }
            else{
                DeleteCase1(child);
            }
         }
         free(n);
     }
     void DeleteCase1(Node* n){
         if (n->parent != NULL){
            DeleteCase2(n);
         }
     }
     void DeleteCase2(Node* n) {
         Node* s = GetSibling(n);
         if (s->color == 1){
            n->parent->color = 1;
            s->color = 0;
            if (n == n->parent->left){
                RotateLeft(n->parent);
            }
            else{
                RotateRight(n->parent);
            }
         }
         DeleteCase3(n);
     }
     void DeleteCase3(Node* n){
         Node* s = GetSibling(n);
         if ((n->parent->color == 0) && (s->color == 0) &&
             (s->left->color == 0) && (s->right->color == 0)){
                s->color = 1;
                DeleteCase1(n->parent);
             }
             else{
                DeleteCase4(n);
             }
     }
     void DeleteCase4(Node* n){
         Node* s = GetSibling(n);
         if ((n->parent->color == 1) && (s->color == 0) &&
      (s->left->color == 0) && (s->right->color == 0)){
          s->color = 1;
          n->parent->color = 0;
      }
      else{
        DeleteCase5(n);
      }
     }
     void DeleteCase5(Node* n){
         Node* s = GetSibling(n);
         if (s->color == 0){
         if ((n == n->parent->left) && (s->right->color == 0) &&
        (s->left->color == 1)){
            s->color = 1;
            s->left->color = 0;
            RotateRight(s);
        }
        else if ((n == n->parent->right) && (s->left->color == 0) &&
               (s->right->color == 1)){
            s->color = 1;
            s->right->color = 0;
            RotateLeft(s);
               }
         }
         DeleteCase6(n);
     }
     void DeleteCase6(Node* n){
         Node* s = GetSibling(n);
         s->color = n->parent->color;
         n->parent->color = 0;
         if (n == n->parent->left){
            s->right->color = 0;
            RotateLeft(n->parent);
         }
         else{
            s->left->color = 0;
            RotateRight(n->parent);
         }
     }

     Node* DeleteLeaf(Node *root, int del){
         if(root!=NULL){
            Node *temp=findminadd(root->right);
            root->key = temp->key;
            root->right = Delete(root->right,temp->key);
            return root;
         }
     }

     Node* Delete(Node *root,int del){
         if(root==NULL){
            return NULL;
         }

         if(root->key == del){
            if(root->left!=NULL && root->right!=NULL){
                Node *temp=findminadd(root->right);
                root->key = temp->key;
                root->right = Delete(root->right,temp->key);
                return root;
            }
            //delete root;
            return NULL;
         }
         Node* ptr=root;
         while(ptr->key!=del){
            if(del<ptr->key){
               ptr=ptr->left;
            }
            if(del>ptr->key){
                ptr=ptr->right;
            }
         }
         if(ptr->left!=NULL && ptr->right==NULL){
         DeleteOneChild(ptr);
         }
         if(ptr->left==NULL && ptr->right!=NULL){
         DeleteOneChild(ptr);
         }
         if(ptr->left!=NULL && ptr->right!=NULL){
            DeleteLeaf(ptr,del);
         }
         if(ptr->left==NULL && ptr->right==NULL){
                deleteNode(ptr);
         }
     }
};

void menu(){
    cout << "------------------" << endl;
    cout << "Red Black Tree" << endl;
    cout << "------------------" << endl;
    cout << " (1) Insert ... (2) InOrder ... (3) PreOrder ..." << endl;
    cout << " (4) PostOrder ... (5) Delete ... (6) Minimum ... "  << endl;
    cout << " (7) Maximum ... (8) Node Color ... (9) Height ...  "  << endl;
    cout << " (10) Search ... (11) Least Common Ancestor ... " << endl;
    cout << " (12) Exit ..." << endl;
    cout << "------------------" << endl;
}

#endif // REDBLACK_H_INCLUDED
