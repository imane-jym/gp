#include "GlobalConfig.h"
#include "miniini/miniini.h"
#include "Log.h"
#include "EnumDefines.h"

#include <stdlib.h>

#include "ConfigData.h"
#include "WorldPacket.h"
#include "Role.h"
#include "CmdDefine.h"

//所有全局参数 如果读取不到 则采用默认值

#define READ_INT_FUN(strRead, nValue) \
do { \
	if (!pConfRole->ReadInt(strRead, nValue)) \
	{ \
		IME_ERROR("GLOBAL INI ERROR [%s]", strRead); \
		return false; \
	} \
} while(0)

#define READ_INTS_FUN(strRead, vecValue) \
do { \
	std::string strTmp = strRead; \
	vecValue.clear(); \
	if (!pConfRole->ReadInts(strTmp, vecValue)) \
	{ \
	IME_ERROR("GLOBAL INI ERROR [%s]", strRead); \
	return false; \
	} \
} while(0)

#define READ_STR_FUN(strRead, strValue) \
do { \
	if (!pConfRole->ReadString(strRead, strValue)) \
	{ \
		IME_ERROR("GLOBAL INI ERROR [%s]", strRead); \
		return false; \
	} \
} while(0)

#define READ_BOOL_FUN(strRead, bValue) \
	do { \
	if (!pConfRole->ReadBool(strRead, bValue)) \
	{ \
		IME_ERROR("GLOBAL INI ERROR [%s]", strRead); \
		return false; \
	} \
} while(0)

#define READ_FLOAT_FUN(strRead, fValue) \
	do { \
	if (!pConfRole->ReadFloat(strRead, fValue)) \
	{ \
		IME_ERROR("GLOBAL INI ERROR [%s]", strRead); \
		return false; \
	} \
} while(0)

#define READ_STR_FUN_SAFE(strRead, strValue, strDefault) \
do { \
	if (!pConfRole->ReadString(strRead, strValue)) \
	{ \
		strValue = strDefault; \
	} \
} while(0)

#define READ_INT_FUN_OTHER(strRead, strValue) \
	if (!configOther->ReadInt(strRead, strValue)) \
	{ \
		IME_ERROR("Missing "strRead" info, use default: %u", strValue);\
	}\

#define READ_INTS_FUN_OTHER(strRead, strValue) \
	strValue.clear();\
	if (!configOther->ReadInts(strRead, strValue)) \
	{ \
		IME_ERROR("Missing "strRead" info");\
	}\

#define READ_STR_FUN_OTHER(strRead, strValue) \
	if (!configOther->ReadString(strRead, strValue)) \
	{ \
		IME_ERROR("Missing "strRead" info, use default: %s", strValue.c_str()); \
	} \

#define READ_STRS_FUN_OTHER(strRead, strValue) \
	strValue.clear();\
	if (!configOther->ReadStrings(strRead, strValue)) \
	{ \
		IME_ERROR("Missing "strRead" info, use default: null"); \
	} \


std::string GlobalConfig::strTestName;
float GlobalConfig::fTestFloat = 0.0f;
int GlobalConfig::nTestInt = 255;
bool GlobalConfig::bTestBool = false;

int GlobalConfig::StrengthConsume = 1;
int GlobalConfig::CombineTime = 0;
int GlobalConfig::FriendLimit = 0;

int GlobalConfig::Prestige = 20;
int GlobalConfig::PrestigePrizeTime = 86400;
int GlobalConfig::RankPrizeTime = 259200;
int GlobalConfig::ChallengeTimes = 20;
int GlobalConfig::ChallengeCD = 600;
int GlobalConfig::CDConsume = 20;
int GlobalConfig::ArenaOpen = 20;
int GlobalConfig::RankingPrizeTimes = 1;

int GlobalConfig::JoinGroupLimit = 20;
int GlobalConfig::CreateGroupLimit = 40;
int GlobalConfig::ConsumeDollarCreate = 50;
int GlobalConfig::FlashMission = 0;
int GlobalConfig::GroupHonourPlus = 1;

int GlobalConfig::BMCDComsume = 200;

int GlobalConfig::MasterLowCon = 10;
int GlobalConfig::MasterMiddleCon = 20;
int GlobalConfig::MasterHighCon = 30;

int GlobalConfig::MasterBack = 1000;
//int GlobalConfig::MasterMiddleBack = 20;
//int GlobalConfig::MasterHighBack = 30;

int GlobalConfig::MasterBlessCD = 20;
int GlobalConfig::MaxRound = 30;

int GlobalConfig::MasterCD1 = 500;
int GlobalConfig::MasterCD2 = 3535;
int GlobalConfig::MasterCD3 = 2352364;
int GlobalConfig::StoryFBossAliveTime = 3600;
int GlobalConfig::ActiveFBossAliveTime = 7200;

