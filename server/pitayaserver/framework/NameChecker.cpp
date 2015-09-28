/*
 * NameChecker.cpp
 *
 *  Created on: 2011-12-9
 *      Author: yq
 */

#include "NameChecker.h"
#include "Trie.h"
#include <iostream>
#include <fstream>
#include <string>
#include <memory.h>
#include "Log.h"

Trie CNameChecker::m_trie;
bool CNameChecker::Init(const char * szFileName)
{
    std::ifstream fin(szFileName);
    std::string s;
    while( getline(fin,s) )
    {
    	m_trie.insert(s.c_str());
    	//IME_LOG("%s",s.c_str());
    	//IME_LOG("%d",s.size());
    }
    //IME_ERROR("testtestetestsetsetse");
    //m_trie.insert("SNDA");

	return true;
}
void CNameChecker::ReplaceString(std::string &strSource)
{
	int i = 0;
	while(ReplaceStringSingle(strSource))
	{
		if (i++ > 16)
			break;
	}
}
bool CNameChecker::ReplaceStringSingle(std::string &strSource)
{
	char szBuf[1024];
	if (strSource.size() > 1000)
		return false;
	sprintf(szBuf,"%s",strSource.c_str());
	IME_LOG("%s",szBuf);
	int nLen = strSource.size();
	bool bFlag = false;
	int nStart,nEnd;
	nStart = nEnd = 0;
	for(int i = 0; !bFlag && i < nLen; ++i)
		for(int j = i + 1; j < nLen; ++j)
		{
			char szTmp[1024];
			memset(szTmp,0,sizeof(szTmp));
			memcpy(szTmp,szBuf+i,j - i + 1);
			//IME_LOG("%s",szTmp);
			if (m_trie.search(szTmp))
			{
				//find
				bFlag = true;
				nStart = i;
				nEnd = j;
				break;
			}
		}

	if (bFlag)
	{
		IME_ERROR("keyword name is %s",strSource.c_str());
		char szTmp[1024];
		memset(szTmp,0,sizeof(szTmp));
		memcpy(szTmp,szBuf,nStart);
		std::string strTmp;
		strTmp = szTmp;
		strTmp += "***";
		if (nEnd < nLen)
		{
			memset(szTmp,0,sizeof(szTmp));
			memcpy(szTmp,szBuf + nEnd + 1,nLen - nEnd - 1);
			strTmp += szTmp;
		}

		strSource = strTmp;
		return true;
	}
	return false;
}

bool CNameChecker::IsDirtyWord(std::string &strSource)
{
	char szBuf[1024];
	if (strSource.size() > 1000)
		return false;
	sprintf(szBuf,"%s",strSource.c_str());
	int nLen = strSource.size();
	for(int i = 0; i < nLen; ++i)
		for(int j = i + 1; j < nLen; ++j)
		{
			char szTmp[1024];
			memset(szTmp,0,sizeof(szTmp));
			memcpy(szTmp,szBuf+i,j - i + 1);
			//IME_LOG("%s",szTmp);
			if (m_trie.search(szTmp))
			{
				//find
				return true;
			}
		}
	return false;
}


