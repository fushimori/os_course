#include <iostream>
#include <map>
#include <zmq.h>

struct Node {
    int id;
    bool exist{true};
    // std::map<std::string, int> values;
    Node* left;
    Node* right;

    Node(int nodeId) : id(nodeId), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root;

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        destroyTree(root);
    }

    void printTree() {
        printTree(root, 0);
    }
    int findParentId(int nodeId) {
        return findParentId(root, nodeId);
    }

    int findInsert(int nodeId) {
        return findInsertionPoint(root, nodeId);
    }

    std::string createNode(int nodeId) {
        Node* newNode = new Node(nodeId);
        if (root == nullptr) {
            root = newNode;
            return "Ok:";
        } else {
            Node* currentNode = root;
            Node* parentNode = nullptr;

            while (currentNode != nullptr) {
                parentNode = currentNode;

                if (nodeId < currentNode->id) {
                    currentNode = currentNode->left;
                } else if (nodeId > currentNode->id) {
                    currentNode = currentNode->right;
                } else {
                    delete newNode; // Узел с таким идентификатором уже существует
                    return "Error: Already exists";
                }
            }

            if (nodeId < parentNode->id) {
                parentNode->left = newNode;
            } else {
                parentNode->right = newNode;
            }

            return "Ok:";
        }
    }
    
    std::string findNode(int nodeId) {
        return findNode(root, nodeId);
    }

    int getSize() {
        return getSize(root);
    }

    void destroyTree(Node* currentNode) {
        if (currentNode == nullptr) {
            return;
        }
        destroyTree(currentNode->left);
        destroyTree(currentNode->right);
        delete currentNode;
    }

    std::string killNodeAndChildren(int nodeId, void* socket) {
        std::string answer = findNode(root, nodeId);
        if (answer == "Ok") {
            Node* node = findNodePointer(root, nodeId);
            sendKillMessage(node, socket);
            return "Ok: All nodes killed";
        }
        return answer;
    }

private:

    void sendKillMessage(Node* node, void* socket) {
        if (node == nullptr || !node->exist) {
            return;
        }
        node->exist = false;
        std::string msg = "kill " + std::to_string(node->id);
        zmq_send(socket, msg.c_str(), msg.length(), 0);
        sendKillMessage(node->left, socket);
        sendKillMessage(node->right, socket);
    }

    std::string findNode(Node* currentNode, int nodeId) {
        if (currentNode == nullptr) {
            return "Error:"  + std::to_string(nodeId) + ": Not found";
        }
        if (currentNode->id == nodeId) {
            if (currentNode->id == -1) {
                return "Error:" + std::to_string(nodeId) + ": Not calculation node";
            } else {
                return "Ok";
            }
        } else {
            std::string leftResult = findNode(currentNode->left, nodeId);
            if (leftResult != "Error:"  + std::to_string(nodeId) + ": Not found") {
                return leftResult;
            }
            std::string rightResult = findNode(currentNode->right, nodeId);
            return rightResult;
        }
    }

    Node* findNodePointer(Node* currentNode, int nodeId) {
        if (currentNode == nullptr) {
            return nullptr;
        }
        if (currentNode->id == nodeId) {
            return currentNode;
        }
        Node* leftResult = findNodePointer(currentNode->left, nodeId);
        if (leftResult != nullptr) {
            return leftResult;
        }
        Node* rightResult = findNodePointer(currentNode->right, nodeId);
        return rightResult;
    }

    int findParentId(Node* currentNode, int nodeId) {
        if (currentNode == nullptr || currentNode->id == nodeId) {
            return -1;
        }
        if ((currentNode->left != nullptr && currentNode->left->id == nodeId) ||
            (currentNode->right != nullptr && currentNode->right->id == nodeId)) {
            return currentNode->id;
        }
        int leftResult = findParentId(currentNode->left, nodeId);
        if (leftResult != -1) {
            return leftResult;
        }
        int rightResult = findParentId(currentNode->right, nodeId);
        return rightResult;
    }
    int findInsertionPoint(Node* currentNode, int nodeId) {
        if (currentNode == nullptr) {
            return -1; 
        }
        if (nodeId < currentNode->id) {
            if (currentNode->left == nullptr) {
                return currentNode->id;
            } else {
                return findInsertionPoint(currentNode->left, nodeId);
            }
        } else if (nodeId > currentNode->id) {
            if (currentNode->right == nullptr) {
                return currentNode->id;
            } else {
                return findInsertionPoint(currentNode->right, nodeId);
            }
        } else {
            return -1;
        }
    }

    void printTree(Node* currentNode, int indentLevel) {
        if (currentNode == nullptr) {
            return;
        }
        
        printTree(currentNode->right, indentLevel + 1); 
        
        for (int i = 0; i < indentLevel; i++) {
            std::cout << "\t";
        }
        std::cout << "|----- " << currentNode->id << std::endl;
        
        printTree(currentNode->left, indentLevel + 1);
    }

    int getSize(Node* currentNode) {
        if (currentNode == nullptr) {
            return 0;
        }
        int leftSize = getSize(currentNode->left);
        int rightSize = getSize(currentNode->right);
        return leftSize + rightSize + 1;
    }
};