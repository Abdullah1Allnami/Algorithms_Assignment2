#include <iostream>

using namespace std;

struct Node {
    int data;
    string color;
    Node *left, *right, *p;

    Node(int data) : data(data), color("red"), left(nullptr), right(nullptr), p(nullptr) {
    }
};

class RBT {
private:
    Node *root;

    void leftRotate(Node *&node) {
        Node *right = node->right;
        node->right = right->left;

        if (right->left != nullptr)
            right->left->p = node; // link RL with it's p
        right->p = node->p;
        if (node->p == nullptr)
            root = right;
        else if (node == node->p->left)
            node->p->left = right;
        else
            node->p->right = right;

        right->left = node;
        node->p = right;
    }

    void rightRotate(Node *&node) {
        Node *left = node->left;
        node->left = left->right;
        if (left->right != nullptr)
            left->right->p = node;

        left->p = node->p;
        if (node->p == nullptr)
            root = left;
        else if (node == node->p->left)
            node->p->left = left;
        else
            node->p->right = left;

        left->right = node;
        node->p = left;
    }

    void fixInsert(Node *&node) {
        while (node->p && node->p->color == "red") {
            Node *gp = node->p->p;

            if (!gp)
                break;

            if (node->p == gp->left) {
                Node *uncle = gp->right;
                if (uncle && uncle->color == "red") // recoloring just
                {
                    node->p->color = "black";
                    uncle->color = "black";
                    gp->color = "red";
                    node = gp;
                } else {
                    if (node == node->p->right) // left right case
                    {
                        node = node->p;
                        leftRotate(node);
                    }
                    node->p->color = "black";
                    gp->color = "red";
                    rightRotate(gp); // left left case
                }
            } else {
                Node *uncle = gp->left;
                if (uncle && uncle->color == "red") // recoloring just
                {
                    node->p->color = "black";
                    uncle->color = "black";
                    gp->color = "red";
                    node = gp;
                } else {
                    if (node == node->p->left) // right left case
                    {
                        node = node->p;
                        rightRotate(node);
                    }
                    node->p->color = "black";
                    gp->color = "red";
                    leftRotate(gp); // right right case
                }
            }
        }
        root->color = "black";
    }

    void fixDelete(Node *&node) {
        while (node != root && (!node || node->color == "black")) {
            if (node == node->p->left) {
                Node *sibling = node->p->right;
                if (sibling && sibling->color == "red") {
                    sibling->color = "black";
                    node->p->color = "red";
                    leftRotate(node->p);
                    sibling = node->p->right; // point to the new sibling
                }
                if ((!sibling || sibling->left == nullptr || sibling->left->color == "black") &&
                    (!sibling || sibling->right == nullptr || sibling->right->color == "black")) {
                    if (sibling)
                        sibling->color = "red";
                    node = node->p;
                } else {
                    if (!sibling || sibling->right == nullptr || sibling->right->color == "black") {
                        if (sibling && sibling->left)
                            sibling->left->color = "black";
                        if (sibling)
                            sibling->color = "red";
                        rightRotate(sibling);
                        sibling = node->p->right;
                    }
                    if (sibling) { // the two children is red
                        sibling->color = node->p->color;
                        node->p->color = "black";
                        if (sibling->right)
                            sibling->right->color = "black";
                        leftRotate(node->p);
                    }
                    node = root;
                }
            } else {
                Node *sibling = node->p->left;
                if (sibling && sibling->color == "red") {
                    sibling->color = "black";
                    node->p->color = "red";
                    rightRotate(node->p);
                    sibling = node->p->left;
                }
                if ((!sibling || sibling->left == nullptr || sibling->left->color == "black") &&
                    (!sibling || sibling->right == nullptr || sibling->right->color == "black")) {
                    if (sibling)
                        sibling->color = "red";
                    node = node->p;
                } else {
                    if (!sibling || sibling->left == nullptr || sibling->left->color == "black") {
                        if (sibling && sibling->right)
                            sibling->right->color = "black";
                        if (sibling)
                            sibling->color = "red";
                        leftRotate(sibling);
                        sibling = node->p->left;
                    }
                    if (sibling) {
                        sibling->color = node->p->color;
                        node->p->color = "black";
                        if (sibling->left)
                            sibling->left->color = "black";
                        rightRotate(node->p);
                    }
                    node = root;
                }
            }
        }
        if (node)
            node->color = "black";
    }

    void transplant(Node *u, Node *v) {
        if (u->p == nullptr)
            root = v;
        else if (u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;

        if (v)
            v->p = u->p;
    }

    Node *minimum(Node *node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

public:
    RBT() : root(nullptr) {
    }

    void insert(int data) {
        Node *new_node = new Node(data);
        Node *p = nullptr;
        Node *cur = root;

        while (cur != nullptr) {
            p = cur;
            if (new_node->data < cur->data)
                cur = cur->left;
            else
                cur = cur->right;
        }

        new_node->p = p; // link it with the father

        if (p == nullptr)
            root = new_node;
        else if (new_node->data < p->data) // link the father with it
            p->left = new_node;
        else
            p->right = new_node;

        fixInsert(new_node);
    }

    void deleteNode(int data) {
        Node *cur = root;
        while (cur != nullptr && cur->data != data) {
            if (data < cur->data)
                cur = cur->left;
            else
                cur = cur->right;
        }

        if (cur == nullptr) // Node to delete not found
            return;

        Node *successor = cur;
        Node *new_cur = nullptr;
        string successor_color = successor->color;

        if (cur->left == nullptr) {
            new_cur = cur->right; // Skip
            transplant(cur, cur->right);
        } else if (cur->right == nullptr) {
            new_cur = cur->left; // Skip
            transplant(cur, cur->left);
        } else // Has left and right
        {
            successor = minimum(cur->right); // Find the successor of the current node
            successor_color = successor->color;
            new_cur = successor->right;

            if (successor->p == cur) // the successsor is the right node
            {
                if (new_cur)
                    new_cur->p = successor;// just skip
            } else {
                transplant(successor, successor->right);
                successor->right = cur->right;
                if (successor->right)
                    successor->right->p = successor;
            }

            transplant(cur, successor);
            successor->left = cur->left;
            if (successor->left)
                successor->left->p = successor;
            successor->color = cur->color;
        }

        delete cur;

        // Fix tree if a black node was removed
        if (successor_color == "black" && new_cur != nullptr) {
            fixDelete(new_cur);
        } else if (new_cur == nullptr && successor_color == "black") {
            // cout << "Deleted node was black and had no children." << endl;
        }
    }

    void inorderTraversal(Node *node) {
        if (node == nullptr)
            return;

        inorderTraversal(node->left);
        cout << node->data << " ";
        inorderTraversal(node->right);
    }

    void display() {
        inorderTraversal(root);
        cout << endl;
    }
};

int main() {
    RBT tree;
    tree.insert(30);
    tree.insert(20);
    tree.insert(25);
    tree.insert(10);
    tree.insert(40);
    tree.insert(35);
    tree.insert(50);
    tree.display();

    tree.deleteNode(30);

    tree.display();
}
