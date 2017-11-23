#include "Util.h"
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/resource.h>

#include "Log.h"

extern volatile bool g_stopEvent;

int CUtil::timeOff = 0;

void CUtil::StrSplit(const std::string & src, const std::string & sep, std::vector<std::string> & vecRes)
{
	vecRes.clear();
	std::string s;
	for(std::string::const_iterator i = src.begin(); i != src.end(); i++)
	{
		if (sep.find(*i) != std::string::npos)
		{
			if (s.length()) vecRes.push_back(s);
			s = "";
		}
		else
		{
			s += *i;
		}
	}
	if (s.length()) vecRes.push_back(s);
}

void CUtil::StrMerge(std::string & src, const std::string & sep, const std::vector<std::string> & vecRes)
{
	src.clear();
        std::vector<std::string>::const_iterator i = vecRes.begin();
        if (i != vecRes.end())
        {
            src = *i;
            i++;
        }
	for(; i != vecRes.end(); i++)
	{
            src += sep;
            src += *i;
	}
}

void CUtil::StrSplit(const std::string & src, const std::string & sep, std::set<uint32_t> & setRes) 
{
	setRes.clear();
	std::string s;
	for(std::string::const_iterator i = src.begin(); i != src.end(); i++)
	{                             
		if (sep.find(*i) != std::string::npos)
		{                                    
			if (s.length()) setRes.insert(atol(s.c_str()));
			s = "";
		}
		else          
		{   
			s += *i;                                                                          
		}
	}
	if (s.length()) setRes.insert(atol(s.c_str()));
}

void CUtil::StrSplit(const std::string & src, const std::string & sep, std::vector<uint32_t> & vecRes) 
{
	vecRes.clear();
	std::string s;
	for(std::string::const_iterator i = src.begin(); i != src.end(); i++)
	{                             
		if (sep.find(*i) != std::string::npos)
		{                                    
			if (s.length()) vecRes.push_back(atol(s.c_str()));
			s = "";
		}
		else          
		{   
			s += *i;                                                                          
		}
	}
	if (s.length()) vecRes.push_back(atol(s.c_str()));
}

void CUtil::StrMerge(std::string & src, const std::string & sep, const std::vector<uint32_t> & vecRes)  
{
	src.clear();
	char temp[50] = {0};
	std::vector<uint32_t>::const_iterator i = vecRes.begin();
	if (i != vecRes.end())
	{
		sprintf(temp, "%u", *i);
		src = temp;
		i++;
	}
	for(; i != vecRes.end(); i++)
	{
		src += sep;
		sprintf(temp, "%u", *i);
		src += temp;
	}
}

void CUtil::StrSplit(const std::string & src, const std::string & sep, std::vector<uint64_t> & vecRes) 
{
	vecRes.clear();
	std::string s;
	for(std::string::const_iterator i = src.begin(); i != src.end(); i++)
	{                             
		if (sep.find(*i) != std::string::npos)
		{                                    
			if (s.length()) vecRes.push_back(atol(s.c_str()));
			s = "";
		}
		else          
		{   
			s += *i;                                                                          
		}
	}
	if (s.length()) vecRes.push_back(atol(s.c_str()));
}

void CUtil::StrMerge(std::string & src, const std::string & sep, const std::vector<uint64_t> & vecRes)  
{
	src.clear();
	char temp[50] = {0};
	std::vector<uint64_t>::const_iterator i = vecRes.begin();
	if (i != vecRes.end())
	{
		sprintf(temp, "%lu", *i);
		src = temp;
		i++;
	}
	for(; i != vecRes.end(); i++)
	{
		src += sep;
		sprintf(temp, "%lu", *i);
		src += temp;
	}
}

