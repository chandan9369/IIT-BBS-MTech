#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_NODES = 8; // Limit of nodes in the ring (2^m)

class Node
{
public:
    int id;
    unordered_map<int, string> data;
    Node *successor;
    Node *predecessor;

    Node(int id) : id(id), successor(this), predecessor(this) {}

    // Store data on the responsible node
    void storeData(int key, const string &value)
    {
        Node *responsibleNode = findSuccessor(key);
        responsibleNode->data[key] = value;
        cout << "Stored key [" << key << "] at Node with id [" << responsibleNode->id << "]." << endl;
    }

    // Lookup data from the responsible node
    string lookupData(int key)
    {
        Node *responsibleNode = findSuccessor(key);
        if (responsibleNode->data.count(key))
        {
            return responsibleNode->data[key];
        }
        else
        {
            return "Key not found";
        }
    }

    // Find the successor node responsible for a given key
    Node *findSuccessor(int key)
    {
        if (inInterval(key, id, successor->id) || key == successor->id)
        {
            return successor;
        }
        else
        {
            Node *current = successor;
            while (!inInterval(key, current->id, current->successor->id))
            {
                current = current->successor;
                if (current == this)
                    break; // Prevent infinite loop
            }
            return current->successor;
        }
    }

    // Check if a key falls within a circular interval
    bool inInterval(int key, int start, int end)
    {
        if (start < end)
        {
            return key > start && key <= end;
        }
        else
        { // Wrap around the ring
            return key > start || key <= end;
        }
    }

    // Transfer data to a target node
    void transferData(Node *targetNode)
    {
        for (const auto &[key, value] : data)
        {
            Node *responsibleNode = targetNode->findSuccessor(key);
            responsibleNode->data[key] = value;
        }
        data.clear();
    }
};

class ChordRing
{
public:
    vector<Node *> nodes;

    // Add a node to the ring
    void addNode(int nodeId)
    {
        Node *newNode = new Node(nodeId);
        if (!nodes.empty())
        {
            Node *randomNode = nodes[0];
            newNode->successor = randomNode->findSuccessor(nodeId);
            newNode->predecessor = newNode->successor->predecessor;
            newNode->successor->predecessor = newNode;
            newNode->predecessor->successor = newNode;
        }
        nodes.push_back(newNode);
        cout << "Node with id [" << nodeId << "] added to the CHORD ring." << endl;
    }

    // Remove a node from the ring
    void removeNode(int nodeId)
    {
        auto it = find_if(nodes.begin(), nodes.end(), [nodeId](Node *node)
                          { return node->id == nodeId; });
        if (it != nodes.end())
        {
            Node *node = *it;
            node->transferData(node->successor); // Transfer data to successor
            node->predecessor->successor = node->successor;
            node->successor->predecessor = node->predecessor;
            delete node;
            nodes.erase(it);
            cout << "Node with id [" << nodeId << "] is removed from the CHORD ring and data transferred to its successor." << endl;
        }
    }

    // Display the current CHORD ring
    void displayRing()
    {
        cout << "\n>>>>>>>>>>>>>> Every Node Structure in CHORD Ring <<<<<<<<<<<<<<\n"
             << endl;
        for (Node *node : nodes)
        {
            cout << "Node with id [" << node->id << "] :" << endl;
            cout << "\t>>> Successor node: " << node->successor->id << endl;
            cout << "\t>>> Predecessor node: " << node->predecessor->id << endl;
            cout << endl;
        }
    }

    // Get a node by its ID
    Node *getNode(int nodeId)
    {
        auto it = find_if(nodes.begin(), nodes.end(), [nodeId](Node *node)
                          { return node->id == nodeId; });
        return (it != nodes.end()) ? *it : nullptr;
    }
};
void simulator()
{
    cout << "############################################################\n";
    cout << "************ CHORD Ring Simulator By Chandan ***************\n";
    cout << "############################################################\n";
}
void showMenu()
{
    cout << "\n>>>>>>>>>>>>>>>>> Menu <<<<<<<<<<<<<<<<<<<<<" << endl;
    cout << "\t1. Add Node" << endl;
    cout << "\t2. Remove Node" << endl;
    cout << "\t3. Store Data" << endl;
    cout << "\t4. Look Your Data" << endl;
    cout << "\t5. Display CHORD Ring" << endl;
    cout << "\t6. Exit" << endl;
    cout << "\nEnter choice from above options: ";
}
int main()
{
    ChordRing chordRing;
    int choice;
    simulator();
    while (true)
    {
        showMenu();
        cin >> choice;

        if (choice == 1)
        {
            int nodeId;
            cout << "Enter id of node to add: ";
            cin >> nodeId;
            chordRing.addNode(nodeId);
        }
        else if (choice == 2)
        {
            int nodeId;
            cout << "Enter id of node to remove: ";
            cin >> nodeId;
            chordRing.removeNode(nodeId);
        }
        else if (choice == 3)
        {
            int nodeId, key;
            string value;
            cout << "Enter Node ID to store data: ";
            cin >> nodeId;
            cout << "Enter integer key: ";
            cin >> key;
            cout << "Enter your data: ";
            cin >> value;
            Node *node = chordRing.getNode(nodeId);
            if (node)
            {
                node->storeData(key, value);
            }
            else
            {
                cout << "Node with id " << nodeId << " does not exist." << endl;
            }
        }
        else if (choice == 4)
        {
            int nodeId, key;
            cout << "Enter Node ID to get your data: ";
            cin >> nodeId;
            cout << "Enter integer key: ";
            cin >> key;
            Node *node = chordRing.getNode(nodeId);
            if (node)
            {
                cout << "Data for key [" << key << "]: {" << node->lookupData(key) << "}." << endl;
            }
            else
            {
                cout << "Node with id " << nodeId << " does not exist." << endl;
            }
        }
        else if (choice == 5)
        {
            chordRing.displayRing();
        }
        else if (choice == 6)
        {
            cout << "Exiting..." << endl;
            break;
        }
        else
        {
            cout << "Invalid choice. Please enter correct choice." << endl;
        }
    }

    return 0;
}
