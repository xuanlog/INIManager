#ifndef _INI_MANAGER_H_
#define _INI_MANAGER_H_

#define USE_UNORDERED_MAP   1   /* 使用unordered_map */

#if USE_UNORDERED_MAP
#include <unordered_map>
#else
#include <map>
#endif
#include <string>

#define CREATE_MEMBER_VAL_FUNC(Type, FuncName, MemberName, Permission) \
Permission: \
    Type m_##MemberName; \
\
public: \
    void Set##FuncName(const Type &tmp_val) { \
        m_##MemberName = tmp_val; \
    } \
\
    Type Get##FuncName(void) const { \
        return m_##MemberName; \
    }

using namespace std;

#if USE_UNORDERED_MAP
using SubNode = unordered_map<string, string>;
#else
using SubNode = map<string, string>;
#endif

class INIManager {
public:
    INIManager(void);

    INIManager(const string &file_path, const string &root_name);

    virtual ~INIManager(void);

    string GetValue(const string &key) const;

    string GetValue(const string &root, const string &key) const;

    int SetValue(const string &key, const string &val);

    int SetValue(const string &root, const string &key, const string &val);

    int GetSize(void) const { return m_map.size(); }

    CREATE_MEMBER_VAL_FUNC(string, Root, root, private)
    CREATE_MEMBER_VAL_FUNC(string, FilePath, file_path, private)

private:
    void WriteFile(void);

    void ReadFile(void);

#if USE_UNORDERED_MAP
    unordered_map<string, SubNode> m_map;
#else
    map<string, SubNode> m_map;
#endif
};

#endif