void CUtil::StrSplit(const std::string & src, const std::string & sep, std::vector<float> & vecRes) 
{
	vecRes.clear();
	std::string s;
	for(std::string::const_iterator i = src.begin(); i != src.end(); i++)
	{                             
		if (sep.find(*i) != std::string::npos)
		{                                    
			if (s.length()) vecRes.push_back(atof(s.c_str()));
			s = "";
		}
		else          
		{   
			s += *i;                                                                          
		}
	}
	if (s.length()) vecRes.push_back(atof(s.c_str()));
}
void CUtil::StrMerge(std::string & src, const std::string & sep, const std::vector<float> & vecRes)  
{
	src.clear();
	char temp[50] = {0};
	std::vector<float>::const_iterator i = vecRes.begin();
	if (i != vecRes.end())
	{
		sprintf(temp, "%f", *i);
		src = temp;
		i++;
	}
	for(; i != vecRes.end(); i++)
	{
		src += sep;
		sprintf(temp, "%f", *i);
		src += temp;
	}
}
void CUtil::StrSplit(const std::string & src, const std::string & sep, std::vector<double> & vecRes) 
{
	vecRes.clear();
	std::string s;
	for(std::string::const_iterator i = src.begin(); i != src.end(); i++)
	{                             
		if (sep.find(*i) != std::string::npos)
		{                                    
			if (s.length()) vecRes.push_back(atof(s.c_str()));
			s = "";
		}
		else          
		{   
			s += *i;                                                                          
		}
	}
	if (s.length()) vecRes.push_back(atof(s.c_str()));
}
void CUtil::StrMerge(std::string & src, const std::string & sep, const std::vector<double> & vecRes)  
{
	src.clear();
	char temp[50] = {0};
	std::vector<double>::const_iterator i = vecRes.begin();
	if (i != vecRes.end())
	{
		sprintf(temp, "%lf", *i);
		src = temp;
		i++;
	}
	for(; i != vecRes.end(); i++)
	{
		src += sep;
		sprintf(temp, "%lf", *i);
		src += temp;
	}
}

void CUtil::StrVecSplit(std::string & src, const std::string & sep1, const std::string & sep2, std::vector<std::vector<std::string> > & vecRes)
{
	vecRes.clear();
	std::vector<std::string> strVec1;
	StrSplit(src, sep1, strVec1);
	for (std::vector<std::string>::iterator iter = strVec1.begin(); iter != strVec1.end(); ++iter)
	{
		std::vector<std::string> strVec2;
		StrSplit(*iter, sep2, strVec2);
		if (strVec2.size() > 0)
			vecRes.push_back(strVec2);
	}
}
void CUtil::StrVecSplit(std::string & src, const std::string & sep1, const std::string & sep2, std::vector<std::vector<uint32_t> > & vecRes)
{
	vecRes.clear();
	std::vector<std::string> strVec1;
	StrSplit(src, sep1, strVec1);
	for (std::vector<std::string>::iterator iter = strVec1.begin(); iter != strVec1.end(); ++iter)
	{
		std::vector<uint32_t> strVec2;
		StrSplit(*iter, sep2, strVec2);
		if (strVec2.size() > 0)
			vecRes.push_back(strVec2);
	}
}

int CUtil::DaemonInit()
{
	int		i;
	pid_t	pid;

	if( ( pid = fork() ) < 0 )
		return ( -1 );
	else if( pid )
	{
		//assert( 0 );
		_exit( 0 );/* parent terminates */
	}

	/* child 1 continues... */
	if( setsid() < 0 )/* become session leader */
		return ( -1 );

	signal( SIGHUP, SIG_IGN );
	if( ( pid = fork() ) < 0 )
		return ( -1 );
	else if( pid )
	{
		//assert( 0 );
		_exit( 0 );/* child 1 terminates */
	}

	/* child 2 continues... */
	//daemon_proc = 1;/* for err_XXX() functions */

	/* close off file descriptors */
	for( i = 0 ; i < 64 ; i ++ )
		close( i );

	/* redirect stdin, stdout, and stderr to /dev/null */
	open( "/dev/null", O_RDONLY );
	open( "/dev/null", O_RDWR );
	open( "/dev/null", O_RDWR );

	//openlog(pname, LOG_PID, facility);

	return ( 0 );/* success */
}

