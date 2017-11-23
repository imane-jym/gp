#ifndef _UTIL_H
#define _UTIL_H

#include <string>
#include <vector>

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>

#include "Defines.h"
#include <map>
#include <set>

#define UNUSED_PARAM(unusedparam) (void)unusedparam /* del warnning */
#define UNUSED_RETURN(unusedreturn) (unusedreturn || 0) /* del warning */
#define MY_ABS(a,b) (a) > (b) ? ((a) - (b)) : ((b) - (a))

struct TimeSection
{
	uint32_t startTime;
	uint32_t endTime;
};

class CUtil
{

public:
	// 随机方式
	static void StrSplit(const std::string & src, const std::string & sep, std::vector<std::string> &vecRes);
	static void StrMerge(std::string & src, const std::string & sep, const std::vector<std::string> &vecRes);
	static void StrSplit(const std::string & src, const std::string & sep, std::set<uint32_t> & setRes);
	static void StrSplit(const std::string & src, const std::string & sep, std::vector<uint32_t> & vecRes);
	static void StrMerge(std::string & src, const std::string & sep, const std::vector<uint32_t> & vecRes);
	static void StrSplit(const std::string & src, const std::string & sep, std::vector<uint64_t> & vecRes);
	static void StrMerge(std::string & src, const std::string & sep, const std::vector<uint64_t> & vecRes);
	static void StrSplit(const std::string & src, const std::string & sep, std::vector<float> & vecRes);
	static void StrMerge(std::string & src, const std::string & sep, const std::vector<float> & vecRes);
	static void StrSplit(const std::string & src, const std::string & sep, std::vector<double> & vecRes);
	static void StrMerge(std::string & src, const std::string & sep, const std::vector<double> & vecRes);


	static void StrVecSplit(std::string & src, const std::string & sep1, const std::string & sep2, std::vector<std::vector<uint32_t> > & vecRes);
	static void StrVecSplit(std::string & src, const std::string & sep1, const std::string & sep2, std::vector<std::vector<std::string> > & vecRes);

	static int DaemonInit();

	static bool SetRLimit();

	static bool CreatePidFile();

	static int RandFactor(std::vector<uint16>& value);

	static int RandFactor(std::vector<int>& value);

	static std::vector<int> RandFactorMulti(std::vector<int>& value, int number);

	// 未检查 输入数据的有效性
	static int RandFactor(const std::vector<int>& value, int nBegin, int nEnd);

	static bool RandEither(uint16 value, uint16 maxValue = 10000);

	static void Swap(int &a, int &b);

	//某区间 随机选择一个 [nBegin, nEnd)
	static int RandByRange(int nBegin, int nEnd);

	// 某区间[nBegin, nEnd),随即选择nNum个不相同随机值，若nNum大于或等于区间中元素个数，返回所有元素。
	static std::vector<int> RandByRangeMulti(int nBegin, int nEnd, int nNum);

	// 洗牌
	//static void RandShuffle(std::vector<int>& vecShuffle, int nNum);

	static void GetLinearList(std::vector<int>& vecValue, int nLen, float k);

	static uint32 PowUint32(uint32 x, uint32 y);
	
	static bool IsToday(uint32 dwTime);

	static bool IsToday6AM(uint32 dwTime);

	//是否同一天，offset 偏移顺时针 正，逆时针 负，秒单位
	static bool IsTodayOffset(uint32 dwTime, int nOffset);

	static std::string Uint32ToString(uint32 dwNum);
	static std::string Uint64ToString(uint64 num);

	static bool IsDigit(const char c);

	static void GetRandSequence(std::vector<int>& vecSeq, int nBegin, int nEnd);

	// 输入：时间 time(NULL)
	// 输出：(year+1900)*10000 + (month + 1)*100 + mday;
	static uint32 GetDateTime(uint32 dwTime)
	{
		struct tm t;
		time_t ttime = dwTime;
		localtime_r(&ttime, &t);
		return (t.tm_year + 1900) * 10000 + (t.tm_mon + 1) * 100 + t.tm_mday;
	}

	static uint32 GetDayTime(uint32 dwTime)
	{
		struct tm t;
		time_t ttime = dwTime;
		localtime_r(&ttime, &t);
		return t.tm_hour * 3600 + t.tm_min * 60 + t.tm_sec;
	}

	// 获得这周时间值，返回格式：一周第一天的GMT时间;
	// 游戏中活动的一周，默认 一周活动的第一天是星期6
	// 输入 日期时间，byOffset,调整一周的第一天是星期几，默认设置成1，第一天是星期6
	static uint32 GetFirstWeekday(uint32 dwDate, uint8 byOffset = 1)
	{
		struct tm t1 = {};
		t1.tm_year = dwDate / 10000 - 1900;
		t1.tm_mon = (dwDate / 100) % 100 - 1;
		t1.tm_mday = dwDate % 100;
		
		time_t tTime = mktime(&t1);

		struct tm t2 = {};
		localtime_r(&tTime, &t2);

		int day;
		day = (t2.tm_wday + byOffset) % 7;

		uint32 dwTime = uint32(tTime - day * 24 * 3600);
		return dwTime;
	}
	// 获得星期几
	// 输入：GMT时间
	// 输出：1，星期一，2，星期二，3，星期三, 7:星期天
	static int GetWeekDay(uint32 dwTime)
	{
		struct tm t;
		time_t ttime = dwTime;
		localtime_r(&ttime, &t);
		if (0 == t.tm_wday)
			return 7;
		return t.tm_wday;
	}
	// 获得相隔天数
	// 输入：GMT时间
	// 输出：
	static int GetBetweenDay(uint32 firstTime,uint32 secondTime)
	{
		uint32 firstZero = get0day(firstTime);
		uint32 secondZero = get0day(secondTime); 
		uint32 value = MY_ABS(firstZero,secondZero);
		return value / (24 * 3600); 
	}
	//取得一天中的0点 
	static uint32 get0day(uint32 date)
	{
		struct tm ptr; 
		time_t dates = (time_t)date;
		localtime_r(&dates,&ptr);
		ptr.tm_min = 0; 
		ptr.tm_hour = 0;
		ptr.tm_sec = 0; 
		time_t oday = mktime(&ptr);
		return oday;
	}


