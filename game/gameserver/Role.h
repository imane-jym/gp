#ifndef _ROLE_H
#define _ROLE_H

#include "Handler.h"
#include "Defines.h"
#include "DBCtrl.h"
#include "DynamicStruct.h"
#include "Log.h"
#include "RoleBase.h"
#include "Util.h"
#include "GlobalConfig.h"
#include "Item.h"
#include "Combat.h"
#include "Shop.h"
#include "CommonType.h"
#include "EnumDefines.h"
#include "Trial.h"
#include <time.h>
#include <stdint.h>
#include <algorithm>


#define PROPERTY_DEF(T,name) \
	T Get##name() { return m_##name;}; \
	void Set##name(T name, bool bSend = false) \
	{ m_##name = name; \
	setProperty(E_ROLE_##name, name); \
	if (bSend) SendProUpdate(); \
	;}; \

#define PROPERTY_DEF_NOUPDATE(T,name) \
	T Get##name() { return m_##name;}; \
	void Set##name(T name) \
	{ m_##name = name; \
	}; \


#define IS_THERE_ARENA(k) (k & 0x1)
#define OPEN_ARENA(k) (k | 0x1)
#define CLOSE_ARENA(k) (k & 0xfe)
#define IS_THERE_GROUP(k) (k & 0x2)
#define OPEN_GROUP(k) (k | 0x2)
#define CLOSE_GROUP(k) (k & 0xfd)
#define FIRST_CHARGE(k) (k | 0x4)
#define IS_ALREADY_FIRST_CHARGE(k) (k & 0x4)
#define OPEN_FIRST_PRIZE(k) (k | 0x8)
#define IS_FIRST_PRIZE(k) (k & 0x8)
#define CLOSE_FIRST_PRIZE(k) (k & 0xf7)
#define IS_ALREADY_DOUBLE_CHARGE(k) (k & 0x10)
#define OPEN_DOUBLE_CHARGE(k) (k | 0x10)

//class CLuaScript;
class CTujian;
class CCardPacket;
class CItem;
class CTujian;
class CTech;
class CArena;
class CMaster;
class CDurationMission;
class CAnnouncement;
class CActive;
class CTimerGift;
class CMission;
class CTrial;
class CTower;

class CRole : public CDynamicStruct, public CRoleBase, public CDuration  //tolua_export
{//tolua_export
public:


public:

	enum
	{
		E_SQL_RESULT_SUCC,				//成功
		E_SQL_RESULT_SQL_ERROR,			//sql语句错误
		E_SQL_RESULT_NO_ROLE,			//新角色
	};
//	typedef enum _E_PROPERTY_ROLE
//	{
//		E_ROLE_dwAccountId,
//		E_ROLE_strAccount,
//		E_ROLE_strUserName,
//		E_ROLE_wLevel,
//		E_ROLE_dwExp,
//		E_ROLE_wVipLv,
//		E_ROLE_dwVipExp,
//		E_ROLE_dwDollar,
//		E_ROLE_dwCoin,
//		E_ROLE_dwEnergy,
//		E_ROLE_dwEnergyMax,
//		E_ROLE_dwEnergyRate,
//		E_ROLE_dwStrength,
//		E_ROLE_dwStrengthMax,
//		E_ROLE_dwStrengthRate,
//		E_ROLE_byTaskLocation1, // new add             
//		E_ROLE_byTaskLocation2,             
//		E_ROLE_byTaskLocation3,             
//		E_ROLE_byTaskLocation4,              
//		E_ROLE_byCardCapacity,
//		E_ROLE_dwExplimit,
//		E_ROLE_byCurrentTeam,
//		E_ROLE_dwLastExplimit,
//		E_ROLE_byFlag,
//		E_ROLE_lFindGroupId,
//		E_ROLE_dwAvoidDurationTime,
//		E_ROLE_dwWorldHistory,
//		E_ROLE_dwFriendLimit,
//		E_ROLE_dwGameFlow,
//		E_ROLE_dwLuck,
//		E_ROLE_byGm,
//		E_ROLE_wNextVipLv,
//		E_ROLE_dwNextVipExp,
//		E_ROLE_dwChargeLuck,
//		E_ROLE_dwStamina
//	}E_PROPERTY_ROLE;

	PROPERTY_DEF(uint32,dwAccountId);
	//PROPERTY_DEF(uint32,strAccount);
	std::string& GetstrAccount() { return m_strAccount; }
	void SetstrAccount(const std::string& strAccount, bool bSend = false)
	{
		m_strAccount = strAccount;
		setProperty(E_ROLE_strAccount, AnyType(m_strAccount,true));
		if (bSend) SendProUpdate();
	};
//	PROPERTY_DEF(uint16,wHeadPic);
	std::string& GetstrUserName() { return m_strUserName; }
	void SetstrUserName(const std::string& strUserName, bool bSend = false)
	{
//		IME_ERROR("Role !!!!!");
		m_strUserName = strUserName;
		setProperty(E_ROLE_strUserName, AnyType(m_strUserName,true));
		if (bSend) SendProUpdate();
	};
	std::string& GetstrSignature() { return m_strSignature; }
	void SetstrSignature(const std::string& strSignature, bool bSend = false)
	{
		m_strSignature = strSignature;
		setProperty(E_ROLE_strSignature, AnyType(m_strSignature,true));
		if (bSend) SendProUpdate();
	};

	PROPERTY_DEF(uint16,wLevel);
	PROPERTY_DEF(uint32,dwExp);
	PROPERTY_DEF(uint16,wVipLv);
	PROPERTY_DEF(uint32,dwVipExp);
	PROPERTY_DEF(uint32,dwDollar);
	PROPERTY_DEF(uint32,dwCoin);
	PROPERTY_DEF(uint32,dwEnergy);
	PROPERTY_DEF(uint32,dwEnergyMax);
	PROPERTY_DEF(uint32,dwEnergyRate);
	PROPERTY_DEF(uint32,dwStrength);
	PROPERTY_DEF(uint32,dwStrengthMax);
	PROPERTY_DEF(uint32,dwStrengthRate);
	PROPERTY_DEF(uint8_t, byTaskLocation1);
	PROPERTY_DEF(uint8_t, byTaskLocation2);
	PROPERTY_DEF(uint8_t, byTaskLocation3);
	PROPERTY_DEF(uint8_t, byTaskLocation4); 
	PROPERTY_DEF(uint8_t, byCardCapacity);
	PROPERTY_DEF(uint32, dwExplimit);
	PROPERTY_DEF(uint32_t, dwLastExplimit);
	PROPERTY_DEF(uint8, byFlag);
	PROPERTY_DEF(uint64, lFindGroupId);
	PROPERTY_DEF(uint32, dwAvoidDurationTime);
	PROPERTY_DEF(uint32, dwWorldHistory);
	PROPERTY_DEF(uint32, dwFriendLimit);
	PROPERTY_DEF(uint32, dwGameFlow);
	PROPERTY_DEF(uint32, dwLuck);
	PROPERTY_DEF(uint8, byGm);
	PROPERTY_DEF(uint16, wNextVipLv);
	PROPERTY_DEF(uint32, dwNextVipExp);
	PROPERTY_DEF(uint32, dwChargeLuck);
	PROPERTY_DEF(uint32, dwStamina);
	PROPERTY_DEF(uint32, dwStaminaMax);
	PROPERTY_DEF(uint32, dwStaminaRate);

//	void SetdwCardAttributes2(uint32_t attr)
//	{
//		m_dwCardAttributes2 = attr;
//	}
//	uint32 GetdwCardAttributes2()
//	{
//		return m_dwCardAttributes2;
//	}
//	void SetdwCardAttributes(uint32_t attr)
//	{
//		m_dwCardAttributes = attr;
//	}
//	uint32 GetdwCardAttributes()
//	{
//		return m_dwCardAttributes;
//	}

	CRole(CHandlerPlayersession* pSess);
	virtual ~CRole();
	void SetSess(CHandlerPlayersession* pSess) { m_pSession = pSess; }

	bool Init(uint32 dwAccountId, const std::string & strAccount);
	void SendLoginData();
	void LoadPlayer();
	int LoadRole();

	void SetInitError() {m_bIsInitOK = false;};

	void SaveAllData();

	void Online();
	void Offline();
	void SendPacket(WorldPacket * packet);
	void SendErrorMsg(uint16 wErrorMsgNum, uint16 wCmd = 0);
	void SendErrorStr(const std::string& strMsg);
	void SendMsgStr(const std::string& strMsg);

	void SendProUpdate();
	CHandlerPlayersession* GetSession() {return m_pSession;}

	bool LoadDataForDuration(uint32 dwAccountId);

	void SaveDataForDuration();
	//////////////////////////// 基本数值计算 //////////////////////////////////////////////
	
	int AddExp(uint32_t exp);	
	int ChangeEnergy(int32_t energy);
	int ChangeStrength(int32_t strength);
	int ChangeEnergyNoLimit(int32_t energy);
	int ChangeStrengthNoLimit(int32_t strength);
	int ChangeStamina(int32_t stamina);
	int ChangeStaminaNoLimit(int32_t stamina);
	int CalExp(bool IsUpdate = false);
	int CalTech(bool IsUpdate = false);
	int ChangeCoin(int32_t coin, int source);
	int ChangeDollar(int32_t dollar, int source);
	int ChangeDollarWithExtra(int32_t &dollar, int source);
	int ChangeWorldHistory(int32_t exp, int source);
	int ChangeVipExp(int32_t exp);

	int LoadProperty();
	int Calculate(bool IsUpdate = false);

	///////////////////////////////////////////////
	void OnTimer5s();
	void OnTimer1s();
	void OnTimer3min();

	void TestScriptByName(const std::string & strName);

	////////////////////////////////////////////////////

	//玩家基本信息
	void SendPlayerBaseInfo();
//	void PackForTeam(WorldPacket &info);

	void OnGMCmd(std::string & strContent);

	bool IsGM() { return m_byGm;};

	uint32_t GetNewObjId() {return ++m_dwInitNumber;};
	uint32_t GetCurrentMaxObjId() {return m_dwInitNumber;};
	uint32_t GetdwInitNumber() {return m_dwInitNumber;};
	CCardPacket *GetclsCardPacket() {return m_clsCardPacket;};
	CItem *GetclsItemPacket() {return m_clsItemPacket;};
	CTujian *GetclsTujian() { return m_clsTujian;};
	CTech *GetclsTech() { return m_clsTech;};
	CArena *GetclsArena() { return m_clsArena;};
	CMaster *GetCMaster() { return m_CMaster;};
	CDurationMission *GetCDurationMission() { return m_CDMission;};
	CActive *GetCActive() { return m_CActive;};
	CRobotCtrl *GetCRobotCtrl() {return m_CRobotCtrl;};
	CTimerGift *GetCTimerGift() {return m_CTimerGift;};
	CMission *GetCMission() {return m_CMission;};
	CTrial *GetCTrial() {return m_CTrial;};
	CTower *getCTower(){return m_CTower;};
//	uint32_t GetRealAttribute() 
//	{
//		if (m_byCurrentTeam == 0)
//		{
//			return m_dwCardAttributes;
//		}
//		else
//		{
//			return m_dwCardAttributes2;
//		}
//	}

	virtual int PrepareDurationData(std::vector<CDuration::cardforduration_t> &vec, std::vector<uint32_t> &vecline);
	void HandlerArenaOpen(WorldPacket &pkg);
	bool IsOpenArena()
	{ return IS_THERE_ARENA(m_byFlag);};
//	void SetRankLevel(uint16 level)
//	{
//		IME_LOG("now %u update %u level",m_wMaxLevel, level);
//		if (level > m_wMaxLevel)
//			m_wMaxLevel = level;
//	}
//	void SetRankAttack(uint32 attack)
//	{
//		IME_LOG("now %u update %u attack",m_dwMaxAttack, attack);
//		if (attack > m_dwMaxAttack)
//			m_dwMaxAttack = attack;
//	}
//	void SetRankDefence(uint32 defence)
//	{
//		IME_LOG("now %u update %u defence",m_dwMaxDefence, defence);
//		if (defence > m_dwMaxDefence)
//			m_dwMaxDefence = defence;
//	}
//	void SendPlayerRank();

//	void HandlerGroupAdd(WorldPacket &pkg);
	void HandlerGroupCreate(WorldPacket &pkg);
	void HandlerGroupAutoInfo(WorldPacket &pkg);
	void HandlerGroupSearch(WorldPacket &pkg);
	void HandlerGroupRequestEnter(WorldPacket &pkg);
	CShop *GetCShop()
	{
		return m_CShop;
	}

	void SendEnergyNotEnough(uint16 wCmd);
	void SendStrengthNotEnough(uint16 wCmd);
	void SendStaminaNotEnough(uint16 wCmd);
	void UpdateEnergyAndStrengthTime();
	void HandlerSellCard(WorldPacket &pkg);
	void HandlerInfoOpt();
	bool IsInitRoleInfo();
	void HandlerInitRole(WorldPacket &pkg);
	void HandlerFriendBossListRequset(WorldPacket &pkg);
	void HandlerFriendBossDurationRequest(WorldPacket &packet);
	void HandlerFriendBossGacha(WorldPacket &packet);
	void HandlerFriendBossHistoryRequest(WorldPacket &pkg);
	void HandlerWorldBroadCastInfo();
	void HandlerWorldBroadCast(WorldPacket &pkg);
	void HandlerGameFlow(WorldPacket &packet);
	void HandlerTurntable(WorldPacket &packet);
	void HandlerFirstPrize(WorldPacket &packet);
	void HandlerUpgrade(WorldPacket &packet);
	void HandlerUpgradeReview(WorldPacket &packet);
	void HandlerJoinPlayerInfo(WorldPacket &packet);
	void PkgWorldBroadCastInfo(uint8_t choose, std::string content, WorldPacket &info);
	int OtherDataGet();
	void OtherDataSave();
	void statisticsRole();

	uint32 GetdwAddFriend()
	{
		return m_dwAddFriend;
	}
	uint32 GetdwDelFriendTime()
	{
		return m_dwDelFriend;
	}

	void ChangedwAddFriend(uint32 number)
	{
		m_dwAddFriend += number;
	}

	void SetbyDuration(uint8 type)
	{
		m_byDuration = type;
	}

	uint8 GetbyDuration()
	{
		return m_byDuration;
	}

	uint32 GetdwDelDMissionCnt()
	{
		return m_dwDelDMissionCnt;
	}
	void SetdwDelDMissionCnt(uint32 number)
	{
		m_dwDelDMissionCnt = number;
	}

	uint32 GetdwHighDelDMissionCnt()
	{
		return m_dwHighDelDMissionCnt;
	}
	void SetdwHighDelDMissionCnt(uint32 number)
	{
		m_dwHighDelDMissionCnt = number;
	}

	uint32_t GetdwEnergyUseCnt()
	{
		return m_dwEnergyUseCnt;
	}

	void SetdwEnergyUseCnt(uint32_t number)
	{
		m_dwEnergyUseCnt = number;
	}

	uint32_t GetdwStrengthUseCnt()
	{
		return m_dwStrengthUseCnt;
	}

	void SetdwStrengthUseCnt(uint32_t number)
	{
		m_dwStrengthUseCnt = number;
	}

	void AddVipGift(int vipgift)
	{
		m_setVipGift.insert(vipgift);
	}

	bool IsAccessGetVipGift(int vipgift)
	{
		if (m_setVipGift.find(vipgift) == m_setVipGift.end())
		{
			return true;
		}
		return false;
	}

	void SetGiftRoleLimit(int key, int number)
	{
		if (m_mapGift.find(key) == m_mapGift.end())
		{
			SGift tmp = {number, 0};
			m_mapGift[key] = tmp;	
		}
		else
		{
			m_mapGift[key].role = number;
		}
	}

	void SetGiftDayLimit(int key, int number)
	{
		if (m_mapGift.find(key) == m_mapGift.end())
		{
			SGift tmp = {0, number};
			m_mapGift[key] = tmp;	
		}
		else
		{
			m_mapGift[key].day = number;
		}
	}

	uint32_t GetGiftRole(int key)
	{
		if (m_mapGift.find(key) == m_mapGift.end())
		{
			return 0;
		}
		else
		{
			return m_mapGift[key].role;
		}
	}

	uint32_t GetGiftDay(int key)
	{
		if (m_mapGift.find(key) == m_mapGift.end())
		{
			return 0;
		}
		else
		{
			return m_mapGift[key].day;
		}
	}

	void SetdwExtraStrengthPro(uint32 pro)
	{
		m_dwExtraStrengthPro = pro;
	}

	uint32 GetdwExtraStrengthPro()
	{
		return m_dwExtraStrengthPro;
	}

	void SetdwLowMasterCount(uint32 count)
	{
		m_dwLowMasterCount = count;
	}

	uint32 GetdwLowMasterCount()
	{
		return m_dwLowMasterCount;
	}

//	void SetstrDevice(std::string str)
//	{
//		m_strDevice = str;	
//	}
//	
//	std::string GetstrDevice()
//	{
//		return m_strDevice;
//	}
//
//	void SetstrDeviceType(std::string str)
//	{
//		m_strDeviceType = str;
//	}
//
//	std::string GetstrDeviceType()
//	{
//		return m_strDeviceType;
//	}
//
//	void SetstrDeviceUid(std::string str)
//	{
//		m_strDeviceUid = str;
//	}
//
//	std::string GetstrDeviceUid()
//	{
//		return m_strDeviceUid;
//	}
//
//
//	void SetstrSecondChannel(std::string str)
//	{
//		m_strSecondChannel = str;
//	}
//
//	std::string GetstrSecondChannel()
//	{
//		return m_strSecondChannel;
//	}

	uint8_t IsFirstGacha()
	{
		return m_byFirstGacha;
	}

	void SetFirstGacha(uint8_t first)
	{
		m_byFirstGacha = first;
	}

	uint8_t IsSecondGacha()
	{
		return m_bySecondGacha;
	}

	void SetSecondGacha(uint8_t second)
	{
		m_bySecondGacha = second;
	}


	void AddwUseEnergy()
	{
		m_wUseEnergy++;
	}
	
	uint32 GetwUseEnergy()
	{
		return m_wUseEnergy;
	}

	void AddwUseStrength()
	{
		m_wUseStrength++;
	}

	uint32 GetwUseStrength()
	{
		return m_wUseStrength;
	}

	void AddwUseStamina()
	{
		m_wUseStamina++;
	}

	uint32 GetwUseStamina()
	{
		return m_wUseStamina;
	}

	void ChangeExtraRolePacketLimit(int number)
	{
		m_dwExtraRolePacketLimit += number;
	}

	uint32 GetExtraRolePacketLimit()
	{
		return m_dwExtraRolePacketLimit;
	}

	void ChangeExtraEquipPacketLimit(int number)
	{
		m_dwExtraEquipPacketLimit += number;
	}

	uint32 GetExtraEquipPacketLimit()
	{
		return m_dwExtraEquipPacketLimit;
	}

	PROPERTY_DEF_NOUPDATE(uint32_t, dwLastLoginTime);
	PROPERTY_DEF_NOUPDATE(uint32_t, dwGetExtraDollar);
	PROPERTY_DEF_NOUPDATE(uint16_t, dwDotDollar);

	void AddVipFirstGacha(uint32 viplevel)
	{
		m_setVipFirstGacha.insert(viplevel);	
	}

	std::set<uint8_t>& GetVipFirstGacha()
	{
		return m_setVipFirstGacha;
	}

	bool IsVipFirstGacha(uint16 viplv)
	{
		if (m_setVipFirstGacha.find(viplv) == m_setVipFirstGacha.end())
			return true;
		else
			return false;
	}

	std::vector<SPlayer> GetFriend()
	{
		return m_vecFriend;
	}

	bool IsFriend(uint32 userid)
	{
		for (int i = 0; i < m_vecFJoin.size(); i++)
		{
			if (m_vecFJoin[i].userid == userid)
			{
				return true;
			}
		}
		return false;
	}

	uint32_t GetTotalJoin() { return m_dwTotalJoin;}
	void SetTotalJoin(uint32 join) {m_dwTotalJoin = join;}
	std::map<uint32_t, uint8_t>& GetMapJoin() { return m_mapJoin;}
	void LoadFriendData();

	void HandlerRoleInfoUpdate(WorldPacket &packet);

	void SendObjEffect(std::vector<CShopEffect::SGiveProduct> tmp);
	void SendObjEffectByHappenBoss();
	void SendObjEffectByHappenRole();

	PROPERTY_DEF_NOUPDATE(uint8_t, byFirstChangeName);
	PROPERTY_DEF_NOUPDATE(uint8_t, byFirstSendRed);

	uint16 GetChannel() { return m_pSession->GetwChannel(); }

	uint32_t GetUJoinCount() { return m_vecUJoin.size(); }
	uint32_t GetFJoinCount() { return m_vecFJoin.size(); }

	uint32 GetPower();

	void SetAttackTimes(uint32 times);
	uint32 getMultipleCardExp();//返回倍数，若返回1则没有倍数正常经验
	void dealMultipleCardExp(uint32 num,uint32 times);

private:
	void SendPlayerCardPacketInfo();
	void initMultipleCardExp();
	void saveMultipleCardExp();

	CHandlerPlayersession * m_pSession;

	uint32 m_dwAccountId;
	std::string m_strAccount;

	std::string m_strUserName;
	uint16 m_wLevel;
	uint32 m_dwExp;
	uint16 m_wVipLv;
	uint32 m_dwVipExp;
	uint32 m_dwDollar;
	uint32 m_dwCoin;
	uint32 m_dwEnergy;
	uint32 m_dwEnergyMax;
	uint32 m_dwEnergyRate;
	uint32 m_dwStrength;
	uint32 m_dwStrengthMax;
	uint32 m_dwStrengthRate;
	uint8_t m_byTaskLocation1;
	uint8_t m_byTaskLocation2;
	uint8_t m_byTaskLocation3;
	uint8_t m_byTaskLocation4;
	uint8_t m_byCardCapacity;
	uint32_t m_dwExplimit;
	uint8_t m_byCurrentTeam;
	uint32_t m_dwLastExplimit;
	uint8_t m_byFlag;
	uint64 m_lFindGroupId;
	uint32_t m_dwAvoidDurationTime;
	uint32_t m_dwWorldHistory;
	uint32_t m_dwFriendLimit;
	uint32_t m_dwGameFlow;
	uint32_t m_dwLuck;
	uint8_t m_byGm;
	uint16_t m_wNextVipLv;
	uint32_t m_dwNextVipExp;
	uint32_t m_dwChargeLuck;
	uint32_t m_dwStamina;
	std::string m_strSignature;

	uint32_t m_dwWorldBroadTime;
	uint32_t m_dwInitNumber;
	bool m_bIsInitOK;
	int32_t m_dwEnergyCnt;
	int32_t m_dwStrengthCnt;
	uint32_t m_dwAddFriend;
	uint32_t m_dwDelFriend;
	uint32_t m_dwAddFriendTime;
	uint32_t m_dwDelFriendTime;
	uint32_t m_dwEveryDayRewardTime;
	uint32_t m_dwUserOnlineTime;
	uint8_t m_byFirstLogin;
	uint32_t m_dwDelDMissionCnt;
	uint32_t m_dwDelDMissionCntTime;
	uint32_t m_dwEnergyUseCnt;
	uint32_t m_dwEnergyUseCntTime;
	uint32_t m_dwStrengthUseCnt;
	uint32_t m_dwStrengthUseCntTime;
	std::set<int> m_setVipGift;
	uint32_t m_dwOneLevelExp;
	uint32_t m_dwGmFlushTime;
	uint32_t m_loginauto;
	uint32_t m_dwHighDelDMissionCnt;
	uint32_t m_dwHighDelDMissionCntTime;
	uint32_t m_dwExtraStrengthPro;
	uint32_t m_dwLowMasterCount;
	uint32_t m_dwLowMasterTime;
	uint32_t m_dwFlowCurrent;
	std::map<uint32_t, uint32_t> m_mapFlow;
	uint8_t m_byFirstGacha;
	uint16_t m_wUseEnergy;
	uint16_t m_wUseStrength;
	uint32_t m_dwEnergyStrengthTime;
	uint32_t m_dwExtraRolePacketLimit;
	uint32_t m_dwExtraEquipPacketLimit;
	uint32_t m_dwLastLoginTime;
	std::set<uint8_t> m_setVipFirstGacha;
	std::map<int, SGift> m_mapGift;
	uint32_t m_dwGiftDayTime;
	uint32_t m_dwGetExtraDollar;
	uint16_t m_dwDotDollar;                   /* accumulate dollar for float */
	uint32_t m_dwCurrentDollarRate;
	uint32_t m_dwNextDollarRate;
	std::vector<SPlayer> m_vecFriend;
	std::map<uint32_t, uint8_t> m_mapJoin;
	uint16_t m_dwTotalJoin;
	std::vector<SJoin> m_vecFJoin;
	std::vector<SJoin> m_vecUJoin;
//	uint8_t m_byIsJoin;
	uint32_t m_dwJoinFlushTime;
	uint32_t m_dwStaminaTime;
	uint32_t m_dwStaminaMax;
	uint32_t m_dwStaminaRate;
	uint16_t m_wUseStamina;
	uint8_t m_byFirstSendRed;
	uint8_t m_byFirstChangeName;
	uint8_t m_bySecondGacha;

	uint32_t m_dwAttackTime;
	uint32_t m_dwAttackEndTime;

//	uint16 m_wMaxLevel; 
//	uint32 m_dwMaxAttack;
//	uint32 m_dwMaxDefence;

	CCardPacket *m_clsCardPacket;
	CItem *m_clsItemPacket;
	CTujian *m_clsTujian;
	CTech *m_clsTech;
	CArena *m_clsArena;
	CShop *m_CShop;
	CMaster *m_CMaster;
	CDurationMission *m_CDMission;
	CActive *m_CActive;
	CRobotCtrl *m_CRobotCtrl;
	CTimerGift *m_CTimerGift;
	CMission *m_CMission;
	CTrial *m_CTrial;
	//TODO
	CTower *m_CTower;

	uint8_t m_byDuration;
	uint16_t m_wOtherDataversion;
//	std::string m_strDevice;
//	std::string m_strDeviceType;
//	std::string m_strSecondChannel;
//	std::string m_strDeviceUid;

	std::vector< std::vector<uint32> > multipleCardExp;//双倍经验丹
};//tolua_export

class CBaseRoleData
{
public:
	typedef struct CARD_BASE
	{
		uint32 index;
		uint16 lv;
		uint32 breaknumber;
		uint32 breakmax;
		uint8 rare;
	} CARD_BASE;
	bool Init(uint32 dwAccountId);
	void PreDataRole(SJoin &tmp);
	void PreDataTeamCardDataForFriend(ByteBuffer &by);
	PROPERTY_DEF_NOUPDATE(uint32, dwAccountId);
	PROPERTY_DEF_NOUPDATE(std::string, strUserName);
	PROPERTY_DEF_NOUPDATE(uint16, wLevel);
//	PROPERTY_DEF_NOUPDATE(uint32, dwExp);
	PROPERTY_DEF_NOUPDATE(uint16, wVipLv);
	PROPERTY_DEF_NOUPDATE(uint32, dwVipExp);
	PROPERTY_DEF_NOUPDATE(uint32, dwDollar);
	PROPERTY_DEF_NOUPDATE(uint32, dwCoin);
	PROPERTY_DEF_NOUPDATE(uint32, dwEnergy);
	PROPERTY_DEF_NOUPDATE(uint32, dwEnergyMax);
	PROPERTY_DEF_NOUPDATE(uint32, dwEnergyRate);
	PROPERTY_DEF_NOUPDATE(uint32, dwStrength);
	PROPERTY_DEF_NOUPDATE(uint32, dwStrengthMax);
	PROPERTY_DEF_NOUPDATE(uint32, dwStrengthRate);
	PROPERTY_DEF_NOUPDATE(uint8_t, byTaskLocation1);
	PROPERTY_DEF_NOUPDATE(uint8_t, byTaskLocation2);
	PROPERTY_DEF_NOUPDATE(uint8_t, byTaskLocation3);
	PROPERTY_DEF_NOUPDATE(uint8_t, byTaskLocation4);
	PROPERTY_DEF_NOUPDATE(uint8_t, byCardCapacity);
	PROPERTY_DEF_NOUPDATE(uint32_t, dwExplimit);
	PROPERTY_DEF_NOUPDATE(uint8_t, byCurrentTeam);
//	PROPERTY_DEF_NOUPDATE(uint32_t, dwLastExplimit);
	PROPERTY_DEF_NOUPDATE(uint8_t, byFlag);
	PROPERTY_DEF_NOUPDATE(uint64, lFindGroupId);
	PROPERTY_DEF_NOUPDATE(uint32_t, dwAvoidDurationTime);
	PROPERTY_DEF_NOUPDATE(uint32_t, dwWorldHistory);
	PROPERTY_DEF_NOUPDATE(uint32_t, dwFriendLimit);
	PROPERTY_DEF_NOUPDATE(uint32_t, dwGameFlow);
	PROPERTY_DEF_NOUPDATE(uint32_t, dwLuck);
	PROPERTY_DEF_NOUPDATE(uint8_t, byGm);
//	PROPERTY_DEF_NOUPDATE(uint16_t, wNextVipLv);
//	PROPERTY_DEF_NOUPDATE(uint32_t, dwNextVipExp);
	PROPERTY_DEF_NOUPDATE(uint32_t, dwChargeLuck);
	PROPERTY_DEF_NOUPDATE(uint32_t, dwStamina);
	PROPERTY_DEF_NOUPDATE(std::string, strSignature);
	PROPERTY_DEF_NOUPDATE(uint32_t, dwTotalForce);

private:
	uint32 m_dwAccountId;
	std::string m_strUserName;
	uint16 m_wLevel;
//	uint32 m_dwExp;
	uint16 m_wVipLv;
	uint32 m_dwVipExp;
	uint32 m_dwDollar;
	uint32 m_dwCoin;
	uint32 m_dwEnergy;
	uint32 m_dwEnergyMax;
	uint32 m_dwEnergyRate;
	uint32 m_dwStrength;
	uint32 m_dwStrengthMax;
	uint32 m_dwStrengthRate;
	uint8_t m_byTaskLocation1;
	uint8_t m_byTaskLocation2;
	uint8_t m_byTaskLocation3;
	uint8_t m_byTaskLocation4;
	uint8_t m_byCardCapacity;
	uint32_t m_dwExplimit;
	uint8_t m_byCurrentTeam;
//	uint32_t m_dwLastExplimit;
	uint8_t m_byFlag;
	uint64 m_lFindGroupId;
	uint32_t m_dwAvoidDurationTime;
	uint32_t m_dwWorldHistory;
	uint32_t m_dwFriendLimit;
	uint32_t m_dwGameFlow;
	uint32_t m_dwLuck;
	uint8_t m_byGm;
//	uint16_t m_wNextVipLv;
//	uint32_t m_dwNextVipExp;
	uint32_t m_dwChargeLuck;
	uint32_t m_dwStamina;
	std::string m_strSignature;
	uint32_t m_dwTotalForce;
	uint32_t m_dwOneLevelExp;
	uint32_t m_dwInitNumber;
	int32_t m_dwEnergyCnt;
	int32_t m_dwStrengthCnt;
	uint32_t m_dwWorldBroadTime;
	uint32_t m_dwStaminaTime;

	uint32_t m_dwLastLoginTime;

	uint32_t m_MasterCard;
	std::map<uint32_t, CARD_BASE> m_mapCards;
};
#endif