int GlobalConfig::NormalGacha = 100;
int GlobalConfig::DollarGacha = 300;

int GlobalConfig::DMissionW1 = 10;		
int GlobalConfig::DMissionW2 = 9;
int GlobalConfig::DMissionW3 = 8;
int GlobalConfig::DMissionW4 = 7;
int GlobalConfig::DMissionW5 = 6;
int GlobalConfig::DMissionW6 = 5;	

int GlobalConfig::FBossStrengthConsume = 1;
int GlobalConfig::FBossPrizeCD = 86400;
int GlobalConfig::StrengthJewelPara = 70;
int GlobalConfig::WorldBroadCD = 60;

int GlobalConfig::AddFriendLimit = 1;
int GlobalConfig::DelFriendLimit = 1;

int GlobalConfig::EverydayLoginCoin = 2000;
int GlobalConfig::EverydayLoginWorldExp = 500;
std::string GlobalConfig::LoginRewardStr = "";
int GlobalConfig::ArenaFirstFlush = 0;
int GlobalConfig::ArenaSecondFlush = 3;
int GlobalConfig::ArenaThirdFlush = 6;
int GlobalConfig::ArenaFourthFlush = 9;
int GlobalConfig::ArenaFifthFlush = 12;
int GlobalConfig::ArenaSixthFlush = 15;
int GlobalConfig::ArenaSeventhFlush = 18;
int GlobalConfig::ArenaEighthFlush = 21;

int GlobalConfig::FriendBossMaxRound = 6;
std::string GlobalConfig::SystemMailSendName = "";

int GlobalConfig::FriendBossHappenLevel = 10;
int GlobalConfig::DurationMissionDelConsume = 100;
int GlobalConfig::HarmFloat = 10;
int GlobalConfig::HighDurationMissionDelConsume = 100;

int GlobalConfig::NormalTurntableConsume = 100;
int GlobalConfig::SuperTurntableConsume = 100;
int GlobalConfig::BossProPara = 10;

//int GlobalConfig::EveryLoginLuck = 60;
//int GlobalConfig::LoginLuckMax = 300;
int GlobalConfig::LuckInitLow = 1;
int GlobalConfig::LuckInitHigh = 30;
int GlobalConfig::MasterLimit1 = 10;
int GlobalConfig::RoleCardLimit = 50;
int GlobalConfig::EquipCardLimit = 100;
int GlobalConfig::FriendLimitLevel = 10;

int GlobalConfig::EnergyWorth = 200;
int GlobalConfig::EnergyWorthGrow = 200;
int GlobalConfig::StrengthWorth = 200;
int GlobalConfig::StrengthWorthGrow = 200;
int GlobalConfig::SwitchCost = 2000;

int GlobalConfig::RecoverEnergyTime1 = 1200;
int GlobalConfig::RecoverEnergyTime2 = 1300;
int GlobalConfig::RecoverEnergyTime3 = 1800;
int GlobalConfig::RecoverEnergyTime4 = 1900;

int GlobalConfig::RecoverEnergy = 20;
int GlobalConfig::RecoverStrength = 10;

int GlobalConfig::RolePacketConsume = 10;
int GlobalConfig::EquipPacketConsume = 10;
int GlobalConfig::RolePacketNumber = 5;
int GlobalConfig::EquipPacketNumber = 5;
int GlobalConfig::InitCoin = 1000;
int GlobalConfig::InitDollar = 1000;
int GlobalConfig::InitPrestige = 500;

std::vector<int> GlobalConfig::vecFirstPrizeType;
std::vector<int> GlobalConfig::vecFirstPrizePara1;
std::vector<int> GlobalConfig::vecFirstPrizePara2;

int GlobalConfig::VipChargeGachaFloors = 3000;

std::string GlobalConfig::BossRankMailTitle;
std::string GlobalConfig::BossRankMailContent;
int GlobalConfig::BossRankCoin = 100;
int GlobalConfig::BossRankWorldExp = 100;

std::string GlobalConfig::JoinMailTitle;
std::string GlobalConfig::JoinMailContent1;
std::string GlobalConfig::JoinMailContent2;
int GlobalConfig::JoinWorldHistroy = 100;
int GlobalConfig::JoinStrangerWorldHistroy = 50;

int GlobalConfig::ActiveDurationMissionDelConsume = 50;
std::string GlobalConfig::AchieveMailTitle;
std::string GlobalConfig::AchieveMailContent;

int GlobalConfig::PlusMaxlimit = 50;
int GlobalConfig::PlusEveryVal = 50;

int GlobalConfig::StaminaWorth = 200;
int GlobalConfig::StaminaWorthGrow = 200;