bool CUtil::SetRLimit()
{
	//return true;

	struct rlimit limit;
	limit.rlim_cur = RLIM_INFINITY;
	limit.rlim_max = RLIM_INFINITY;

	if(-1 == setrlimit(RLIMIT_CORE, &limit))
	{
		IME_ERROR("set limit core failed");
		return false;
	}

	struct rlimit limitF;
	limitF.rlim_cur = 1024*6;
	limitF.rlim_max = 1024*6;

	if(-1 == setrlimit(RLIMIT_NOFILE, &limitF))
	{
		IME_ERROR("set limit nofile failed");
		return false;
	}
	
	return true;
}

bool CUtil::CreatePidFile()
{
	int fd = open("./.pid", O_TRUNC | O_RDWR);
	if (fd == -1)
	{
		fd = open("./.pid", O_CREAT | O_RDWR, 0644);
		if (fd == -1)
		{
			IME_ERROR("CREATE PID FILE 'pid' FAIL");
			return false;
		}
		IME_LOG("CREATE PID FILE SUCC");
	}
	pid_t pid = getpid();
	char sz_pid[64];
	memset(sz_pid, 0, sizeof(sz_pid));
	sprintf(sz_pid, "%d", pid);

	IME_LOG("WRITE PID FILE [pid=%d]", pid);
	UNUSED_RETURN(write(fd, sz_pid, strlen(sz_pid)));
	close(fd);
	return true;
}

int CUtil::RandFactor(std::vector<uint16>& value)
{
	uint32 sum = 0;
	for (unsigned int i = 0; i < value.size(); ++i)
	{
		sum += value[i];
	}
	if (!sum)
	{
		return 0;
	}
	uint32 randValue = rand() % sum;
	uint32 min = 0;
	uint32 max = 0;
	for(unsigned int i = 0; i < value.size(); ++i)
	{
		min = max;
		max += value[i];
		if (randValue >= min && randValue < max)
		{
			//IME_LOG("RAND FACTOR[RandValue=%u RetI=%u size=%u]", randValue, i, value.size());
			return i;
		}
	}
	IME_ERROR("ERROR CODE[vec size=%u]", (uint32)value.size());
	IME_LOG("ERROR CODE[vec size=%u]", (uint32)value.size());
	return -1;
}

int CUtil::RandFactor(std::vector<int>& value)
{
	int sum = 0;
	for (size_t i = 0; i < value.size(); ++i)
	{
		sum += value[i];
	}
	
	if (!sum)
	{
		return 0;
	}
	int randValue = rand() % sum;
	int min = 0;
	int max = 0;
	for(size_t i = 0; i < value.size(); ++i)
	{
		min = max;
		max += value[i];
		if (randValue >= min && randValue < max)
		{
			//IME_LOG("RAND FACTOR[RandValue=%u RetI=%u size=%u]", randValue, i, value.size());
			return i;
		}
	}
	IME_ERROR("ERROR CODE[vec size=%u]", (uint32)value.size());
	return -1;
}

std::vector<int> CUtil::RandFactorMulti(std::vector<int>& value, int number)
{
	std::vector<int> temp = value;
	std::vector<int> vecIndex;
	for (unsigned int i = 0; i < value.size(); i++)
	{
		vecIndex.push_back(i);
	}

	std::vector<int> vecResult;
	for (int i = 0; i < number; i++)
	{
		int result = CUtil::RandFactor(temp);
		if (result < (int)vecIndex.size())
		{
			vecResult.push_back(vecIndex[result]);	
			temp.erase(temp.begin() + result);
			vecIndex.erase(vecIndex.begin() + result);
		}
	}
	return vecResult;
}

int CUtil::RandFactor(const std::vector<int>& value, int nBegin, int nEnd)
{
	if ((size_t)nEnd > value.size())
	{
		nEnd = value.size();
	}

	int sum = 0;
	for (int i = nBegin; i < nEnd; ++i)
	{
		sum += value[i];
	}
	if (!sum)
	{
		return 0;
	}
	int randValue = rand() % sum;
	int min = 0;
	int max = 0;
	for(int i = nBegin; i < nEnd; ++i)
	{
		min = max;
		max += value[i];
		if (randValue >= min && randValue < max)
		{
			//IME_LOG("RAND FACTOR[RandValue=%u RetI=%u size=%u]", randValue, i, value.size());
			return i;
		}
	}
	IME_ERROR("ERROR CODE[vec size=%u]", (uint32)value.size());
	return -1;
}

