#include <iostream>

using namespace std;

struct Node
{
    int data;
    string color;
    Node *left, *right, *parent;

    Node(int data) : data(data), color("red"), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBT
{
private:
    Node *root;

    void leftRotate(Node *&node)
    {
        Node *right = node->right;
        node->right = right->left;
        if (right->left != nullptr)
            right->left->parent = node; // link RL with it's parent

        right->parent = node->parent;
        if (node->parent == nullptr)
            root = right;
        else if (node == node->parent->left)
            node->parent->left = right;
        else
            node->parent->right = right;

        right->left = node;
        node->parent = right;
    }

    void rightRotate(Node *&node)
    {
        Node *left = node->left;
        node->left = left->right;
        if (left->right != nullptr)
            left->right->parent = node;

        left->parent = node->parent;
        if (node->parent == nullptr)
            root = left;
        else if (node == node->parent->left)
            node->parent->left = left;
        else
            node->parent->right = left;

        left->right = node;
        node->parent = left;
    }

    void fixInsert(Node *&node)
    {
        while (node->parent && node->parent->color == "red")
        {
            Node *gp = node->parent->parent;

            if (!gp)
                break;

            if (node->parent == gp->left)
            {
                Node *uncle = gp->right;
                if (uncle && uncle->color == "red") // recoloring just
                {
                    node->parent->color = "black";
                    uncle->color = "black";
                    gp->color = "red";
                    node = gp;
                }
                else
                {
                    if (node == node->parent->right) // left right case
                    {
                        node = node->parent;
                        leftRotate(node);
                    }
                    node->parent->color = "black";
                    gp->color = "red";
                    rightRotate(gp); // left left case
                }
            }
            else
            {
                Node *uncle = gp->left;
                if (uncle && uncle->color == "red") // recoloring just
                {
                    node->parent->color = "black";
                    uncle->color = "black";
                    gp->color = "red";
                    node = gp;
                }
                else
                {
                    if (node == node->parent->left) // right left case
                    {
                        node = node->parent;
                        rightRotate(node);
                    }
                    node->parent->color = "black";
                    gp->color = "red";
                    leftRotate(gp); // right right case
                }
            }
        }
        root->color = "black";
    }

    void fixDelete(Node *&node)
    {
        while (node != root && (!node || node->color == "black"))
        {
            if (node == node->parent->left)
            {
                Node *sibling = node->parent->right;
                if (sibling && sibling->color == "red")
                {
                    sibling->color = "black";
                    node->parent->color = "red";
                    leftRotate(node->parent);
                    sibling = node->parent->right;
                }
                if ((!sibling || sibling->left == nullptr || sibling->left->color == "black") &&
                    (!sibling || sibling->right == nullptr || sibling->right->color == "black"))
                {
                    if (sibling)
                        sibling->color = "red";
                    node = node->parent;
                }
                else
                {
                    if (!sibling || sibling->right == nullptr || sibling->right->color == "black")
                    {
                        if (sibling && sibling->left)
                            sibling->left->color = "black";
                        if (sibling)
                            sibling->color = "red";
                        rightRotate(sibling);
                        sibling = node->parent->right;
                    }
                    if (sibling)
                    {
                        sibling->color = node->parent->color;
                        node->parent->color = "black";
                        if (sibling->right)
                            sibling->right->color = "black";
                        leftRotate(node->parent);
                    }
                    node = root;
                }
            }
            else
            {
                Node *sibling = node->parent->left;
                if (sibling && sibling->color == "red")
                {
                    sibling->color = "black";
                    node->parent->color = "red";
                    rightRotate(node->parent);
                    sibling = node->parent->left;
                }
                if ((!sibling || sibling->left == nullptr || sibling->left->color == "black") &&
                    (!sibling || sibling->right == nullptr || sibling->right->color == "black"))
                {
                    if (sibling)
                        sibling->color = "red";
                    node = node->parent;
                }
                else
                {
                    if (!sibling || sibling->left == nullptr || sibling->left->color == "black")
                    {
                        if (sibling && sibling->right)
                            sibling->right->color = "black";
                        if (sibling)
                            sibling->color = "red";
                        leftRotate(sibling);
                        sibling = node->parent->left;
                    }
                    if (sibling)
                    {
                        sibling->color = node->parent->color;
                        node->parent->color = "black";
                        if (sibling->left)
                            sibling->left->color = "black";
                        rightRotate(node->parent);
                    }
                    node = root;
                }
            }
        }
        if (node)
            node->color = "black";
    }

    void transplant(Node *u, Node *v)
    {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        if (v)
            v->parent = u->parent;
    }

    Node *minimum(Node *node)
    {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

public:
    RBT() : root(nullptr) {}

    void insert(int data)
    {
        Node *new_node = new Node(data);
        Node *parent = nullptr;
        Node *cur = root;

        while (cur != nullptr)
        {
            parent = cur;
            if (new_node->data < cur->data)
                cur = cur->left;
            else
                cur = cur->right;
        }

        new_node->parent = parent; // link it with the father

        if (parent == nullptr)
            root = new_node;
        else if (new_node->data < parent->data) // link the father with it
            parent->left = new_node;
        else
            parent->right = new_node;

        fixInsert(new_node);
    }

    void deleteNode(int data)
    {
        Node *cur = root;
        while (cur != nullptr && cur->data != data)
        {
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

        if (cur->left == nullptr)
        {
            new_cur = cur->right; // Skip
            transplant(cur, cur->right);
        }
        else if (cur->right == nullptr)
        {
            new_cur = cur->left; // Skip
            transplant(cur, cur->left);
        }
        else // Has left and right
        {
            successor = minimum(cur->right); // Find the successor of the current node
            successor_color = successor->color;
            new_cur = successor->right;

            if (successor->parent == cur) // the successsor is the right node 
            {
                if (new_cur)
                    new_cur->parent = successor; 
            }
            else
            {
                transplant(successor, successor->right);
                successor->right = cur->right;
                if (successor->right)
                    successor->right->parent = successor;
            }

            transplant(cur, successor);
            successor->left = cur->left;
            if (successor->left)
                successor->left->parent = successor;
            successor->color = cur->color;
        }

        delete cur;

        // Fix tree if a black node was removed
        if (successor_color == "black" && new_cur != nullptr)
        {
            fixDelete(new_cur);
        }
        else if (new_cur == nullptr && successor_color == "black")
        {
            // cout << "Deleted node was black and had no children." << endl;
        }
    }

    void inorderTraversal(Node *node)
    {
        if (node == nullptr)
            return;

        inorderTraversal(node->left);
        cout << node->data << " ";
        inorderTraversal(node->right);
    }

    void display()
    {
        inorderTraversal(root);
        cout << endl;
    }
};

int main()
{
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
