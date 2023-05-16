#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Node {
    public:
        int value;
        Node* left;
        Node* right;

        Node(int v, Node* l, Node* r) {
            this->value = v;
            this->left = l;
            this->right = r;
        }
        

};
class BST {
    private:
        Node* root;
        void deleteTree(Node *root) {
            if(root == NULL) {
                return;
            }
            if(root->left != NULL) {
                deleteTree(root->left);
            }
            if(root->right != NULL) {
                deleteTree(root->right);
            }
            delete root;
        }

        vector<int> inOrder(Node* root) {
            vector<int> res;
            if(root == NULL) {
                return res;
            }
            vector<int> leftValues = inOrder(root->left);
            vector<int> rightValues = inOrder(root->right);
            res.insert(res.end(), leftValues.begin(), leftValues.end());
            res.push_back(root->value);
            res.insert(res.end(), rightValues.begin(), rightValues.end());
            return res;
        }

        int getHeight(Node* root) {
            if(root == NULL) {
                return 0;
            }
            return max(getHeight(root->left), getHeight(root->right)) + 1;
        }
    public:
        BST() {
            root = NULL;
        }

        BST(Node* r) {
            root = r;
        }
        ~BST() {
            deleteTree(root);
        }

        void insertKey(int newKey) {
            if(root == NULL) {
                root = new Node(newKey, NULL, NULL);
                return;
            }

            Node *current = root;
            Node *prev = NULL;
            while (current != NULL) {
                prev = current;
                if(newKey < current->value) {
                    current = current->left;
                } else {
                    current = current->right;
                }
            }
            if(newKey < prev->value) {
                prev->left = new Node(newKey, NULL, NULL);
            } else {
                prev->right = new Node(newKey, NULL, NULL);
            }

        }

        bool hasKey(int searchKey) {
            Node* current = root;
            while(current != NULL) {
                if(current->value == searchKey) {
                    return true;
                } else if(current->value < searchKey) {
                    current = current->right;
                } else {
                    current = current->left;
                }
            }
            return false;
        }

        vector<int> inOrder() {
            return inOrder(root);
        }

        int getHeight() {
            return getHeight(root);
        }

};

int main() {
    BST bst;
    cout << "Enter the numbers to be stored (end with a letter): "<< endl;
    while(true) {
        string val;
        cin >> val;
        try {
            int intVal = stoi(val);
            bst.insertKey(intVal);
        } catch(invalid_argument ia) { //Stop reading if a non-integer is given
            break;
        }
    }
    cout << "Which number do you want to lookup? " << endl;
    int searchKey;
    cin >> searchKey;
    bool found = bst.hasKey(searchKey);
    cout << searchKey << " is in the tree: ";
    if(found) {
        cout << "yes" << endl;
    } 
    else {
        cout << "no" << endl;
    }
    cout << "The numbers in sorted order: ";
    vector<int> sortedOrder = bst.inOrder();
    for(int i = 0; i < sortedOrder.size(); i++) {
        cout << sortedOrder[i] << " ";
    }
    cout << endl;
    cout << "Height of the tree: " << bst.getHeight() << endl;
    return 0;
}