bool CUtil::RandEither(uint16 value, uint16 valueMax)
{
	if (value == 0)
	{
		return false;
	}
	if (value >= valueMax)
	{
		return true;
	}
	uint32 randValue = rand() % valueMax;
//	IME_ERROR("rand %u", randValue);
	if (randValue < value)
	{
		return true;
	}
	return false;
}

void CUtil::Swap(int &a, int &b)
{
	int nTmp = a;
	a = b;
	b = nTmp;
}
//某区间 随机选择一个 [nBegin, nEnd)
int CUtil::RandByRange(int nBegin, int nEnd)
{
	if (nBegin == nEnd -1)
	{
		return nBegin;
	}

	if (nBegin == nEnd)
		return nEnd;
	if (nBegin > nEnd)
		Swap(nBegin,nEnd);
	int nRand = rand() % (nEnd - nBegin);
	return nRand + nBegin;
}

std::vector<int> CUtil::RandByRangeMulti(int nBegin, int nEnd, int nNum)
{
	std::vector<int> vecRand;
	if (nBegin == nEnd)
	{
		return vecRand;
	}
	if (nBegin > nEnd)
	{
		Swap(nBegin,nEnd);
	}
	if (nNum >= (nEnd - nBegin))
	{
		for (int i = nBegin; i < nEnd; ++i)
		{
			vecRand.push_back(i);
		}
		return vecRand;
	}
	int nRandLen = nEnd - nBegin;
	std::vector<int> vecAll(nRandLen);
	for (size_t i = 0; i < vecAll.size(); ++i)
	{
		vecAll[i] = i;
	}
	for (int i = 0; i < nNum; ++i)
	{
		int RandNum = RandByRange(0, nRandLen);
		vecRand.push_back(nBegin + vecAll[RandNum]);
		Swap(vecAll[RandNum], vecAll[nRandLen - 1]);
		--nRandLen;
	}
	return vecRand;
}

void CUtil::GetLinearList(std::vector<int>& vecValue, int nLen, float k)
{
	vecValue.resize(nLen, 0);
	for (int i = 0; i < nLen; ++i)
	{
		int nY = 5 * k * i;
		if (!nY)
		{
			nY = 1;
		}
		vecValue[nLen - i - 1] = nY;
	}
}

uint32 CUtil::PowUint32( uint32 x, uint32 y )
{
	if (!y)
	{
		return 1;
	}
	uint32 res = x;
	while (--y)
	{
		 res = res * x;
	}
	return res;
}

bool CUtil::IsToday( uint32 dwTime )
{
	time_t timeInput = dwTime;
	time_t timeNow = GetTime();
	struct tm stcTmInput;
	struct tm stcTmNow;
	localtime_r(&timeInput, &stcTmInput);
	localtime_r(&timeNow, &stcTmNow);
	if (stcTmInput.tm_year == stcTmNow.tm_year && stcTmInput.tm_yday == stcTmNow.tm_yday)
	{// 同一天
		return true;
	}
	return false;
}

bool CUtil::IsToday6AM(uint32 dwTime)
{
	time_t time6hour = 3600 * 6;
	time_t timeInput = (time_t)dwTime - time6hour;
	time_t timeNow = time(NULL) - time6hour;
	struct tm stcTmInput;
	struct tm stcTmNow;
	localtime_r(&timeInput, &stcTmInput);
	localtime_r(&timeNow, &stcTmNow);
	if (stcTmInput.tm_year == stcTmNow.tm_year && stcTmInput.tm_yday == stcTmNow.tm_yday)
	{// 同一天
		return true;
	}
	return false;
}

bool CUtil::IsTodayOffset(uint32 dwTime, int nOffset)
{
	time_t timeInput = (time_t)dwTime - nOffset;
	time_t timeNow = time(NULL) - nOffset;
	struct tm stcTmInput;
	struct tm stcTmNow;
	localtime_r(&timeInput, &stcTmInput);
	localtime_r(&timeNow, &stcTmNow);
	if (stcTmInput.tm_year == stcTmNow.tm_year && stcTmInput.tm_yday == stcTmNow.tm_yday)
	{// 同一天
		return true;
	}
	return false;
}