int GlobalConfig::TrialFriendWorldHistory = 100;

int GlobalConfig::WorldHistoryGachaPro = 100;
int GlobalConfig::HeroGachaPro = 100;
int GlobalConfig::DollarGachaPro = 100;

int GlobalConfig::ChangeConsume = 100;

int GlobalConfig::ArenaLosePrestigeGet = 10;
int GlobalConfig::ArenaWinPrestigeGet = 5;

int GlobalConfig::FirstChargeMulti = 200;
int GlobalConfig::ChargeSegmentLine = 0;

int GlobalConfig::SecondChargeCardIndex = 60108;
std::string GlobalConfig::NoticeTypeName = "";

std::string GlobalConfig::NoticeSpecificCard = "";
std::string GlobalConfig::NoticeSpecificItem = "";
int GlobalConfig::NoticeSpecificDollarLimit = 100;
std::vector<int> GlobalConfig::ChannelSpecialGift;
std::vector<int> GlobalConfig::ChannelSpecial;
std::vector<std::string> GlobalConfig::ChannelSpecialMailTitle;
std::vector<std::string> GlobalConfig::ChannelSpecialMailContent;

std::vector<int> GlobalConfig::DropIndex;
std::vector<int> GlobalConfig::DropLimit;

int GlobalConfig::DefaultDropType = 0;
int GlobalConfig::DefaultDropPara1 = 0;
int GlobalConfig::DefaultDropPara2 = 0;

//TODO cyl
int GlobalConfig::TowerLimit = 0;
int GlobalConfig::ThreeAttackCost = 100;
int GlobalConfig::TwoAttackCost = 50;
int GlobalConfig::BossNatureCost = 50;
int GlobalConfig::BuyBreakBlockadeTimesCost = 100;
int GlobalConfig::BuyBreakBlockadeTimesLimit = 0;

std::string GlobalConfig::TowerMailContent = "";
std::string GlobalConfig::TowerMailTitle ="";


int GlobalConfig::RecoverChallengesConsume = 50;
std::vector<int> GlobalConfig::InitSkillCard;

	
void GlobalConfig::SendData(CRole *pCRole)
{
	WorldPacket info(CMD_SC_GLOBAL_INFO);
	info << JoinGroupLimit;
	info << CreateGroupLimit;
	info << ConsumeDollarCreate;
	info << DurationMissionDelConsume;
	info << HighDurationMissionDelConsume;
	info << NormalGacha;
	info << DollarGacha;
	info << StrengthJewelPara;
	info << StrengthConsume;
	info << FBossStrengthConsume;
	info << SwitchCost;
	info << GlobalConfig::RolePacketConsume;
	info << GlobalConfig::RolePacketNumber;
	info << GlobalConfig::EquipPacketConsume;
	info << GlobalConfig::EquipPacketNumber;
	info << (uint32_t) vecFirstPrizeType.size();
	for (int i = 0; i < vecFirstPrizeType.size(); i++)
	{
		info << vecFirstPrizeType[i];
		info << vecFirstPrizePara1[i];
		info << vecFirstPrizePara2[i];
	}
	info << GlobalConfig::ActiveDurationMissionDelConsume;
	info << GlobalConfig::ChangeConsume;
	info << GlobalConfig::PlusEveryVal;
	info << BuyBreakBlockadeTimesCost;
	info << ThreeAttackCost;
	info << TwoAttackCost;
	info << BossNatureCost;
	info << GlobalConfig::RecoverChallengesConsume;
	pCRole->SendPacket(&info);
}