	static bool IsSameHour(uint32 cur, uint32 last)
	{
		struct tm t1,t2;
		time_t ttime = cur;
		localtime_r(&ttime, &t1);
		time_t ttime2 = last;
		localtime_r(&ttime2, &t2);
		return t1.tm_wday == t2.tm_wday && t1.tm_hour == t2.tm_hour;
	}

	// 游戏业务公式
	// 计算最大活力值
	static uint32 CalcEnergyMax(uint8 byCastleLv)
	{
		return (uint32)byCastleLv + 30;
	}
	// 计算血库上限血量
	static uint32 CalcCurePointMax(uint8 byHealthLv)
	{
		return (uint32)byHealthLv* 150;
	}
	// 计算好友攻击加成
	static uint16 CalcFrdAtkPlus(uint16 wFrdCnt)
	{
		uint16 wAtkPlus = wFrdCnt / 5;
		if (wAtkPlus > 20)
		{
			wAtkPlus = 20;
		}
		return wAtkPlus;
	}
	static uint32 CalcArenaRewCoin(uint8 byRank, uint8 byCastleLv)
	{
		return uint32(10000 - 500*(byRank - 1)) * (byCastleLv / 10);
	}

	// 用户名字和id连接
	static void JoinNameAndUserId(std::string& strJoinOutput, const std::string& strName, uint32 dwUserId)
	{
		char szUserId[32];
		memset(szUserId, 0, sizeof(szUserId));
		sprintf(szUserId, "(ID:%u)", dwUserId);
		strJoinOutput = strName + szUserId;
	}

	static char error_buf[1024];
	static inline const char* FormatError( const char* fmt, ... )
	{
		va_list ap;
		va_start( ap, fmt );
		vsprintf( error_buf, fmt, ap );
		va_end( ap );
		return error_buf;
	}
	
	static std::string StrReplace(std::string src, std::string seek, std::string str)
	{
		size_t pos = src.find(seek);
		if (pos == std::string::npos)
			return src;
		src.replace(pos, seek.size(), str); 
		return src;
	}
	static uint32 GetSecondClock(uint8_t c, uint32_t t);
	static uint32 GetSecondFrom0Clock();                                      
	static void GetSecondFrom0Clock(uint32 t, uint32 &base, uint32 &remain);  

	static int GetUTF8StringLength(std::string& str);                         

	static std::string &replace_all(std::string &str, const std::string strold, const std::string strnew)
	{
		while(true)
		{
			size_t pos;
			if ((pos = str.find(strold)) != std::string::npos)
				str.replace(pos, strold.size(), strnew);
			else
				break;
		}
		return str;
	}

	static std::string GetRandString(std::string pre, uint32_t number);

	static int StrSplitTime(const std::string &str, std::vector<TimeSection> &section, const std::string &firstSeq, const std::string &secondSeq);

	static bool IsInSection(uint32_t t, const std::vector<TimeSection> &section);
	
	static uint32_t GetTime();
	static int timeOff;

	static void shutDown();

	static uint32_t str2Time(std::string str, const char *format = NULL);

	static uint8 GetCurHour(uint32 dwTime);
	static uint8 GetCurMin(uint32 dwTime);
	static uint8 GetCurWeekDay(uint32 dwTime);
	static uint8 GetCurMDay(uint32 dwTime);
	static uint8 GetCurMon(uint32 dwTime);
	static uint32 SharpWeek(uint32 dwTime);
	static uint32 SharpDayT(uint32 dwTime);
	//某区间 随机选择一个 [nBegin, nEnd)
	static inline const int RandByRanges( uint32 nBegin, uint32 nEnd )
	{
		if ( nBegin == nEnd - 1 )
			return nBegin; 
		if ( nBegin == nEnd ) return nEnd; 
		int num =  MY_ABS(nEnd, nBegin) ;
		int nRand =  rand()  % num;
		return nRand + nBegin;
	}
	template < class T >
	static inline T RandByDiffRateChoice(std::map< T, T >& mapRateChoice )
	{
		if ( mapRateChoice.empty() ) return 0;
		T rateTotal = 0;
		for ( typename std::map< T, T >::iterator it = mapRateChoice.begin(); it != mapRateChoice.end(); ++it )
			rateTotal += it->second;
		const T randVal = RandByRanges( (T)0, rateTotal ); 
		T min = 0;
		T max = 0;
		for ( typename std::map< T, T >::iterator it = mapRateChoice.begin(); it != mapRateChoice.end(); )
		{
			min = max;
			max += it->second;
			if ( randVal >= min && randVal < max )
			{
				return it->first;
			}
			++it; 
		}
		return 0; 
	}
};

#endif
