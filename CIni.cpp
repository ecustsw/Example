#include <cerrno>
#include <cstring>
#include <cctype>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "CIni.h"

#define EACH_LEN 128

CIni::CIni()
{
    m_state = NEW;
}

CIni::~CIni()
{

}

void CIni::loadIni(const char *file)
{
    m_fileno = open(file,O_RDONLY);
    if(m_fileno < 0)
    {
        printf("error when open ini file:%s",strerror(errno));
        return;
    }

    parseFile();

    close(m_fileno);
}

int CIni::parseFile()
{
    int ret = 0;
    char buf[EACH_LEN] = {0};//size of each read
    int cur = 0;
    while(1)
    {
        if(cur == 0 || cur == EACH_LEN ||buf[cur] == '\0')//read next
        {
            memset(buf,0,sizeof(buf));
            if((ret = next(buf,EACH_LEN)) < 0)
            {
                printf("error when read ini file:%s",strerror(errno));
                break;
            }
            if(buf[0] == '\0')//end of file
                break;
        }
        if((cur = state(buf,cur)) < 0)//may look forward
            break;
        cur++;
    }

    return ret;
}

int CIni::state(const char *ptr,int pos)
{
    int ret = pos;
    char c = ptr[pos];
    char cc = 0;

    if(m_state == VALUEEND || m_state == COMMEMTENDED)
    {
        if(c == '[')
        {
            m_state = NEW;
            m_lastSection.clear();
        }
        else if(isspace(c))
        {
            return ret;
        }
        else
        {
            m_state = RECEIVERIGHTSQUARE;
            m_lastV.clear();
        }
    }

    switch(m_state)
    {
        case NEW :
        case NEWPENDING :
            if(c == '[')
            {
                m_state = RECEIVELEFTSQUARE;
                m_lastSection.clear();
            }
            else if(c == '/')
            {
                if(pos + 1 == EACH_LEN)
                {
                    m_state = NEWPENDING;
                }
                else if((cc = ptr[pos + 1]) == '\0')//end of file
                {
                    ret = -1;
                    printf("parse error unexpected end%s-%d",__FILE__,__LINE__);
                    return ret;
                }
                else
                {
                    if(cc == '/')
                    {
                        m_lastState = m_state;
                        m_state = COMMEMTENDING;
                        ret = pos + 1;//skip next
                    }
                    else//begin of file is not comment,not okey!
                    {
                        ret = -1;
                        printf("parse error %s-%d",__FILE__,__LINE__);
                    }
                }
            }
            else if(isblank(c))
            {
                //skip it
            }
            else
            {
                ret = -1;
                printf("parse error %s-%d",__FILE__,__LINE__);
            }
            break;
        case RECEIVELEFTSQUARE :
            if(c == ']')
            {
                INISECTION section;
                m_iniFile.push_back({m_lastSection,section});
                m_state = RECEIVERIGHTSQUARE;
                m_lastV.clear();
                break;
            }
            else if(isdigit(c) || isalpha(c))
            {
                m_lastSection.append({c});
            }
            else
            {
                ret = -1;
                printf("parse error %s-%d",__FILE__,__LINE__);
            }
            break;
        case RECEIVERIGHTSQUARE :
            if(pos + 1 == EACH_LEN)
            {

            }
            else if((cc = ptr[pos + 1]) == '\0')
            {
                if(m_lastV.size() > 0)
                {
                    ret = -1;
                    printf("parse error,incomplete key %s-%d",__FILE__,__LINE__);
                    return ret;
                }
            }
            else if(c == '/' && cc == '/')
            {
                m_lastState = m_state;
                m_state = COMMEMTENDING;
                ret = pos + 1;//skip next
            }
            else
            {
                if(c == '=')
                {
                    m_state = RECEIVEEQUAL;
                    Set(m_lastSection.c_str(),m_lastV.c_str());
                    m_lastV.clear();
                    break;
                }
                else if(c == '\n')
                {
                    if(m_lastV.size() == 0)
                        break;

                    ret = -1;
                    printf("parse error,no allowed new line %s-%d",__FILE__,__LINE__);
                }
                else if(isspace(c))
                {

                }
                else
                {
                    m_lastV.append({c});
                }
            }
            break;
        case RECEIVEEQUAL :
            if(pos + 1 == EACH_LEN)
            {

            }
            else if((cc = ptr[pos + 1]) == '\0')//deal the situation: no new line at end of file!!
            {
                m_lastV.append({c});
                Set(m_lastSection.c_str(),m_lastV.c_str());
                m_lastV.clear();
            }
            else
            {
                if(c == '\n' || (c == '/' && cc == '/'))//which state to go?
                {
                    if(c == '\n')
                        m_state = VALUEEND;//next loop to decide which state to go
                    else
                        m_state =  COMMEMTENDING;

                    Set(m_lastSection.c_str(),m_lastV.c_str());
                    m_lastV.clear();
                }
                else if(isspace(c)) 
                {

                }
                else
                {
                    m_lastV.append({c});
                }   
            }
            break;
        case COMMEMTENDING :
            if(c == '\n')
            {
                m_state = COMMEMTENDED;
            }
            break;
        default:
            break;
    }
    return ret;
}

int CIni::next(char *buf,int size)
{
    int ret = read(m_fileno,buf,size);
    return ret;
}

//first is key,next is value.according to ini file pattern
void CIni::Set(const char *section,const char *v)
{
    INIFILEITE ite = m_iniFile.begin();
    for(;ite != m_iniFile.end();ite++)
    {
        if(memcmp(ite->first.c_str(),section,ite->first.size()) == 0)
            break;
    }
    if(ite != m_iniFile.end())
    {
        ite->second.push_back(v);
    }
    return;
}

void CIni::Get(const char *section,const char *key,char *value)
{
    INIFILEITE ite = m_iniFile.begin();
    for(;ite != m_iniFile.end();ite++)
    {
        if(memcmp(ite->first.c_str(),section,ite->first.size()) == 0)
            break;
    }
    if(ite != m_iniFile.end())
    {
        INISECTIONITE ite1 = ite->second.begin();
        for(;ite1 != ite->second.end();)
        {
            if(memcmp(key,ite1->c_str(),ite1->size()) == 0)
            {
                ite1++;
                memcpy(value,ite1->c_str(),ite1->size());
                value[ite1->size()] = '\0';
                break;
            }
            ite1 += 2;
        }
    }  
    return;  
}