std::string CUtil::Uint32ToString(uint32 dwNum)
{

	std::string strRet;
	if (dwNum == 0)
		return "0";
	while(dwNum)
	{
		int nNow = dwNum % 10;
		strRet.append(1,nNow - 0 + '0');
		dwNum /= 10;
	}

	reverse(strRet.begin(),strRet.end());
	return strRet;
}

std::string CUtil::Uint64ToString(uint64 dwNum)
{
	std::string strRet;
	if (dwNum == 0)
		return "0";
	while(dwNum)
	{
		int nNow = dwNum % 10;
		strRet.append(1,nNow - 0 + '0');
		dwNum /= 10;
	}

	reverse(strRet.begin(),strRet.end());
	return strRet;
}

bool CUtil::IsDigit(const char c)
{
	return c >= '0' && c <= '9';
}

void CUtil::GetRandSequence( std::vector<int>& vecSeq, int nBegin, int nEnd )
{
	vecSeq.clear();
	if (nBegin < nEnd)
	{
		Swap(nBegin, nEnd);
	}
	
	std::vector<uint32> vecTmp;
	for (int i = nBegin; i < nEnd; ++i)
	{
		vecSeq.push_back(i);
	}

	for (size_t i = 0; i < vecSeq.size(); ++i)
	{
		int nRand = rand() % vecSeq.size();
		if (i != (size_t)nRand)
		{
			Swap(vecSeq[i], vecSeq[nRand]);
		}
	}
}
char CUtil::error_buf[1024];

uint32 CUtil::GetSecondClock(uint8_t c, uint32_t t)
{
	time_t timeNow = t;
	struct tm stcTmNow;
	localtime_r(&timeNow, &stcTmNow);
	stcTmNow.tm_hour = c;
	stcTmNow.tm_sec = 0;
	stcTmNow.tm_min = 0;
	return mktime(&stcTmNow);
}

uint32 CUtil::GetSecondFrom0Clock()
{
	time_t timeNow = time(NULL);
	struct tm stcTmNow;
	localtime_r(&timeNow, &stcTmNow);
	return stcTmNow.tm_hour * 3600 + stcTmNow.tm_min * 60 + stcTmNow.tm_sec;
}

void CUtil::GetSecondFrom0Clock(uint32 t, uint32 &base, uint32 &remain)
{
	time_t timeNow = t;
	struct tm stcTmNow;
	localtime_r(&timeNow, &stcTmNow);
	remain =  stcTmNow.tm_hour * 3600 + stcTmNow.tm_min * 60 + stcTmNow.tm_sec;
	base = t - remain;
	return;
}

