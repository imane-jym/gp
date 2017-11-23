/*
 * =====================================================================================
 *
 *       Filename:  CsvReader.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月22日 10时23分42秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CsvReader.h"
#include "Util.h"

using namespace std;

bool CCsvReader::Init(std::string content)
{
	m_table.clear();
	m_ErrorStr = "";

	stringstream ss(content);
	string line;
	string one;
	vector<string> oneline;
	int ret = 1;
	while(getline(ss, line))
	{
		if (line[0] == '#')
			continue;
		if (!line.empty() && line[line.length() - 1] == '\r')
			line.erase(line.length() -1 );
		if (ret == 1)
		{
			if (line.empty())
				continue;
			one += line;
		}
		else
		{
			one += "\n" + line;
		}
		do
		{
			std::string cell;
			ret = ReadCell(one, cell);
			if (ret == 0) // still have cell in this line
			{
				oneline.push_back(cell);
				continue;
			}
			else if (ret == 1) //no other cells in this line. It is over.
			{
				oneline.push_back(cell);
				m_table.push_back(oneline);
				oneline.clear();
				one = "";
				break;
			}
			else // It has cell. but line is end. need more content
			{
				break;
			}
		}while(1);

//		CUtil::StrSplit(line, ",", oneline);
	}	
	if (ret != 1)
		return false;
	return true;
}

int CCsvReader::ReadCell(std::string &line, std::string &cell)
{
	int quote_count = 0;
	int open = 0;
	int len = 0;
	cell.clear();
	for (unsigned int i = 0; i < line.size(); i++)
	{
		if (line[i] == '"')
		{
			if (i + 1 < line.size() && line[i + 1] == '"')
			{
				cell += '"'; 
				len = len + 2;	
				i++;
			}
			else
			{
				quote_count++;
				open = quote_count & 1 ? 1 : 0;
				len++;
			}
		}
//		else if (line[i] == '\\')
//		{
//			if (i + 1 < line.size() && line[i + 1] == '"')
//			{
//				cell += '"'; 
//				len = len + 2;	
//				i++;
//			}
//			else
//			{
//				cell += line[i];
//				len++;
//			}
//		}
		else if (line[i] == ',')
		{
			if (open)
			{
				cell += line[i];
				len++;
			}
			else
			{
				len++;	
				line = line.substr(len);
				return 0;
			}
		}
		else
		{
			cell += line[i];
			len++;
		}
	}

	if (open)
	{
		return 2;
	}
	else 
	{
		if (len != 0)
			line = line.substr(len);
		return 1;
	}
}

bool CCsvReader::InitByFile(std::string path)
{
	m_table.clear();
	m_ErrorStr = "";
	fstream fs(path.c_str());
	if (fs.fail())
		return false;

	string line;
	string one;
	vector<string> oneline;
	int ret = 1;
	while(getline(fs, line))
	{
		if (line[0] == '#')
			continue;
		if (!line.empty() && line[line.length() - 1] == '\r')
			line.erase(line.length() -1 );
		if (ret == 1)
		{
			if (line.empty())
				continue;
			one += line;
		}
		else
		{
			one += "\n" + line;
		}
		do
		{
			std::string cell;
			ret = ReadCell(one, cell);
			if (ret == 0) // still have cell in this line
			{
				oneline.push_back(cell);
				continue;
			}
			else if (ret == 1) //no other cells in this line. It is over.
			{
				oneline.push_back(cell);
				m_table.push_back(oneline);
				oneline.clear();
				one = "";
				break;
			}
			else // It has cell. but line is end. need more content
			{
				break;
			}
		}while(1);

//		CUtil::StrSplit(line, ",", oneline);
	}	
	if (ret != 1)
		return false;
	return true;
}

std::string CCsvReader::GetCell(int line, int row)
{
	if ((size_t)line < m_table.size() && (size_t)row < m_table[line].size())
	{
		return m_table[line][row];
	}
	m_ErrorStr = CUtil::FormatError("unvalid line %u row %u", line, row);
	return "";
}

std::string CCsvReader::GetErrorStr()
{
	return m_ErrorStr;
}

int CCsvReader::GetLineCount()
{
	return m_table.size();
}

int CCsvReader::GetRowCount()
{
	if (m_table.size() <= 0)
		return 0;
	return m_table[0].size();
}

void CCsvReader::Dump()
{
	for (unsigned int i = 0; i < m_table.size(); i++)
	{
		printf("=====row %u======\n", i);
		for (unsigned int j = 0; j < m_table[i].size(); j++)
		{
			printf("cell is [%s]\n", m_table[i][j].c_str());
		}
	}
}
