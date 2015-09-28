#pragma once

#ifndef _CONFIG_READER_H_
#define _CONFIG_READER_H_

#include<vector>
#include<string>
#include<cassert>

//.csv file reader
class CConfigReader {
public:
	typedef std::string path_type;
	typedef std::string cell_type;
	typedef std::vector<cell_type> row_type;
	typedef std::vector<row_type> table_type;
	typedef unsigned int size_type;
	
	CConfigReader():_initialized(false),m_error(0){}
	
	bool init(path_type path);

	//property
	bool initialized() const {
		return _initialized;
	}
	size_type size() const {
		return _table.size();
	}
	size_type rowCount() const {
		return _table.size();
	}
	size_type colCount() {
		if( !initialized() ) 
                {
                    m_error = 1;
                    return 0;
                }
		if( !_table.size() )
                { 
                    m_error = 1;
                    return 0;
                }
		return _table[0].size();
	}
	size_type colCount(int nRow) {
		if ( !initialized() )
                {
                    m_error = 1;
                    return 0;
                }
		if ((size_t)nRow >= _table.size())
                {
                    m_error = 1;
                    return 0;
                }
		return _table[nRow].size();
	}
	row_type row(size_type r) {
		if( r>=_table.size() )
                {
                    m_error = 1;
                    return row_type();
                }
		return _table[r];
	}
	cell_type cell(size_type r,size_type c) {
		row_type current_row = row(r);
		if( c>=current_row.size() )
                {
                    m_error = 1;
                    return cell_type();
                }
		return current_row[c];
	}
        //reset errno after every call
        bool is_error()
        {
            int temp = m_error;
            m_error = 0;
            return temp;
        }

private:
	table_type _table;
	bool _initialized;
    int m_error;
};

#endif
