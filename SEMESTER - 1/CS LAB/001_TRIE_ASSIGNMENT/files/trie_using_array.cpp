#include <bits/stdc++.h>
using namespace std;

#define endl '\n'
#define int long long

struct Node
{
    // every node will have 26 reference node for alphabets
    Node *links[26];
    // flag is indicating whether certain words ends or not
    bool flag = false;

    // function for checking if "ch" node is present or not?
    bool hasKey(char ch)
    {
        return links[ch - 'a'] != NULL;
    }

    // function for inserting a node at character "ch" node
    void put(char ch, Node *node)
    {
        links[ch - 'a'] = node;
    }

    // getting the reference node at "ch" node
    Node *get(char ch)
    {
        return links[ch - 'a'];
    }

    // function for setting the end of word
    void setEnd()
    {
        flag = true;
    }

    // checking if node word end or not
    bool isEnd()
    {
        return flag;
    }
};

// TRIE Data Structure
class Trie
{
private:
    Node *root;

public:
    // constructor function for Trie class
    Trie()
    {
        root = new Node();
    }

    // insert operation in trie data structure
    void insert(string word)
    {
        Node *curr = root;
        for (auto ch : word)
        {
            // check if there "ch" node is present in currNode or not?
            if (!curr->hasKey(ch))
            {
                // if not present then create a node & put that character
                curr->put(ch, new Node());
            }

            // move to the next node at "ch"
            curr = curr->get(ch);
        }
        // mark the end of word in the trie
        curr->setEnd();
    }

    // function for searching a word in trie
    bool search(string word)
    {
        Node *curr = root;
        for (auto ch : word)
        {
            // if "ch" is not present then word doesn't exist
            if (!curr->hasKey(ch))
                return false;

            // "ch" node is present then move to that node
            curr = curr->get(ch);
        }
        // finally check if the search word ends here or not?
        return curr->isEnd();
    }
    // function for checking if there is any words startWith "prefix"
    bool startWith(string prefix)
    {
        Node *curr = root;
        for (auto ch : prefix)
        {
            // check if "ch" node is present or not?
            if (!curr->hasKey(ch))
                return false;
            // move to reference node
            curr = curr->get(ch);
        }
        // return true as we have word startWith "prefix"
        return true;
    }

    // delete functionality on trie
    void erase(string word)
    {
        Node *curr = root;
        for (auto ch : word)
        {
            // check if "ch" is present or not?
            if (!curr->hasKey(ch))
                return;

            curr = curr->get(ch);
        }
        // for deleting it just mark end to be false
        curr->flag = false;
    }
};

signed main()
{
    int n;
    cout << "Number of words to input: ";
    cin >> n;

    Trie *trie = new Trie();
    for (int i = 1; i <= n; i++)
    {
        string s;
        cin >> s;
        trie->insert(s);
    }

    while (true)
    {
        cout << "What do you want to do with trie(enter S - for search, D - for delete): ";
        char ch;
        cin >> ch;
        if (ch == 'S')
        {
            string s;
            cout << "Enter string to search: ";
            cin >> s;

            if (trie->search(s))
                cout << "String " << s << " is present in trie." << endl;
            else
                cout << "String " << s << " is not present in trie." << endl;
        }
        else if (ch == 'D')
        {
            string d;
            cout << "Enter a string to delete : ";
            cin >> d;

            trie->erase(d);
        }
        else
        {
            cout << "Please enter correct key!!." << endl;
            continue;
        }

        cout << "Do you want to stop(press Y)?: ";
        char c;
        cin >> c;
        if (c == 'Y')
            break;
    }

    return 0;
}
