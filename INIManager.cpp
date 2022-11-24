#include "INIManager.h"
#include <fstream>

INIManager::INIManager(void)
{
    SetRoot("Config");
    SetFilePath("Config.ini");
    ReadFile();
}

INIManager::INIManager(const string &file_path, const string &root_name)
{
    SetRoot(root_name);
    SetFilePath(file_path);
    ReadFile();
}

INIManager::~INIManager(void)
{
    WriteFile();
}

string INIManager::GetValue(const string &key) const
{
    return GetValue(m_root, key);
}

string INIManager::GetValue(const string &root, const string &key) const
{
    if (key.empty()) {
        return "";
    }

    auto itr = m_map.find(root);
    if (itr != m_map.end()) {
        auto s_itr = itr->second.find(key);
        if (s_itr != itr->second.end()) {
            return s_itr->second;
        }
    }

    return "";
}

int INIManager::SetValue(const string &key, const string &val)
{
    return SetValue(m_root, key, val);
}

int INIManager::SetValue(const string &root, const string &key, const string &val)
{
    if (key.empty()) {
        return -1;
    }

    auto itr = m_map.find(root);
    if (m_map.end() == itr) {
        SubNode tmp_node;
        tmp_node.insert(pair<string, string>(key, val));
        m_map.insert(pair<string, SubNode>(root, tmp_node));
    } else {
        itr->second[key] = val;
    }

    return 0;
}

void INIManager::WriteFile(void)
{
    ofstream fs;

    fs.open(m_file_path, ios::out);
    if (!fs.is_open()) {
        return;
    }

    for (auto &itr : m_map) {
        fs << "[" << itr.first << "]" << std::endl;
        for (auto &s_itr : itr.second) {
            fs << s_itr.first << " = " << s_itr.second << std::endl;
        }
    }

    fs.close();
}

void INIManager::ReadFile(void)
{
    ifstream fs;

    fs.open(m_file_path, ios::in);
    if (!fs.is_open()) {
        return;
    }

    string line;
    string root;
    string key;
    string value;
    int line_size = 0;
    int equal_pos = 0;

    while (getline(fs, line)) {
        line_size = line.size();
        equal_pos = line.find('=');

        if ((0 == line.find('['))
            && (line.find(']') == line_size - 1)) {
            root = line.substr(1, line_size - 2);
        } else if (equal_pos != string::npos) {
            key = line.substr(0, equal_pos - 1);
            value = line.substr(equal_pos + 2);
        }

        SetValue(root, key, value);
    }

    fs.close();
}
