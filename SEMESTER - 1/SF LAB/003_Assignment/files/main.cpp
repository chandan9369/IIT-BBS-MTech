#include <bits/stdc++.h>
using namespace std;

#define forn(i, s, n) for (int i = s; i < n; i++)

#define pb push_back
#define pi pair<int, int>
#define vi vector<int>

#define endl '\n'
#define int long long

class Parser
{
public:
    string index; // it stores the initial index value
    void setIndex(const string &s)
    {
        index = s;
    }
    vector<string> hexBytes; // vector for storing the bytes in a single line
    Parser()
    {
        index = "";
        hexBytes.clear();
    }

    // function, which parse the line to get the data bytes
    void parseLine(const string &line)
    {
        string byte = "";
        bool isEnd = false; // flag to track if we are parsing bytes

        for (auto ch : line)
        {
            // colons
            if (ch == ':')
            {
                // byte = "";
                continue;
            }
            // vertical line
            if (ch == '|')
            {
                // case where we get it first time
                if (!isEnd)
                {
                    isEnd = true;
                    continue;
                }
                else
                {
                    // case where we get it for second time
                    // then break as we completed our parsing
                    break;
                }
            }
            // space
            if (ch == ' ')
            {
                // case where we get space while parsing index
                if (!isEnd)
                {
                    // store this index value
                    setIndex(byte);
                    // erase the byte
                }
                else
                {
                    // case, where we are parsed a byte
                    if (!byte.empty())
                    {
                        hexBytes.pb(byte); // store byte for calculation
                    }
                    byte.clear();
                }
            }
            // data byte
            else
            {
                byte += ch; // store data byte for calculation
            }
        }
    }

    // printing all bytes stored while parsing line
    void displayBytes()
    {
        cout << "\nParsed Bytes: " << endl;
        for (auto b : hexBytes)
        {
            cout << b << " ";
        }
        cout << endl;
    }
};

// class for file properties
class FileNode
{
public:
    string name;
    bool isDeleted;

    FileNode(string fileName)
    {
        name = fileName;
        isDeleted = false;
    }
};

// function to get the value for hex character
int hexValue(char ch)
{
    if (ch >= 'A' && ch <= 'F')
    {
        return (ch - 'A') + 10;
    }
    else if (ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }
    else
    {
        throw invalid_argument("Invalid hex character");
    }
}

// function for converting hexString to decimal
int hexToDecimal(string hexStr)
{
    int decimal = 0;
    forn(i, 0, hexStr.length())
    {
        int val = hexValue(hexStr[i]);
        decimal = decimal * 16 + val;
    }
    return decimal;
}

// function for converting decimal value to hex string
string decimalToHex(int decimal)
{
    stringstream ss;
    ss << hex << uppercase << decimal;
    return ss.str();
}

// function which convert the little endian to big endian
string littleToBig(const string &byte1, const string &byte2)
{
    return byte2 + byte1;
}

int getValue(string a, string b)
{
    string byte = littleToBig(a, b);
    int val = hexToDecimal(byte);

    return val;
}

signed
main()
{
    ifstream file;
    file.open("./hexText.txt");

    if (!file)
    {
        cerr << "Error while opening file !!" << endl;
        return 1;
    }

    // TASK - 1
    // reading a line from txt file
    string line;
    getline(file, line);

    // initialize the parser
    Parser obj;

    // parse a single line first to do task 1
    obj.parseLine(line);

    int sectorSize = getValue(obj.hexBytes[11], obj.hexBytes[12]);
    cout << "Sector size: " << sectorSize << endl;
    int reservedArea = getValue(obj.hexBytes[14], obj.hexBytes[15]);
    cout << "Number of reserved areas: " << reservedArea << endl;

    int fats = hexToDecimal(obj.hexBytes[0]);
    int sectorPerFats = getValue(obj.hexBytes[6], obj.hexBytes[7]);

    int rootDirOffset = (reservedArea + fats * sectorPerFats) * sectorSize;

    cout << "Offset for Root directory: " << decimalToHex(rootDirOffset) << endl;

    rootDirOffset >>= 4;
    rootDirOffset -= 2;

    // TASK - 2: extract file name in root directory
    vector<FileNode> files;
    string fileName = "";
    while (getline(file, line))
    {
        // initialize the parser
        obj.parseLine(line);

        if (obj.hexBytes[0] == "00")
            break;

        if (obj.hexBytes[11] == "0F")
        {
            string tempName;
            for (int i = 1; i <= 10; i += 2)
            {
                if (obj.hexBytes[i] != "FF" && obj.hexBytes[i] != "00")
                    tempName += static_cast<char>(hexToDecimal(obj.hexBytes[i]));
            }

            if (obj.hexBytes[14] != "FF" && obj.hexBytes[14] != "00")
                tempName += static_cast<char>(hexToDecimal(obj.hexBytes[14]));

            getline(file, line);
            obj.parseLine(line);

            for (int i = 0; i <= 9; i += 2)
            {
                if (obj.hexBytes[i] != "FF" && obj.hexBytes[i] != "00")
                    tempName += static_cast<char>(hexToDecimal(obj.hexBytes[i]));
            }

            for (int i = 12; i <= 15; i += 2)
            {
                if (obj.hexBytes[i] != "FF" && obj.hexBytes[i] != "00")
                    tempName += static_cast<char>(hexToDecimal(obj.hexBytes[i]));
            }

            fileName = tempName + fileName;
        }
        else
        {
            if (!fileName.empty())
            {
                FileNode node(fileName);
                if (obj.hexBytes[0] == "E5")
                    node.isDeleted = true;

                files.pb(node);
                fileName.clear();
            }
        }
    }

    // Optionally display parsed file information
    cout << "Parsed Files:" << endl;
    for (const auto &file : files)
    {
        cout << (file.isDeleted ? "[Deleted] " : "[Active] ") << file.name << endl;
    }
    return 0;
}