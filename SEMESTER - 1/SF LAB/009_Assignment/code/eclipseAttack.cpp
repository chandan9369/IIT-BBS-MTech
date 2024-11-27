// #include <iostream>
// #include <unordered_map>
// #include <vector>
// #include <cmath>
// #include <algorithm>

// using namespace std;

// const int MAX_NODES = 32; // Limit of nodes in the ring (2^m)

// int hashKey(const string &key)
// {
//     unsigned int hash = 0;
//     for (char c : key)
//     {
//         hash = (hash * 31 + c) % MAX_NODES;
//     }
//     return hash;
// }

// class Node
// {
// public:
//     int id;
//     unordered_map<string, string> data;
//     Node *successor;
//     Node *predecessor;
//     bool isMalicious = false;

//     Node(int id) : id(id), successor(this), predecessor(this) {}

//     // Store data on the responsible node
//     void storeData(const string &key, const string &value)
//     {
//         int keyHash = hashKey(key);
//         Node *responsibleNode = findSuccessor(keyHash);
//         responsibleNode->data[key] = value;
//         cout << "Stored key '" << key << "' at Node " << responsibleNode->id << endl;
//     }

//     // Lookup data from the responsible node
//     string lookupData(const string &key)
//     {
//         int keyHash = hashKey(key);
//         Node *responsibleNode = findSuccessor(keyHash);
//         if (responsibleNode->data.count(key))
//         {
//             return responsibleNode->data[key];
//         }
//         else
//         {
//             return "Key not found";
//         }
//     }

//     // Find the successor node responsible for a given hash
//     Node *findSuccessor(int keyHash)
//     {
//         if (inInterval(keyHash, id, successor->id) || keyHash == successor->id)
//         {
//             return successor;
//         }
//         else
//         {
//             Node *current = successor;
//             while (!inInterval(keyHash, current->id, current->successor->id))
//             {
//                 current = current->successor;
//                 if (current == this)
//                     break; // Prevent infinite loop in a broken ring
//             }
//             return current->successor;
//         }
//     }

//     // Check if a key falls within a circular interval
//     bool inInterval(int key, int start, int end)
//     {
//         if (start < end)
//         {
//             return key > start && key <= end;
//         }
//         else
//         { // Wrap around the ring
//             return key > start || key <= end;
//         }
//     }

//     // Transfer data to a target node
//     void transferData(Node *targetNode)
//     {
//         for (const auto &[key, value] : data)
//         {
//             int keyHash = hashKey(key);
//             Node *responsibleNode = targetNode->findSuccessor(keyHash);
//             responsibleNode->data[key] = value;
//         }
//         data.clear();
//     }
// };

// class ChordRing
// {
// public:
//     vector<Node *> nodes;

//     // Add a node to the ring
//     void addNode(int nodeId)
//     {
//         Node *newNode = new Node(nodeId);
//         if (!nodes.empty())
//         {
//             Node *randomNode = nodes[0];
//             newNode->successor = randomNode->findSuccessor(nodeId);
//             newNode->predecessor = newNode->successor->predecessor;
//             newNode->successor->predecessor = newNode;
//             newNode->predecessor->successor = newNode;
//         }
//         nodes.push_back(newNode);
//         cout << "Node " << nodeId << " added to the CHORD ring." << endl;
//     }

//     // Remove a node from the ring
//     void removeNode(int nodeId)
//     {
//         auto it = find_if(nodes.begin(), nodes.end(), [nodeId](Node *node)
//                           { return node->id == nodeId; });
//         if (it != nodes.end())
//         {
//             Node *node = *it;
//             node->transferData(node->successor); // Transfer data to successor
//             node->predecessor->successor = node->successor;
//             node->successor->predecessor = node->predecessor;
//             delete node;
//             nodes.erase(it);
//             cout << "Node " << nodeId << " removed from the CHORD ring and data transferred to its successor." << endl;
//         }
//     }

