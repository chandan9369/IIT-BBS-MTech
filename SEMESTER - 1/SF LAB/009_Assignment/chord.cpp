#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const int MAX_NODES = 32; // Limit of nodes in the ring (2^m)

int hashKey(const string &key)
{
    unsigned int hash = 0;
    for (char c : key)
    {
        hash = (hash * 31 + c) % MAX_NODES;
    }
    return hash;
}

class Node
{
public:
    int id;
    unordered_map<string, string> data;
    Node *successor;
    Node *predecessor;

    Node(int id) : id(id), successor(this), predecessor(this) {}

    void storeData(const string &key, const string &value)
    {
        int keyHash = hashKey(key);
        Node *responsibleNode = findSuccessor(keyHash);
        responsibleNode->data[key] = value;
        cout << "Stored key '" << key << "' at Node " << responsibleNode->id << endl;
    }

    string lookupData(const string &key)
    {
        int keyHash = hashKey(key);
        Node *responsibleNode = findSuccessor(keyHash);
        if (responsibleNode->data.count(key))
        {
            return responsibleNode->data[key];
        }
        else
        {
            return "Key not found";
        }
    }

    Node *findSuccessor(int keyHash)
    {
        if (inInterval(keyHash, id, successor->id))
        {
            return successor;
        }
        else
        {
            return closestPrecedingNode(keyHash)->findSuccessor(keyHash);
        }
    }

    Node *closestPrecedingNode(int keyHash)
    {
        if (inInterval(id, keyHash, successor->id))
            return successor;
        return this;
    }

    bool inInterval(int key, int start, int end)
    {
        if (start < end)
            return key > start && key <= end;
        else
            return key > start || key <= end;
    }

    void transferData(Node *targetNode)
    {
        for (const auto &[key, value] : data)
        {
            int keyHash = hashKey(key);
            Node *responsibleNode = targetNode->findSuccessor(keyHash);
            responsibleNode->data[key] = value;
        }
        data.clear();
    }
};

class ChordRing
{
public:
    vector<Node *> nodes;

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
        cout << "Node " << nodeId << " added to the CHORD ring." << endl;
    }

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
            cout << "Node " << nodeId << " removed from the CHORD ring and data transferred to its successor." << endl;
        }
    }

    void displayRing()
    {
        cout << "CHORD Ring: " << endl;
        for (Node *node : nodes)
        {
            cout << "Node " << node->id << " -> Successor: " << node->successor->id
                 << ", Predecessor: " << node->predecessor->id << endl;
        }
    }

    Node *getNode(int nodeId)
    {
        auto it = find_if(nodes.begin(), nodes.end(), [nodeId](Node *node)
                          { return node->id == nodeId; });
        return (it != nodes.end()) ? *it : nullptr;
    }
};

int main()
{
    ChordRing chordRing;
    int choice;

    int cnt = 0;
    while (true)
    {
        cnt++;
        cout << "\n--- CHORD Ring Menu ---" << endl;
        cout << "1. Add Node" << endl;
        cout << "2. Remove Node" << endl;
        cout << "3. Store Data" << endl;
        cout << "4. Lookup Data" << endl;
        cout << "5. Display CHORD Ring" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            int nodeId;
            cout << "Enter Node ID to add: ";
            cin >> nodeId;
            chordRing.addNode(nodeId);
        }
        else if (choice == 2)
        {
            int nodeId;
            cout << "Enter Node ID to remove: ";
            cin >> nodeId;
            chordRing.removeNode(nodeId);
        }
        else if (choice == 3)
        {
            int nodeId;
            string key, value;
            cout << "Enter Node ID to store data: ";
            cin >> nodeId;
            cout << "Enter key: ";
            cin >> key;
            cout << "Enter value: ";
            cin >> value;
            Node *node = chordRing.getNode(nodeId);
            if (node)
            {
                node->storeData(key, value);
            }
            else
            {
                cout << "Node " << nodeId << " does not exist." << endl;
            }
        }
        else if (choice == 4)
        {
            int nodeId;
            string key;
            cout << "Enter Node ID to lookup data: ";
            cin >> nodeId;
            cout << "Enter key: ";
            cin >> key;
            Node *node = chordRing.getNode(nodeId);
            if (node)
            {
                cout << "Data for key '" << key << "': " << node->lookupData(key) << endl;
            }
            else
            {
                cout << "Node " << nodeId << " does not exist." << endl;
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
            cout << "Invalid choice. Please try again." << endl;
        }
        if (cnt > MAX_NODES)
        {
            break;
        }
    }

    return 0;
}