bool GlobalConfig::Init(std::string strPath)
{
	INIFile configIni;
	if (!configIni.OpenFile(strPath.c_str()))
	{
		IME_ERROR("Cannot open %s",strPath.c_str());
		return false;
	}

	//section test
	INISection * configTest = configIni.GetSection("test");
	if (!configTest)
	{
		IME_ERROR("Miss section [test] in %s",strPath.c_str());
		return false;
	}

	if (!configTest->ReadString("test_name",strTestName))
	{
		IME_ERROR("Missing test_name info, use default: %s",strTestName.c_str());
	}

	if (!configTest->ReadFloat("test_float",fTestFloat))
	{
		IME_ERROR("Missing test_float info, use default: %f",fTestFloat);
	}

	if (!configTest->ReadInt("test_int",nTestInt))
	{
		IME_ERROR("Missing test_int info, use default: %d",nTestInt);
	}

	if (!configTest->ReadBool("test_bool",bTestBool))
	{
		IME_ERROR("Missing test_bool info, use default: %d",bTestBool);
	}
	
	INISection *configDuration = configIni.GetSection("Duration");
	if (!configDuration)
	{
		IME_ERROR("Miss section [Duration] in %s",strPath.c_str());
		return false;
	}

	if (!configDuration->ReadInt("strength_consume", StrengthConsume))
	{
		IME_ERROR("Missing strength_consume info, use default: %d", StrengthConsume);
	}

	INISection *configCombine = configIni.GetSection("Combine");
	if (!configCombine)
	{
		IME_ERROR("Miss section [Combine] in %s",strPath.c_str());
		return false;
	}

	if (!configCombine->ReadInt("combine_time", CombineTime))
	{
		IME_ERROR("Missing combinetime info, use default: %d", CombineTime);
	}

	INISection *configFriend = configIni.GetSection("Friends");
	if (!configFriend)
	{
		IME_ERROR("Miss section [Friends] in %s",strPath.c_str());
		return false;
	}

	if (!configFriend->ReadInt("max_friends", FriendLimit))
	{
		IME_ERROR("Missing max_friends info, use default: %d", FriendLimit);
	}

	INISection *configArea = configIni.GetSection("Arena");
	if (!configFriend)
	{
		IME_ERROR("Miss section [Area] in %s",strPath.c_str());
		return false;
	}

	if (!configArea->ReadInt("prestige_plus", Prestige))
	{
		IME_ERROR("Missing prestige_plus info, use default: %d", Prestige);
	}
	if (!configArea->ReadInt("prestige_prize", PrestigePrizeTime))
	{
		IME_ERROR("Missing prestige_prize info, use default: %d", PrestigePrizeTime);
	}
	if (!configArea->ReadInt("ranking_prize", RankPrizeTime))
	{
		IME_ERROR("Missing ranking_prize info, use default: %d", RankPrizeTime);
	}
	if (!configArea->ReadInt("challenge_times", ChallengeTimes))
	{
		IME_ERROR("Missing challenge_times info, use default: %d", ChallengeTimes);
	}
	if (!configArea->ReadInt("challenge_time", ChallengeCD))
	{
		IME_ERROR("Missing challenge_time info, use default: %d", ChallengeCD);
	}
	if (!configArea->ReadInt("cd_consume", CDConsume))
	{
		IME_ERROR("Missing cd_consume info, use default: %d", CDConsume);
	}
	if (!configArea->ReadInt("arena_open", ArenaOpen))
	{
		IME_ERROR("Missing arena_open info, use default: %d", ArenaOpen);
	}
	if (!configArea->ReadInt("ranking_prize_times", RankingPrizeTimes))
	{
		IME_ERROR("Missing ranking_prize_times info, use default: %d", RankingPrizeTimes);
	}
	
	GlobalConfig::JoinGroupLimit = 20;
	GlobalConfig::CreateGroupLimit = 40;
	
//	int32 *pVal32; 
//	pVal32 = CConfGlobal::Find(E_GLOBAL_JOIN_GROUP_LIMIT, JoinGroupLimit);
//	if (pVal32 == NULL)
//	{
//		IME_ERROR("Missing joinGroupLimit info, use default: %d", JoinGroupLimit);
//	}
//	
//	pVal32 = CConfGlobal::Find(E_GLOBAL_CREATE_GROUP_LIMIT, CreateGroupLimit);
//	if (pVal32 == NULL)
//	{
//		IME_ERROR("Missing joinGroupLimit info, use default: %d", CreateGroupLimit);
//	}

	INISection *configGroup = configIni.GetSection("Group");
	if (!configGroup)
	{
		IME_ERROR("Miss section [Group] in %s",strPath.c_str());
		return false;
	}

	if (!configGroup->ReadInt("consume_dollar_create", ConsumeDollarCreate))
	{
		IME_ERROR("Missing consume_dollar_create info, use default: %d", ConsumeDollarCreate);
	}
	if (!configGroup->ReadInt("flash_mission", FlashMission))
	{
		IME_ERROR("Missing flash_mission info, use default: %d", FlashMission);
	}
	if (!configGroup->ReadInt("group_honour_plus", GroupHonourPlus))
	{
		IME_ERROR("Missing group_honour_plus info, use default: %d", GroupHonourPlus);
	}

	INISection *configMission = configIni.GetSection("Mission");
	if (!configMission)
	{
		IME_ERROR("Miss section [Mission] in %s",strPath.c_str());
	}

	if (!configMission->ReadInt("cd_consume", BMCDComsume))
	{
		IME_ERROR("Missing cd_consume info, use default: %d", BMCDComsume);
	}

	INISection *configMaster = configIni.GetSection("Master");
	if (!configMaster)
	{
		IME_ERROR("Miss section [Master] in %s",strPath.c_str());
	}

	if (!configMaster->ReadInt("master_consume1", MasterLowCon))
	{
		IME_ERROR("Missing master_consume1 info, use default: %d", MasterLowCon);
	}

	if (!configMaster->ReadInt("master_consume2", MasterMiddleCon))
	{
		IME_ERROR("Missing master_consume2 info, use default: %d", MasterMiddleCon);
	}

	if (!configMaster->ReadInt("master_consume3", MasterHighCon))
	{
		IME_ERROR("Missing master_consume3 info, use default: %d", MasterHighCon);
	}

	if (!configMaster->ReadInt("master_back1", MasterBack))
	{
		IME_ERROR("Missing master_back1 info, use default: %d", MasterBack);
	}

//	if (!configMaster->ReadInt("master_back2", MasterMiddleBack))
//	{
//		IME_ERROR("Missing master_back2 info, use default: %d", MasterMiddleBack);
//	}
//
//	if (!configMaster->ReadInt("master_back3", MasterHighBack))
//	{
//		IME_ERROR("Missing master_back3 info, use default: %d", MasterHighBack);
//	}

	if (!configMaster->ReadInt("blast_time", MasterBlessCD))
	{
		IME_ERROR("Missing blast_time info, use default: %d", MasterBlessCD);
	}

	if (!configMaster->ReadInt("master_cd1", MasterCD1))
	{
		IME_ERROR("Missing master_cd1 info, use default: %d", MasterCD1);
	}

	if (!configMaster->ReadInt("master_cd2", MasterCD2))
	{
		IME_ERROR("Missing master_cd2 info, use default: %d", MasterCD2);
	}

	if (!configMaster->ReadInt("master_cd3", MasterCD3))
	{
		IME_ERROR("Missing master_cd3 info, use default: %d", MasterCD3);
	}

	INISection *configOther = configIni.GetSection("other");
	if (!configOther)
	{
		IME_ERROR("Miss section [other] in %s",strPath.c_str());
	}

	if (!configOther->ReadInt("max_round", MaxRound))
	{
		IME_ERROR("Missing max_round info, use default: %d", MaxRound);
	}
	
	if (!configOther->ReadInt("story_fboss_alive_time", StoryFBossAliveTime))
	{
		IME_ERROR("Missing story_fboss_alive_time info, use default: %d", StoryFBossAliveTime);
	}

	if (!configOther->ReadInt("active_fboss_alive_time", ActiveFBossAliveTime))
	{
		IME_ERROR("Missing active_fboss_alive_time info, use default: %d", ActiveFBossAliveTime);
	}

	if (!configOther->ReadInt("normal_consume", NormalGacha))
	{
		IME_ERROR("Missing normal_consume info, use default: %d", NormalGacha);
	}

	if (!configOther->ReadInt("dollar_consume", DollarGacha))
	{
		IME_ERROR("Missing dollar_consume info, use default: %d", DollarGacha);
	}

	if (!configOther->ReadInt("dmissionw1", DMissionW1))
	{
		IME_ERROR("Missing DMissionW1 info, use default: %d", DMissionW1);
	}

	if (!configOther->ReadInt("dmissionw2", DMissionW2))
	{
		IME_ERROR("Missing DMissionW2 info, use default: %d", DMissionW2);
	}

	if (!configOther->ReadInt("dmissionw3", DMissionW3))
	{
		IME_ERROR("Missing DMissionW3 info, use default: %d", DMissionW3);
	}

	if (!configOther->ReadInt("dmissionw4", DMissionW4))
	{
		IME_ERROR("Missing DMissionW4 info, use default: %d", DMissionW4);
	}

	if (!configOther->ReadInt("dmissionw5", DMissionW5))
	{
		IME_ERROR("Missing DMissionW5 info, use default: %d", DMissionW5);
	}

	if (!configOther->ReadInt("dmissionw6", DMissionW6))
	{
		IME_ERROR("Missing DMissionW6 info, use default: %d", DMissionW6);
	}

	if (!configOther->ReadInt("friend_boss_strength_consume", FBossStrengthConsume))
	{
		IME_ERROR("Missing friend_boss_strength_consume info, use default: %d", FBossStrengthConsume);
	}

	if (!configOther->ReadInt("friend_boss_prize_cd", FBossPrizeCD))
	{
		IME_ERROR("Missing friend_boss_prize_cd info, use default: %d", FBossPrizeCD);
	}

	if (!configOther->ReadInt("strength_jewel_para", StrengthJewelPara))
	{
		IME_ERROR("Missing strength_jewel_para info, use default: %d", StrengthJewelPara);
	}

	if (!configOther->ReadInt("world_broad_cd", WorldBroadCD))
	{
		IME_ERROR("Missing world_broad_cd info, use default: %d", WorldBroadCD);
	}

	if (!configOther->ReadInt("add_friend_limit", AddFriendLimit))
	{
		IME_ERROR("Missing add_friend_limit info, use default: %d", AddFriendLimit);
	}

	if (!configOther->ReadInt("del_friend_limit", DelFriendLimit))
	{
		IME_ERROR("Missing del_friend_limit info, use default: %d", DelFriendLimit);
	}

	if (!configOther->ReadInt("everyday_login_coin", EverydayLoginCoin))
	{
		IME_ERROR("Missing everyday_login_coin info, use default: %d", EverydayLoginCoin);
	}

	if (!configOther->ReadInt("everyday_login_worldexp", EverydayLoginWorldExp))
	{
		IME_ERROR("Missing everyday_login_worldexp info, use default: %d", EverydayLoginWorldExp);
	}

	if (!configOther->ReadString("everyday_login", LoginRewardStr))
	{
		IME_ERROR("Missing everyday_login info, use default: %s", LoginRewardStr.c_str());
	}

	if (!configOther->ReadInt("arena_first_flush", ArenaFirstFlush))
	{
		IME_ERROR("Missing arena_first_flush info, use default: %d", ArenaFirstFlush);
	}
	if (!configOther->ReadInt("arena_second_flush", ArenaSecondFlush))
	{
		IME_ERROR("Missing arena_second_flush info, use default: %d", ArenaSecondFlush);
	}

	if (!configOther->ReadInt("arena_third_flush", ArenaThirdFlush))
	{
		IME_ERROR("Missing arena_third_flush info, use default: %d", ArenaThirdFlush);
	}

	if (!configOther->ReadInt("friend_boss_max_round", FriendBossMaxRound))
	{
		IME_ERROR("Missing friend_boss_max_round info, use default: %d", FriendBossMaxRound);
	}
	if (!configOther->ReadString("system_mail_send_name", SystemMailSendName))
	{
		IME_ERROR("Missing system_mail_send_name info, use default: %s", SystemMailSendName.c_str());
	}
	if (!configOther->ReadInt("boss_happen_level", FriendBossHappenLevel))
	{
		IME_ERROR("Missing boss_happen_level info, use default: %u", FriendBossHappenLevel);
	}
	if (!configOther->ReadInt("duration_mission_del_consume", DurationMissionDelConsume))
	{
		IME_ERROR("Missing duration_mission_del_consume info, use default: %u", DurationMissionDelConsume);
	}
	if (!configOther->ReadInt("harm_float", HarmFloat))
	{
		IME_ERROR("Missing harm_float info, use default: %u", HarmFloat);
	}

	if (!configOther->ReadInt("hard_duraction_mission_del_consume", HighDurationMissionDelConsume))
	{
		IME_ERROR("Missing hard_duraction_mission_del_consume info, use default: %u", HighDurationMissionDelConsume);
	}

	if (!configOther->ReadInt("normal_turntable_consume", NormalTurntableConsume))
	{
		IME_ERROR("Missing normal_turntable_consume info, use default: %u", NormalTurntableConsume);
	}

	if (!configOther->ReadInt("super_turntable_consume", SuperTurntableConsume))
	{
		IME_ERROR("Missing Super_turntable_consume info, use default: %u", SuperTurntableConsume);
	}

	if (!configOther->ReadInt("boss_pro_para", BossProPara))
	{
		IME_ERROR("Missing boss_pro_para info, use default: %u", BossProPara);
	}

//	if (!configOther->ReadInt("every_login_luck", EveryLoginLuck))
//	{
//		IME_ERROR("Missing every_login_luck info, use default: %u", EveryLoginLuck);
//	}
//	if (!configOther->ReadInt("login_luck_max", LoginLuckMax))
//	{
//		IME_ERROR("Missing login_luck_max info, use default: %u", LoginLuckMax);
//	}
	if (!configOther->ReadInt("luck_init_low", LuckInitLow))
	{
		IME_ERROR("Missing luck_init_low info, use default: %u", LuckInitLow);
	}
	if (!configOther->ReadInt("luck_init_high", LuckInitHigh))
	{
		IME_ERROR("Missing luck_init_max info, use default: %u", LuckInitHigh);
	}
	if (!configOther->ReadInt("master_limited1", MasterLimit1))
	{
		IME_ERROR("Missing master_limited1 info, use default: %u", MasterLimit1);
	}
	if (!configOther->ReadInt("role_card_limit", RoleCardLimit))
	{
		IME_ERROR("Missing role_card_limit info, use default: %u", RoleCardLimit);
	}
	if (!configOther->ReadInt("equip_card_limit", EquipCardLimit))
	{
		IME_ERROR("Missing equip_card_limit info, use default: %u", EquipCardLimit);
	}
	if (!configOther->ReadInt("friend_limit_level", FriendLimitLevel))
	{
		IME_ERROR("Missing friend_limit_level info, use default: %u", FriendLimitLevel);
	}
	if (!configOther->ReadInt("energy_worth", EnergyWorth))
	{
		IME_ERROR("Missing energy_worth info, use default: %u", EnergyWorth);
	}
	if (!configOther->ReadInt("energy_worth_grow", EnergyWorthGrow))
	{
		IME_ERROR("Missing energy_worth_grow info, use default: %u", EnergyWorthGrow);
	}
	if (!configOther->ReadInt("strength_worth", StrengthWorth))
	{
		IME_ERROR("Missing strength_worth info, use default: %u", StrengthWorth);
	}
	if (!configOther->ReadInt("strength_worth_grow", StrengthWorthGrow))
	{
		IME_ERROR("Missing strength_worth_grow info, use default: %u", StrengthWorthGrow);
	}
	if (!configOther->ReadInt("switch_cost", SwitchCost))
	{
		IME_ERROR("Missing switch_cost info, use default: %u", SwitchCost);
	}

	if (!configOther->ReadInt("recover_energy_time1", RecoverEnergyTime1))
	{
		IME_ERROR("Missing recover_energy_time1 info, use default: %u", RecoverEnergyTime1);
	}
	if (!configOther->ReadInt("recover_energy_time2", RecoverEnergyTime2))
	{
		IME_ERROR("Missing recover_energy_time2 info, use default: %u", RecoverEnergyTime2);
	}
	if (!configOther->ReadInt("recover_energy_time3", RecoverEnergyTime3))
	{
		IME_ERROR("Missing recover_energy_time3 info, use default: %u", RecoverEnergyTime3);
	}
	if (!configOther->ReadInt("recover_energy_time4", RecoverEnergyTime4))
	{
		IME_ERROR("Missing recover_energy_time4 info, use default: %u", RecoverEnergyTime4);
	}

	if (!configOther->ReadInt("recover_energy", RecoverEnergy))
	{
		IME_ERROR("Missing recover_energy info, use default: %u", RecoverEnergy);
	}
	if (!configOther->ReadInt("recover_strength", RecoverStrength))
	{
		IME_ERROR("Missing recover_strength info, use default: %u", RecoverStrength);
	}

	if (!configOther->ReadInt("role_packet_consume", RolePacketConsume))
	{
		IME_ERROR("Missing role_packet_consume info, use default: %u", RolePacketConsume);
	}

	if (!configOther->ReadInt("equip_packet_consume", EquipPacketConsume))
	{
		IME_ERROR("Missing equip_packet_consume info, use default: %u", EquipPacketConsume);
	}
	READ_INT_FUN_OTHER("role_packet_number", RolePacketNumber);
	READ_INT_FUN_OTHER("equip_packet_number", EquipPacketNumber);
	READ_INT_FUN_OTHER("init_coin", InitCoin);
	READ_INT_FUN_OTHER("init_dollar", InitDollar);
	READ_INT_FUN_OTHER("init_prestige", InitPrestige);
	READ_INTS_FUN_OTHER("first_reward_type", vecFirstPrizeType);
	READ_INTS_FUN_OTHER("first_reward1_para", vecFirstPrizePara1);
	READ_INTS_FUN_OTHER("first_reward2_para", vecFirstPrizePara2);
	if (vecFirstPrizeType.size() != 5 && vecFirstPrizePara1.size() != 5 && vecFirstPrizePara2.size() != 5)
	{
		IME_ERROR("first prize is not 5");
		return false;
	}
	READ_INT_FUN_OTHER("vip_recharge_rmb", VipChargeGachaFloors);

	READ_STR_FUN_OTHER("boss_rank_mail_title", BossRankMailTitle);
	READ_STR_FUN_OTHER("boss_rank_mail_content", BossRankMailContent);
	READ_INT_FUN_OTHER("boss_rank_mail_reward1_coin", BossRankCoin);
	READ_INT_FUN_OTHER("boss_rank_mail_reward2_worldexp", BossRankWorldExp);

	READ_STR_FUN_OTHER("join_attack_prize_mail_title", JoinMailTitle);
	READ_STR_FUN_OTHER("join_attack_prize_mail_content1", JoinMailContent1);
	READ_STR_FUN_OTHER("join_attack_prize_mail_content2", JoinMailContent2);
	READ_INT_FUN_OTHER("join_attack_prize_world_history", JoinWorldHistroy);
	READ_INT_FUN_OTHER("join_attack_prize_stranger_world_history", JoinStrangerWorldHistroy);

	READ_INT_FUN_OTHER("active_duration_mission_del_consume", ActiveDurationMissionDelConsume);
	READ_STR_FUN_OTHER("achieve_mail_title", AchieveMailTitle);
	READ_STR_FUN_OTHER("achieve_mail_content", AchieveMailContent);

	READ_INT_FUN_OTHER("plus_max_limit", PlusMaxlimit);
	READ_INT_FUN_OTHER("plus_every_point", PlusEveryVal);

	READ_INT_FUN_OTHER("stamina_worth", StaminaWorth);
	READ_INT_FUN_OTHER("stamina_worthgrow", StaminaWorthGrow);

	READ_INT_FUN_OTHER("trial_friend_worldhistory", TrialFriendWorldHistory);

	READ_INT_FUN_OTHER("world_history_gacha_pro", WorldHistoryGachaPro);
	READ_INT_FUN_OTHER("hero_gacha_pro", HeroGachaPro);
	READ_INT_FUN_OTHER("dollar_gacha_pro", DollarGachaPro);

	READ_INT_FUN_OTHER("change_consume", ChangeConsume);
	READ_INT_FUN_OTHER("arena_lost_prestige_get", ArenaLosePrestigeGet);
	READ_INT_FUN_OTHER("arena_win_prestige_get", ArenaWinPrestigeGet);
	READ_INT_FUN_OTHER("first_charge_multi", FirstChargeMulti);
	READ_INT_FUN_OTHER("charge_segment_line", ChargeSegmentLine);
	READ_INT_FUN_OTHER("second_charge_card_index", SecondChargeCardIndex);
	READ_STR_FUN_OTHER("notice_type_name", NoticeTypeName);
	READ_STR_FUN_OTHER("notice_specific_card", NoticeSpecificCard);
	READ_STR_FUN_OTHER("notice_specific_item", NoticeSpecificItem);
	READ_INT_FUN_OTHER("notice_specific_dollar_limit", NoticeSpecificDollarLimit);

	READ_INTS_FUN_OTHER("channel_special_gift", ChannelSpecialGift);
	READ_INTS_FUN_OTHER("channel_special", ChannelSpecial);
	READ_STRS_FUN_OTHER("channel_special_mail_title", ChannelSpecialMailTitle);
	READ_STRS_FUN_OTHER("channel_special_mail_content", ChannelSpecialMailContent);

	int a = ChannelSpecialGift.size();
	if (a != ChannelSpecial.size() || a != ChannelSpecialMailTitle.size() || a != ChannelSpecialMailContent.size())
	{
		IME_ERROR("channel size is not equal gift size");
		return false;
	}

	READ_INTS_FUN_OTHER("drop_index", DropIndex);
	READ_INTS_FUN_OTHER("drop_limit", DropLimit);
	if (DropIndex.size() != DropLimit.size())
	{
		IME_ERROR("drop index size is not equal drop limit");
		return false;
	}
	READ_INT_FUN_OTHER("default_drop_type", DefaultDropType);
	READ_INT_FUN_OTHER("default_drop_para1", DefaultDropPara1);
	READ_INT_FUN_OTHER("default_drop_para2", DefaultDropPara2);
	//tower
	READ_INT_FUN_OTHER("tower_limited", TowerLimit);
	READ_INT_FUN_OTHER("triple_attack_cost", ThreeAttackCost);
	READ_INT_FUN_OTHER("double_attack_cost", TwoAttackCost);
	READ_INT_FUN_OTHER("boss_nature_cost", BossNatureCost);
	READ_INT_FUN_OTHER("tower_buy_times_cost",BuyBreakBlockadeTimesCost);
	READ_INT_FUN_OTHER("tower_buy_times_limit",BuyBreakBlockadeTimesLimit);
	READ_STR_FUN_OTHER("tower_mail_content",TowerMailContent);
	READ_STR_FUN_OTHER("tower_mail_title",TowerMailTitle);

	if (GlobalConfig::DefaultDropType != E_OBJ_DOLLAR && GlobalConfig::DefaultDropType != E_OBJ_COIN)
	{
		IME_ERROR("default type just support dollar and coin");
		return false;
	}
	std::vector<SProduct> vproduct;
	SProduct duct;
	duct.type = GlobalConfig::DefaultDropType;
	duct.para1 = GlobalConfig::DefaultDropPara1;
	duct.para2 = GlobalConfig::DefaultDropPara2;
	vproduct.push_back(duct);
	if (CShopEffect::IsObjValid(vproduct))
	{
		IME_ERROR("default product is unvalid");
		return false;
	}

	READ_INT_FUN_OTHER("recover_challenges_consume", RecoverChallengesConsume);
	READ_INTS_FUN_OTHER("init_skill_card", InitSkillCard);
	return true;
}