//     // Display the current CHORD ring
//     void displayRing()
//     {
//         cout << "CHORD Ring: " << endl;
//         for (Node *node : nodes)
//         {
//             cout << "Node " << node->id << " -> Successor: " << node->successor->id
//                  << ", Predecessor: " << node->predecessor->id
//                  << (node->isMalicious ? " [Malicious]" : "") << endl;
//         }
//     }

//     // Get a node by its ID
//     Node *getNode(int nodeId)
//     {
//         auto it = find_if(nodes.begin(), nodes.end(), [nodeId](Node *node)
//                           { return node->id == nodeId; });
//         return (it != nodes.end()) ? *it : nullptr;
//     }

//     // Simulate an eclipse attack by surrounding a target node with malicious nodes
//     void simulateEclipseAttack(int targetNodeId, int numMaliciousNodes)
//     {
//         Node *targetNode = getNode(targetNodeId);
//         if (!targetNode)
//         {
//             cout << "Target node " << targetNodeId << " does not exist in the ring." << endl;
//             return;
//         }

//         cout << "Simulating eclipse attack on Node " << targetNodeId << " with " << numMaliciousNodes << " malicious nodes." << endl;

//         // Create and add malicious nodes around the target node
//         for (int i = 0; i < numMaliciousNodes; ++i)
//         {
//             int maliciousId = (targetNodeId + i + 1) % MAX_NODES;
//             Node *maliciousNode = new Node(maliciousId);
//             maliciousNode->isMalicious = true;

//             // Insert malicious node between target node and its successor
//             maliciousNode->successor = targetNode->successor;
//             maliciousNode->predecessor = targetNode;
//             targetNode->successor->predecessor = maliciousNode;
//             targetNode->successor = maliciousNode;

//             nodes.push_back(maliciousNode);
//             cout << "Malicious Node " << maliciousId << " added around Node " << targetNodeId << endl;
//         }
//     }
// };

// int main()
// {
//     ChordRing chordRing;
//     int choice;

//     while (true)
//     {
//         cout << "\n--- CHORD Ring Menu ---" << endl;
//         cout << "1. Add Node" << endl;
//         cout << "2. Remove Node" << endl;
//         cout << "3. Store Data" << endl;
//         cout << "4. Lookup Data" << endl;
//         cout << "5. Display CHORD Ring" << endl;
//         cout << "6. Simulate Eclipse Attack" << endl;
//         cout << "7. Exit" << endl;
//         cout << "Enter choice: ";
//         cin >> choice;

//         if (choice == 1)
//         {
//             int nodeId;
//             cout << "Enter Node ID to add: ";
//             cin >> nodeId;
//             chordRing.addNode(nodeId);
//         }
//         else if (choice == 2)
//         {
//             int nodeId;
//             cout << "Enter Node ID to remove: ";
//             cin >> nodeId;
//             chordRing.removeNode(nodeId);
//         }
//         else if (choice == 3)
//         {
//             int nodeId;
//             string key, value;
//             cout << "Enter Node ID to store data: ";
//             cin >> nodeId;
//             cout << "Enter key: ";
//             cin >> key;
//             cout << "Enter value: ";
//             cin >> value;
//             Node *node = chordRing.getNode(nodeId);
//             if (node)
//             {
//                 node->storeData(key, value);
//             }
//             else
//             {
//                 cout << "Node " << nodeId << " does not exist." << endl;
//             }
//         }
//         else if (choice == 4)
//         {
//             int nodeId;
//             string key;
//             cout << "Enter Node ID to lookup data: ";
//             cin >> nodeId;
//             cout << "Enter key: ";
//             cin >> key;
//             Node *node = chordRing.getNode(nodeId);
//             if (node)
//             {
//                 cout << "Data for key '" << key << "': " << node->lookupData(key) << endl;
//             }
//             else
//             {
//                 cout << "Node " << nodeId << " does not exist." << endl;
//             }
//         }
//         else if (choice == 5)
//         {
//             chordRing.displayRing();
//         }
//         else if (choice == 6)
//         {
//             int targetNodeId, numMaliciousNodes;
//             cout << "Enter Target Node ID for eclipse attack: ";
//             cin >> targetNodeId;
//             cout << "Enter number of malicious nodes to add: ";
//             cin >> numMaliciousNodes;
//             chordRing.simulateEclipseAttack(targetNodeId, numMaliciousNodes);
//         }
//         else if (choice == 7)
//         {
//             cout << "Exiting..." << endl;
//             break;
//         }
//         else
//         {
//             cout << "Invalid choice. Please try again." << endl;
//         }
//     }