int CUtil::GetUTF8StringLength(std::string& str)
{
	float n = 0;

	int len = str.length();
	for ( int i = 0; i < len; i++ )
	{
		char ch = str.at(i);

		if ( 0x80 != (0xC0 & ch) )
		{
			// check English letter and number
			if ( ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' ) || ( ch >= '0' && ch <= '9' ) )
			{
				n += 0.5f;
			}
			else
			{
				n += 1.0f;
			}
		}
	}

	return n;
}

std::string CUtil::GetRandString(std::string pre, uint32_t number)
{
	std::string src("0123456789abcdefghijklmnopqrstuvwsyz");
	std::string res = pre;
	for (unsigned int i = 0; i < number; i++)
	{
		res += src[rand() % src.size()];
	}
	return res;
}

int CUtil::StrSplitTime(const std::string &str, std::vector<TimeSection> &section, const std::string &firstSeq, const std::string &secondSeq)
{
	std::vector<std::string> vectemp1;
	std::vector<TimeSection> sectemp;
	StrSplit(str, firstSeq, vectemp1);
	for (unsigned int i = 0; i < vectemp1.size(); i++)
	{
		std::vector<uint32_t> vec2;
		StrSplit(vectemp1[i], secondSeq, vec2);
		if (vec2.size() != 2)
		{
			IME_ERROR("colomun is not two index i %u parse str %s", i, vectemp1[i].c_str());
			return -1;
		}
		TimeSection one;
		one.startTime = vec2[0];
		one.endTime = vec2[1];
		sectemp.push_back(one);
	}
	if (sectemp.empty())
	{
		IME_ERROR("parse string is empty");
		return -1;
	}
	return 0;
}

bool CUtil::IsInSection(uint32_t t, const std::vector<TimeSection> &section)
{
	for (unsigned int i = 0; i < section.size(); i++)
	{
		if (t >= section[i].startTime && t <= section[i].endTime)
		{
			return true;
		}
	}
	return false;
}

uint32_t CUtil::GetTime()
{
	return timeOff + time(NULL);
}

void CUtil::shutDown()
{
	g_stopEvent = true;
	IME_LOG("server was shut down!!!");
}

uint32_t CUtil::str2Time(std::string str, const char *format)
{
	struct tm tm1;
	memset(&tm1, 0, sizeof(struct tm));
	char *ret = NULL;
	if (format == NULL)
	{
		ret = strptime(str.c_str(), "%Y:%m:%d-%H:%M:%S", &tm1);
	}
	else
	{
		ret = strptime(str.c_str(), format, &tm1);
	}
	if (ret == NULL)
	{
		IME_ERROR("format error str %s", str.c_str());
		return 0;
	}
	int t = mktime(&tm1);
	if (t == -1)
		return 0;
	return t;
}
uint8 CUtil::GetCurHour(uint32 dwTime)
{
	time_t tTimea = (time_t)dwTime;
	struct tm tTime;
	memset(&tTime, 0, sizeof(tTime));
	struct tm *ptTime;

	ptTime = localtime(&tTimea);
	if(NULL != ptTime)
	{
		memcpy(&tTime, ptTime, sizeof(tTime));
	}

	return tTime.tm_hour;
}
uint8 CUtil::GetCurMin(uint32 dwTime)
{
	time_t tTimea = (time_t)dwTime;
	struct tm tTime;
	memset(&tTime, 0, sizeof(tTime));
	struct tm *ptTime;

	ptTime = localtime(&tTimea);
	if(NULL != ptTime)
	{
		memcpy(&tTime, ptTime, sizeof(tTime));
	}

	return tTime.tm_min;
}
uint8 CUtil::GetCurWeekDay(uint32 dwTime)
{
	time_t tTimea = (time_t)dwTime;
	struct tm tTime;
	memset(&tTime, 0, sizeof(tTime));
	struct tm *ptTime;

	ptTime = localtime(&tTimea);
	if(NULL != ptTime)
	{
		memcpy(&tTime, ptTime, sizeof(tTime));
	}

	return tTime.tm_wday;
}
uint8 CUtil::GetCurMDay(uint32 dwTime)
{
	time_t tTimea = (time_t)dwTime;
	struct tm tTime;
	memset(&tTime, 0, sizeof(tTime));
	struct tm *ptTime;

	ptTime = localtime(&tTimea);
	if(NULL != ptTime)
	{
		memcpy(&tTime, ptTime, sizeof(tTime));
	}

	return tTime.tm_mday;
}
uint8 CUtil::GetCurMon(uint32 dwTime)
{
	time_t tTimea = (time_t)dwTime;
	struct tm tTime;
	memset(&tTime, 0, sizeof(tTime));
	struct tm *ptTime;

	ptTime = localtime(&tTimea);
	if(NULL != ptTime)
	{
		memcpy(&tTime, ptTime, sizeof(tTime));
	}

	return tTime.tm_mon;
}
uint32  CUtil::SharpWeek(uint32 dwTime)
{
	time_t t = dwTime;
	struct tm t_tm;
	localtime_r(&t,&t_tm);
	if(t_tm.tm_wday == 0) t_tm.tm_wday = 7;
	t = t + (1 - t_tm.tm_wday) * 86400;
	return SharpDayT(static_cast<uint32>(t));
}
uint32 CUtil::SharpDayT(uint32 dwTime)
{
	time_t t = dwTime;
	struct tm t_tm;
	localtime_r(&t,&t_tm);
	t_tm.tm_hour = 0;
	t_tm.tm_min = 0;
	t_tm.tm_sec = 0;
	time_t t2 = mktime(&t_tm);
	return static_cast<uint32>(t2);
}
