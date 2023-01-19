#include<bits/stdc++.h>
#include"RedBlack.h"
using namespace std;
int main(){
    int choice;
    int key;
    RedBlack *b = new RedBlack();
    Node *new1;
    while(true){
        menu();
        cout << "Enter choice: ";
        cin >> choice;
        switch(choice){
        case 1:
            cout << "Enter element = ";
            cin >> key;
            new1 = new Node(key);
            b->root = b->Insert(b->root,new1);
            cout << key << " inserted into the tree." << endl;
            break;
        case 2:
            cout << "InOrder Traversal: " << endl;
            b->InOrder(b->root);
            break;
        case 3:
            cout << "PreOrder Traversal: " << endl;
            b->PreOrder(b->root);
            break;
        case 4:
            cout << "PostOrder Traversal: " << endl;
            b->PostOrder(b->root);
            break;
        case 5:
            cout << "Delete Node: " << endl;
            int del;
            cin>> del;
            //if(b->root->left==NULL && b->root->right==NULL){
                //b->deleteNode(b->root);
            //}
            //else{
                b->Delete(b->root,del);
            //}
            cout << del << " deleted from the tree.";
            break;
        case 6:
             cout << "Minimum: " << endl;
             b->Minimum(b->root);
             break;
        case 7:
             cout << "Maximum: " << endl;
             b->Maximum(b->root);
             break;
        case 8:
             cout << "Return Color Of: " << endl;
             int cl;
             cin>>cl;
             b->Color(b->root,cl);
             break;
        case 9:
            cout << "The Height: ";
            int h;
            h = b->Height(b->root);
            cout << h << endl;
            break;
        case 10:
            int s;
            cout << "Enter node value:";
            cin >> s;
            cout << "Element found: ";
            if(b->Search(b->root,s))
                cout << "true" << endl;
            else
                cout << "false" << endl;

            break;
        case 11:
            int x,y;
            cout << "Enter two nodes: " << endl;
            cin >> x;
            cin >> y;
            if(b->Search(b->root,x)&&b->Search(b->root,y)){
                Node *lca;
                lca = b->LCA(b->root,x,y);
                cout << "The least common ancestor between "<<x<<" and "
                     <<y<<" is "<< lca->key << endl;
            }
            else{
                cout << x << " and " << y << " not in tree" << endl;
            }
            break;
        case 12:
            cout << "Thank you!" << endl;
            exit(0);
        default:
            cout << "Choose between 1 to 12." << endl;
            break;
        }
    }
    delete b;
    return 0;
}