//     return 0;
// }

#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_NODES = 32; // Limit of nodes in the ring (2^m)

class Node
{
public:
    int id;
    unordered_map<int, string> data;
    Node *successor;
    Node *predecessor;
    bool isMalicious = false;

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
        if (responsibleNode->isMalicious)
        {
            return "Fail to retrieve the correct data !!";
        }
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
    void addNode(int nodeId, bool isMalicious = false)
    {
        Node *newNode = new Node(nodeId);
        newNode->isMalicious = isMalicious;
        if (!nodes.empty())
        {
            Node *randomNode = nodes[0];
            newNode->successor = randomNode->findSuccessor(nodeId);
            newNode->predecessor = newNode->successor->predecessor;
            newNode->successor->predecessor = newNode;
            newNode->predecessor->successor = newNode;
        }
        nodes.push_back(newNode);
        cout << "Node with id [" << nodeId << "] added to the CHORD ring." << (isMalicious ? " [Malicious]" : "") << endl;
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
            cout << "\t>>> Status: " << (node->isMalicious ? "Malicious" : "Normal") << endl;
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

    // Simulate an eclipse attack by adding malicious nodes around a target node
    void simulateEclipseAttack(int targetNodeId, int numMaliciousNodes)
    {
        Node *targetNode = getNode(targetNodeId);
        if (!targetNode)
        {
            cout << "Target node " << targetNodeId << " does not exist in the ring." << endl;
            return;
        }

        cout << "Simulating eclipse attack on Node " << targetNodeId << " with " << numMaliciousNodes << " malicious nodes." << endl;

        // Add malicious nodes around the target node
        for (int i = 0; i < numMaliciousNodes; ++i)
        {
            int maliciousId = (targetNodeId + i + 1) % MAX_NODES;
            addNode(maliciousId, true);

            // Get the newly added malicious node
            Node *maliciousNode = getNode(maliciousId);
            if (maliciousNode)
            {
                // Set up the malicious node to "eclipse" the target node
                maliciousNode->successor = targetNode->successor;
                targetNode->successor = maliciousNode;
            }
        }
    }
};

void simulator()
{
    cout << "############################################################\n";
    cout << "************** Simulator with Eclipse Attack ***************\n";
    cout << "############################################################\n";
}

void showMenu()
{
    cout << "\n>>>>>>>>>>>>>>>>> Menu <<<<<<<<<<<<<<<<<<<<<" << endl;
    cout << "\t1. Add Node" << endl;
    cout << "\t2. Remove Node" << endl;
    cout << "\t3. Store Data" << endl;
    cout << "\t4. Look Up Data" << endl;
    cout << "\t5. Display CHORD Ring" << endl;
    cout << "\t6. Simulate Eclipse Attack" << endl;
    cout << "\t7. Exit" << endl;
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
            cout << "Enter Node ID to look up data: ";
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
            int targetNodeId, numMaliciousNodes;
            cout << "Enter Target Node ID for eclipse attack: ";
            cin >> targetNodeId;
            cout << "Enter number of malicious nodes to add: ";
            cin >> numMaliciousNodes;
            chordRing.simulateEclipseAttack(targetNodeId, numMaliciousNodes);
        }
        else if (choice == 7)
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
