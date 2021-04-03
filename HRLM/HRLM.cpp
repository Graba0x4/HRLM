#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
#include <map>
#include <string>
using namespace std;

vector<string> split(string str, string delimiter);

class Tag
{
private:
    map<string, string> _attributes;
    string _name;
    vector<Tag> _childTags;
    Tag* Parent = nullptr;

public:
    Tag() {}
    Tag(string name, map<string, string> map) : _name(name), _attributes(map) {}

    Tag& GetLastChildTag()
    {
        if (_childTags.size() == 0)
            throw new exception();
        else
            return _childTags.back();
    }

    Tag* FindNestedTag(string name)
    {

        if (this->IsParentTag() == false)
            return nullptr;

        for (auto& c : _childTags)
        {
            if (c.GetTagName() == name)
                return &c;
        }

        return nullptr;
    }

    Tag* GetParent()
    {
        return this->Parent;
    }

    string GetTagName()
    {
        return _name;
    }

    bool IsParentTag()
    {
        if (_childTags.size() == 0)
            return false;
        else
            return true;
    }

    void AddChildTag(Tag& t)
    {
        t.Parent = this;
        _childTags.push_back(t);
    }

    string GetAttrValue(string attr)
    {
        for (auto& pair : _attributes)
        {
            if (pair.first == attr)
                return pair.second.substr(1, pair.second.length() - 2);
        }
        return "Not Found!";
    }

    string TagToStr(string space = "")
    {
        string tag = "";

        tag += space + "Name: " + this->_name + "\n";
        tag += space + "IsParent: ";
        tag += this->IsParentTag() ? "1" : "0";
        tag += "\n";
        tag += space + "ChildCount: ";
        tag += to_string(this->_childTags.size());
        tag += "\n";
        tag += space + "Attr:\n";

        for (auto& i : _attributes)
        {
            tag += space + "  " + i.first + ": " + i.second + "\n";
        }

        tag += space + "---------------------------------\n";

        if (this->IsParentTag())
        {
            space += "  ";

            for (auto& i : _childTags)
            {
                tag += i.TagToStr(space);
            }
        }

        return tag;
    }
};

// tagfactory
Tag Create(string line)
{


    vector<string> vec;
    string name = "";

    line.erase(line.begin(), line.begin() + 1);
    line.erase(line.end() - 1, line.end());

    name = line.substr(0, line.find(" "));

    line.erase(0, line.find(" "));

    vec = split(line, " ");


    vec.erase(vec.begin(), vec.begin() + 1); // erase tag name

    map<string, string> map;

    for (auto i = 0; i < vec.size(); i++)
    {
        if (vec[i] == "=")
        {
            map.insert(pair<string, string>(vec[i - 1], vec[i + 1]));
        }
    }


    return Tag(name, map);
}

vector<string> split(string str, string delimiter)
{
    vector<string> temp;

    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != string::npos)
    {
        token = str.substr(0, pos);
        temp.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    temp.push_back(str);
    return temp;
}


int main()
{
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */

    int N, Q;
    cin >> N >> Q;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    string input;
    vector<Tag> domHRLM;
    Tag* tag = nullptr;
    Tag temp;

    for (int i = 0; i < N; i++)
    {

        getline(cin, input);
        temp = Create(input);

        if (domHRLM.size() == 0)
        {
            domHRLM.push_back(temp);
            tag = &domHRLM.back();
            continue;
        }

        if (temp.GetTagName()[0] == '/')
        {
            if (tag != nullptr)
                tag = tag->GetParent();
        }
        else
        {
            if (tag != nullptr)
            {
                tag->AddChildTag(temp);
                tag = &tag->GetLastChildTag();
            }
            else
            {
                domHRLM.push_back(temp);
                tag = &domHRLM.back();
            }
        }
    }

    for (auto& tgs : domHRLM)
        cout << tgs.TagToStr() << endl;

    input = "";
    vector<string> queryTags;
    string attrName = "";
    for (int i = 0; i < Q; i++)
    {
        tag = nullptr;
        getline(cin, input);
        attrName = input.substr(input.find("~") + 1);
        input.erase(input.find("~"));
        queryTags = split(input, ".");

        for (string qtag : queryTags)
        {
            if (tag == nullptr)
            {
                for (auto& t : domHRLM)
                {
                    if (t.GetTagName() == qtag)
                    {
                        tag = &t;
                        break;
                    }
                }
            }
            else
                tag = tag->FindNestedTag(qtag);
        }

        if (tag != nullptr)
            cout << tag->GetAttrValue(attrName) << endl;
        else cout << "Not Found!" << endl;
    }

    domHRLM.clear();
    system("PAUSE");

    return 0;
}
