#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#define endl '\n'

struct Node
{
    char ch;
    Node *sibling;
    Node *child;
    bool isEnd;
    Node(char ch)
    {
        this->ch = ch;
        this->child = NULL;
        this->sibling = NULL;
        this->isEnd = false;
    }
};

class Trie
{
private:
    Node *root;

    void helper(Node *curr, string s)
    {
        if (curr == NULL)
        {
            return;
        }
        if (curr->ch != '$')
        {
            s += curr->ch;
        }
        if (curr->isEnd)
        {
            cout << s << endl;
        }

        // move to child node for next character in node
        helper(curr->child, s);
        // remove current sibiling character & move to next sibiling
        helper(curr->sibling, s.substr(0, s.length() - 1));
    }

public:
    Trie()
    {
        root = new Node('$');
    }

    void insert(string word)
    {
        Node *curr = root;
        for (auto c : word)
        {
            // Case - 1: there is no child node
            if (curr->child == NULL)
            {
                // so, we can attach a new node here
                Node *node = new Node(c);
                curr->child = node;
                curr = curr->child;
            }
            // Case - 2: there is a child node
            else
            {
                // Case - 2.A: character is already present
                if (curr->child->ch == c)
                {
                    // So, no need to insert
                    curr = curr->child;
                }
                // Case - 2.B: character is not same as child node
                else
                {
                    if (curr->child->ch > c)
                    {
                        Node *node = new Node(c);
                        node->sibling = curr->child;
                        curr->child = node;
                        curr = curr->child;
                    }
                    else
                    {
                        curr = curr->child;
                        // search if character is already present
                        while (curr->sibling != NULL && curr->sibling->ch < c)
                        {
                            curr = curr->sibling;
                        }
                        // character is not present so, we need to insert it
                        if (curr->sibling == NULL)
                        {
                            Node *node = new Node(c);
                            curr->sibling = node;
                        }
                        else if (curr->sibling->ch != c)
                        {
                            Node *node = new Node(c);
                            node->sibling = curr->sibling;
                            curr->sibling = node;
                        }
                        // move your pointer
                        curr = curr->sibling;
                    }
                }
            }
        }
        // mark the end as true, to declare that word is present
        curr->isEnd = true;
    }

    bool search(string word)
    {
        Node *curr = root;
        for (auto c : word)
        {
            // CASE - 1: child node is not present
            if (curr->child == NULL)
            {
                // so, word is not present
                return false;
            }
            // Case - 2: there is a child node
            curr = curr->child;
            // Case - 2.A: character is not present at node
            if (curr->ch != c)
            {
                // so, we need to search it in sibiling node
                while (curr->sibling != NULL && curr->sibling->ch != c)
                {
                    curr = curr->sibling;
                }
                // case, when character is not present in sibiling node also
                if (curr->sibling == NULL)
                {
                    return false;
                }
                curr = curr->sibling;
            }
        }
        return curr->isEnd;
    }

    void sortedPrint()
    {
        Node *curr = root;
        helper(curr, "");
    }
};

void printChoice()
{
    cout << "Enter 1 for inserting a word !!" << endl;
    cout << "Enter 2 for searching a word !!" << endl;
    cout << "Enter 3 for printing words in sorted order !!" << endl;
    cout << "Enter 4 for quitting !!" << endl;
}

int main()
{
    Trie *trie = new Trie();

    // Now populate the trie with few words
    // opening words.txt file
    ifstream myFile("words.txt");

    // Checking if file opened correctly or not
    if (!myFile)
    {
        cout << "Error opening the file." << endl;
        myFile.close();
        return 1;
    }

    string line;
    while (!myFile.eof())
    {
        getline(myFile, line);
        trie->insert(line);
    }
    // Close the file
    myFile.close();

    while (true)
    {
        cout << "####################################################\n";
        cout << "\t\tInstruction for Trie Operations" << endl;
        cout << "####################################################\n";

        printChoice();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 1)
        {
            cout << "Enter word to insert : ";
            string s;
            cin >> s;
            trie->insert(s);
        }
        else if (choice == 2)
        {
            cout << "Enter word to search : ";
            string s;
            cin >> s;
            cout << endl;
            if (trie->search(s))
            {
                cout << "!!! Word found in trie !!!" << endl;
            }
            else
            {
                cout << "!!! Word is not present in trie !!!" << endl;
            }
        }
        else if (choice == 3)
        {
            trie->sortedPrint();
        }
        else if (choice == 4)
        {
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            cout << "\t\tOperations is completed" << endl;
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            break;
        }
        else
        {
            cout << "Please enter choice according to instruction";
        }
        cout << endl;
    }

    delete trie;
    return 0;
}
