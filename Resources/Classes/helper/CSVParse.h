#ifndef _CSVPARSE_
#define _CSVPARSE_

#include "cocos2d.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;
/**CSV文件 解析类 注意：mac 上的excel导出的csv格式有问题*/
class CSVParse : public cocos2d::Ref
{
    
public:
    CSVParse(string sep = ","):
    fieldsep(sep),
    cols(0)
    {}
    ~CSVParse();
private:
    string                      fieldsep;        // separator characters
    vector<vector<string> >     data;
    int                         cols;
    
private:    
    void    split(vector<string>& field,string line);
    int     advplain(const string& line, string& fld, int);
    int     advquoted(const string& line, string& fld, int);
    
public: 
	static CSVParse* create(const char* csvFile);
	static CSVParse* createMemery(const char* mem, unsigned int size);

	//打开文件
	bool            openFile(const char* fileName);
	bool            openFile2(const char* fileName);
	bool			openMemery(const char* mem, unsigned int size);
    //获取数据
    const char* getData(unsigned int rows,unsigned int cols);
	const char* getDatas(unsigned int rows,const char* colsName);
    //判断
    int             findColsData(unsigned int cols,const char* value);

	int				findRowsData(unsigned int rows, const char* value);
    //获取一共的列数
    inline  int     getCols(){return cols;}
    //获取文件的行数
    inline  int     getRows(){return data.size();}

	int				getDataValue(unsigned int rows, unsigned int cols);
	int				getDataValues(unsigned int rows, const char* colsName);
	float			getDataFloat(unsigned int rows, unsigned int cols);
	float			getDataFloats(unsigned int rows, const char* colsName);
	float			getDataSecond(unsigned int rows, unsigned int cols);
	float			getDataSeconds(unsigned int rows, const char* colsName);
};

#endif
