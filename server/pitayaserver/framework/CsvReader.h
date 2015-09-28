/*
 * =====================================================================================
 *
 *       Filename:  CsvReader.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月22日 10时26分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _CSVREADER_H_
#define _CSVREADER_H_
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

class CCsvReader
{
	public:
		bool Init(std::string content);
		bool InitByFile(std::string path);
		std::string GetCell(int line, int row);
		std::string GetErrorStr();
		int GetLineCount();
		int GetRowCount(); // first line 's row number
		void Dump();
	private:
		int ReadCell(std::string &line, std::string &cell);
		std::vector<std::vector<std::string> >  m_table;
		std::string m_ErrorStr;
};

#endif
