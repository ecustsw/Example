#ifndef __CINIFINE_H_
#define __CINIFILE_H_

#include <vector>
#include <string>
#include <utility>

using INIFILE = std::vector<std::pair<std::string,std::vector<std::string>>>;
using INIFILEITE = std::vector<std::pair<std::string,std::vector<std::string>>>::iterator;

using INISECTION = std::vector<std::string>;//key+value
using INISECTIONITE = std::vector<std::string>::iterator;

using STRING=std::string;

class CIni
{
public:
    CIni();
    ~CIni();

    enum ParseState
    {
        NEW = 0,
        NEWPENDING,
        RECEIVELEFTSQUARE,
        RECEIVERIGHTSQUARE,
        RECEIVEEQUAL,
        VALUEEND,
        COMMEMTENDED,
        COMMEMTENDING
    };
    
    void loadIni(const char *file);
    void Set(const char *section,const char *v);
    void Get(const char *section,const char *key,char *value);

private:
    int parseFile();
    int state(const char *ptr,int pos);
    int next(char *buf,int size);
    INIFILE m_iniFile;
    STRING m_lastV;
    STRING m_lastSection;
    int m_fileno;
    ParseState m_state;
    ParseState m_lastState;//for comment bak
};
#endif