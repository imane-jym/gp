#include"ConfigReader.h"
#include<cstdio>

//hidden private function
bool readRow(FILE*fp, CConfigReader::row_type& pRow);
char readCell(FILE*fp, CConfigReader::cell_type& pCell);

bool CConfigReader::init(CConfigReader::path_type path) {
	if( _initialized ) return false;
	
	FILE* fp = fopen(path.c_str(), "r");
	if( !fp ) return false;
	
	while(true) {
		row_type current_row;
		if( !readRow(fp,current_row) ) break;
		_table.push_back(current_row);
	}

	fclose(fp);
	return (_initialized = true);
}

bool readRow(FILE *fp, CConfigReader::row_type &pRow) {
	while(true) {
		CConfigReader::cell_type current_cell;
		char c=readCell(fp,current_cell);
		if( current_cell.size()>0 || c!=EOF )
			pRow.push_back(current_cell);
		if( c!=',' )break;
	}
	return ( pRow.size()>0 );
}

char readCell(FILE *fp, CConfigReader::cell_type &pCell) {
	char c;
	while(true) { //leading \r \n
		c=fgetc(fp);
		if( c==EOF ) return c; //�ļ���β
		if( c!='\r' && c!='\n' ) break;
	}
	int quote_count = 0;
	if( c=='"' ) {
		quote_count = 1;
		c=fgetc(fp);
	}
	while(true) {
		if( c=='"' ) {
			quote_count++;
			if( quote_count&1 ) pCell+=c; //����������ʱ�������
		} else if( c==',' ) {
			if( quote_count&1 ) pCell+=c; //����������ʱ','���Ƿָ���
			else break;
		} else if( c=='\n' || c=='\r' || c==EOF ) {
			break;
		} else {
			pCell+=c;
		}
		c=fgetc(fp);
	}
	return c; //�������һ���ַ�
}
