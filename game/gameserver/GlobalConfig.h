#ifndef _GLOBAL_CONFIG_H
#define _GLOBAL_CONFIG_H


#include <string>
#include <vector>
#include "Defines.h"

class CRole;
class GlobalConfig
{
public:
	//初始化 全局参数
	static bool Init(std::string strPath);
	
	static void SendData(CRole *pCRole);
	//section test

	static std::string strTestName;		// XXX的名字
	static float fTestFloat;			// XXX参数 用于XXX
	static int nTestInt;				// XXX参数 用于XXX
	static bool bTestBool;				// XXX参数 用于XXX

	static int StrengthConsume;
	static int CombineTime;

	static int FriendLimit;

	static int Prestige;
	static int PrestigePrizeTime;
	static int RankPrizeTime;
	static int ChallengeTimes;
	static int ChallengeCD;
	static int CDConsume;
	static int ArenaOpen;
	static int RankingPrizeTimes;

	static int JoinGroupLimit;
	static int CreateGroupLimit;
	static int ConsumeDollarCreate;

	static int FlashMission;
	static int GroupHonourPlus;

	static int BMCDComsume;

	static int MasterLowCon;
	static int MasterMiddleCon;
	static int MasterHighCon;

	static int MasterBack;

	static int MasterBlessCD;

	static int MasterCD1;
	static int MasterCD2;
	static int MasterCD3;

	static int MaxRound;
	static int StoryFBossAliveTime;
	static int ActiveFBossAliveTime;

	static int NormalGacha;
	static int DollarGacha;

	static int DMissionW1;		
	static int DMissionW2;
	static int DMissionW3;
	static int DMissionW4;
	static int DMissionW5;
	static int DMissionW6;	

	static int FBossStrengthConsume;
	static int FBossPrizeCD;

	static int StrengthJewelPara;
	static int WorldBroadCD;
	
	static int AddFriendLimit;
	static int DelFriendLimit;                  /* no use */

	static int EverydayLoginCoin;
	static int EverydayLoginWorldExp;

	static std::string LoginRewardStr;

	static int ArenaFirstFlush;
	static int ArenaSecondFlush;
	static int ArenaThirdFlush;
	static int ArenaFourthFlush;
	static int ArenaFifthFlush;
	static int ArenaSixthFlush;
	static int ArenaSeventhFlush;
	static int ArenaEighthFlush;
	
	static int FriendBossMaxRound;
	static std::string SystemMailSendName;

	static int FriendBossHappenLevel;
	static int DurationMissionDelConsume;

	static int HarmFloat;
	static int HighDurationMissionDelConsume;
	
	static int NormalTurntableConsume;
	static int SuperTurntableConsume;
	static int BossProPara;

//	static int EveryLoginLuck;
//	static int LoginLuckMax;
	static int LuckInitLow;
	static int LuckInitHigh;

	static int MasterLimit1;
	
	static int RoleCardLimit;
	static int EquipCardLimit;

	static int FriendLimitLevel;
	static int EnergyWorth;
	static int EnergyWorthGrow;
	static int StrengthWorth;
	static int StrengthWorthGrow;
	static int SwitchCost;

	static int RecoverEnergyTime1;
	static int RecoverEnergyTime2;
	static int RecoverEnergyTime3;
	static int RecoverEnergyTime4;

	static int RecoverEnergy;
	static int RecoverStrength;

	static int RolePacketConsume;
	static int EquipPacketConsume;
	static int RolePacketNumber;
	static int EquipPacketNumber;
	
	static int InitCoin;
	static int InitDollar;
	static int InitPrestige;
	static std::vector<int> vecFirstPrizeType;
	static std::vector<int> vecFirstPrizePara1;
	static std::vector<int> vecFirstPrizePara2;

	static int VipChargeGachaFloors;
	
	static std::string BossRankMailTitle;
	static std::string BossRankMailContent;
	static int BossRankCoin;
	static int BossRankWorldExp;
	static std::string JoinMailTitle;
	static std::string JoinMailContent1;
	static std::string JoinMailContent2;
	static int JoinWorldHistroy;
	static int JoinStrangerWorldHistroy;

	static int ActiveDurationMissionDelConsume;
	static std::string AchieveMailTitle;
	static std::string AchieveMailContent;
	static int PlusMaxlimit;
	static int PlusEveryVal;

	static int StaminaWorth;
	static int StaminaWorthGrow;

	static int TrialFriendWorldHistory;

	static int WorldHistoryGachaPro;
	static int HeroGachaPro;
	static int DollarGachaPro;

	static int ChangeConsume;

	static int ArenaLosePrestigeGet;
	static int ArenaWinPrestigeGet;

	static int FirstChargeMulti;
	static int ChargeSegmentLine;

	static int SecondChargeCardIndex;
	static std::string NoticeTypeName;

	static std::string NoticeSpecificCard;
	static std::string NoticeSpecificItem;
	static int NoticeSpecificDollarLimit;

	static std::vector<int> ChannelSpecialGift;
	static std::vector<int> ChannelSpecial;
	static std::vector<std::string> ChannelSpecialMailTitle;
	static std::vector<std::string> ChannelSpecialMailContent;

	static std::vector<int> DropIndex;
	static std::vector<int> DropLimit;
	static int DefaultDropType;
	static int DefaultDropPara1;
	static int DefaultDropPara2;

	//tower
	static int TowerLimit;//每天闯塔层数限制
	static int ThreeAttackCost;//塔3攻击倍数消耗
	static int TwoAttackCost;//塔2倍攻击倍数消耗
	static int BossNatureCost;//#塔刷新属性消耗
	static int BuyBreakBlockadeTimesCost;//塔次数购买消耗
	static int BuyBreakBlockadeTimesLimit;//塔购买次数限制


	static std::string TowerMailContent;
	static std::string TowerMailTitle;

	static int RecoverChallengesConsume;

	static std::vector<int> InitSkillCard;

private:

};

#endif
