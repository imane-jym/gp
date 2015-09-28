/*
 * =====================================================================================
 *
 *       Filename:  Other.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年02月04日 13时21分11秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Other.h"
#include "DBCtrl.h"
#include "ConfigData.h"
#include "Util.h"
#include "ErrorMsg.h"
#include "CmdDefine.h"
#include "Role.h"
//#include "RandomEvent.h"
#include "World.h"
#include "EnumDefines.h"
#include "CommonType.h"
#include "Shop.h"
#include "CardPacket.h"
#include "Mission.h"
#include "CsvReader.h"
#include "Mail.h"
#include "CenterDBCtrl.h"

bool mybig(SFBRank a, SFBRank b)
{
	return a.devote > b.devote;
}

CAnnouncement::CAnnouncement()
{

}

bool CAnnouncement::Init()
{
	if (!CDBCtrl::GetAnnouncementData(m_vecContent))
	{
		IME_ERROR("init announcemenetdata fail");
		return false;
	}
	return true;
}

void
CAnnouncement::HandlerInfo (CRole *pCRole)
{
	WorldPacket pkg(CMD_SC_ANNOUNCEMENT_INFO);
	pkg << (uint16_t) m_vecContent.size();
	for (int i = 0; i < m_vecContent.size(); i++)
	{
		pkg << m_vecContent[i];
	}
	pCRole->SendPacket(&pkg);
	return ;
}		/* -----  end of method CAnnouncement::HandlerInfo  ----- */

void
CAnnouncement::HandlerUpdateContent (const std::string &strAd, uint32 dwId)
{
	if (dwId > 10)
	{
		return;
	}
	m_vecContent[dwId - 1] = strAd;
	CDBCtrl::SaveAnnouncementData(strAd, dwId);

	WorldPacket pkg(CMD_SC_ANNOUNCEMENT_INFO);
	pkg << (uint16_t) m_vecContent.size();
	for (int i = 0; i < m_vecContent.size(); i++)
	{
		pkg << m_vecContent[i];
	}
	sSessCtrl->SendPkgToAll(pkg);
	return ;
}		/* -----  end of method CAnnouncement::HandlerUpdateContent  ----- */

CActive::CActive()
{
	m_dwGiftTime = 0;
	m_dwGetNumber = 0;
	m_dwMaxNumber = 0;
	m_dwCD = 0;
	m_dwAcceptTime = 0;
	m_dwRandomIndex = 0;
	m_IsInit = false;

//	m_bySign = 1;
	m_dwSign = 0;
	m_dwSignTime = 0;
	m_dwSignFlushTime = 0;
	m_dwUsedExtra = 0;
}

int
CActive::Init (CRole *pCRole)
{
	m_pCRole = pCRole;
	std::string temp;
	ByteBuffer by;
	if (!CDBCtrl::GetUserDataActive(temp, m_pCRole->GetdwAccountId()))
	{
		IME_ERROR("can not get active data userid %u", m_pCRole->GetdwAccountId());
		return -1;
	}
	by.append(temp.data(), temp.size());
	DeSeriliazeDBGift(by);

	if (!CDBCtrl::GetUserOneActiveData(m_pCRole->GetdwAccountId(), LOGIN_SIGN_ACTIVE_ID, temp))
	{
		temp = "";
		if (!CDBCtrl::InsertUserOneAciveData(m_pCRole->GetdwAccountId(), LOGIN_SIGN_ACTIVE_ID, temp))
		{
			IME_ERROR("insert login check active fail");
			return -1;
		}
	}
	by.clear();
	by.append(temp.data(), temp.size());
	DeSeriliazeDBSign(by);
	
	if (!CDBCtrl::GetUserOneActiveData(m_pCRole->GetdwAccountId(), RECOVER_ACTIVE_ID, temp))
	{
		temp = "";
		if (!CDBCtrl::InsertUserOneAciveData(m_pCRole->GetdwAccountId(), RECOVER_ACTIVE_ID, temp))
		{
			IME_ERROR("insert login check active fail");
			return -1;
		}
	}
	by.clear();
	by.append(temp.data(), temp.size());
	DeSeriliazeDBRecover(by);

	if (!CDBCtrl::GetUserOneActiveData(m_pCRole->GetdwAccountId(), FRIEND_BOSS_RANK_ACTIVE_ID, temp))
	{
		temp = "";
		if (!CDBCtrl::InsertUserOneAciveData(m_pCRole->GetdwAccountId(), FRIEND_BOSS_RANK_ACTIVE_ID, temp))
		{
			IME_ERROR("insert boss rank active fail");
			return -1;
		}
	}
	by.clear();
	by.append(temp.data(), temp.size());
	DeSeriliazeDBFBRank(by);

	uint32_t number;
	uint32_t key,val1,val2;
	temp = "";
	if (!CDBCtrl::GetUserOneActiveData(m_pCRole->GetdwAccountId(), CHARGE_SINGLE_ACTIVE_ID, temp))
	{
		if (!CDBCtrl::InsertUserOneAciveData(m_pCRole->GetdwAccountId(), CHARGE_SINGLE_ACTIVE_ID, temp))
		{
			IME_ERROR("insert boss rank active fail");
			return -1;
		}
	}
	by.clear();
	by.append(temp.data(), temp.size());
	by >> m_dwSingleChargeTime;
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> key;
		by >> val1;
		by >> val2;
		m_mapSingleChargeVal[key] = val1;
		m_mapSingleChargeAgree[key] = val2;
	}

	temp = "";
	if (!CDBCtrl::GetUserOneActiveData(m_pCRole->GetdwAccountId(), CHARGE_ACCUMULATE_ACTIVE_ID, temp))
	{
		if (!CDBCtrl::InsertUserOneAciveData(m_pCRole->GetdwAccountId(), CHARGE_ACCUMULATE_ACTIVE_ID, temp))
		{
			IME_ERROR("insert boss rank active fail");
			return -1;
		}
	}
	by.clear();
	by.append(temp.data(), temp.size());
	by >> m_dwAccumulateChargeTime;
	by >> m_dwAccumulateVal;
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> key;
		m_setAccumulateCharge.insert(key);
	}

	temp = "";
	if (!CDBCtrl::GetUserOneActiveData(m_pCRole->GetdwAccountId(), CHARGE_CYCLE_ACTIVE_ID, temp))
	{
		if (!CDBCtrl::InsertUserOneAciveData(m_pCRole->GetdwAccountId(), CHARGE_CYCLE_ACTIVE_ID, temp))
		{
			IME_ERROR("insert boss rank active fail");
			return -1;
		}
	}
	by.clear();
	by.append(temp.data(), temp.size());
	by >> m_dwCycleChargeTime;
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> key;
		by >> val1;
		m_mapCycleChargeVal[key] = val1;
	}
	by >> m_dwCyclePoint;
//	IME_ERROR("get data cycle point %u", m_dwCyclePoint);

	temp = "";
	if (!CDBCtrl::GetUserOneActiveData(m_pCRole->GetdwAccountId(), LOGIN_CYCLE_ACTIVE_ID, temp))
	{
		if (!CDBCtrl::InsertUserOneAciveData(m_pCRole->GetdwAccountId(), LOGIN_CYCLE_ACTIVE_ID, temp))
		{
			IME_ERROR("insert boss rank active fail");
			return -1;
		}
	}
	by.clear();
	by.append(temp.data(), temp.size());
	by >> m_dwCycleLoginTime;
	by >> m_dwLoginCount;
	by >> m_byOk;
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> val1;
		m_vecCycleLogin.push_back(val1);
	}
	by >> m_dwCycleLoginTime2;

	temp = "";
	if (!CDBCtrl::GetUserOneActiveData(m_pCRole->GetdwAccountId(), LOGIN_EXTRA_ACTIVE_ID, temp))
	{
		if (!CDBCtrl::InsertUserOneAciveData(m_pCRole->GetdwAccountId(), LOGIN_EXTRA_ACTIVE_ID, temp))
		{
			IME_ERROR("insert boss rank active fail");
			return -1;
		}
	}
	by.clear();
	by.append(temp.data(), temp.size());
	by >> m_dwAccumulateFlushTime;
	by >> m_dwAccumulateLoginTime;
	by >> m_dwAccumulateLoginCount;
	by >> m_byAccumulateOk;
//	IME_ERROR("first accumulate login count %u, ok %u", m_dwAccumulateLoginCount, m_byAccumulateOk);

	temp = "";
	if (!CDBCtrl::GetUserOneActiveData(m_pCRole->GetdwAccountId(), FUND_ACTIVE_ID, temp))
	{
		if (!CDBCtrl::InsertUserOneAciveData(m_pCRole->GetdwAccountId(), FUND_ACTIVE_ID, temp))
		{
			IME_ERROR("insert boss rank active fail");
			return -1;
		}
	}
	by.clear();
	by.append(temp.data(), temp.size());
	by >> number;
	for (int i = 0; i < number; i++)
	{
		SFundArr tmp;
		by >> key;
		by >> tmp.GetTime;
		by >> tmp.EndTime;
		by >> tmp.EveryPrize.obj1.type;
		by >> tmp.EveryPrize.obj1.para1;
		by >> tmp.EveryPrize.obj1.para2;
		by >> tmp.EveryPrize.obj2.type;
		by >> tmp.EveryPrize.obj2.para1;
		by >> tmp.EveryPrize.obj2.para2;
		by >> tmp.EveryPrize.obj3.type;
		by >> tmp.EveryPrize.obj3.para1;
		by >> tmp.EveryPrize.obj3.para2;
		by >> tmp.CostCoin;
		by >> tmp.CostDollar;
		by >> tmp.mailtitle;
		by >> tmp.mailcontent;
		by >> tmp.ReturnCoin;
		by >> tmp.ReturnDollar;
		by >> tmp.Desc;
		m_mapFund[key] = tmp;
	}

//	if (m_dwLoginCount == 0)
//		m_dwLoginCount = CConfCycleLogin::GetFirstRecord();
//	STC_CONF_CYCLE_LOGIN *pConf = CConfCycleLogin::Find(m_dwLoginCount);
//	if (pConf == NULL)
//	{
//		IME_ERROR("can not find index %u in csv", m_dwLoginCount);
//		return -1;
//	}

//	IME_DEBUG("sign %u, signtime %u, signUsedExtra %u", m_dwSign, m_dwSignTime, m_dwUsedExtra);
	if (LoadAllConf())
	{
		return -1;
	}
	
	OnTimer5s();

	m_IsInit = true;
	return 0;
}		/* -----  end of method CActive::Init  ----- */

int CActive::LoadAllConf()
{
//	if (LoadConfGift())
//	{
//		IME_ERROR("load conf gift fail");
//		return -1;
//	}
	return 0;
}

void CActive::SaveAllData()
{
//	SaveDataGift();	
	SaveDataSign();
	SaveDataRecover();
	SaveDataFriendbossRank();
	SaveCommonActiveData(0);
}

void CActive::OnTimer5s()
{
	if (!CUtil::IsToday(m_dwGiftTime))	
	{
		m_dwGiftTime = time(NULL); 
		m_dwGetNumber = 0;
		m_dwAcceptTime = 0;
	}

	uint32_t now = time(NULL);
	uint32_t datestr = CUtil::GetDateTime(now);
	uint32_t flushPoint = datestr / 100 * 100 + 1;
	uint32_t updatedatestr = CUtil::GetDateTime(m_dwSignFlushTime);
//	IME_DEBUG("flush %u, updatedatestr %u", flushPoint, updatedatestr);
	if (flushPoint > updatedatestr)
	{
		m_dwSign = 0;
		m_dwUsedExtra = 0;
		m_dwSignFlushTime = now;
	}

	RecoverOntimer5s();
	FriendBossRankOntimer5s();
	CommonActiveOntimer5s();
}

void
CActive::HandlerInfo ()
{
//	HandlerInfoGift();
	HandlerInfoSign();
	HandlerInfoRecover();
	HandlerFriendBossRank();
	HandlerCommonActiveInfo(0);
}		/* -----  end of method CActive::HandlerInfo  ----- */

//int
//CActive::LoadConfGift ()
//{
//	STC_CONF_GIFT_PACKET *pConf = CConfGiftPacket::Find(1);
//	if(pConf == NULL)
//	{
//		IME_ERROR("can not gift record in gift packet csv");
//		return -1;
//	}
//	m_dwMaxNumber = pConf->dwNumber;
//	m_dwCD = pConf->dwCD;
//	m_dwRandomIndex = pConf->dwRandomEventIndex;
//	return 0;
//}		/* -----  end of method CActive::LoadConf  ----- */
//
//void
//CActive::SaveDataGift ()
//{
//	if (!m_IsInit)
//		return;
//	ByteBuffer by;
//	SeriliazeDBGift(by);
//	std::string temp;
//	temp.assign((const char *)by.contents(), by.size());
//	CDBCtrl::SaveUserDataActive(temp, m_pCRole->GetdwAccountId());
//	return ;
//}		/* -----  end of method CActive::SaveData  ----- */


void
CActive::SeriliazeDBGift(ByteBuffer &by)
{
	by << m_dwGetNumber;
	by << m_dwGiftTime;
	by << m_dwAcceptTime;
	return ;
}		/* -----  end of method CActive::SerializeDB  ----- */


void
CActive::DeSeriliazeDBGift(ByteBuffer &by)
{
//	if (by.size() == 0)
//		return;
	by >> m_dwGetNumber;
	by >> m_dwGiftTime;
	by >> m_dwAcceptTime;
	return ;
}		/* -----  end of method CActive::DeserializeDB  ----- */

//void
//CActive::HandlerInfoGift ()
//{
//	WorldPacket info(CMD_SC_GIFT_PACKET_INFO);
//	info << m_dwGetNumber;
//	int32_t tmp = m_dwAcceptTime + m_dwCD - time(NULL);
//	if (tmp < 0 ) tmp = 0;
//	info << (uint32_t) tmp;
//	return ;
//}		/* -----  end of method CActive::HandlerInfo  ----- */

//void
//CActive::HandlerRequestGiftPacket (WorldPacket &packet)
//{
//	if (m_dwAcceptTime + m_dwCD > time(NULL))
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_GIFT_PACKET_IN_CD, CMD_SC_REQUEST_GIFT_PACKET_RESULT);
//		return;
//	}
//
//	if (m_dwGetNumber >= m_dwMaxNumber)
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_GIFT_PACKET_MAX_NUMBER, CMD_SC_REQUEST_GIFT_PACKET_RESULT);
//		return;
//	}
//	
//	STC_CONF_RANDOM_EVENT tmp = {0};	
//	STC_CONF_RANDOM_EVENT *pConf = CConfRandomEvent::Find(m_dwRandomIndex);
//	if (pConf == NULL)
//	{
//		IME_ERROR("can not find RandomEvent in RandomEvent csv index %u", m_dwRandomIndex);
//		return;
//	}
//	memcpy(&tmp, pConf, sizeof(STC_CONF_RANDOM_EVENT));
//	CRandomEvent::ExecRandomEvent(m_pCRole, 100, tmp);
//	m_dwGetNumber++;
//	m_dwAcceptTime = time(NULL);
//	HandlerInfo();
//	WorldPacket info(CMD_SC_REQUEST_GIFT_PACKET_RESULT);
//	info << (uint16_t) 0;
//	m_pCRole->SendPacket(&info);
//	return ;
//}		/* -----  end of method CActive::Handler  ----- */

void CActive::HandlerInfoSign()
{
	WorldPacket info(CMD_SC_SIGN_ACTIVE_INFO);
	info << m_dwSign;

	int vipextra = 0;
	STC_CONF_VIP *pVip = CConfVip::Find(m_pCRole->GetwVipLv());
	if (pVip != NULL)
		vipextra = pVip->dwExtraSign;

	int remain = (int) LOGIN_SIGN_ACTIVE_EXTRA_COUNT + vipextra - m_dwUsedExtra;
	remain = remain < 0 ? 0 : remain;
	info << (uint32_t) remain;
	m_pCRole->SendPacket(&info);
}

void CActive::SaveDataSign()
{
	if (!m_IsInit)
		return;
	ByteBuffer by;
	SeriliazeDBSign(by);
	std::string temp;
	temp.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveUserOneActiveData(temp, m_pCRole->GetdwAccountId(), LOGIN_SIGN_ACTIVE_ID);
	return;
}

void
CActive::SeriliazeDBSign(ByteBuffer &by)
{
	by << m_dwSign;
	by << m_dwSignTime;
	by << m_dwSignFlushTime;
	by << m_dwUsedExtra;
	return ;
}		/* -----  end of method CActive::SerializeDB  ----- */


void
CActive::DeSeriliazeDBSign(ByteBuffer &by)
{
//	if (by.size() == 0)
//		return;
	by >> m_dwSign;
	by >> m_dwSignTime;
	by >> m_dwSignFlushTime;
	by >> m_dwUsedExtra;
	return ;
}		/* -----  end of method CActive::DeserializeDB  ----- */

uint8_t CActive::IsSigned()
{
//	int t = CUtil::GetDateTime(time(NULL));
//	IME_ERROR("sign time %u, t %u", m_dwSignTime, t);
	if (m_dwSign >= EVERY_MONTH_SIGN_DAY)
	{
//		IME_DEBUG("bigger every month");
		return 1;
	}
	else
	{
		if (CUtil::IsToday(m_dwSignTime))
		{
//			IME_DEBUG("same day %u", m_dwSignTime);
			return 1;
		}
		else
		{
//			IME_DEBUG("have not signed %u", m_dwSignTime);
			return 0;
		}
	}
}

void CActive::HandlerSignClear()
{
	m_dwSign = 0;
	HandlerInfoSign();
	m_dwSignTime = 0;
}

void CActive::HandlerLoginSignActive(WorldPacket &packet)
{
	uint8_t choose;
	packet >> choose;
	IME_CHAR_LOG("action [userid %u sign every day choose %u]", m_pCRole->GetdwAccountId(), choose);
	if (m_pCRole->GetclsCardPacket()->IsFull(1, 0))
	{
		IME_ERROR("userid %u card packet is not enough", m_pCRole->GetdwAccountId());
		m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_LOGIN_SIGN_ACTIVE_RESULT);
		return;
	}
	CShopEffect::SGiveProduct givetmp = {0};	
	if (choose == 0) // normal sign
	{
		if (CUtil::IsToday(m_dwSignTime))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_LOGIN_ALREADY_SIGN, CMD_SC_LOGIN_SIGN_ACTIVE_RESULT);
			return;
		}

		if (m_dwSign >= EVERY_MONTH_SIGN_DAY)
		{
			m_dwSignTime = time(NULL);
			WorldPacket info(CMD_SC_LOGIN_SIGN_ACTIVE_RESULT);
			info << (uint16_t) 0;
			info << choose;
			info << (uint32_t) 0;
			info << (uint32_t) 0;
			info << (uint32_t) 0;
			m_pCRole->SendPacket(&info);
			return;
		}

		m_dwSign++;
		uint32_t t = CUtil::GetDateTime(time(NULL));
		uint32_t month = t % 10000 / 100;
		uint32_t key = month * 100 + m_dwSign;
		STC_CONF_LOGIN_SIGN *pConf = CConfLoginSign::Find(key);
		if (pConf == NULL)
		{
			IME_ERROR("can not find data index %u in login sign csv", key);
			m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_LOGIN_SIGN_ACTIVE_RESULT);
			return;
		}
		if (pConf->byType != 0)
		{
			CShopEffect::GiveGameProduct(m_pCRole, pConf->byType, pConf->dwPara1, pConf->dwPara2, givetmp, SOURCE_ACTIVE); 
		}
		m_dwSignTime = time(NULL);
		m_pCRole->HandlerInfoOpt();

	}
	else // extra sign 
	{
		uint32_t vipextra = 0;
		STC_CONF_VIP *pVip = CConfVip::Find(m_pCRole->GetwVipLv());
		if (pVip != NULL)
			vipextra = pVip->dwExtraSign;

		if (m_dwUsedExtra >= LOGIN_SIGN_ACTIVE_EXTRA_COUNT + vipextra)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_LOGIN_SIGN_ACTIVE_MAX, CMD_SC_LOGIN_SIGN_ACTIVE_RESULT);
			return;
		}

		if (m_dwSign >= EVERY_MONTH_SIGN_DAY)
		{
			WorldPacket info(CMD_SC_LOGIN_SIGN_ACTIVE_RESULT);
			info << (uint16_t) 0;
			info << choose;
			info << (uint32_t) 0;
			info << (uint32_t) 0;
			info << (uint32_t) 0;
			m_pCRole->SendPacket(&info);
			return;
		}

		m_dwUsedExtra++;
		m_dwSign++;
		uint32_t t = CUtil::GetDateTime(time(NULL));
		uint32_t month = t % 10000 / 100;
		uint32_t key = month * 100 + m_dwSign;
		STC_CONF_LOGIN_SIGN *pConf = CConfLoginSign::Find(key);
		if (pConf == NULL)
		{
			IME_ERROR("can not find data index %u in login sign csv", key);
			m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_LOGIN_SIGN_ACTIVE_RESULT);
			return;
		}
		if (pConf->byType != 0)
		{
			CShopEffect::GiveGameProduct(m_pCRole, pConf->byType, pConf->dwPara1, pConf->dwPara2, givetmp, SOURCE_ACTIVE); 
		}
	}
	
	m_pCRole->GetCMission()->Sign(0, 1);

	HandlerInfoSign();
	WorldPacket info(CMD_SC_LOGIN_SIGN_ACTIVE_RESULT);
	info << (uint16_t) 0;
	info << choose;
	info << givetmp.effect;
	info << givetmp.para1;
	info << givetmp.para2;
	m_pCRole->SendPacket(&info);
	return;
}

void CActive::HandlerInfoRecover()
{
	WorldPacket info(CMD_SC_RECOVER_INFO);
	uint32_t s,e;
	uint32_t cd = GetRecoverCD(s,e);
	info << cd;
	if (m_byRecover)
	{
		if (cd)
		{
			info << (uint8_t) 0;
			info << (uint8_t) 0;
		}
		else
		{
			if (m_wEnergyUsed >= EVERY_USE_ENERGY)
			{
				info << (uint8_t) 0;
			}
			else
			{
				info << (uint8_t) 1;
			}
			if (m_wStrengthUsed >= EVERY_USE_STRENGTH)
			{
				info << (uint8_t) 0;
			}
			else
			{
				info << (uint8_t) 1;
			}
		}
	}
	else
	{
		info << (uint8_t) 0;
		info << (uint8_t) 0;
	}
	m_pCRole->SendPacket(&info);
}

uint32 CActive::GetRecoverCD(uint32_t &starttime, uint32_t &endtime)
{
	uint32_t now = time(NULL);
	uint32_t sec = CUtil::GetDayTime(now);
	uint32_t time1 = GlobalConfig::RecoverEnergyTime1 / 100 * 3600 + GlobalConfig::RecoverEnergyTime1 % 100;
	uint32_t time2 = GlobalConfig::RecoverEnergyTime2 / 100 * 3600 + GlobalConfig::RecoverEnergyTime2 % 100;
	uint32_t time3 = GlobalConfig::RecoverEnergyTime3 / 100 * 3600 + GlobalConfig::RecoverEnergyTime3 % 100;
	uint32_t time4 = GlobalConfig::RecoverEnergyTime4 / 100 * 3600 + GlobalConfig::RecoverEnergyTime4 % 100;
	uint32_t base = now - sec;

	if (sec < time1)
	{
//		m_dwRecoverStartTime = base + time1; 
		endtime = base + time2;
		starttime = base + time1;
		return time1 - sec;
	}
	else if (sec >= time1 && sec <= time2)
	{
		endtime = base + time2;
		starttime = base + time1;
		return 0;
	}
	else if (sec > time2 && sec < time3)
	{
		endtime = base + time4;
		starttime = base + time3;
		return time3 - sec;
	}
	else if (sec >= time3 && sec <= time4)
	{
		endtime = base + time4;
		starttime = base + time3;
		return 0;
	}
	else
	{
		endtime = 86400 + base + time2;
		starttime = 86400 + base + time1;
		return 86400 + time1 - sec;
	}
}

void CActive::SaveDataRecover()
{
	if (!m_IsInit)
		return;
	ByteBuffer by;
	SeriliazeDBRecover(by);
	std::string temp;
	temp.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveUserOneActiveData(temp, m_pCRole->GetdwAccountId(), RECOVER_ACTIVE_ID);
	return;
}

void
CActive::SeriliazeDBRecover(ByteBuffer &by)
{
	by << m_wEnergyUsed;
	by << m_wStrengthUsed;
	by << m_byRecover;
	by << m_dwRecoverEndTime;
	return ;
}

void
CActive::DeSeriliazeDBRecover(ByteBuffer &by)
{
//	if (by.size() == 0)
//		return;
	by >> m_wEnergyUsed;
	by >> m_wStrengthUsed;
	by >> m_byRecover;
	by >> m_dwRecoverEndTime;
	return ;
}

void
CActive::RecoverOntimer5s()
{
	uint32_t now = time(NULL);
//	IME_DEBUG("recover %u, energyuse %u, strengthuse %u, endtime %u", m_byRecover, m_wEnergyUsed, m_wStrengthUsed, m_dwRecoverEndTime);
	if (m_byRecover)
	{
		if (now > m_dwRecoverEndTime)
		{
			m_wEnergyUsed = 0;
			m_wStrengthUsed = 0;
			m_byRecover = 0;
		}
	}
	else
	{
		uint32_t startime, endtime;
		if (GetRecoverCD(startime, endtime) == 0)
		{
			m_byRecover = 1;
			m_dwRecoverEndTime = endtime;	
			HandlerInfoRecover();
		}	
	}
}

void CActive::HandlerRecover(WorldPacket &packet)
{
	uint8_t choose;
	packet >> choose;
	IME_CHAR_LOG("action [userid %u recover choose %u]", m_pCRole->GetdwAccountId(), choose);
	if (!m_byRecover)
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_ACTIVE_NOT_OPEN, CMD_SC_RECOVER_RESULT);
		return;
	}
	if (choose == 1)
	{
		if (m_wEnergyUsed >= EVERY_USE_ENERGY)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_USE_ENERY, CMD_SC_RECOVER_RESULT);
			return;
		}
		uint32_t RecoverEnergy = GlobalConfig::RecoverEnergy;
		STC_CONF_VIP *pVipConf = CConfVip::Find(m_pCRole->GetwVipLv());
		if (pVipConf == NULL)
		{
			IME_ERROR("vip lv %u can not find in vip csv", m_pCRole->GetwVipLv());
		}
		else
		{
			RecoverEnergy = RecoverEnergy * (pVipConf->dwVipRecover + 100) / 100.0;
		}

		m_pCRole->ChangeEnergyNoLimit(RecoverEnergy);
		m_wEnergyUsed++;
		m_pCRole->SendProUpdate();

		HandlerInfoRecover();
	}
	else
	{
		if (m_wStrengthUsed >= EVERY_USE_STRENGTH)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_USE_STRENGTH, CMD_SC_RECOVER_RESULT);
			return;
		}
		uint32_t RecoverStrength = GlobalConfig::RecoverStrength;
		STC_CONF_VIP *pVipConf = CConfVip::Find(m_pCRole->GetwVipLv());
		if (pVipConf == NULL)
		{
			IME_ERROR("vip lv %u can not find in vip csv", m_pCRole->GetwVipLv());
		}
		else
		{
			RecoverStrength = RecoverStrength * (pVipConf->dwVipRecover + 100) / 100.0;
		}

		m_pCRole->ChangeStrengthNoLimit(RecoverStrength);
		m_wStrengthUsed++;
		m_pCRole->SendProUpdate();
		HandlerInfoRecover();
	}
	WorldPacket info(CMD_SC_RECOVER_RESULT);
	info << (uint16_t) 0;
	info << choose;
	m_pCRole->SendPacket(&info);
}

void
CActive::SeriliazeDBFBRank(ByteBuffer &by)
{
	by << m_dwFBRankFlushTime;
	by << m_dwDevote;
	return ;
}

void
CActive::DeSeriliazeDBFBRank(ByteBuffer &by)
{
//	if (by.size() == 0)
//		return;
	by >> m_dwFBRankFlushTime;
	by >> m_dwDevote;
	return ;
}

void
CActive::ChangeDevote(int32_t devote)
{
	if (CActiveCtrl::FriendBossRank)
	{
		int a = m_dwDevote;
		a += devote;
		if (m_dwDevote != 0)
		{
			// sure devote != zero must always != 0
			a = a <= 0 ? 1 : a;
		}
		else
		{
			a = a < 0 ? 0 : a;
		}
		m_dwDevote = a;

		SFBRankRole tmp;
		m_pCRole->GetclsCardPacket()->PreDataForRank(tmp, m_dwDevote);
		int rank = -1;	
		CActiveCtrl::UpdateRank(tmp, rank);
		if (rank != m_dwRank || devote != 0)
			CActiveCtrl::RankData(rank, m_pCRole);
		HandlerDevoteInfo();
		return;
	}
}

void
CActive::HandlerDevoteInfo()
{
	WorldPacket info(CMD_SC_FRIEND_BOSS_RANK_DEVOTE_INFO);
	info << m_dwDevote;
	m_pCRole->SendPacket(&info);
}

void
CActive::HandlerFriendBossRank()
{
//	if (CActiveCtrl::FriendBossRank)
//	{
		if (m_pCRole->IsInitRoleInfo())
		{
//			if (m_dwFBRankFlushTime < CActiveCtrl::FBRankFlushTime)
//			{
//				m_dwDevote = 0;
//				m_dwFBRankFlushTime = CActiveCtrl::FBRankFlushTime;
//			}
			SFBRankRole tmp;
			m_pCRole->GetclsCardPacket()->PreDataForRank(tmp, m_dwDevote);
			int rank = -1;	
			if (m_dwDevote != 0)
			{
				CActiveCtrl::UpdateRank(tmp, rank);
			}
//			IME_ERROR("rank %d", rank);
			CActiveCtrl::RankData(rank, m_pCRole);
			m_dwRank = rank;

			HandlerDevoteInfo();
		}
//	}
}

void
CActive::SaveDataFriendbossRank()
{
	if (!m_IsInit)
		return;
	ByteBuffer by;
	SeriliazeDBFBRank(by);
	std::string temp;
	temp.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveUserOneActiveData(temp, m_pCRole->GetdwAccountId(), FRIEND_BOSS_RANK_ACTIVE_ID);
	return;
}

void
CActive::FriendBossRankOntimer5s()
{
	if (m_dwFBRankFlushTime < CActiveCtrl::FBRankFlushTime)
	{
		if (m_dwDevote != 0)
		{
			CMail::SBaseAttr tmpAttr = {0};
			tmpAttr.coin = GlobalConfig::BossRankCoin;
			tmpAttr.worldhistory = GlobalConfig::BossRankWorldExp;

			uint32_t mail_id = 0;
			CDBCtrl::stuMailDetail tmp = {0};
			CMail::AddEmailSysDiy(m_pCRole->GetdwAccountId(), mail_id, tmp, 0, 0, 0, GlobalConfig::BossRankMailContent, tmpAttr, GlobalConfig::BossRankMailTitle);   

			CRole *pPeerCRole = sWorld->FindUserRole(m_pCRole->GetdwAccountId());
			if (pPeerCRole != NULL)
			{
				CMail::EmailUpdate(pPeerCRole, CMail::ADD, &tmp, 0);
			}
		}
		m_dwDevote = 0;
		m_dwFBRankFlushTime = CActiveCtrl::FBRankFlushTime;
		HandlerFriendBossRank();
	}
}

void CActive::HandlerCommonActiveOpt(WorldPacket &packet)
{
	uint32_t activeindex;
	uint32_t targetid;
	packet >> activeindex;
	packet >> targetid;
	IME_CHAR_LOG("action [userid %u active prize get activeindex %u targetid %u]", m_pCRole->GetdwAccountId(), activeindex, targetid);

	if (activeindex == CHARGE_SINGLE_ACTIVE_ID)
	{
		if (!CActiveCtrl::SingleCharge)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ACTIVE_NOT_OPEN, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}
		
		if (CActiveCtrl::mapSingleChargePrize.find(targetid) == CActiveCtrl::mapSingleChargePrize.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_FIND_TARGET, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		if (m_mapSingleChargeAgree.find(targetid) == m_mapSingleChargeAgree.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_ARRIVE_CONDITION, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}
		if (m_mapSingleChargeVal[targetid] >= m_mapSingleChargeAgree[targetid])
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_ARRIVE_CONDITION, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		//todo get prize
		std::vector<SProduct> vectmp;
		vectmp.push_back(CActiveCtrl::mapSingleChargePrize[targetid].obj1);
		vectmp.push_back(CActiveCtrl::mapSingleChargePrize[targetid].obj2);
		vectmp.push_back(CActiveCtrl::mapSingleChargePrize[targetid].obj3);
		vectmp.push_back(CActiveCtrl::mapSingleChargePrize[targetid].obj4);

		if (m_pCRole->GetclsCardPacket()->IsFullSpecial(vectmp))
		{
			IME_ERROR("card packet is full userid %u", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		std::vector<CShopEffect::SGiveProduct> vecObjtmp;
		for(int i = 0; i < vectmp.size(); i++)
		{
			CShopEffect::SGiveProduct tmp = {0};
			CShopEffect::GiveGameProductSpecial(m_pCRole, vectmp[i].type, vectmp[i].para1, vectmp[i].para2, tmp, SOURCE_ACTIVE);
			vecObjtmp.push_back(tmp);
		}

		m_mapSingleChargeVal[targetid]++;
		HandlerCommonActiveInfo(activeindex);
		m_pCRole->SendObjEffect(vecObjtmp);
	}
	else if (activeindex == CHARGE_ACCUMULATE_ACTIVE_ID)
	{
		if (!CActiveCtrl::AccumulateCharge)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ACTIVE_NOT_OPEN, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;	
		}

		if (CActiveCtrl::mapAccumulateChargePrize.find(targetid) == CActiveCtrl::mapAccumulateChargePrize.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_FIND_TARGET, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		if (m_dwAccumulateVal < CActiveCtrl::mapAccumulateChargePrize[targetid].prize || m_setAccumulateCharge.find(targetid) != m_setAccumulateCharge.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_ARRIVE_CONDITION, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		//todo get prize
		std::vector<SProduct> vectmp;
		vectmp.push_back(CActiveCtrl::mapAccumulateChargePrize[targetid].obj1);
		vectmp.push_back(CActiveCtrl::mapAccumulateChargePrize[targetid].obj2);
		vectmp.push_back(CActiveCtrl::mapAccumulateChargePrize[targetid].obj3);
		vectmp.push_back(CActiveCtrl::mapAccumulateChargePrize[targetid].obj4);

		if (m_pCRole->GetclsCardPacket()->IsFullSpecial(vectmp))
		{
			IME_ERROR("card packet is full userid %u", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		std::vector<CShopEffect::SGiveProduct> vecObjtmp;
		for(int i = 0; i < vectmp.size(); i++)
		{
			CShopEffect::SGiveProduct tmp = {0};
			CShopEffect::GiveGameProductSpecial(m_pCRole, vectmp[i].type, vectmp[i].para1, vectmp[i].para2, tmp, SOURCE_ACTIVE);
			vecObjtmp.push_back(tmp);
		}

		m_setAccumulateCharge.insert(targetid);
		HandlerCommonActiveInfo(activeindex);
		m_pCRole->SendObjEffect(vecObjtmp);
	}
	else if (activeindex == CHARGE_CYCLE_ACTIVE_ID)
	{
		if (!CActiveCtrl::CycleCharge)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ACTIVE_NOT_OPEN, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		if (CActiveCtrl::mapCycleChargePrize.find(targetid) == CActiveCtrl::mapCycleChargePrize.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_FIND_TARGET, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		uint32_t getted = 0;
		if (m_mapCycleChargeVal.find(targetid) != m_mapCycleChargeVal.end())
		{
			getted = m_mapCycleChargeVal[targetid];
		}

		if (getted >= CActiveCtrl::mapCycleChargePrize[targetid].number && CActiveCtrl::mapCycleChargePrize[targetid].number != 0)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_ARRIVE_CONDITION, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		if (m_dwCyclePoint < CActiveCtrl::mapCycleChargePrize[targetid].prize)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_ARRIVE_CONDITION, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		//todo get prize
		std::vector<SProduct> vectmp;
		vectmp.push_back(CActiveCtrl::mapCycleChargePrize[targetid].obj1);
		vectmp.push_back(CActiveCtrl::mapCycleChargePrize[targetid].obj2);
		vectmp.push_back(CActiveCtrl::mapCycleChargePrize[targetid].obj3);
		vectmp.push_back(CActiveCtrl::mapCycleChargePrize[targetid].obj4);

		if (m_pCRole->GetclsCardPacket()->IsFullSpecial(vectmp))
		{
			IME_ERROR("card packet is full userid %u", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		std::vector<CShopEffect::SGiveProduct> vecObjtmp;
		for(int i = 0; i < vectmp.size(); i++)
		{
			CShopEffect::SGiveProduct tmp = {0};
			CShopEffect::GiveGameProductSpecial(m_pCRole, vectmp[i].type, vectmp[i].para1, vectmp[i].para2, tmp, SOURCE_ACTIVE);
			vecObjtmp.push_back(tmp);
		}
		
		m_dwCyclePoint -= CActiveCtrl::mapCycleChargePrize[targetid].prize;

		m_setAccumulateCharge.insert(targetid);
		HandlerCommonActiveInfo(activeindex);
		m_pCRole->SendObjEffect(vecObjtmp);
	}
	else if (activeindex == LOGIN_CYCLE_ACTIVE_ID)
	{
		CConfCycleLogin::MapData *p = CConfCycleLogin::GetMapData();
		CConfCycleLogin::MapData::iterator it;
		int i = 0;
		for (it = p->begin(); it != p->end(); it++)
		{
			i++;
			if (it->second.dwIndex == targetid)
			{
				break;
			}
		}
		if (it == p->end())
			i = 0;

		if (i == 0 || i > m_vecCycleLogin.size())
		{
//			IME_ERROR("request targetid %u now is %u or already get it", targetid, m_dwLoginCount);
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_ARRIVE_CONDITION, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}
		if (m_vecCycleLogin[i - 1] != 1)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_ARRIVE_CONDITION, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}	

		STC_CONF_CYCLE_LOGIN *pConf = CConfCycleLogin::Find(targetid);
		if (pConf == NULL)
		{
			IME_ERROR("can not find index %u in csv", targetid);
			m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		//todo get prize
		std::vector<SProduct> vectmp;
		SProduct d;
		d.type = pConf->wObj1Type;
		d.para1 = pConf->dwObj1Para1;
		d.para2 = pConf->dwObj1Para2;
		vectmp.push_back(d);
		d.type = pConf->wObj2Type;
		d.para1 = pConf->dwObj2Para1;
		d.para2 = pConf->dwObj2Para2;
		vectmp.push_back(d);
		d.type = pConf->wObj3Type;
		d.para1 = pConf->dwObj3Para1;
		d.para2 = pConf->dwObj3Para2;
		vectmp.push_back(d);

		if (m_pCRole->GetclsCardPacket()->IsFullSpecial(vectmp))
		{
			IME_ERROR("card packet is full userid %u", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		std::vector<CShopEffect::SGiveProduct> vecObjtmp;
		for(int i = 0; i < vectmp.size(); i++)
		{
			CShopEffect::SGiveProduct tmp = {0};
			CShopEffect::GiveGameProductSpecial(m_pCRole, vectmp[i].type, vectmp[i].para1, vectmp[i].para2, tmp, SOURCE_ACTIVE);
			vecObjtmp.push_back(tmp);
		}

//		m_byOk = 0;			
		m_vecCycleLogin[i - 1] = 2;
		HandlerCommonActiveInfo(activeindex);
		m_pCRole->SendObjEffect(vecObjtmp);
	}
	else if (activeindex == LOGIN_EXTRA_ACTIVE_ID)
	{
		if (!CActiveCtrl::ExtraLogin)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ACTIVE_NOT_OPEN, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		if (targetid != m_dwAccumulateLoginCount || !m_byAccumulateOk)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_ARRIVE_CONDITION, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		std::map<uint32_t, SExtraLogin>::iterator it;
		it = CActiveCtrl::mapExtraLogin.find(targetid);
		if (it == CActiveCtrl::mapExtraLogin.end())
		{
			IME_ERROR("can not find index %u in csv", targetid);
			m_pCRole->SendErrorMsg(ERRNO_MSG_ACTIVE_NOT_OPEN, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		//todo get prize
		std::vector<SProduct> vectmp;
		SProduct d;
		d.type = it->second.obj1.type;
		d.para1 = it->second.obj1.para1;
		d.para2 = it->second.obj1.para2;
		vectmp.push_back(d);
		d.type = it->second.obj2.type;
		d.para1 = it->second.obj2.para1;
		d.para2 = it->second.obj2.para2;
		vectmp.push_back(d);
		d.type = it->second.obj3.type;
		d.para1 = it->second.obj3.para1;
		d.para2 = it->second.obj3.para2;
		vectmp.push_back(d);

		if (m_pCRole->GetclsCardPacket()->IsFullSpecial(vectmp))
		{
			IME_ERROR("card packet is full userid %u", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		std::vector<CShopEffect::SGiveProduct> vecObjtmp;
		for(int i = 0; i < vectmp.size(); i++)
		{
			CShopEffect::SGiveProduct tmp = {0};
			CShopEffect::GiveGameProductSpecial(m_pCRole, vectmp[i].type, vectmp[i].para1, vectmp[i].para2, tmp, SOURCE_ACTIVE);
			vecObjtmp.push_back(tmp);
		}

		m_byAccumulateOk = 0;			
		HandlerCommonActiveInfo(activeindex);
		m_pCRole->SendObjEffect(vecObjtmp);
	}
	else if (activeindex == FUND_ACTIVE_ID)
	{
		if (!CActiveCtrl::Fund)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ACTIVE_NOT_OPEN, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}

		std::map<uint32_t, SFundActive>::iterator it;
		it = CActiveCtrl::mapFund.find(targetid);
		if (it == CActiveCtrl::mapFund.end())
		{
			IME_ERROR("can not find index %u in csv", targetid);
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_FIND_TARGET, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}
		
		if (m_mapFund.find(targetid) != m_mapFund.end())
		{
			IME_ERROR("you already buy this fund index %u", targetid);
			m_pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_GET_TARGET, CMD_SC_COMMON_ACTIVE_GET_RESULT);
			return;
		}	

		uint32_t order_id = 0;
		if (CActiveCtrl::mapFund[targetid].CostDollar > 0)
		{
			if (m_pCRole->GetdwDollar() < CActiveCtrl::mapFund[targetid].CostDollar)
			{
				m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_COMMON_ACTIVE_GET_RESULT);
				return;
			}

			if (!CDBCtrl::GetShopOrderId(m_pCRole->GetdwAccountId(), order_id))
			{
				IME_ERROR("get order id fail userid %u", m_pCRole->GetdwAccountId());
				m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_GET_ORDER_ID, CMD_SC_COMMON_ACTIVE_GET_RESULT);
				return;
			}
		}
		if (CActiveCtrl::mapFund[targetid].CostCoin > 0)
		{
			if (m_pCRole->GetdwCoin() < CActiveCtrl::mapFund[targetid].CostCoin)
			{
				m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_ENOUGH_COIN, CMD_SC_COMMON_ACTIVE_GET_RESULT);
				return;
			}
		}

		SFundArr tmp;
		tmp.GetTime = 0;
		tmp.EndTime = time(NULL) + (CActiveCtrl::mapFund[targetid].Day - 1) * 86400;
		tmp.EveryPrize.obj1 = CActiveCtrl::mapFund[targetid].obj1;
		tmp.EveryPrize.obj2 = CActiveCtrl::mapFund[targetid].obj2;
		tmp.EveryPrize.obj3 = CActiveCtrl::mapFund[targetid].obj3;
		tmp.CostCoin = CActiveCtrl::mapFund[targetid].CostCoin;
		tmp.CostDollar = CActiveCtrl::mapFund[targetid].CostDollar;
		tmp.mailtitle = CActiveCtrl::FundMailTitle;
		tmp.mailcontent = CActiveCtrl::FundMailContent;
		tmp.ReturnCoin = CActiveCtrl::mapFund[targetid].ReturnCoin;
		tmp.ReturnDollar = CActiveCtrl::mapFund[targetid].ReturnDollar;
		tmp.Desc = CActiveCtrl::mapFund[targetid].desc;
		m_mapFund[targetid] = tmp;

		m_pCRole->ChangeCoin(-CActiveCtrl::mapFund[targetid].CostCoin, SOURCE_ACTIVE);
		m_pCRole->ChangeDollar(-CActiveCtrl::mapFund[targetid].CostDollar, SOURCE_ACTIVE);
		m_pCRole->SendProUpdate();
		if (CActiveCtrl::mapFund[targetid].CostDollar > 0)
			CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, CActiveCtrl::mapFund[targetid].CostDollar, targetid, 1, E_DC_ACTIVE);

		//todo get prize
		HandlerCommonActiveInfo(activeindex);
	}
	else
	{
		IME_ERROR("request unvalid activeindex %u", activeindex);
		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_COMMON_ACTIVE_GET_RESULT);
	}

	WorldPacket res(CMD_SC_COMMON_ACTIVE_GET_RESULT);
	res << (uint16_t) 0;
	res << (uint32_t) activeindex;
	res << (uint32_t) targetid;
	m_pCRole->SendPacket(&res);
	return;
}

void CActive::SaveCommonActiveData(uint32_t activeindex)
{
	if (!m_IsInit)
		return;
	if (activeindex == 0 || activeindex == CHARGE_SINGLE_ACTIVE_ID)
	{
		ByteBuffer by;
		by << m_dwSingleChargeTime;
		by << (uint32_t)m_mapSingleChargeVal.size();
		std::map<uint32_t, uint32_t>::iterator it;
		for (it = m_mapSingleChargeVal.begin(); it != m_mapSingleChargeVal.end(); it++)
		{
			by << it->first;
			by << it->second;
			by << m_mapSingleChargeAgree[it->first];
		}
		std::string temp;
		temp.assign((const char *)by.contents(), by.size());
		CDBCtrl::SaveUserOneActiveData(temp, m_pCRole->GetdwAccountId(), CHARGE_SINGLE_ACTIVE_ID);
	}
	if (activeindex == 0 || activeindex == CHARGE_ACCUMULATE_ACTIVE_ID)
	{
		ByteBuffer by;
		by << m_dwAccumulateChargeTime;
		by << m_dwAccumulateVal;
		std::set<uint32_t>::iterator it;
		by << (uint32_t) m_setAccumulateCharge.size();
		for (it = m_setAccumulateCharge.begin(); it != m_setAccumulateCharge.end(); it++)
		{
			by << *it;
		}
		std::string temp;
		temp.assign((const char *)by.contents(), by.size());
		CDBCtrl::SaveUserOneActiveData(temp, m_pCRole->GetdwAccountId(), CHARGE_ACCUMULATE_ACTIVE_ID);
	}
	if (activeindex == 0 || activeindex == CHARGE_CYCLE_ACTIVE_ID)
	{
		ByteBuffer by;
		by << m_dwCycleChargeTime;
		std::map<uint32_t, uint32_t>::iterator it;
		by << (uint32_t) m_mapCycleChargeVal.size();
		for (it = m_mapCycleChargeVal.begin(); it != m_mapCycleChargeVal.end(); it++)
		{
			by << it->first;
			by << it->second;
		}
		by << m_dwCyclePoint;
//		IME_ERROR("save data cycle point %u", m_dwCyclePoint);
		std::string temp;
		temp.assign((const char *)by.contents(), by.size());
		CDBCtrl::SaveUserOneActiveData(temp, m_pCRole->GetdwAccountId(), CHARGE_CYCLE_ACTIVE_ID);
	}
	if (activeindex == 0 || activeindex == LOGIN_CYCLE_ACTIVE_ID)
	{
		ByteBuffer by;
		by << m_dwCycleLoginTime;
		by << m_dwLoginCount;
		by << m_byOk;
		by << (uint32_t)m_vecCycleLogin.size();
		for (int i = 0; i < m_vecCycleLogin.size(); i++)
		{
			by << m_vecCycleLogin[i];
		}
		by << m_dwCycleLoginTime2;
		std::string temp;
		temp.assign((const char *)by.contents(), by.size());
		CDBCtrl::SaveUserOneActiveData(temp, m_pCRole->GetdwAccountId(), LOGIN_CYCLE_ACTIVE_ID);
	}
	if (activeindex == 0 || activeindex == LOGIN_EXTRA_ACTIVE_ID)
	{
		ByteBuffer by;
		by << m_dwAccumulateFlushTime;
		by << m_dwAccumulateLoginTime;
		by << m_dwAccumulateLoginCount;
		by << m_byAccumulateOk;
		std::string temp;
		temp.assign((const char *)by.contents(), by.size());
		CDBCtrl::SaveUserOneActiveData(temp, m_pCRole->GetdwAccountId(), LOGIN_EXTRA_ACTIVE_ID);
	}
	if (activeindex == 0 || activeindex == FUND_ACTIVE_ID)
	{
		ByteBuffer by;
		by << (uint32_t) m_mapFund.size();
		std::map<uint32_t, SFundArr>::iterator it;
		for (it = m_mapFund.begin(); it != m_mapFund.end(); it++)
		{
			by << it->first;
			by << it->second.GetTime;
			by << it->second.EndTime;
			by << it->second.EveryPrize.obj1.type;
			by << it->second.EveryPrize.obj1.para1;
			by << it->second.EveryPrize.obj1.para2;
			by << it->second.EveryPrize.obj2.type;
			by << it->second.EveryPrize.obj2.para1;
			by << it->second.EveryPrize.obj2.para2;
			by << it->second.EveryPrize.obj3.type;
			by << it->second.EveryPrize.obj3.para1;
			by << it->second.EveryPrize.obj3.para2;
			by << it->second.CostCoin;
			by << it->second.CostDollar;
			by << it->second.mailtitle;
			by << it->second.mailcontent;
			by << it->second.ReturnCoin;
			by << it->second.ReturnDollar;
			by << it->second.Desc;
		}
		std::string temp;
		temp.assign((const char *)by.contents(), by.size());
		CDBCtrl::SaveUserOneActiveData(temp, m_pCRole->GetdwAccountId(), FUND_ACTIVE_ID);
	}
	return;
}

void CActive::HandlerCommonActiveInfo(uint32_t activeindex)
{
	if (activeindex == 0 || activeindex == CHARGE_SINGLE_ACTIVE_ID)
	{
		WorldPacket info(CMD_SC_SINGLE_CHARGE_INFO);
		info << (uint32_t)m_mapSingleChargeVal.size();
		std::map<uint32_t, uint32_t>::iterator it;
		for (it = m_mapSingleChargeVal.begin(); it != m_mapSingleChargeVal.end(); it++)
		{
			info << it->first;
			info << it->second;
			uint32_t index = it->first;
			if (CActiveCtrl::mapSingleChargePrize.find(index) != CActiveCtrl::mapSingleChargePrize.end())
			{
				if (m_mapSingleChargeVal[index] < m_mapSingleChargeAgree[index] && (m_mapSingleChargeVal[index] < CActiveCtrl::mapSingleChargePrize[index].number || CActiveCtrl::mapSingleChargePrize[index].number == 0))
				{
					info << (uint8_t) 1;
				}
				else
					info << (uint8_t) 0;
			}
			else
			{
				IME_ERROR("active single charge data index %u is not in operation active", index);
				info << (uint8_t) 0;
			}
		}
		m_pCRole->SendPacket(&info);
	}
	if (activeindex == 0 || activeindex == CHARGE_ACCUMULATE_ACTIVE_ID)
	{
		WorldPacket info(CMD_SC_ACCUMULATE_CHARGE_INFO);
		
		info << (uint32_t) m_dwAccumulateVal;
		info << (uint32_t) CActiveCtrl::mapAccumulateChargePrize.size();
		std::map<uint32_t, SChargePrize>::iterator it;
		for (it = CActiveCtrl::mapAccumulateChargePrize.begin(); it != CActiveCtrl::mapAccumulateChargePrize.end(); it++)
		{
			uint32_t index = it->first;
			info << index;
			uint32_t have = 0;
			if (m_setAccumulateCharge.find(index) != m_setAccumulateCharge.end())
			{
				have = 1;	
			}

			if (it->second.prize <= m_dwAccumulateVal)
			{
				if (have)
				{
					info << have;
					info << (uint8_t) 0;
				}
				else
				{
					info << have;
					info << (uint8_t) 1;
				}
			}
			else
			{
				info << have;
				info << (uint8_t) 0;
			}
		}
		
		m_pCRole->SendPacket(&info);
	}
	if (activeindex == 0 || activeindex == CHARGE_CYCLE_ACTIVE_ID)
	{
		WorldPacket info(CMD_SC_CYCLE_CHARGE_INFO);
		
		info << (uint32_t) m_dwCyclePoint;
		info << (uint32_t) CActiveCtrl::mapCycleChargePrize.size();
		std::map<uint32_t, SChargePrize>::iterator it;
		for (it = CActiveCtrl::mapCycleChargePrize.begin(); it != CActiveCtrl::mapCycleChargePrize.end(); it++)
		{
			uint32_t index = it->first;
			info << index;
			if (m_mapCycleChargeVal.find(index) == m_mapCycleChargeVal.end())
			{
				m_mapCycleChargeVal[index] = 0;
			}

			info << m_mapCycleChargeVal[index];
			if (it->second.prize <= m_dwCyclePoint)
			{
				if (m_mapCycleChargeVal[index] < CActiveCtrl::mapCycleChargePrize[index].number || CActiveCtrl::mapCycleChargePrize[index].number == 0)
					info << (uint8_t) 1;
				else
					info << (uint8_t) 0;
			}
			else
			{
				info << (uint8_t) 0;
			}
		}
		m_pCRole->SendPacket(&info);
	}
	if (activeindex == 0 || activeindex == LOGIN_CYCLE_ACTIVE_ID)
	{
		WorldPacket info(CMD_SC_CYCLE_LOGIN_ACTIVE_INFO);
		CConfCycleLogin::MapData *p = CConfCycleLogin::GetMapData();
		CConfCycleLogin::MapData::iterator it;
		info << (uint32_t)p->size();
		int i = 0;
		for (it = p->begin(); it != p->end(); it++)
		{
			info << it->second.dwIndex;
			info << it->second.wObj1Type;
			info << it->second.dwObj1Para1;
			info << it->second.dwObj1Para2;
			info << it->second.wObj2Type;
			info << it->second.dwObj2Para1;
			info << it->second.dwObj2Para2;
			info << it->second.wObj3Type;
			info << it->second.dwObj3Para1;
			info << it->second.dwObj3Para2;
			info << it->second.dwNextIndex;
			info << it->second.strDesc;
			if (i < m_vecCycleLogin.size())
			{
				info << (uint8_t) m_vecCycleLogin[i];
			}
			else
			{
				info << (uint8_t) 0;
			}
			i++;
		}
		m_pCRole->SendPacket(&info);
	}
	if (activeindex == 0 || activeindex == LOGIN_EXTRA_ACTIVE_ID)
	{
		WorldPacket info(CMD_SC_EXTRA_LOGIN_ACTIVE_INFO);
		info << m_dwAccumulateLoginCount;
		info << m_byAccumulateOk;
		m_pCRole->SendPacket(&info);
//	IME_ERROR("send data accumulate login count %u, ok %u", m_dwAccumulateLoginCount, m_byAccumulateOk);
	}
	if (activeindex == 0 || activeindex == FUND_ACTIVE_ID)
	{
		WorldPacket info(CMD_SC_FUND_ACTIVE_INFO);
		info << (uint32_t) m_mapFund.size();
		std::map<uint32_t, SFundArr>::iterator it;
		for (it = m_mapFund.begin(); it != m_mapFund.end(); it++)
		{
			info << it->first;
			info << it->second.EndTime;
			info << it->second.Desc;
		}
		m_pCRole->SendPacket(&info);
	}
	return;
}

void CActive::CommonActiveOntimer5s()
{
	if (m_dwSingleChargeTime < CActiveCtrl::SingleChargeFlushTime)
	{
		m_dwSingleChargeTime = CActiveCtrl::SingleChargeFlushTime;
		m_mapSingleChargeVal.clear();
		m_mapSingleChargeAgree.clear();
		HandlerCommonActiveInfo(CHARGE_SINGLE_ACTIVE_ID);
	}
	if (m_dwAccumulateChargeTime < CActiveCtrl::AccumulateChargeFlushTime)
	{
		m_dwAccumulateChargeTime = CActiveCtrl::AccumulateChargeFlushTime;
		m_dwAccumulateVal = 0;
		m_setAccumulateCharge.clear();
		HandlerCommonActiveInfo(CHARGE_ACCUMULATE_ACTIVE_ID);
	}
	if (m_dwCycleChargeTime < CActiveCtrl::CycleChargeFlushTime)
	{
		m_dwCycleChargeTime = CActiveCtrl::CycleChargeFlushTime;
		m_mapCycleChargeVal.clear();
		m_dwCyclePoint = 0;
		HandlerCommonActiveInfo(CHARGE_CYCLE_ACTIVE_ID);
	}

	if (!CUtil::IsToday(m_dwCycleLoginTime2))
	{
		m_dwCycleLoginTime2 = time(NULL);

		CConfCycleLogin::MapData *p = CConfCycleLogin::GetMapData();
		if (m_vecCycleLogin.size() < p->size())
			m_vecCycleLogin.push_back(1);

		HandlerCommonActiveInfo(LOGIN_CYCLE_ACTIVE_ID);

//		if (m_dwLoginCount == 0 || m_byOk == 0)
//		{
//			if (m_dwLoginCount == 0)
//			{
//				m_dwLoginCount = CConfCycleLogin::GetFirstRecord();
//				m_byOk = 1;
//				HandlerCommonActiveInfo(LOGIN_CYCLE_ACTIVE_ID);
//			}
//			else
//			{
//				STC_CONF_CYCLE_LOGIN *pConf = CConfCycleLogin::Find(m_dwLoginCount);
//				if (pConf == NULL)
//				{
//					IME_ERROR("can not find index %u in csv", m_dwLoginCount);
//					m_dwLoginCount = 0;
//				}
//				else
//				{
//					m_dwLoginCount = pConf->dwNextIndex;
//					m_byOk = 1;
//					HandlerCommonActiveInfo(LOGIN_CYCLE_ACTIVE_ID);
//				}
//			}
//		}
	}

//	IME_ERROR("2 accumulate login count %u, ok %u", m_dwAccumulateLoginCount, m_byAccumulateOk);
	if (m_dwAccumulateFlushTime < CActiveCtrl::ExtraLoginFlushTime)
	{
		m_dwAccumulateFlushTime = CActiveCtrl::ExtraLoginFlushTime;
		m_byAccumulateOk = 0;
		m_dwAccumulateLoginTime = 0;
		m_dwAccumulateLoginCount = 0;
	}
//	IME_ERROR("3 accumulate login count %u, ok %u", m_dwAccumulateLoginCount, m_byAccumulateOk);

	if (CActiveCtrl::mapExtraLogin.size() != 0)
	{
		if (!CUtil::IsToday(m_dwAccumulateLoginTime))
		{
//	IME_ERROR("4 accumulate login count %u, ok %u", m_dwAccumulateLoginCount, m_byAccumulateOk);
			m_dwAccumulateLoginTime = time(NULL);
			if (m_dwAccumulateLoginCount == 0 || m_byAccumulateOk == 0)
			{
				if (m_dwAccumulateLoginCount == 0)
				{
					m_dwAccumulateLoginCount = CActiveCtrl::GetFirstRecord();
					m_byAccumulateOk= 1;
					HandlerCommonActiveInfo(LOGIN_EXTRA_ACTIVE_ID);
				}
				else
				{
					std::map<uint32_t, SExtraLogin>::iterator it = CActiveCtrl::mapExtraLogin.find(m_dwAccumulateLoginCount);
					if (it == CActiveCtrl::mapExtraLogin.end())
					{
						if (m_dwAccumulateLoginCount != 65535)
						{
							IME_ERROR("can not find index %u in extra login", it->first);
						}
					}
					else
					{
						m_dwAccumulateLoginCount = it->second.nextindex;
						m_byAccumulateOk = 1;
						HandlerCommonActiveInfo(LOGIN_EXTRA_ACTIVE_ID);
					}
				}
			}
//	IME_ERROR("5 accumulate login count %u, ok %u", m_dwAccumulateLoginCount, m_byAccumulateOk);
		}
	}
	
	std::map<uint32_t, SFundArr>::iterator it;
	uint8_t update = 0;
	for (it = m_mapFund.begin(); it != m_mapFund.end();)
	{	
		uint32_t m_dwFundGetTime = it->second.GetTime;
		uint32_t m_dwFundEndTime = it->second.EndTime;
		SFund m_dwEveryPrize = it->second.EveryPrize;
		uint32_t m_dwFundCostCoin = it->second.CostCoin;
		uint32_t m_dwFundCostDollar = it->second.CostDollar;
		uint32_t m_dwFundReturnCoin = it->second.ReturnCoin;
		uint32_t m_dwFundReturnDollar = it->second.ReturnDollar;
		std::string m_mailtitle = it->second.mailtitle;
		std::string m_mailcontent = it->second.mailcontent;

		uint8_t isdel = 0;
		do
		{
//			IME_DEBUG("fund id %u end time %u get time %u", it->first, m_dwFundEndTime, m_dwFundGetTime);
			if (m_dwFundEndTime != 0 && !CUtil::IsToday(m_dwFundGetTime))
			{
				m_dwFundGetTime = it->second.GetTime = time(NULL);

				uint32_t now = CUtil::GetDateTime(m_dwFundGetTime);
				uint32_t end = CUtil::GetDateTime(m_dwFundEndTime);
//				IME_DEBUG("fund id %u end time %u get time %u, now %u, end %u", it->first, m_dwFundEndTime, m_dwFundGetTime, now, end);

				if (now > end)
				{
					//todo return guaranty
					CMail::SBaseAttr tmpAttr = {0};
					tmpAttr.coin = m_dwFundReturnCoin;
					tmpAttr.dollar = m_dwFundReturnDollar;
					uint32_t mail_id = 0;
					CDBCtrl::stuMailDetail tmp = {0};
					CMail::AddEmailSysDiy(m_pCRole->GetdwAccountId(), mail_id, tmp, 0, 0, 0, m_mailcontent, tmpAttr, m_mailtitle);   

//					CRole *pPeerCRole = sWorld->FindUserRole(m_pCRole->GetdwAccountId());
//					if (pPeerCRole != NULL)
//					{
						CMail::EmailUpdate(m_pCRole, CMail::ADD, &tmp, 0);
//					}

					isdel = 1;
					update = 1;
					break;
				}
				else if (now == end)
				{
					//fund every day prize
					CMail::SBaseAttr tmpAttr = {0};
					tmpAttr.coin = m_dwFundReturnCoin;
					tmpAttr.dollar = m_dwFundReturnDollar;
					uint32_t mail_id = 0;
					CDBCtrl::stuMailDetail tmp = {0};
					CMail::AddEmailSysDiy(m_pCRole->GetdwAccountId(), mail_id, tmp, 0, 0, 0, m_mailcontent, tmpAttr, m_mailtitle);   

//					CRole *pPeerCRole = sWorld->FindUserRole(m_pCRole->GetdwAccountId());
//					if (pPeerCRole != NULL)
//					{
						CMail::EmailUpdate(m_pCRole, CMail::ADD, &tmp, 0);
//					}
						isdel = 1;
						update = 1;
				}

				std::vector<uint16_t> vectype;
				std::vector<uint32_t> vecpara1;
				std::vector<uint32_t> vecpara2;
				if (m_dwEveryPrize.obj1.type != 0)
				{
					vectype.push_back(m_dwEveryPrize.obj1.type);
					vecpara1.push_back(m_dwEveryPrize.obj1.para1);
					vecpara2.push_back(m_dwEveryPrize.obj1.para2);
				}
				if (m_dwEveryPrize.obj2.type != 0)
				{
					vectype.push_back(m_dwEveryPrize.obj2.type);
					vecpara1.push_back(m_dwEveryPrize.obj2.para1);
					vecpara2.push_back(m_dwEveryPrize.obj2.para2);
				}
				if (m_dwEveryPrize.obj3.type != 0)
				{
					vectype.push_back(m_dwEveryPrize.obj3.type);
					vecpara1.push_back(m_dwEveryPrize.obj3.para1);
					vecpara2.push_back(m_dwEveryPrize.obj3.para2);
				}

				for (int k = 0; k < vectype.size(); k++)
				{
					uint16_t type = vectype[k];
					uint32_t para1 = vecpara1[k];
					uint32_t para2 = vecpara2[k];

					CMail::SBaseAttr tmpAttr = {0};
					uint32_t mail_id = 0;
					CDBCtrl::stuMailDetail tmp = {0};
					CMail::AddEmailSysDiy(m_pCRole->GetdwAccountId(), mail_id, tmp, type, para1, para2, m_mailcontent, tmpAttr, m_mailtitle);   

//					CRole *pPeerCRole = sWorld->FindUserRole(m_pCRole->GetdwAccountId());
//					if (pPeerCRole != NULL)
//					{
						CMail::EmailUpdate(m_pCRole, CMail::ADD, &tmp, 0);
//					}
				}
			}
		} while(0);
		
//		IME_DEBUG("fund id %u end time %u get time %u isdel %u", it->first, m_dwFundEndTime, m_dwFundGetTime, isdel);
		if (isdel)
		{
			std::map<uint32_t, SFundArr>::iterator itt = it;
			it++;
			m_mapFund.erase(itt);	
		}
		else
		{
			it++;
		}

	}
	if (update)
		HandlerCommonActiveInfo(FUND_ACTIVE_ID);
}

void CActive::ChangeCommonActive(uint32_t activeindex, SCommonActivePara para, uint8_t IsUpdate)
{
	if (activeindex == CHARGE_SINGLE_ACTIVE_ID)
	{
		if (!CActiveCtrl::SingleCharge)
			return;

		uint8_t IsNotice = 0;
		std::map<uint32_t, SChargePrize>::iterator itp;
		for (itp = CActiveCtrl::mapSingleChargePrize.begin(); itp != CActiveCtrl::mapSingleChargePrize.end(); itp++)
		{
			if (itp->second.prize == para.para1)
			{
				std::map<uint32_t, uint32_t>::iterator it = m_mapSingleChargeVal.find(itp->first);
				if (it == m_mapSingleChargeVal.end())
				{
					m_mapSingleChargeVal[itp->first] = 0;
					m_mapSingleChargeAgree[itp->first] = 1;
					IsNotice = 1;
				}
				else
				{
					m_mapSingleChargeAgree[itp->first]++;
					IsNotice = 1;
				}
			}
		}

		if (IsNotice && IsUpdate)
		{
			HandlerCommonActiveInfo(activeindex);
		}
	}
	else if (activeindex == CHARGE_ACCUMULATE_ACTIVE_ID)
	{
		if (!CActiveCtrl::AccumulateCharge)
			return;	

		m_dwAccumulateVal += para.para1;
		if (IsUpdate)
		{
			HandlerCommonActiveInfo(activeindex);
		}
	}
	else if (activeindex == CHARGE_CYCLE_ACTIVE_ID)
	{
		if (!CActiveCtrl::CycleCharge)
			return;

		m_dwCyclePoint += para.para1;
		if (IsUpdate)
		{
			HandlerCommonActiveInfo(activeindex);
		}
	}
}

void CActive::HandlerCDKeyGet(WorldPacket &packet)
{
	std::string cdkey;
	packet >> cdkey;
	IME_CHAR_LOG("action [cdkey get cdkey %s userid %u]", cdkey.c_str(), m_pCRole->GetdwAccountId());
	STC_CDKEY t;
	t.strCDkey = cdkey;
	if (!CenterDBCtrl::GetCDKEYAndVerify(t))
	{
//		IME_ERROR("cdkey get fail cdkey %s", cdkey.c_str());
		m_pCRole->SendErrorMsg(ERRNO_MSG_CDKEY_NONE, CMD_SC_CDKEY_RESULT);
		return;
	}
	
	CDKEY_PRIZE prize;
	CCsvReader csv;
	if (!csv.Init(t.strPrize))
	{
		IME_ERROR("init CDKEY prize fail cdkey %s", t.strCDkey.c_str());
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_CDKEY_RESULT);
		return;
	}
	std::vector<SProduct> vproduct;
	int i = 0;
	int j = 0;
	READER_CSV_INT(prize.coin);
	READER_CSV_INT(prize.dollar);
	READER_CSV_INT(prize.worldhistory);
	for (int m = 0; m < 6; m++)
	{
		READER_CSV_INT(prize.obj[m].type);
		READER_CSV_INT(prize.obj[m].para1);
		READER_CSV_INT(prize.obj[m].para2);
		vproduct.push_back(prize.obj[m]);
	}
	READER_CSV_STR(prize.title);
	READER_CSV_STR(prize.content);
	if (CShopEffect::IsObjValid(vproduct))
	{
		IME_ERROR("prize product type para fail");
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_CDKEY_RESULT);
		return;
	}

	if (csv.GetErrorStr() != "")
	{
		IME_ERROR("cdkey prize error cdkey %s", t.strCDkey.c_str());
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_CDKEY_RESULT);
		return;
	}

	if (t.byStatus != 0)
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_CDKEY_ALREADY_USE, CMD_SC_CDKEY_RESULT);
		return;
	}

//	if (t.dwChannel != m_pCRole->GetChannel())
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_CDKEY_CHANNEL, CMD_SC_CDKEY_RESULT);
//		return;
//	}

	int number = CenterDBCtrl::GetCount(t.dwBatchId, m_pCRole->GetdwAccountId(), m_pCRole->GetChannel());
	if (number >= t.dwLimitNumber || number == -1)
	{
//		IME_ERROR("cdkey get overload limit %d now %d", t.limit_number, number);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CDKEY_LIMIT_NUMBER, CMD_SC_CDKEY_RESULT);
		return;
	}

	uint32_t now = time(NULL);
	if (now <= t.dwStartTime || now >= t.dwEndTime)
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_CDKEY_INVALID, CMD_SC_CDKEY_RESULT);
		return;
	}
	
	if (!CenterDBCtrl::UpdateCDKEY(t.strCDkey, m_pCRole->GetdwAccountId()))
	{
		IME_ERROR("update cd key to used fail cdkey %s", t.strCDkey.c_str());
		m_pCRole->SendErrorMsg(ERRNO_MSG_DB, CMD_SC_CDKEY_RESULT);
		return;
	}

	CMail::SBaseAttr tmpAttr = {0};
	tmpAttr.coin = prize.coin;
	tmpAttr.worldhistory = prize.worldhistory;
	tmpAttr.dollar = prize.dollar;

	uint32_t mail_id = 0;
	CDBCtrl::stuMailDetail tmp = {0};
	CMail::AddEmailSysDiy(m_pCRole->GetdwAccountId(), mail_id, tmp, 0, 0, 0, prize.content, tmpAttr, prize.title);   

	CMail::EmailUpdate(m_pCRole, CMail::ADD, &tmp, 0);
	for (int m = 0; m < 6; m++)
	{
		if (prize.obj[m].type == 0)
			continue;
		CMail::SBaseAttr a = {0};
		CMail::AddEmailSysDiy(m_pCRole->GetdwAccountId(), mail_id, tmp, prize.obj[m].type, prize.obj[m].para1, prize.obj[m].para2, prize.content, a, prize.title);
		CMail::EmailUpdate(m_pCRole, CMail::ADD, &tmp, 0);
	}

	WorldPacket info(CMD_SC_CDKEY_RESULT);
	info << (uint16_t) 0;
	m_pCRole->SendPacket(&info);
}

CTimerGift::CTimerGift()
{
	m_IsInit = false;
//	m_IsStart = false;
	m_dwGiftIndex = -1;
}

int CTimerGift::Init(CRole *pCRole)
{
	m_pCRole = pCRole;
	std::string temp;
	ByteBuffer by;
	if (!CDBCtrl::GetUserDataTimerGift(temp, m_pCRole->GetdwAccountId()))
	{
		IME_ERROR("can not get active data userid %u", m_pCRole->GetdwAccountId());
		return -1;
	}
	by.append(temp.data(), temp.size());
	DeSeriliazeDB(by);
	OnTimer5s();
	m_dwGiftTime = time(NULL);
	
	m_IsInit = true;
	return 0;
}

void CTimerGift::SaveData()
{
	if (!m_IsInit)
		return;
	ByteBuffer by;
	SeriliazeDB(by);
	std::string temp;
	temp.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveUserDataTimerGift(temp, m_pCRole->GetdwAccountId());
	return;
}

void CTimerGift::OnTimer5s()
{
	if (!CUtil::IsToday(m_dwGiftTime))
	{
		m_dwGiftIndex = INIT_TIMER_GIFT_INDEX;
		m_dwGiftTime = time(NULL);
		HandlerInfo();
	}
}

void CTimerGift::SeriliazeDB(ByteBuffer &by)
{
	by << m_dwGiftTime;
	by << m_dwGiftIndex;
	return ;
}	

void CTimerGift::DeSeriliazeDB(ByteBuffer &by)
{
	by >> m_dwGiftTime;
	by >> m_dwGiftIndex;
	return;
}

void CTimerGift::StartTimer()
{
	m_dwGiftIndex = INIT_TIMER_GIFT_INDEX;
	m_dwGiftTime = time(NULL);
	HandlerInfo();
}

void CTimerGift::HandlerInfo()
{
	WorldPacket info(CMD_SC_TIMER_GIFT_INFO);
	STC_CONF_TIMER_GIFT *pConf = CConfTimerGift::Find(m_dwGiftIndex + 1);
	if (pConf == NULL)
	{
		info << (uint32_t) 0;
		info << (uint32_t) 0;
		m_pCRole->SendPacket(&info);
		return;
	}
	
	int64_t cd = (int64_t)m_dwGiftTime + pConf->dwCD - time(NULL);
//	IME_ERROR("time %u, cd %u, now %u, m_dwGiftIndex %u", m_dwGiftTime, pConf->dwCD, time(NULL), m_dwGiftIndex + 1);
	cd = cd < 0 ? 0 : cd;
	info << (uint32_t) cd;	
	info << (uint32_t)m_dwGiftIndex + 1;
	m_pCRole->SendPacket(&info);
}

void CTimerGift::HandlerRequestGiftPacket(WorldPacket &packet)
{
	uint32_t giftindex;
	packet >> giftindex;
	IME_CHAR_LOG("action [userid %u get timer gift %u]", m_pCRole->GetdwAccountId(), giftindex);
	if (m_dwGiftIndex < 0)
	{
		IME_ERROR("timer gift have not started");
		m_pCRole->SendErrorMsg(ERRNO_MSG_TIMER_GIFT_NOT_START, CMD_SC_REQUSET_TIMER_GIFT_RESULT);
		return;
	}
	if (giftindex != m_dwGiftIndex + 1)
	{
		IME_ERROR("request giftindex is unvalid %u it should be %u", giftindex, m_dwGiftIndex + 1);
		m_pCRole->SendErrorMsg(ERRNO_MSG_TIMER_GIFT_OVERLOAD, CMD_SC_REQUSET_TIMER_GIFT_RESULT);
		return;
	}
	STC_CONF_TIMER_GIFT *pConf = CConfTimerGift::Find(giftindex);
	if (pConf == NULL)
	{
		IME_ERROR("can not find data index %u in timer gift csv", giftindex);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_REQUSET_TIMER_GIFT_RESULT);
		return;
	}

	if (m_dwGiftTime + pConf->dwCD > time(NULL))
	{
		IME_ERROR("this timer gift in CD index %u, time %u, cd %u, now %u", giftindex, m_dwGiftTime, pConf->dwCD, time(NULL));
		m_pCRole->SendErrorMsg(ERRNO_MSG_TIMER_GIFT_IN_CD, CMD_SC_REQUSET_TIMER_GIFT_RESULT);
		return;
	}

	CShopEffect::SGiveProduct tmp = {0};
	if (pConf->byType < E_OBJ_MAX)
	{
		CShopEffect::GiveGameProduct(m_pCRole, pConf->byType, pConf->dwPara1, pConf->dwPara2, tmp, SOURCE_ACTIVE);
	}

	m_dwGiftIndex = giftindex;
	m_dwGiftTime = time(NULL);

	HandlerInfo();

	WorldPacket info(CMD_SC_REQUSET_TIMER_GIFT_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) giftindex;
	info << tmp.effect;
	info << (uint32_t)tmp.para1;
	info << (uint32_t)tmp.para2;
	m_pCRole->SendPacket(&info);
	return;
}

uint8_t CActiveCtrl::IsUpdate = 0;
//uint8_t CActiveCtrl::LoginSign = 0; 

//uint8_t CActiveCtrl::CardMultiBoss = 0;
//std::vector<int> CActiveCtrl::vecCardMulti;

uint8_t CActiveCtrl::FriendBoss = 0;
uint32_t CActiveCtrl::FriendBossRealStartTime = 0;
//uint32_t CActiveCtrl::FriendBossStartTime = 0;
//uint32_t CActiveCtrl::FriendBossEndTime = 0;
uint32_t CActiveCtrl::FriendBossIndex = 0;
uint32_t CActiveCtrl::FriendBossPro = 0;
//uint32_t CActiveCtrl::FriendBossItemId = 0;
//uint32_t CActiveCtrl::FriendBossItemProBase = 0;
//uint32_t CActiveCtrl::FriendBossCardId = 0;
//uint32_t CActiveCtrl::FriendBossCardProBase = 0;
//uint32_t CActiveCtrl::FriendBossProIncrease = 0;

uint8_t CActiveCtrl::NormalTurntable = 0;
uint8_t CActiveCtrl::SuperTurntable = 0;
std::vector<STruntable> CActiveCtrl::vecNormalTurnTable;
std::vector<STruntable> CActiveCtrl::vecSuperTurnTable;

uint8_t CActiveCtrl::ExpAdd = 0;
uint8_t CActiveCtrl::ExpMulti = 0;
std::vector<uint32_t> CActiveCtrl::vecExpMission;

uint8_t CActiveCtrl::CoinAdd = 0;
uint8_t CActiveCtrl::CoinMulti = 0;
std::vector<uint32_t> CActiveCtrl::vecCoinMission;

uint8_t CActiveCtrl::ObjDrop = 0;
uint8_t CActiveCtrl::ObjDropMulti = 0;
std::vector<uint32_t> CActiveCtrl::vecObjDropMission;

uint8_t CActiveCtrl::FriendBossRank;
std::vector<SFBRankGift> CActiveCtrl::vecFBRankPrize;
std::vector<SRank> CActiveCtrl::vecFBRankSec;
std::map<uint32_t, SFBRankRole> CActiveCtrl::mapPlayerRank; /* userid -> player info */
std::vector<SFBRank> CActiveCtrl::vecFBRank; 
std::map<uint32_t, SFBRankRole> CActiveCtrl::mapPlayerLastRank; /* userid -> player info */
std::vector<SFBRank> CActiveCtrl::vecFBLastRank;
std::string CActiveCtrl::FBContent;
uint32_t CActiveCtrl::FBRankFlushTime;
uint32_t CActiveCtrl::FBRankGrowTime = time(NULL);

//charge single
uint8_t CActiveCtrl::SingleCharge;
std::map<uint32_t, SChargePrize> CActiveCtrl::mapSingleChargePrize;
uint32_t CActiveCtrl::SingleChargeFlushTime;

//charge accumulate
uint8_t CActiveCtrl::AccumulateCharge;
std::map<uint32_t, SChargePrize> CActiveCtrl::mapAccumulateChargePrize;
uint32_t CActiveCtrl::AccumulateChargeFlushTime;

//charge cycle
uint8_t CActiveCtrl::CycleCharge;
std::map<uint32_t, SChargePrize> CActiveCtrl::mapCycleChargePrize;
uint32_t CActiveCtrl::CycleChargeFlushTime;

//extra login
uint8_t CActiveCtrl::ExtraLogin;
std::map<uint32_t, SExtraLogin> CActiveCtrl::mapExtraLogin;
uint32_t CActiveCtrl::ExtraLoginFlushTime;

//fund
uint8_t CActiveCtrl::Fund;
std::map<uint32_t, SFundActive> CActiveCtrl::mapFund;
std::string CActiveCtrl::FundMailTitle;
std::string CActiveCtrl::FundMailContent;

//active task
uint8_t CActiveCtrl::Task[TASK_ACTIVE_MAX];
std::vector<std::vector<uint32_t> > CActiveCtrl::vecTask;
uint32_t CActiveCtrl::TaskFlushTime[TASK_ACTIVE_MAX];

std::map<uint32_t, SActiveData> CActiveCtrl::m_mapT; 

bool CActiveCtrl::IsActiveStart(uint32_t activeid)
{
	switch(activeid)
	{
//		case LOGIN_SIGN_ACTIVE_ID:
//			return true;
//		case CARD_MULTI_BOSS_ACTIVE_ID:
//			return true;
		case FRIEND_BOSS_ACTIVE_ID:
			return FriendBoss;
		case TURN_NORMAL_TABLE_ACTIVE_ID:
			return NormalTurntable;
		case TURN_SUPER_TABLE_ACTIVE_ID:
			return SuperTurntable;
		case EXP_ADD_ACTIVE_ID:
			return ExpAdd;
		case COIN_ADD_ACTIVE_ID:
			return CoinAdd;
		case OBJ_DROP_ACTIVE_ID:
			return ObjDrop;
		case FRIEND_BOSS_RANK_ACTIVE_ID:
			return FriendBossRank;
		case CHARGE_SINGLE_ACTIVE_ID:
			return SingleCharge;
		case CHARGE_ACCUMULATE_ACTIVE_ID:
			return AccumulateCharge;
		case CHARGE_CYCLE_ACTIVE_ID:
			return CycleCharge;
		case LOGIN_EXTRA_ACTIVE_ID:
			return ExtraLogin;
		case FUND_ACTIVE_ID:
			return Fund;
		case TASK1_ACTIVE_ID:
			return Task[0];
		case TASK2_ACTIVE_ID:
			return Task[1];
		case TASK3_ACTIVE_ID:
			return Task[2];
		case TASK4_ACTIVE_ID:
			return Task[3];
		case TASK5_ACTIVE_ID:
			return Task[4];
		case TASK6_ACTIVE_ID:
			return Task[5];
		case TASK7_ACTIVE_ID:
			return Task[6];
		default:
			return false;
	}
	return false;
}

bool CActiveCtrl::GetData()
{
//	LoginSign = 1; 
//	CardMultiBoss = 1;	

	std::string tmp;
	vecNormalTurnTable.clear();
	vecSuperTurnTable.clear();

//	if (!CDBCtrl::GetGlobalActiveData(CARD_MULTI_BOSS_ACTIVE_ID, tmp))
//	{
//		if (CDBCtrl::InsertGlobalActiveData(CARD_MULTI_BOSS_ACTIVE_ID, tmp))
//		{
//			IME_ERROR("insert global active data");
//			return false;
//		}
//	}	
//	else
//	{
//		ByteBuffer by;
//		by.append(tmp.contents(), tmp.size());	
//		by >> CardMultiBoss;
//		uint32_t number;
//		uint32_t val;
//		by >> number;
//		for (int i = 0; i < number; i++)
//		{
//			by >> val;
//			vecCardMulti.push_back(val);
//		}
//	}

	if (!CDBCtrl::GetGlobalActiveData(FRIEND_BOSS_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(FRIEND_BOSS_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		by >> FriendBoss;
		by >> FriendBossRealStartTime;
		by >> FriendBossIndex;
		by >> FriendBossPro;
	}

	tmp.clear();
	if (!CDBCtrl::GetGlobalActiveData(TURN_NORMAL_TABLE_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(TURN_NORMAL_TABLE_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		by >> NormalTurntable;
		uint32_t number;
		by >> number;
		STruntable stmp;
		for (int i = 0; i < number; i++)
		{
			by >> stmp.type;
			by >> stmp.para1;
			by >> stmp.para2;
			by >> stmp.pro;
			vecNormalTurnTable.push_back(stmp);
		}
	}

	tmp.clear();
	if (!CDBCtrl::GetGlobalActiveData(TURN_SUPER_TABLE_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(TURN_SUPER_TABLE_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		by >> SuperTurntable;
		uint32_t number;
		by >> number;
		STruntable stmp;
		for (int i = 0; i < number; i++)
		{
			by >> stmp.type;
			by >> stmp.para1;
			by >> stmp.para2;
			by >> stmp.pro;
			vecSuperTurnTable.push_back(stmp);
		}
	}

	tmp.clear();
	if (!CDBCtrl::GetGlobalActiveData(EXP_ADD_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(EXP_ADD_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		by >> ExpAdd;
		by >> ExpMulti;
		uint32_t number;
		by >> number;
		uint32_t val; 
		for (int i = 0; i < number; i++)
		{
			by >> val;
			vecExpMission.push_back(val);
		}
	}

	tmp.clear();
	if (!CDBCtrl::GetGlobalActiveData(COIN_ADD_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(COIN_ADD_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		by >> CoinAdd;
		by >> CoinMulti;
		uint32_t number;
		by >> number;
		uint32_t val; 
		for (int i = 0; i < number; i++)
		{
			by >> val;
			vecCoinMission.push_back(val);
		}
	}

	tmp.clear();
	if (!CDBCtrl::GetGlobalActiveData(OBJ_DROP_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(OBJ_DROP_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		by >> ObjDrop;
		by >> ObjDropMulti;
		uint32_t number;
		by >> number;
		uint32_t val; 
		for (int i = 0; i < number; i++)
		{
			by >> val;
			vecObjDropMission.push_back(val);
		}
	}

	tmp.clear();
	if (!CDBCtrl::GetGlobalActiveData(FRIEND_BOSS_RANK_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(FRIEND_BOSS_RANK_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		by >> FriendBossRank;
		uint32_t number;
		by >> number;
		vecFBRankPrize.clear();
		for (int i = 0; i < number; i++)
		{
			SFBRankGift tmp;
			by >> tmp.obj1.type;
			by >> tmp.obj1.para1;
			by >> tmp.obj1.para2;
			by >> tmp.obj2.type;
			by >> tmp.obj2.para1;
			by >> tmp.obj2.para2;
			by >> tmp.obj3.type;
			by >> tmp.obj3.para1;
			by >> tmp.obj3.para2;
			by >> tmp.tips;
			vecFBRankPrize.push_back(tmp);
		}
		IME_DEBUG("boss rank prize %u", number);
		by >> number;
		IME_DEBUG("boss rank rank %u", number);
		vecFBRankSec.clear();
		for (int i = 0; i < number; i++)
		{
			SRank tmp;
			by >> tmp.start;
			by >> tmp.end;
			vecFBRankSec.push_back(tmp);
			IME_DEBUG("seq %u start %u end %u", i, tmp.start, tmp.end);
		}
		
		by >> number;
		mapPlayerRank.clear();
		vecFBRank.clear();
		uint32_t key;
		for (int i = 0; i < number; i++)
		{
			SFBRankRole tmp;
			uint16_t n;
			by >> key;
//			by >> tmp.userid;
//			by >> tmp.level;
//			by >> tmp.name;
//			by >> n;
//			for (int j = 0; j < n; j++)
//			{
//				uint32_t cardindex;
//				by >> cardindex;
//				tmp.cardindex.push_back(cardindex);
//			}
//			by >> tmp.masterindex;
			by >> tmp.devote;
			IME_DEBUG("seq %u userid %u, devote %u", i, key, tmp.devote);
			if (!IS_ROBORT(key))
			{
				CRole *pCRoleOffline = sWorld->FindUserRole(key);
				CRole temp(NULL);
				if (pCRoleOffline == NULL)
				{
					pCRoleOffline = &temp;
					assert(pCRoleOffline != NULL);
					if (!pCRoleOffline->LoadDataForDuration(key))
					{
						IME_ERROR("load peer role from DB fail userid %u", key);
						continue;
					}
				}
				pCRoleOffline->GetclsCardPacket()->PreDataForRank(tmp, tmp.devote);
			}
			else
			{
				uint32_t devote = tmp.devote;
				CRobot *pCRobot = new (std::nothrow) CRobot();
				if (pCRobot == NULL)
				{
					IME_ERROR("new a obj fail %u");
					return false;
				}
				if (pCRobot->Init(key))
				{
					IME_ERROR("init robot fail %u", key);
					return false;
				}
				if (pCRobot->PreDataForRank(tmp))
				{
					IME_ERROR("pre data for rank fail robot id %u", key);
					return false;
				}
				tmp.devote = devote;
			}

			mapPlayerRank[key] = tmp;
			SFBRank b;
			b.devote = tmp.devote;
			b.userid = tmp.userid;
			vecFBRank.push_back(b);
		}
		std::stable_sort(vecFBRank.begin(), vecFBRank.end(), mybig);

		by >> number;
		mapPlayerLastRank.clear();
		vecFBLastRank.clear();
		for (int i = 0; i < number; i++)
		{
			SFBRankRole tmp;
			uint16_t n;
			by >> key;
//			by >> tmp.userid;
//			by >> tmp.level;
//			by >> tmp.name;
//			by >> n;
//			for (int j = 0; j < n; j++)
//			{
//				uint32_t cardindex;
//				by >> cardindex;
//				tmp.cardindex.push_back(cardindex);
//			}
//			by >> tmp.masterindex;
			by >> tmp.devote;
			IME_DEBUG("last seq %u userid %u, devote %u", i, key, tmp.devote);

			if (!IS_ROBORT(key))
			{
				CRole *pCRoleOffline = sWorld->FindUserRole(key);
				CRole temp(NULL);
				if (pCRoleOffline == NULL)
				{
					pCRoleOffline = &temp;
					assert(pCRoleOffline != NULL);
					if (!pCRoleOffline->LoadDataForDuration(key))
					{
						IME_ERROR("load peer role from DB fail userid %u", key);
						continue;
					}
					pCRoleOffline->GetclsCardPacket()->PreDataForRank(tmp, tmp.devote);
				}
			}
			else
			{
				uint32_t devote = tmp.devote;
				CRobot *pCRobot = new (std::nothrow) CRobot();
				if (pCRobot == NULL)
				{
					IME_ERROR("new a obj fail %u");
					return false;
				}
				if (pCRobot->Init(key))
				{
					IME_ERROR("init robot fail %u", key);
					return false;
				}
				if (pCRobot->PreDataForRank(tmp))
				{
					IME_ERROR("pre data for rank fail robot id %u", key);
					return false;
				}
				tmp.devote = devote;
			}
			
			mapPlayerLastRank[key] = tmp;
			SFBRank b;
			b.devote = tmp.devote;
			b.userid = tmp.userid;
			vecFBLastRank.push_back(b);
		}
		std::stable_sort(vecFBLastRank.begin(), vecFBLastRank.end(), mybig);

		by >> FBContent;
		by >> FBRankFlushTime;
	}

	tmp.clear();
	if (!CDBCtrl::GetGlobalActiveData(CHARGE_SINGLE_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(CHARGE_SINGLE_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		by >> SingleCharge;
		uint32_t number;
		by >> number;
		uint32_t key; 
		SChargePrize tmp;
		for (int i = 0; i < number; i++)
		{
			by >> key;
			by >> tmp.obj1.type;
			by >> tmp.obj1.para1;
			by >> tmp.obj1.para2;
			by >> tmp.obj2.type;
			by >> tmp.obj2.para1;
			by >> tmp.obj2.para2;
			by >> tmp.obj3.type;
			by >> tmp.obj3.para1;
			by >> tmp.obj3.para2;
			by >> tmp.obj4.type;
			by >> tmp.obj4.para1;
			by >> tmp.obj4.para2;
			by >> tmp.prize;
			by >> tmp.number;
			by >> tmp.desc;
			mapSingleChargePrize[key] = tmp;
		}
		by >> SingleChargeFlushTime;
	}

	tmp.clear();
	if (!CDBCtrl::GetGlobalActiveData(CHARGE_ACCUMULATE_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(CHARGE_ACCUMULATE_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		by >> AccumulateCharge;
		uint32_t number;
		by >> number;
		uint32_t key; 
		SChargePrize tmp;
		for (int i = 0; i < number; i++)
		{
			by >> key;
			by >> tmp.obj1.type;
			by >> tmp.obj1.para1;
			by >> tmp.obj1.para2;
			by >> tmp.obj2.type;
			by >> tmp.obj2.para1;
			by >> tmp.obj2.para2;
			by >> tmp.obj3.type;
			by >> tmp.obj3.para1;
			by >> tmp.obj3.para2;
			by >> tmp.obj4.type;
			by >> tmp.obj4.para1;
			by >> tmp.obj4.para2;
			by >> tmp.prize;
			by >> tmp.number;
			by >> tmp.desc;
			mapAccumulateChargePrize[key] = tmp;
		}
		by >> AccumulateChargeFlushTime;
	}

	tmp.clear();
	if (!CDBCtrl::GetGlobalActiveData(CHARGE_CYCLE_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(CHARGE_CYCLE_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		by >> CycleCharge;
		uint32_t number;
		by >> number;
		uint32_t key; 
		SChargePrize tmp;
		for (int i = 0; i < number; i++)
		{
			by >> key;
			by >> tmp.obj1.type;
			by >> tmp.obj1.para1;
			by >> tmp.obj1.para2;
			by >> tmp.obj2.type;
			by >> tmp.obj2.para1;
			by >> tmp.obj2.para2;
			by >> tmp.obj3.type;
			by >> tmp.obj3.para1;
			by >> tmp.obj3.para2;
			by >> tmp.obj4.type;
			by >> tmp.obj4.para1;
			by >> tmp.obj4.para2;
			by >> tmp.prize;
			by >> tmp.number;
			by >> tmp.desc;
			mapCycleChargePrize[key] = tmp;
		}
		by >> CycleChargeFlushTime;
	}

	tmp.clear();
	if (!CDBCtrl::GetGlobalActiveData(LOGIN_EXTRA_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(LOGIN_EXTRA_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		by >> ExtraLogin;
		uint32_t number;
		by >> number;
		uint32_t key; 
		SExtraLogin tmp;
		for (int i = 0; i < number; i++)
		{
			by >> key;
			by >> tmp.obj1.type;
			by >> tmp.obj1.para1;
			by >> tmp.obj1.para2;
			by >> tmp.obj2.type;
			by >> tmp.obj2.para1;
			by >> tmp.obj2.para2;
			by >> tmp.obj3.type;
			by >> tmp.obj3.para1;
			by >> tmp.obj3.para2;
			by >> tmp.nextindex;
			by >> tmp.desc;
			mapExtraLogin[key] = tmp;
		}
		by >> ExtraLoginFlushTime;
	}

	tmp.clear();
	if (!CDBCtrl::GetGlobalActiveData(FUND_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(FUND_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		by >> Fund;
		uint32_t number;
		by >> number;
		uint32_t key; 
		SFundActive tmp;
		for (int i = 0; i < number; i++)
		{
			by >> key;
			by >> tmp.obj1.type;
			by >> tmp.obj1.para1;
			by >> tmp.obj1.para2;
			by >> tmp.obj2.type;
			by >> tmp.obj2.para1;
			by >> tmp.obj2.para2;
			by >> tmp.obj3.type;
			by >> tmp.obj3.para1;
			by >> tmp.obj3.para2;
			by >> tmp.CostCoin;
			by >> tmp.CostDollar;
			by >> tmp.Day;
			by >> tmp.desc;
			by >> tmp.ReturnCoin;
			by >> tmp.ReturnDollar;
			mapFund[key] = tmp;
		}
		by >> FundMailTitle;
		by >> FundMailContent;
//		IME_ERROR("mail title %s, content %s", FundMailTitle.c_str(), FundMailContent.c_str());
	}

	vecTask.resize(TASK_ACTIVE_MAX);
	for (int m = 0; m < TASK_ACTIVE_MAX; m++)
	{
		tmp.clear();
		if (!CDBCtrl::GetGlobalActiveData(TASK1_ACTIVE_ID + m, tmp))
		{
			if (!CDBCtrl::InsertGlobalActiveData(TASK1_ACTIVE_ID + m, tmp))
			{
				IME_ERROR("insert global active data");
				return false;
			}
		}	
		else
		{
			ByteBuffer by;
			by.append(tmp.data(), tmp.size());	
			by >> Task[m];
			uint32_t number;
			by >> number;
			uint32_t key; 
			std::vector<uint32_t> vect;
			for (int i = 0; i < number; i++)
			{
				by >> key;
				vect.push_back(key);
			}
			vecTask[m] = vect;

			by >> TaskFlushTime[m];
		}
	}

	if (!InitActive())
	{
		IME_ERROR("init active data fail");
		return false;
	}

	return true;
}

void CActiveCtrl::SaveData()
{
	ByteBuffer by;
//	by << CardMultiBoss;
//	by << (uint32_t) vecCardMulti.size();
//	for (int i = 0; i < vecCardMulti.size(); i++)
//	{
//		by << vecCardMulti[i];
//	}	
//	std::string str;
//	str.assign(by.contents(), by.size());
//	CDBCtrl::SaveGlobalActiveData(CARD_MULTI_BOSS_ACTIVE_ID, str);

	by.clear();
	by << FriendBoss;
	by << FriendBossRealStartTime;
	by << FriendBossIndex;
	by << FriendBossPro;
//	by << FriendBossStartTime;
//	by << FriendBossEndTime;
	std::string str;
	str.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveGlobalActiveData(str, FRIEND_BOSS_ACTIVE_ID);

	by.clear();
	by << NormalTurntable;
	by << (uint32_t) vecNormalTurnTable.size();
	for (int i = 0; i < vecNormalTurnTable.size(); i++)
	{
		by << vecNormalTurnTable[i].type;
		by << vecNormalTurnTable[i].para1;
		by << vecNormalTurnTable[i].para2;
		by << vecNormalTurnTable[i].pro;
	}
	str.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveGlobalActiveData(str, TURN_NORMAL_TABLE_ACTIVE_ID);

	by.clear();
	by << SuperTurntable;
	by << (uint32_t) vecSuperTurnTable.size();
	for (int i = 0; i < vecSuperTurnTable.size(); i++)
	{
		by << vecSuperTurnTable[i].type;
		by << vecSuperTurnTable[i].para1;
		by << vecSuperTurnTable[i].para2;
		by << vecSuperTurnTable[i].pro;
	}
	str.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveGlobalActiveData(str, TURN_SUPER_TABLE_ACTIVE_ID);

	by.clear();
	by << ExpAdd;
	by << ExpMulti;
	by << (uint32_t) vecExpMission.size();
	for (int i = 0; i < vecExpMission.size(); i++)
	{
		by << vecExpMission[i];
	}
	str.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveGlobalActiveData(str, EXP_ADD_ACTIVE_ID);

	by.clear();
	by << CoinAdd;
	by << CoinMulti;
	by << (uint32_t) vecCoinMission.size();
	for (int i = 0; i < vecCoinMission.size(); i++)
	{
		by << vecCoinMission[i];
	}
	str.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveGlobalActiveData(str, COIN_ADD_ACTIVE_ID);

	by.clear();
	by << ObjDrop;
	by << ObjDropMulti;
	by << (uint32_t) vecObjDropMission.size();
	for (int i = 0; i < vecObjDropMission.size(); i++)
	{
		by << vecObjDropMission[i];
	}
	str.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveGlobalActiveData(str, OBJ_DROP_ACTIVE_ID);
	
	by.clear();
	by << FriendBossRank;
	by << (uint32_t) vecFBRankPrize.size();
//	IME_ERROR("rank prize %u", vecFBRankPrize.size());
	for (int i = 0; i < vecFBRankPrize.size(); i++)
	{
		by << vecFBRankPrize[i].obj1.type;
		by << vecFBRankPrize[i].obj1.para1;
		by << vecFBRankPrize[i].obj1.para2;
		by << vecFBRankPrize[i].obj2.type;
		by << vecFBRankPrize[i].obj2.para1;
		by << vecFBRankPrize[i].obj2.para2;
		by << vecFBRankPrize[i].obj3.type;
		by << vecFBRankPrize[i].obj3.para1;
		by << vecFBRankPrize[i].obj3.para2;
		by << vecFBRankPrize[i].tips;
	}
	by << (uint32_t) vecFBRankSec.size();
//	IME_ERROR("rank rank size %u", vecFBRankSec.size());
	for (int i = 0; i < vecFBRankSec.size(); i++)
	{
		by << vecFBRankSec[i].start;
		by << vecFBRankSec[i].end;
	}
	by << (uint32_t) mapPlayerRank.size();
//	IME_ERROR("rank rank player %u", mapPlayerRank.size());
	std::map<uint32_t, SFBRankRole>::iterator it = mapPlayerRank.begin();
	for ( ;it != mapPlayerRank.end(); it++)
	{
		by << it->first;
//		by << it->second.userid;
//		by << it->second.level;
//		by << it->second.name;
//		uint16_t number = it->second.cardindex.size();
//		by << (uint16_t) number;
//		for (int j = 0; j < number; j++)
//		{
//			by << it->second.cardindex[j];
//		}
//		by << it->second.masterindex;
		by << it->second.devote;
	}

	by << (uint32_t) mapPlayerLastRank.size();
	std::map<uint32_t, SFBRankRole>::iterator itl = mapPlayerLastRank.begin();
//	IME_ERROR("rank rank last player %u", mapPlayerLastRank.size());
	for ( ;itl != mapPlayerLastRank.end(); itl++)
	{
		by << itl->first;
//		by << itl->second.userid;
//		by << itl->second.level;
//		by << itl->second.name;
//		uint16_t number = itl->second.cardindex.size();
//		by << (uint16_t) number;
//		for (int j = 0; j < number; j++)
//		{
//			by << itl->second.cardindex[j];
//		}
//		by << itl->second.masterindex;
		by << itl->second.devote;
	}
	by << FBContent;
	by << FBRankFlushTime;

	str.assign((const char *)by.contents(), by.size());
//	IME_ERROR("total size %u", by.size());
	CDBCtrl::SaveGlobalActiveData(str, FRIEND_BOSS_RANK_ACTIVE_ID);

	by.clear();
	by << SingleCharge;
	by << (uint32_t) mapSingleChargePrize.size();
	std::map<uint32_t, SChargePrize>::iterator itc;
	for (itc = mapSingleChargePrize.begin(); itc != mapSingleChargePrize.end(); itc++)
	{
		by << itc->first;
		by << itc->second.obj1.type;
		by << itc->second.obj1.para1;
		by << itc->second.obj1.para2;
		by << itc->second.obj2.type;
		by << itc->second.obj2.para1;
		by << itc->second.obj2.para2;
		by << itc->second.obj3.type;
		by << itc->second.obj3.para1;
		by << itc->second.obj3.para2;
		by << itc->second.obj4.type;
		by << itc->second.obj4.para1;
		by << itc->second.obj4.para2;
		by << itc->second.prize;
		by << itc->second.number;
		by << itc->second.desc;
	}
	by << SingleChargeFlushTime;
	str.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveGlobalActiveData(str, CHARGE_SINGLE_ACTIVE_ID);

	by.clear();
	by << AccumulateCharge;
	by << (uint32_t) mapAccumulateChargePrize.size();
	for (itc = mapAccumulateChargePrize.begin(); itc != mapAccumulateChargePrize.end(); itc++)
	{
		by << itc->first;
		by << itc->second.obj1.type;
		by << itc->second.obj1.para1;
		by << itc->second.obj1.para2;
		by << itc->second.obj2.type;
		by << itc->second.obj2.para1;
		by << itc->second.obj2.para2;
		by << itc->second.obj3.type;
		by << itc->second.obj3.para1;
		by << itc->second.obj3.para2;
		by << itc->second.obj4.type;
		by << itc->second.obj4.para1;
		by << itc->second.obj4.para2;
		by << itc->second.prize;
		by << itc->second.number;
		by << itc->second.desc;
	}
	by << AccumulateChargeFlushTime;
	str.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveGlobalActiveData(str, CHARGE_ACCUMULATE_ACTIVE_ID);

	by.clear();
	by << CycleCharge;
	by << (uint32_t) mapCycleChargePrize.size();
	for (itc = mapCycleChargePrize.begin(); itc != mapCycleChargePrize.end(); itc++)
	{
		by << itc->first;
		by << itc->second.obj1.type;
		by << itc->second.obj1.para1;
		by << itc->second.obj1.para2;
		by << itc->second.obj2.type;
		by << itc->second.obj2.para1;
		by << itc->second.obj2.para2;
		by << itc->second.obj3.type;
		by << itc->second.obj3.para1;
		by << itc->second.obj3.para2;
		by << itc->second.obj4.type;
		by << itc->second.obj4.para1;
		by << itc->second.obj4.para2;
		by << itc->second.prize;
		by << itc->second.number;
		by << itc->second.desc;
	}
	by << CycleChargeFlushTime;
	str.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveGlobalActiveData(str, CHARGE_CYCLE_ACTIVE_ID);

	by.clear();
	by << ExtraLogin;
	by << (uint32_t) mapExtraLogin.size();
	std::map<uint32_t, SExtraLogin>::iterator ite;
	for (ite = mapExtraLogin.begin(); ite != mapExtraLogin.end(); ite++)
	{
		by << ite->first;
		by << ite->second.obj1.type;
		by << ite->second.obj1.para1;
		by << ite->second.obj1.para2;
		by << ite->second.obj2.type;
		by << ite->second.obj2.para1;
		by << ite->second.obj2.para2;
		by << ite->second.obj3.type;
		by << ite->second.obj3.para1;
		by << ite->second.obj3.para2;
		by << ite->second.nextindex;
		by << ite->second.desc;
	}
	by << ExtraLoginFlushTime;
	str.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveGlobalActiveData(str, LOGIN_EXTRA_ACTIVE_ID);

	by.clear();
	by << Fund;
	by << (uint32_t) mapFund.size();
	std::map<uint32_t, SFundActive>::iterator itf;
	for (itf = mapFund.begin(); itf != mapFund.end(); itf++)
	{
		by << itf->first;
		by << itf->second.obj1.type;
		by << itf->second.obj1.para1;
		by << itf->second.obj1.para2;
		by << itf->second.obj2.type;
		by << itf->second.obj2.para1;
		by << itf->second.obj2.para2;
		by << itf->second.obj3.type;
		by << itf->second.obj3.para1;
		by << itf->second.obj3.para2;
		by << itf->second.CostCoin;
		by << itf->second.CostDollar;
		by << itf->second.Day;
		by << itf->second.desc;
		by << itf->second.ReturnCoin;
		by << itf->second.ReturnDollar;
	}
	by << FundMailTitle;
	by << FundMailContent;
//	IME_ERROR("mail title %s, content %s", FundMailTitle.c_str(), FundMailContent.c_str());
	str.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveGlobalActiveData(str, FUND_ACTIVE_ID);

	for (int m = 0; m < TASK_ACTIVE_MAX; m++)
	{
		by.clear();
		by << Task[m];
		by << (uint32_t) vecTask[m].size();
		for (int i = 0; i < vecTask[m].size(); i++)
		{
			by << vecTask[m][i];
		}
		by << TaskFlushTime[m];
		str.assign((const char *)by.contents(), by.size());
		CDBCtrl::SaveGlobalActiveData(str, TASK1_ACTIVE_ID + m);
	}

	SaveActive();
}

void CActiveCtrl::HandlerActiveBase(CRole *pCRole, uint8_t ignore)
{
	if (ignore || IsUpdate)
	{
		WorldPacket activepkg(CMD_SC_ACTIVE_BASE_INFO);
		WorldPacket pkg(CMD_SC_ACTIVE_BASE_INFO);
		std::map<int, SActive> mapSpecial;
		mapSpecial[FRIEND_BOSS_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(FRIEND_BOSS_ACTIVE_ID);
		mapSpecial[TURN_NORMAL_TABLE_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(TURN_NORMAL_TABLE_ACTIVE_ID);	
		mapSpecial[TURN_SUPER_TABLE_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(TURN_SUPER_TABLE_ACTIVE_ID);
		mapSpecial[EXP_ADD_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(EXP_ADD_ACTIVE_ID);
		mapSpecial[COIN_ADD_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(COIN_ADD_ACTIVE_ID);
		mapSpecial[OBJ_DROP_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(OBJ_DROP_ACTIVE_ID);
		mapSpecial[CHARGE_SINGLE_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(CHARGE_SINGLE_ACTIVE_ID);
		mapSpecial[CHARGE_ACCUMULATE_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(CHARGE_ACCUMULATE_ACTIVE_ID);
		mapSpecial[CHARGE_CYCLE_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(CHARGE_CYCLE_ACTIVE_ID);
		mapSpecial[LOGIN_EXTRA_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(LOGIN_EXTRA_ACTIVE_ID);
		mapSpecial[FUND_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(FUND_ACTIVE_ID);
		mapSpecial[TASK1_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(TASK1_ACTIVE_ID);
		mapSpecial[TASK2_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(TASK2_ACTIVE_ID);
		mapSpecial[TASK3_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(TASK3_ACTIVE_ID);
		mapSpecial[TASK4_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(TASK4_ACTIVE_ID);
		mapSpecial[TASK5_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(TASK5_ACTIVE_ID);
		mapSpecial[TASK6_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(TASK6_ACTIVE_ID);
		mapSpecial[TASK7_ACTIVE_ID].byIsOpen = CActiveCtrl::IsPrepare(TASK7_ACTIVE_ID);
		CConfActive::MapData::iterator it;
		CConfActive::MapData tmpmap = CConfActive::GetData();
		uint16_t number = 0;

		std::vector<WorldPacket> vect;
		vect.resize(ACTIVE_INIT_SPACE);

		for (it = tmpmap.begin(); it != tmpmap.end(); it++)
		{
			WorldPacket pkg2;
			if (mapSpecial.find(it->first) == mapSpecial.end())
			{
				pkg2 << it->second.dwIndex;
				pkg2 << it->second.byType;
				pkg2 << it->second.strName;
				pkg2 << it->second.strDesc;
				pkg2 << it->second.wOpr;
				pkg2 << it->second.dwPara;
				pkg2 << (uint32_t) 0;
				pkg2 << (uint32_t) 0;
				pkg2 << (uint32_t) 0;
				number++;

				STC_CONF_ACTIVE *pConf = CConfActive::Find(it->second.dwIndex);
				if (pConf == NULL)
				{
					IME_ERROR("can not find active index %u", it->second.dwIndex);
					return;
				}
				else
				{
					if (pConf->dwSort <= ACTIVE_INIT_SPACE && pConf->dwSort > 0)
					{
						vect[pConf->dwSort - 1] = pkg2;
					}
					else
					{
						IME_ERROR("active sort big active init space, index %u, sort %u", pConf->dwIndex, pConf->dwSort);
						return;
					}
				}
			}
			else
			{
				if (mapSpecial[it->first].byIsOpen)
				{
					pkg2 << it->second.dwIndex;
					pkg2 << it->second.byType;
					pkg2 << CActiveCtrl::m_mapT[it->first].strTitle;
					pkg2 << CActiveCtrl::m_mapT[it->first].strContent;
					pkg2 << it->second.wOpr;
					pkg2 << it->second.dwPara;
					int32_t cd = 0;
					uint32_t start = CActiveCtrl::m_mapT[it->first].pGm->GetStartTime();
					uint32_t end = CActiveCtrl::m_mapT[it->first].pGm->GetEndTime();
					cd = (int32_t) start - time(NULL);
					cd = cd < 0 ? 0 : cd;
					pkg2 << (uint32_t) cd;
					pkg2 << (uint32_t) start;
					pkg2 << (uint32_t) end;
					number++;

					STC_CONF_ACTIVE *pConf = CConfActive::Find(it->second.dwIndex);
					if (pConf == NULL)
					{
						IME_ERROR("can not find active index %u", it->second.dwIndex);
						return;
					}
					else
					{
						if (pConf->dwSort <= ACTIVE_INIT_SPACE && pConf->dwSort > 0)
						{
							vect[pConf->dwSort - 1] = pkg2;
						}
						else
						{
							IME_ERROR("active sort big active init space, index %u, sort %u", pConf->dwIndex, pConf->dwSort);
							return;
						}
					}
				}
			}
		}
		for (int i = 0; i < vect.size(); i++)                     
		{                                                         
		    pkg.append(vect[i].contents(), vect[i].size());
//			IME_DEBUG("NO %u, size %u", i, vect[i].size());
		}
		
		activepkg << number;
		activepkg.append(pkg.contents(), pkg.size());
		if (pCRole != NULL)
		{
			pCRole->SendPacket(&activepkg);
		}
		else
		{
			sSessCtrl->SendPkgToAll(activepkg);
		}
	}
	return;
}

void CActiveCtrl::OnTimer1s()
{
	UpdateActive();

	if (vecFBRank.size() != 0 )
	{
//		IME_ERROR("rankgrow time %u", FBRankGrowTime);
		if (!CUtil::IsToday(FBRankGrowTime))
		{
			IME_ERROR("1 rankgrow time %u", FBRankGrowTime);
			FBRankGrowTime = time(NULL);
			for (int i = 0; i < vecFBRank.size(); i++)
			{
				if (IS_ROBORT(vecFBRank[i].userid))
				{
					STC_CONF_ROB_ROBOT *pConf = CConfRobRobot::Find(vecFBRank[i].userid);
					if (pConf == NULL)
					{
						IME_ERROR("can not find robot id %u in robot csv", vecFBRank[i].userid);
						continue;
					}
					
//					IME_ERROR("2 %u userid %u rankgrow time %u, adddevote %u, devote %u", i, vecFBRank[i].userid, FBRankGrowTime, pConf->dwDevoteGrow, vecFBRank[i].devote);
					vecFBRank[i].devote += pConf->dwDevoteGrow;
					mapPlayerRank[vecFBRank[i].userid].devote += pConf->dwDevoteGrow;
//					IME_ERROR("3 %u userid %u rankgrow time %u, adddevote %u, devote %u", i, vecFBRank[i].userid, FBRankGrowTime, pConf->dwDevoteGrow, vecFBRank[i].devote);
				}
			}
			std::stable_sort(vecFBRank.begin(), vecFBRank.end(), mybig);

//			CWorld::MapSessionByUserId::iterator it;
//			for (it = sWorld->GetUserMap().begin(); it != sWorld->GetUserMap().end(); it++)
//			{
//				it->second->GetRole()->GetCActive()->HandlerFriendBossRank();
//			}
		}
	}
}

void CActiveCtrl::HandlerActiveInfo(CRole *pCRole, uint8_t ignore)
{
	if (ignore || IsUpdate)
	{
		WorldPacket info(CMD_SC_ACTIVE_OPEN_INFO);
		info << (uint16_t) 3;
		info << (uint32_t) FRIEND_BOSS_ACTIVE_ID;
		info << (uint8_t) FriendBoss;

		info << (uint32_t) TURN_NORMAL_TABLE_ACTIVE_ID;
		info << (uint8_t) NormalTurntable;
		info << (uint32_t) vecNormalTurnTable.size();
		for (int i = 0; i < vecNormalTurnTable.size(); i++)
		{
			info << vecNormalTurnTable[i].type;
			info << vecNormalTurnTable[i].para1;
			info << vecNormalTurnTable[i].para2;
		}
		info << (uint32_t) GlobalConfig::NormalTurntableConsume;

		info << (uint32_t) TURN_SUPER_TABLE_ACTIVE_ID;
		info << (uint8_t) SuperTurntable;
		info << (uint32_t) vecSuperTurnTable.size();
		for (int i = 0; i < vecSuperTurnTable.size(); i++)
		{
			info << vecSuperTurnTable[i].type;
			info << vecSuperTurnTable[i].para1;
			info << vecSuperTurnTable[i].para2;
		}
		info << (uint32_t) GlobalConfig::SuperTurntableConsume;
		
		if (pCRole != NULL)
		{
			pCRole->SendPacket(&info);
		}
		else
		{
			sSessCtrl->SendPkgToAll(info);
		}

		WorldPacket info2(CMD_SC_ACTIVE_OPEN_INFO);
		info2 << (uint16_t) 1;
		info2 << (uint32_t) FRIEND_BOSS_RANK_ACTIVE_ID;
		info2 << (uint8_t) FriendBossRank;
//		info2 << (uint16_t) 10;
//		for (int i = 0; i < 10; i++)
//		{
//			if (PreDataForRank(vecFBLastRank[i].userid, 0, info2))
//			{
//				IME_ERROR("pre data for rank fail");
//			}
//		}
		info2 << FBContent;
		info2 << (uint16_t)vecFBRankPrize.size();
		for (int i = 0; i < vecFBRankPrize.size(); i++)
		{
			info2 << vecFBRankSec[i].start;
			info2 << vecFBRankSec[i].end;
			info2 << (uint16_t) 3;
			info2 << vecFBRankPrize[i].obj1.type;
			info2 << vecFBRankPrize[i].obj1.para1;
			info2 << vecFBRankPrize[i].obj1.para2;
			info2 << vecFBRankPrize[i].obj2.type;
			info2 << vecFBRankPrize[i].obj2.para1;
			info2 << vecFBRankPrize[i].obj2.para2;
			info2 << vecFBRankPrize[i].obj3.type;
			info2 << vecFBRankPrize[i].obj3.para1;
			info2 << vecFBRankPrize[i].obj3.para2;
			info2 << vecFBRankPrize[i].tips;
		}
		
		if (pCRole != NULL)
		{
			pCRole->SendPacket(&info2);
		}
		else
		{
			sSessCtrl->SendPkgToAll(info2);
		}

		WorldPacket info3(CMD_SC_ACTIVE_OPEN_INFO);
		info3 << (uint16_t) 3;
		info3 << (uint32_t) CHARGE_SINGLE_ACTIVE_ID;
		info3 << (uint8_t) SingleCharge;
		info3 << (uint16_t)mapSingleChargePrize.size();
		std::map<uint32_t, SChargePrize>::iterator it;
		for (it = mapSingleChargePrize.begin(); it != mapSingleChargePrize.end(); it++)
		{
			info3 << it->first;
			info3 << it->second.obj1.type;
			info3 << it->second.obj1.para1;
			info3 << it->second.obj1.para2;
			info3 << it->second.obj2.type;
			info3 << it->second.obj2.para1;
			info3 << it->second.obj2.para2;
			info3 << it->second.obj3.type;
			info3 << it->second.obj3.para1;
			info3 << it->second.obj3.para2;
			info3 << it->second.obj4.type;
			info3 << it->second.obj4.para1;
			info3 << it->second.obj4.para2;
			info3 << it->second.prize;
			info3 << it->second.number;
			info3 << it->second.desc;
		}

		info3 << (uint32_t) CHARGE_ACCUMULATE_ACTIVE_ID;
		info3 << (uint8_t) AccumulateCharge;
		info3 << (uint16_t)mapAccumulateChargePrize.size();
		for (it = mapAccumulateChargePrize.begin(); it != mapAccumulateChargePrize.end(); it++)
		{
			info3 << it->first;
			info3 << it->second.obj1.type;
			info3 << it->second.obj1.para1;
			info3 << it->second.obj1.para2;
			info3 << it->second.obj2.type;
			info3 << it->second.obj2.para1;
			info3 << it->second.obj2.para2;
			info3 << it->second.obj3.type;
			info3 << it->second.obj3.para1;
			info3 << it->second.obj3.para2;
			info3 << it->second.obj4.type;
			info3 << it->second.obj4.para1;
			info3 << it->second.obj4.para2;
			info3 << it->second.prize;
			info3 << it->second.number;
			info3 << it->second.desc;
		}

		info3 << (uint32_t) CHARGE_CYCLE_ACTIVE_ID;
		info3 << (uint8_t) CycleCharge;
		info3 << (uint16_t)mapCycleChargePrize.size();
		for (it = mapCycleChargePrize.begin(); it != mapCycleChargePrize.end(); it++)
		{
			info3 << it->first;
			info3 << it->second.obj1.type;
			info3 << it->second.obj1.para1;
			info3 << it->second.obj1.para2;
			info3 << it->second.obj2.type;
			info3 << it->second.obj2.para1;
			info3 << it->second.obj2.para2;
			info3 << it->second.obj3.type;
			info3 << it->second.obj3.para1;
			info3 << it->second.obj3.para2;
			info3 << it->second.obj4.type;
			info3 << it->second.obj4.para1;
			info3 << it->second.obj4.para2;
			info3 << it->second.prize;
			info3 << it->second.number;
			info3 << it->second.desc;
		}
		if (pCRole != NULL)
		{
			pCRole->SendPacket(&info3);
		}
		else
		{
			sSessCtrl->SendPkgToAll(info3);
		}

		WorldPacket info4(CMD_SC_ACTIVE_OPEN_INFO);
		info4 << (uint16_t) 1;
		info4 << (uint32_t) LOGIN_EXTRA_ACTIVE_ID;
		info4 << (uint8_t) ExtraLogin;
		info4 << (uint16_t) mapExtraLogin.size();
		std::map<uint32_t, SExtraLogin>::iterator ite = mapExtraLogin.begin();
		for (ite = mapExtraLogin.begin(); ite != mapExtraLogin.end(); ite++)
		{
			info4 << ite->first;
			info4 << ite->second.obj1.type;
			info4 << ite->second.obj1.para1;
			info4 << ite->second.obj1.para2;
			info4 << ite->second.obj2.type;
			info4 << ite->second.obj2.para1;
			info4 << ite->second.obj2.para2;
			info4 << ite->second.obj3.type;
			info4 << ite->second.obj3.para1;
			info4 << ite->second.obj3.para2;
			info4 << ite->second.nextindex;
			info4 << ite->second.desc;
		}
		if (pCRole != NULL)
		{
			pCRole->SendPacket(&info4);
		}
		else
		{
			sSessCtrl->SendPkgToAll(info4);
		}

		WorldPacket info5(CMD_SC_ACTIVE_OPEN_INFO);

		info5 << (uint16_t) (1 + TASK_ACTIVE_MAX);
		info5 << (uint32_t) FUND_ACTIVE_ID;
		info5 << (uint8_t) Fund;
		info5 << (uint16_t) mapFund.size();
		std::map<uint32_t, SFundActive>::iterator itf = mapFund.begin();
		for (itf = mapFund.begin(); itf != mapFund.end(); itf++)
		{
			info5 << itf->first;
			info5 << itf->second.obj1.type;
			info5 << itf->second.obj1.para1;
			info5 << itf->second.obj1.para2;
			info5 << itf->second.obj2.type;
			info5 << itf->second.obj2.para1;
			info5 << itf->second.obj2.para2;
			info5 << itf->second.obj3.type;
			info5 << itf->second.obj3.para1;
			info5 << itf->second.obj3.para2;
			info5 << itf->second.CostCoin;
			info5 << itf->second.CostDollar;
			info5 << itf->second.Day;
			info5 << itf->second.desc;
			info5 << itf->second.ReturnCoin;
			info5 << itf->second.ReturnDollar;
		}

		for (int m = 0; m < TASK_ACTIVE_MAX; m++)
		{
			info5 << (uint32_t) TASK1_ACTIVE_ID + m;

			info5 << (uint8_t) Task[m];

			uint32_t cd = 0;
			if (m_mapT.find(TASK1_ACTIVE_ID + m) == m_mapT.end())
			{
			}
			else
			{
				int a = (int)(m_mapT[TASK1_ACTIVE_ID + m].dwEnd_t - time(NULL));
//				IME_ERROR("end %u, now %u, a %d", m_mapT[TASK1_ACTIVE_ID + m].dwEnd_t, time(NULL), a);
				cd = a < 0 ? 0 : a;
			}

			info5 << (uint16_t) vecTask[m].size();
			for (int i = 0; i < vecTask[m].size(); i++)
			{
				info5 << vecTask[m][i];
				info5 << cd;
			}
//			info5 << TaskFlushTime[m];
		}

		if (pCRole != NULL)
		{
			pCRole->SendPacket(&info5);
		}
		else
		{
			sSessCtrl->SendPkgToAll(info5);
		}
	}
}

void CActiveCtrl::GetTurnResult(CRole *pCRole, uint8_t choose, uint32_t number, std::vector<int> &vectmp)
{
	vectmp.clear();
	vectmp.resize(TURNTABLE_CONTENT_COUNT, 0);
	if (choose == TURNTABLE_NORMAL_CHOOSE)
	{
		std::vector<int> vecpro;
		std::vector<int> vecNoCardPro;
		int ii = 0;
		int GetCardCount = 0;
		for (int i = 0; i < vecNormalTurnTable.size(); i++)
		{
			vecpro.push_back(vecNormalTurnTable[i].pro);
			if (vecNormalTurnTable[i].type == E_OBJ_CARD || vecNormalTurnTable[i].type == E_OBJ_SKILL || vecNormalTurnTable[i].type == E_OBJ_AWAKE_CARD)
			{
				vecNoCardPro.push_back(0);
			}
			else
			{
				vecNoCardPro.push_back(vecNormalTurnTable[i].pro);
				ii++;
			}
		}
		if (ii == 0)
			return;

//		SObjEffect ObjEffect;
		int choose = 0;
		for (int i = 0; i < number; i++)
		{
			if (GetCardCount < TURNTABLE_MAX_CARD_GET)
			{	
				choose = CUtil::RandFactor(vecpro);
			}
			else
			{
				choose = CUtil::RandFactor(vecNoCardPro);
			}

			if (choose >= (int)vecNormalTurnTable.size())
			{
				IME_ERROR("rand choose fail");
				return;	
			}
			else
			{
				CShopEffect::SGiveProduct tmp;
				CShopEffect::GiveGameProduct(pCRole, vecNormalTurnTable[choose].type, vecNormalTurnTable[choose].para1, vecNormalTurnTable[choose].para2, tmp, SOURCE_ACTIVE);
//				ObjEffect.type = tmp.effect;
//				ObjEffect.para1 = tmp.para1;
//				ObjEffect.para2 = tmp.para2;
//				ObjEffect.number = 1;
//				int j = 0;
//				for (j = 0; j < vectmp.size(); j++)
//				{
//					if (vectmp[j].type == tmp.effect )
//					{
//						vectmp[j].number++; 
//						break;
//					}
//				}
//				if (j == vectmp.size())
//				{
//					vectmp.push_back(choose);
//				}

				vectmp[choose]++;
				if (vecNormalTurnTable[i].type == E_OBJ_CARD || vecNormalTurnTable[i].type == E_OBJ_SKILL || vecNormalTurnTable[i].type == E_OBJ_AWAKE_CARD)
				{
					GetCardCount++;
				}
			}
		}
	}
	else
	{
		std::vector<int> vecpro;
		std::vector<int> vecNoCardPro;
		int ii = 0;
		int GetCardCount = 0;
		for (int i = 0; i < vecSuperTurnTable.size(); i++)
		{
			vecpro.push_back(vecSuperTurnTable[i].pro);
			if (vecSuperTurnTable[i].type == E_OBJ_CARD || vecSuperTurnTable[i].type == E_OBJ_SKILL || vecSuperTurnTable[i].type == E_OBJ_AWAKE_CARD)
			{
				vecNoCardPro.push_back(0);
			}
			else
			{
				vecNoCardPro.push_back(vecSuperTurnTable[i].pro);
				ii++;
			}
		}
		if (ii == 0)
			return;

//		SObjEffect ObjEffect;
		int choose = 0;
		for (int i = 0; i < number; i++)
		{
			if (GetCardCount < TURNTABLE_MAX_CARD_GET)
			{	
				choose = CUtil::RandFactor(vecpro);
			}
			else
			{
				choose = CUtil::RandFactor(vecNoCardPro);
			}

			if (choose >= (int)vecSuperTurnTable.size())
			{
				IME_ERROR("rand choose fail");
				return;	
			}
			else
			{
				CShopEffect::SGiveProduct tmp;
				CShopEffect::GiveGameProduct(pCRole, vecSuperTurnTable[choose].type, vecSuperTurnTable[choose].para1, vecSuperTurnTable[choose].para2, tmp, SOURCE_ACTIVE);
//				ObjEffect.type = tmp.effect;
//				ObjEffect.para1 = tmp.para1;
//				ObjEffect.para2 = tmp.para2;
//				ObjEffect.number = 1;
//				int j = 0;
//				for (j = 0; j < vectmp.size(); j++)
//				{
//					if (vectmp[j].type == tmp.effect )
//					{
//						vectmp[j].number++; 
//						break;
//					}
//				}
//				if (j == vectmp.size())
//				{
//					vectmp.push_back(ObjEffect);
//				}
				vectmp[choose]++;
				if (vecSuperTurnTable[i].type == E_OBJ_CARD || vecSuperTurnTable[i].type == E_OBJ_SKILL || vecSuperTurnTable[i].type == E_OBJ_AWAKE_CARD)
				{
					GetCardCount++;
				}
			}
		}
	}
}

//bool CActiveCtrl::IsCardMultiBossActive(uint32_t CardIndex)
//{
//	std::vector<int>::iterator it;
//	it = find(vecCardMulti.begin(), vecCardMulti.end(), CardIndex);
//	if (it == vecCardMulti.end())
//		return false;
//	return true;
//}

//bool CActiveCtrl::IsFriendBossActive(uint32_t friendbossindex)
//{
//	std::vector<int>::iterator it;
//	tmp.push_back(61);
//	tmp.push_back(10061);
//	it = find(vecFriendBoss.begin(), vecFriendBoss.end(), friendbossindex);
//	if (it == vecFriendBoss.end())
//		return false;
//	return true;
//}

uint32_t CActiveCtrl::GetMultiExp(uint32_t taskindex, uint32_t val)
{
	if (!ExpAdd)
		return val;
	if (vecExpMission.size() == 0)
		return val * ExpMulti / 100.0;
	if (find(vecExpMission.begin(), vecExpMission.end(), taskindex) != vecExpMission.end())
	{
		return val * ExpMulti / 100.0;
	}
	else
	{
		return val;
	}
}

uint32_t CActiveCtrl::GetMultiCoin(uint32_t taskindex, uint32_t val)
{
	if (!CoinAdd)
		return val;
	if (vecCoinMission.size() == 0)
		return val * CoinMulti / 100.0;
	if (find(vecCoinMission.begin(), vecCoinMission.end(), taskindex) != vecCoinMission.end())
	{
		return val * CoinMulti / 100.0;
	}
	else
	{
		return val;
	}
}

uint32_t CActiveCtrl::GetMultiObjDrop(uint32_t taskindex, uint32_t val)
{
	if (!ObjDrop)
		return val;
	if (vecObjDropMission.size() == 0)
		return val * ObjDropMulti / 100.0;
	if (find(vecObjDropMission.begin(), vecObjDropMission.end(), taskindex) != vecObjDropMission.end())
	{
		return val * ObjDropMulti / 100.0;
	}
	else
	{
		return val;
	}
}

void CActiveCtrl::UpdateRank(SFBRankRole tmp, int &rank)
{
	if (!FriendBossRank)
	{
		return;
	}
	if (tmp.devote == 0)
	{
		rank = -1;
		return;
	}

	uint8_t fullrank = 1;
	if (vecFBRank.size() < MAX_FRIEND_BOSS_RANK_NUMBER)
	{
		fullrank = 0;
	}

	if (mapPlayerRank.find(tmp.userid) == mapPlayerRank.end())
	{
		// not rank
		SFBRank tmprank = {tmp.devote, tmp.userid};
		vecFBRank.push_back(tmprank);
		mapPlayerRank[tmp.userid] = tmp;
		std::stable_sort(vecFBRank.begin(), vecFBRank.end(), mybig);
		std::pair<std::vector<SFBRank>::iterator, std::vector<SFBRank>::iterator> bound;
		bound = std::equal_range(vecFBRank.begin(), vecFBRank.end(), tmprank, mybig);
		rank = -1;
		for (std::vector<SFBRank>::iterator it = bound.first; it != bound.second; it++)
		{
			if (it->userid == tmp.userid)
			{
				rank = it - vecFBRank.begin() + 1;
				break;
			}
		}
		if (rank == -1)
		{
			IME_ERROR("it is not possible userid %u devote %u", tmp.userid, tmp.devote);
		}

		if (fullrank)
		{
			if (rank > (int)MAX_FRIEND_BOSS_RANK_NUMBER)
			{
				rank = 0;
			}
			uint32_t userid = vecFBRank[MAX_FRIEND_BOSS_RANK_NUMBER].userid;
			mapPlayerRank.erase(userid);
			vecFBRank.erase(vecFBRank.begin() + MAX_FRIEND_BOSS_RANK_NUMBER);
		}
	}
	else
	{
		// already rank
		SFBRank tmprank = {mapPlayerRank[tmp.userid].devote, tmp.userid};

		std::pair<std::vector<SFBRank>::iterator, std::vector<SFBRank>::iterator> bound;
		bound = std::equal_range(vecFBRank.begin(), vecFBRank.end(), tmprank, mybig);
		for (std::vector<SFBRank>::iterator it = bound.first; it != bound.second; it++)
		{
			if (it->userid == tmp.userid)
			{
				it->devote = tmp.devote;
				break;
			}
		}
		mapPlayerRank[tmp.userid] = tmp;

		std::stable_sort(vecFBRank.begin(), vecFBRank.end(), mybig);
		tmprank.devote = tmp.devote;
		bound = std::equal_range(vecFBRank.begin(), vecFBRank.end(), tmprank, mybig);
		rank = -1;
		for (std::vector<SFBRank>::iterator it = bound.first; it != bound.second; it++)
		{
			if (it->userid == tmp.userid)
			{
				rank = it - vecFBRank.begin() + 1;
				break;
			}
		}
		if (rank == -1)
		{
			IME_ERROR("it is not possible userid %u devote %u", tmp.userid, tmp.devote);
		}
	}
}

void CActiveCtrl::RankData(int rank, CRole *pCRole)
{
	int s = 0;
	int e = -1;
	if (rank > 0)
	{
		s = rank - 5;
		e = rank + 5;	
		if (s < 10)
			s = 11;
	}
	else if (rank <= 0)
	{
		s = (int)vecFBRank.size() - 10;
		e = (int)vecFBRank.size();
		if (s < 10)
			s = 11;
	}

	int lastrank = -1;
	for (std::vector<SFBRank>::iterator it = vecFBLastRank.begin(); it != vecFBLastRank.end(); it++)
	{
		if (it->userid == pCRole->GetdwAccountId())
		{
			lastrank = it - vecFBLastRank.begin() + 1;
			break;
		}
	}
	int l_s = 0;
	int l_e = -1;
	if (lastrank > 0)
	{
		l_s = lastrank - 5;
		l_e = lastrank + 5;
		if (l_s < 10)
			l_s = 11;
	}
	else if (lastrank <= 0)
	{
		l_s = (int)vecFBLastRank.size() - 10;
		l_e = (int)vecFBLastRank.size();
		if (l_s < 10)
			l_s = 11;
	}

//	IME_ERROR("now s %u e %u rank %d, l_s %u l_e %u, lastrank %d", s, e, rank, l_s, l_e, lastrank);
	e = std::min(e, (int)vecFBRank.size());
	l_e = std::min(l_e, (int)vecFBLastRank.size());

	WorldPacket info(CMD_SC_FRIEND_BOSS_RANK_INFO);
	int number = e - s + 1 < 0 ? 0 : e - s + 1;
	int top = std::min(10, (int)vecFBRank.size());
	info << (uint16_t) (number + top);
	for (int i = 1; i <= top; i++)
	{
		uint32_t userid = vecFBRank[i - 1].userid;
		info << mapPlayerRank[userid].userid;
		info << mapPlayerRank[userid].level;
		info << mapPlayerRank[userid].name;
//		IME_ERROR("name %s, level %u, userid %u", mapPlayerRank[userid].name.c_str(), mapPlayerRank[userid].level, mapPlayerRank[userid].userid);
		uint16_t number = std::min(3, (int)mapPlayerRank[userid].cardindex.size());
		info << (uint16_t) number;
		for (int j = 0; j < number; j++)
		{
			info << mapPlayerRank[userid].cardindex[j];
		}
		info << mapPlayerRank[userid].masterindex;
		info << mapPlayerRank[userid].devote;
		info << (uint32_t) i;
	}
	for (int i = s; i <= e; i++)
	{
		uint32_t userid = vecFBRank[i - 1].userid;
		info << mapPlayerRank[userid].userid;
		info << mapPlayerRank[userid].level;
		info << mapPlayerRank[userid].name;
		uint16_t number = std::min(3, (int)mapPlayerRank[userid].cardindex.size());
		info << (uint16_t) number;
		for (int j = 0; j < number; j++)
		{
			info << mapPlayerRank[userid].cardindex[j];
		}
		info << mapPlayerRank[userid].masterindex;
		info << mapPlayerRank[userid].devote;
		info << (uint32_t) i;
	}
	info << rank;

	number = l_e - l_s + 1 < 0 ? 0 : l_e - l_s + 1;
	top = std::min(10, (int)vecFBLastRank.size());
	info << (uint16_t) (number + top);
	for (int i = 1; i <= top; i++)
	{
		uint32_t userid = vecFBLastRank[i - 1].userid;
		info << mapPlayerLastRank[userid].userid;
		info << mapPlayerLastRank[userid].level;
		info << mapPlayerLastRank[userid].name;
//		IME_ERROR("rank %u, name %s, level %u, userid %u", i, mapPlayerLastRank[userid].name.c_str(), mapPlayerLastRank[userid].level, mapPlayerLastRank[userid].userid);
		uint16_t number = std::min(3, (int)mapPlayerLastRank[userid].cardindex.size());
		info << (uint16_t) number;
		for (int j = 0; j < number; j++)
		{
			info << mapPlayerLastRank[userid].cardindex[j];
		}
		info << mapPlayerLastRank[userid].masterindex;
		info << mapPlayerLastRank[userid].devote;
		info << (uint32_t) i;
	}
	for (int i = l_s; i <= l_e; i++)
	{
		uint32_t userid = vecFBLastRank[i - 1].userid;
		info << mapPlayerLastRank[userid].userid;
		info << mapPlayerLastRank[userid].level;
		info << mapPlayerLastRank[userid].name;
		uint16_t number = std::min(3, (int)mapPlayerLastRank[userid].cardindex.size());
		info << (uint16_t) number;
		for (int j = 0; j < number; j++)
		{
			info << mapPlayerLastRank[userid].cardindex[j];
		}
		info << mapPlayerLastRank[userid].masterindex;
		info << mapPlayerLastRank[userid].devote;
		info << (uint32_t) i;
	}
	info << lastrank;
	pCRole->SendPacket(&info);
}

int CActiveCtrl::PreDataForRank(uint32_t userid, uint8_t flag, WorldPacket &pkg)
{
	if (flag)
	{
		if (mapPlayerLastRank.find(userid) != mapPlayerLastRank.end())
		{
			pkg << mapPlayerLastRank[userid].userid;
			pkg << mapPlayerLastRank[userid].level;
			pkg << mapPlayerLastRank[userid].name;
			uint16_t number = std::min(3, (int)mapPlayerLastRank[userid].cardindex.size());
			pkg << (uint16_t) number;
			for (int i = 0; i < number; i++)
			{
				pkg << mapPlayerLastRank[userid].cardindex[i];
			}
			pkg << mapPlayerLastRank[userid].masterindex;
			pkg << mapPlayerLastRank[userid].devote;
			return 0;
		}
		return -1;
	}
	else
	{
		if (mapPlayerRank.find(userid) != mapPlayerRank.end())
		{
			pkg << mapPlayerRank[userid].userid;
			pkg << mapPlayerRank[userid].level;
			pkg << mapPlayerRank[userid].name;
			uint16_t number = std::min(3, (int)mapPlayerRank[userid].cardindex.size());
			pkg << (uint16_t) number;
			for (int i = 0; i < number; i++)
			{
				pkg << mapPlayerRank[userid].cardindex[i];
			}
			pkg << mapPlayerRank[userid].masterindex;
			pkg << mapPlayerRank[userid].devote;
			return 0;
		}
		return -1;
	}
}

uint32_t CActiveCtrl::GetFirstRecord()
{
	std::map<uint32_t, SExtraLogin>::iterator it = mapExtraLogin.begin();
	return (it == mapExtraLogin.end() ? 0 : it->first);
}

bool CActiveCtrl::TaskCheck(uint32_t index)
{
	for (int i = 0; i < vecTask.size(); i++)
	{
		if (std::find(vecTask[i].begin(), vecTask[i].end(), index) != vecTask[i].end())
		{
			return true;
		}
	}
	return false;
}

bool CActiveCtrl::ActivePreHandler(uint32_t index)
{
	if (index == FRIEND_BOSS_ACTIVE_ID)
	{
		CActiveCtrl::FriendBossRealStartTime = time(NULL);
	}
	else if (index == FRIEND_BOSS_RANK_ACTIVE_ID)
	{
		if (m_mapT.find(index) == m_mapT.end())
		{
			IME_ERROR("can not find active index %u", index);
			return false;
		}
		return m_mapT[index].pGm->PreHandler();
	}
	return true;
}

bool CActiveCtrl::ActiveAfrHandler(uint32_t index)
{
	if (index == FRIEND_BOSS_RANK_ACTIVE_ID)
	{
		if (m_mapT.find(index) == m_mapT.end())
		{
			IME_ERROR("can not find active index %u", index);
			return false;
		}
		return m_mapT[index].pGm->AfrHandler();
	}
	return true;
}

bool CActiveCtrl::IsPrepare(uint32_t activeindex)
{
	if (m_mapT.find(activeindex) == m_mapT.end())
		return false;
	GmCommandStatus status = m_mapT[activeindex].pGm->GetStatus();
	if (status == E_GM_COMMAND_STATUS_PENDING || status == E_GM_COMMAND_STATUS_RUNNING)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CActiveCtrl::CalculateTime(GmCommand *cmd, SActiveData &act, uint32_t dwTime)
{
	if ( cmd->GetStatus() == E_GM_COMMAND_STATUS_NEW )
	{
		if ( !cmd->Validate() )
		{
			cmd->SetStatus( E_GM_COMMAND_STATUS_ERROR );
			return false;
		}
		if (act.byType == TIME_DELAY_ALL)
		{
			cmd->SetStartTime(act.vecPara[0] + act.dwStart_t);
			cmd->SetEndTime(act.dwEnd_t);
			cmd->SetStatus( E_GM_COMMAND_STATUS_PENDING );
			IsUpdate = 1;
			return true;
		}
		else if (act.byType == TIME_EVERYDAY)
		{
			uint32_t tsec = CUtil::GetDayTime(dwTime);
			int choose = 0;
			for (int i = 1; i < act.vecPara.size(); i += 2)
			{
				uint32_t st = act.vecPara[i];
				uint32_t valts = 0;
				uint32_t valte = 0;
				valts = st / 100 * 3600 + st % 100 * 60;
				st = act.vecPara[i + 1];
				valte = st / 100 * 3600 + st % 100 * 60;

				uint32_t nextvalts = 0;
				uint32_t nextvalte = 0;
				if (i + 2 >= act.vecPara.size())
				{
					st = act.vecPara[1];
					nextvalts = st / 100 * 3600 + st % 100 * 60 + 86400;
					st = act.vecPara[2];
					nextvalte = st / 100 * 3600 + st % 100 * 60 + 86400;
				}
				else
				{
					st = act.vecPara[i + 2];
					nextvalts = st / 100 * 3600 + st % 100 * 60;
					st = act.vecPara[i + 3];
					nextvalte = st / 100 * 3600 + st % 100 * 60;
				}

				if (tsec <= valte)
				{
					cmd->SetStartTime(valts + dwTime - tsec);
					cmd->SetEndTime(valte + dwTime - tsec);
					cmd->SetStatus( E_GM_COMMAND_STATUS_PENDING );
					IsUpdate = 1;
					return true;
				}
				if (tsec > valte && tsec <= nextvalte)
				{
					cmd->SetStartTime(nextvalts + dwTime - tsec);
					cmd->SetEndTime(nextvalte + dwTime - tsec);
					cmd->SetStatus( E_GM_COMMAND_STATUS_PENDING );
					IsUpdate = 1;
					return true;
				}
			}
			cmd->SetStatus( E_GM_COMMAND_STATUS_ERROR );
			cmd->SetErrorMsg("it no possible");
			IME_SYSTEM_ERROR( "ActiveCommand", "it no possible, params = %s",  cmd->GetParams().c_str());
			return false;
		}
		else
		{
			cmd->SetStatus(E_GM_COMMAND_STATUS_ERROR);
			cmd->SetErrorMsg(format_error("unrecognizable time type %u", act.byType));
			IME_SYSTEM_ERROR( "ActiveCommand", "unrecognizable time type %u", act.byType);
			return false;
		}
	}
	return true;
}

void CActiveCtrl::UpdateActive()
{
	uint32 dwTime = time(NULL);
	for ( std::map<uint32_t, SActiveData>::iterator it = m_mapT.begin();
			it != m_mapT.end(); it++ )
	{
		GmCommand* cmd = it->second.pGm;

		do
		{
//			IME_LOG("active %u, para size %u", it->first, it->second.vecPara.size() * 4);
//			IME_LOG("1st active %u, start time %u, end time %u status %u, isupdate %u", it->first, cmd->GetStartTime(), cmd->GetEndTime(), cmd->GetStatus(), CActiveCtrl::IsUpdate);
			if (!CalculateTime(cmd, it->second, dwTime))
			{
				break;
			}
//			IME_LOG("2nd active %u, start time %u, end time %u status %u, isupdate %u", it->first, cmd->GetStartTime(), cmd->GetEndTime(), cmd->GetStatus(), CActiveCtrl::IsUpdate);

			if ( cmd->GetStatus() == E_GM_COMMAND_STATUS_PENDING )
			{
				if ( cmd->GetStartTime() == 0 || cmd->GetStartTime() < dwTime )
				{
					IME_LOG("CActiveCtrl active %u trigger", it->first);
					cmd->SetStartTime( dwTime ); //
					if ( !cmd->Trigger() )
					{
						cmd->SetStatus( E_GM_COMMAND_STATUS_ERROR );
						break;
					}
					cmd->SetStatus( E_GM_COMMAND_STATUS_RUNNING );
					CActiveCtrl::IsUpdate = 1;
				}
			}

			if ( cmd->GetStatus() == E_GM_COMMAND_STATUS_RUNNING )
			{
				cmd->Tick();
				if ( cmd->GetEndTime() == 0 || cmd->GetEndTime() < dwTime )
				{
					IME_LOG("CActiveCtrl active %u complete", it->first);
					if ( !cmd->Complete() )
					{
						cmd->SetStatus( E_GM_COMMAND_STATUS_ERROR );
						break;
					}

					if (it->second.byType == TIME_DELAY_ALL)
					{
						cmd->SetStatus( E_GM_COMMAND_STATUS_COMPLETE );
					}
					else
					{
						cmd->SetStatus(E_GM_COMMAND_STATUS_NEW);
					}
					CActiveCtrl::IsUpdate = 1;
				}
			}

//			IME_LOG("2active %u, para size %u", it->first, it->second.vecPara.size() * 4);
			if (!CalculateTime(cmd, it->second, dwTime))
			{
				break;
			}
//			IME_LOG("3active %u, para size %u", it->first, it->second.vecPara.size() * 4);
//			IME_LOG("3rd active %u, start time %u, end time %u status %u, isupdate %u", it->first, cmd->GetStartTime(), cmd->GetEndTime(), cmd->GetStatus(), CActiveCtrl::IsUpdate);

		} while(0);
	}
}

void CActiveCtrl::AddActive(uint32_t activeindex, std::string strParams, std::string &errormsg, uint32_t startt, uint32_t endt)
{
	if (m_mapT.find(activeindex) != m_mapT.end())
	{
		errormsg = format_error("this active %u have already register", activeindex);
		return;
	}

	std::string strTimeParam;
	std::string strTitle;
	std::string strContent;
	std::vector<std::string> vParam;
	CUtil::StrSplit( strParams, "|", vParam );
	if (vParam.size() < 3)
	{
		errormsg = "no params";
		return;
	}
	strTimeParam = vParam[0];
	strTitle = vParam[1];
	strContent = vParam[2];
	vParam.erase(vParam.begin(), vParam.begin() + 3);
	CUtil::StrMerge(strParams, "|", vParam);

	uint32_t paramlength = 1000;
	if (activeindex == FRIEND_BOSS_RANK_ACTIVE_ID || activeindex == CHARGE_SINGLE_ACTIVE_ID || activeindex == CHARGE_ACCUMULATE_ACTIVE_ID || activeindex == CHARGE_CYCLE_ACTIVE_ID || activeindex == LOGIN_EXTRA_ACTIVE_ID)
	{
		paramlength = 3000;
	}	
	if (strParams.size() > paramlength)
	{
		errormsg = format_error("params %u (small than %u byte) is so big", strParams.size(), paramlength);
		return;
	}

	if (strTitle.size() > 50 || strContent.size() > 350)
	{
		errormsg = "title (small 50 byte) and content (big 350 byte) too long";
		return;
	}

	std::vector<uint32_t> vTParam;
	CUtil::StrSplit( strTimeParam, ",", vTParam);
	if (vTParam.size() < 1 || vTParam.size() >= 10)
	{
		errormsg = "time params error too small or too big 1 - 10";
		return;
	}
	uint16_t timetype = vTParam[0];		
	if (timetype == TIME_DELAY_ALL)
	{
		if (vTParam.size() != 2)
		{
			errormsg = "delay time param error";
			return;
		}

	}
	else if (timetype == TIME_EVERYDAY)
	{
		if (vTParam.size() < 2)
		{
			errormsg = "time every param error";
			return;
		}
		uint32_t paramsnumber = vTParam[1];
		if (paramsnumber == 0)
		{
			errormsg = "time every param error";
			return;
		}
		if (vTParam.size() >= MAX_ACTIVE_TIME_COUNT || paramsnumber + 2 != vTParam.size())
		{
			errormsg = "time every param error";
			return;
		}
		if (vTParam.size() % 2 != 0)
		{
			errormsg = "time every param error";
			return;
		}
	}
	else
	{
		errormsg = format_error("time type can not supported type %u", timetype);
		return;
	}
	
	vTParam.erase(vTParam.begin());

	GmCommand* ret = CreateActive(activeindex, strParams, E_GM_COMMAND_STATUS_NEW, 0, 0, "");

	if (ret)
	{
		SActiveData tmp;
		tmp.byType = timetype;
		tmp.vecPara = vTParam;
		tmp.dwStart_t = startt;
		tmp.dwEnd_t = endt;
		tmp.pGm = ret;
		tmp.strTitle = strTitle;
		tmp.strContent = strContent;
		m_mapT[activeindex] = tmp;

		if (!ActivePreHandler(activeindex))
		{
			errormsg = format_error("this active %u Prehandler exec fail", activeindex);
			delete m_mapT[activeindex].pGm;
			m_mapT.erase(activeindex);
			return;
		}

		return;
	}
	else
	{
		errormsg = format_error("create this active %u fail", activeindex);
		return;
	}
}

GmCommand* CActiveCtrl::CreateActive(uint32_t activeindex, std::string strParams, uint8_t status, uint32_t start_t, uint32_t end_t, std::string errormsg)
{
	GmCommand *ret = NULL;
	if (activeindex == FRIEND_BOSS_ACTIVE_ID)
		ret = new FriendBossActive(); 
	else if (activeindex == TURN_NORMAL_TABLE_ACTIVE_ID)
		ret = new TurntableNormalActive();
	else if (activeindex == TURN_SUPER_TABLE_ACTIVE_ID)
		ret = new TurntableSuperActive();
	else if (activeindex == EXP_ADD_ACTIVE_ID)
		ret = new ExpAddActive();
	else if (activeindex == COIN_ADD_ACTIVE_ID)
		ret = new CoinAddActive();
	else if (activeindex == OBJ_DROP_ACTIVE_ID)
		ret = new ObjDropActive();
	else if (activeindex == FRIEND_BOSS_RANK_ACTIVE_ID)
		ret = new BossRankActive();
	else if (activeindex == CHARGE_SINGLE_ACTIVE_ID || activeindex == CHARGE_ACCUMULATE_ACTIVE_ID || activeindex == CHARGE_CYCLE_ACTIVE_ID)
		ret = new CommonChargeActive();
	else if (activeindex == LOGIN_EXTRA_ACTIVE_ID)
		ret = new ExtraLoginActive();
	else if (activeindex == TASK1_ACTIVE_ID || activeindex == TASK2_ACTIVE_ID || activeindex == TASK3_ACTIVE_ID || activeindex == TASK4_ACTIVE_ID || activeindex == TASK5_ACTIVE_ID || activeindex == TASK6_ACTIVE_ID || activeindex == TASK7_ACTIVE_ID)
		ret = new TaskActive();
	else if (activeindex == FUND_ACTIVE_ID)
		ret = new FundActive();

	if ( ret )
	{
		ret->SetAutoId(0);
		ret->SetOpr("");
		ret->SetTargetType(E_GM_COMMAND_TARGET_TYPE_SERVER);
		ret->SetTargetId(0);
		ret->SetStartTime(start_t);
		ret->SetEndTime(end_t);
		ret->SetBackup("");
		ret->SetStatus((GmCommandStatus)status);
		ret->SetParams(strParams);
		ret->SetErrorMsg(errormsg);

		if ( !(ret->Validate()) ) // read param again
		{
			IME_ERROR("active Validate fail activeid %u", activeindex);
			return NULL;
		}
		return ret;
	}
	else
	{
		IME_ERROR("new active fail activeid %u", activeindex);
		return NULL;
	}
}

void CActiveCtrl::DelActive(uint32_t activeindex, std::string &errormsg)
{
	if (m_mapT.find(activeindex) == m_mapT.end())
	{
		errormsg = format_error("can not find activeindex %u", activeindex);
		return;
	}

	ActiveAfrHandler(activeindex);

	if (m_mapT[activeindex].pGm->GetStatus() == E_GM_COMMAND_STATUS_RUNNING)
	{
		if (!m_mapT[activeindex].pGm->Complete())
		{
			errormsg = format_error("complete fail activeindex %u", activeindex);
		}
	}
	else if (m_mapT[activeindex].pGm->GetStatus() == E_GM_COMMAND_STATUS_ERROR)
	{
		errormsg = m_mapT[activeindex].pGm->GetErrorMsg();
	}
	
	delete m_mapT[activeindex].pGm;
	m_mapT.erase(activeindex);
	return;
}

bool CActiveCtrl::InitActive()
{
	std::string tmp;
	if (!CDBCtrl::GetGlobalActiveData(COMMON_ACTIVE_ID, tmp))
	{
		if (!CDBCtrl::InsertGlobalActiveData(COMMON_ACTIVE_ID, tmp))
		{
			IME_ERROR("insert global active data");
			return false;
		}
		return true;
	}	
	else
	{
		ByteBuffer by;
		by.append(tmp.data(), tmp.size());	
		uint32_t number;
		uint32_t activeindex;
		uint8_t timetype;
		uint32_t numberval;
		uint32_t val;
		uint32_t start_t;
		uint32_t end_t;
		std::vector<uint32_t> vecPara;
		uint8_t status;
		uint32_t gmstart;
	   	uint32_t gmend;	
		std::string title;
	   	std::string content;	
		std::string activeparam;
		std::string errormsg;
		by >> number;
		for (int i = 0; i < number; i++)
		{
			vecPara.clear();
			by >> activeindex;
			by >> timetype;
			by >> numberval;
			for (int j = 0; j < numberval; j++)
			{
				by >> val;
				vecPara.push_back(val);
			}
			by >> start_t;
			by >> end_t;
			by >> status;
			by >> activeparam;
			by >> gmstart;
			by >> gmend;
			by >> errormsg;
			by >> title;
			by >> content;

			IME_DEBUG("CActiveCtrl init active %u status %u", activeindex, status);
			GmCommand* ret = CreateActive(activeindex, activeparam, status, gmstart, gmend, errormsg);

			if (ret)
			{
				SActiveData tmp;
				tmp.byType = timetype;
				tmp.vecPara = vecPara;
				tmp.dwStart_t = start_t;
				tmp.dwEnd_t = end_t;
				tmp.pGm = ret;
				tmp.strTitle = title;
				tmp.strContent = content;
				m_mapT[activeindex] = tmp;
			}
			else
			{
				IME_ERROR("create this active %u fail", activeindex);
				continue;
			}
//			IME_LOG("activeindex %u, para size %u", activeindex, vecPara.size() * 4);
		}
		return true;
	}
}

extern void hexdump(const char *p, int len);

void CActiveCtrl::SaveActive() {
	ByteBuffer by;
	by << (uint32_t) m_mapT.size();
	std::map<uint32_t, SActiveData>::iterator it;
	for (it = m_mapT.begin(); it != m_mapT.end(); it++)
	{
		by << it->first;
		by << it->second.byType;
		by << (uint32_t) it->second.vecPara.size();
		for (int i = 0; i < it->second.vecPara.size(); i++)
		{
			by << it->second.vecPara[i];
		}
		by << it->second.dwStart_t;
		by << it->second.dwEnd_t;
		by << (uint8_t)it->second.pGm->GetStatus();
		by << it->second.pGm->GetParams();
		by << it->second.pGm->GetStartTime();
		by << it->second.pGm->GetEndTime();
		by << it->second.pGm->GetErrorMsg();
		by << it->second.strTitle;
		by << it->second.strContent;
		hexdump((const char *)by.contents(), by.size());
//		IME_ERROR("active id %u, size %u, params size %u", it->first, by.size(), it->second.pGm->GetParams().size());
//		IME_ERROR("params %s %u, errormsg %s %u, strTitle %s %u, strContent %s %u", it->second.pGm->GetParams().c_str(), it->second.pGm->GetParams().size(), it->second.pGm->GetErrorMsg().c_str(), it->second.pGm->GetErrorMsg().size(),  it->second.strTitle.c_str(), it->second.strTitle.size(), it->second.strContent.c_str(), it->second.strContent.size());
//		IME_ERROR("vecpara %u, params %u, errormsg %u, strTitle %u, strContent %u", it->second.vecPara.size() * 4, it->second.pGm->GetParams().size(), it->second.pGm->GetErrorMsg().size(), it->second.strTitle.size(), it->second.strContent.size());
	}

	std::string str;
	str.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveGlobalActiveData(str, COMMON_ACTIVE_ID);
}

////////////////////////////
//// FriendBossActive [state]
bool FriendBossActive::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 2);

	m_BossIndex = GET_CMD_PARAM_INT(0);
	m_BossPro = GET_CMD_PARAM_INT(1);
//	m_ItemId = GET_CMD_PARAM_INT(2);
//	m_ItemProBase = GET_CMD_PARAM_INT(3);
//	m_CardId = GET_CMD_PARAM_INT(4);
//	m_CardProBase = GET_CMD_PARAM_INT(5);
//	m_ProIncrease = GET_CMD_PARAM_INT(6);

	STC_CONF_FRIEND_BOSS *pConf = CConfFriendBoss::Find(m_BossIndex);
	if (pConf == NULL)
	{
		SetErrorMsg(format_error("friend boss index %u can not find in boss csv", m_BossIndex));
		return false;
	}
	if (pConf->byType != FRIEND_BOSS_ACTIVE)
	{
		SetErrorMsg(format_error("boss index %u is not active boss", m_BossIndex));
		return false;
	}

//	STC_CONF_GOODS *pConfGoods = CConfGoods::Find(m_ItemId);
//	if (pConfGoods == NULL)
//	{
//		SetErrorMsg(format_error("item index %u can not find in item csv", m_ItemId));
//		return false;
//	}
//	STC_CONF_CARD_BASE *pConfCard = CConfCard::Find(m_CardId);
//	if (pConfCard == NULL)
//	{
//		SetErrorMsg(format_error("card index %u can not find in card csv", m_CardId));
//		return false;
//	}

	return true;
}

bool FriendBossActive::Trigger()
{
	if (CActiveCtrl::FriendBoss)
	{
		SetErrorMsg("friend boss active already start");
		return false;
	}
	CActiveCtrl::FriendBoss = 1;	
//	CActiveCtrl::FriendBossRealStartTime = time(NULL);
	CActiveCtrl::IsUpdate = 1;
	CActiveCtrl::FriendBossIndex = m_BossIndex;
	CActiveCtrl::FriendBossPro = m_BossPro;
//	CActiveCtrl::FriendBossItemId = m_ItemId;
//	CActiveCtrl::FriendBossItemProBase = m_ItemProBase;
//	CActiveCtrl::FriendBossCardId = m_CardId;
//	CActiveCtrl::FriendBossCardProBase = m_CardProBase;
//	CActiveCtrl::FriendBossProIncrease = m_ProIncrease;
	return true;
}

bool FriendBossActive::Complete()
{
	CActiveCtrl::SetFriendBoss(0);
	CActiveCtrl::FriendBossIndex = 0;
	return true;
}

///////////////////////////////////
// TurntableNormalActive [gm_auto_id]

bool TurntableNormalActive::Validate()
{
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, TURNTABLE_CONTENT_COUNT * 4 + 1);

	STruntable tmp;
	m_vecNormal.clear();

	int alliscard = 1;
	for (int i = 0; i < TURNTABLE_CONTENT_COUNT; i++)
	{
		tmp.type = GET_CMD_PARAM_INT(1 + i * 4);
		tmp.para1 = GET_CMD_PARAM_INT(1 + i * 4 + 1);
		tmp.para2 = GET_CMD_PARAM_INT(1 + i * 4 + 2);
		tmp.pro = GET_CMD_PARAM_INT(1 + i * 4 + 3);
		if (tmp.type >= E_OBJ_MAX)
		{
			SetErrorMsg( format_error("Send gift type is not supported position %u", i * 4));
			return false;
		}
		if (tmp.type != E_OBJ_CARD && tmp.type != E_OBJ_SKILL && tmp.type != E_OBJ_AWAKE_CARD)
		{
			alliscard = 0;
		}
		m_vecNormal.push_back(tmp);
	}
	if (alliscard)
	{
		SetErrorMsg("turn all type is card");
		return false;
	}
	return true;
}

bool TurntableNormalActive::Trigger()
{
	if (CActiveCtrl::NormalTurntable)
	{
		SetErrorMsg("Turntable normal already start");
		return false;
	}
	CActiveCtrl::NormalTurntable = 1;
	CActiveCtrl::vecNormalTurnTable = m_vecNormal;
	return true;
}

bool TurntableNormalActive::Complete()
{
	CActiveCtrl::NormalTurntable = 0;
	return true;
}

///////////////////////////////////
// TurntableSuperActive [gm_auto_id]

bool TurntableSuperActive::Validate()
{
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, TURNTABLE_CONTENT_COUNT * 4 + 1);

	STruntable tmp;
	m_vecSuper.clear();

	int alliscard = 1;
	for (int i = 0; i < TURNTABLE_CONTENT_COUNT; i++)
	{
		tmp.type = GET_CMD_PARAM_INT(1 + i * 4);
		tmp.para1 = GET_CMD_PARAM_INT(1 + i * 4 + 1);
		tmp.para2 = GET_CMD_PARAM_INT(1 + i * 4 + 2);
		tmp.pro = GET_CMD_PARAM_INT(1 + i * 4 + 3);
		if (tmp.type >= E_OBJ_MAX)
		{
			SetErrorMsg( format_error("Send gift type is not supported position %u", i * 4));
			return false;
		}
		if (tmp.type != E_OBJ_CARD && tmp.type != E_OBJ_SKILL && tmp.type != E_OBJ_AWAKE_CARD)
		{
			alliscard = 0;
		}
		m_vecSuper.push_back(tmp);
	}
	if (alliscard)
	{
		SetErrorMsg("turn all type is card");
		return false;
	}

	return true;
}

bool TurntableSuperActive::Trigger()
{
	if (CActiveCtrl::SuperTurntable)
	{
		SetErrorMsg("Turntable super already start");
		return false;
	}
	CActiveCtrl::SuperTurntable = 1;
	CActiveCtrl::vecSuperTurnTable = m_vecSuper;
	return true;
}

bool TurntableSuperActive::Complete()
{
	CActiveCtrl::SuperTurntable = 0;
	return true;
}

///////////////////////////////////
bool ExpAddActive::Validate()
{
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 3);
	m_vecMission.clear();

	m_byMulti = GET_CMD_PARAM_INT(0);
	uint8_t type = GET_CMD_PARAM_INT(1);
	std::string mission = GET_CMD_PARAM_STRING(2);
	if (type == 1)
	{
		CUtil::StrSplit(mission, ",", m_vecMission);	
	}
	return true;
}

bool ExpAddActive::Trigger()
{
	if (CActiveCtrl::ExpAdd)
	{
		SetErrorMsg("Exp add already start");
		return false;
	}
	CActiveCtrl::ExpAdd = 1;
	CActiveCtrl::ExpMulti = m_byMulti;
	CActiveCtrl::vecExpMission = m_vecMission;
	return true;
}

bool ExpAddActive::Complete()
{
	CActiveCtrl::ExpAdd = 0;
	return true;
}

///////////////////////////////////
bool CoinAddActive::Validate()
{
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 3);
	m_vecMission.clear();

	m_byMulti = GET_CMD_PARAM_INT(0);
	uint8_t type = GET_CMD_PARAM_INT(1);
	std::string mission = GET_CMD_PARAM_STRING(2);
	if (type == 1)
	{
		CUtil::StrSplit(mission, ",", m_vecMission);	
	}
	return true;
}

bool CoinAddActive::Trigger()
{
	if (CActiveCtrl::CoinAdd)
	{
		SetErrorMsg("Exp add already start");
		return false;
	}
	CActiveCtrl::CoinAdd = 1;
	CActiveCtrl::CoinMulti = m_byMulti;
	CActiveCtrl::vecCoinMission = m_vecMission;
	return true;
}

bool CoinAddActive::Complete()
{
	CActiveCtrl::CoinAdd = 0;
	return true;
}

///////////////////////////////////
bool ObjDropActive::Validate()
{
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 3);
	m_vecMission.clear();

	m_byMulti = GET_CMD_PARAM_INT(0);
	uint8_t type = GET_CMD_PARAM_INT(1);
	std::string mission = GET_CMD_PARAM_STRING(2);
	if (type == 1)
	{
		CUtil::StrSplit(mission, ",", m_vecMission);	
	}
	return true;
}

bool ObjDropActive::Trigger()
{
	if (CActiveCtrl::ObjDrop)
	{
		SetErrorMsg("Exp add already start");
		return false;
	}
	CActiveCtrl::ObjDrop = 1;
	CActiveCtrl::ObjDropMulti = m_byMulti;
	CActiveCtrl::vecObjDropMission = m_vecMission;
	return true;
}

bool ObjDropActive::Complete()
{
	CActiveCtrl::ObjDrop = 0;
	return true;
}

///////////////////////////////////
bool BossRankActive::Validate()
{
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 4);

	content = GET_CMD_PARAM_STRING(0);
	prize = GET_CMD_PARAM_STRING(1);
	mailtitle = GET_CMD_PARAM_STRING(2);
	mailcontent = GET_CMD_PARAM_STRING(3);
	return true;
}

bool BossRankActive::Trigger()
{
	if (CActiveCtrl::FriendBossRank)
	{
		SetErrorMsg("Exp add already start");
		return false;
	}
	CActiveCtrl::FriendBossRank = 1;
	return true;
}

bool BossRankActive::Complete()
{
	CActiveCtrl::FriendBossRank = 0;
	return true;
}

bool BossRankActive::PreHandler()
{
	std::vector<SFBRankGift> m;
	std::vector<SRank> n;
	SFBRankGift a;
	SRank b;

	std::map<uint32_t, SFBRankRole> mapA;
	std::vector<SFBRank> vecB;

	CCsvReader csv;
	if (!csv.Init(prize))
	{
		IME_ERROR("boss rank prehandler fail");
		return false;
	}
	IME_DEBUG("prize %s", prize.c_str());
	for (int i = 0; i < csv.GetLineCount(); i++)
	{
		int j = 0;
		READER_CSV_INT(b.start);
		READER_CSV_INT(b.end);
		READER_CSV_INT(a.obj1.type);
		READER_CSV_INT(a.obj1.para1);
		READER_CSV_INT(a.obj1.para2);
		READER_CSV_INT(a.obj2.type);
		READER_CSV_INT(a.obj2.para1);
		READER_CSV_INT(a.obj2.para2);
		READER_CSV_INT(a.obj3.type);
		READER_CSV_INT(a.obj3.para1);
		READER_CSV_INT(a.obj3.para2);
		READER_CSV_STR(a.tips);
		IS_READER_CSV_ERROR;

		if (b.start < 1 || b.end < 1 || b.end < b.start)
		{
			IME_ERROR("rank is unvalid start %u, end %u", b.start, b.end);
			return false;
		}

		m.push_back(a);
		n.push_back(b);
	}

	CConfRobRobot::MapData::reverse_iterator it;
	CConfRobRobot::MapData *p = CConfRobRobot::GetAllData();
	int robotnumber = 1;
	for (it = p->rbegin(); it != p->rend(); it++)
	{
		if (robotnumber > MAX_FRIEND_BOSS_RANK_NUMBER)
			break;
		CRobot *pCRobot = new (std::nothrow)CRobot();
		if (pCRobot == NULL)
		{
			IME_ERROR("new a obj fail %u");
			return false;
		}
		if (pCRobot->Init(it->first))
		{
			IME_ERROR("init robot fail %u", it->first);
			return false;
		}
		SFBRankRole tmp;
		if (pCRobot->PreDataForRank(tmp))
		{
			IME_ERROR("pre data for rank fail robot id %u", it->first);
			return false;
		}
		
		mapA[it->first] = tmp;
		SFBRank tmp2;
		tmp2.devote = tmp.devote;
		tmp2.userid = tmp.userid;
		vecB.push_back(tmp2);
	}	
	std::sort(vecB.begin(), vecB.end(), mybig);

	CActiveCtrl::vecFBRankPrize = m;
	CActiveCtrl::vecFBRankSec = n;
	CActiveCtrl::FBContent = content;
	CActiveCtrl::mapPlayerRank = mapA;
	CActiveCtrl::vecFBRank = vecB;
	CActiveCtrl::FBRankFlushTime = time(NULL);
	CActiveCtrl::IsUpdate = 1;

//	CWorld::MapSessionByUserId::iterator it;
//	for (it = sWorld->GetUserMap().begin(); it != sWorld->GetUserMap().end(); it++)
//	{
//		it->second->GetRole()->GetCActive()->HandlerFriendBossRank();
//	}

	return true;
}

bool BossRankActive::AfrHandler()
{
	for (int i = 0; i < CActiveCtrl::vecFBRankSec.size(); i++)
	{
		for (int j = CActiveCtrl::vecFBRankSec[i].start; j <= CActiveCtrl::vecFBRankSec[i].end; j++)
		{
			IME_ERROR("i %u j %u", i, j);
			if (j > (int)CActiveCtrl::vecFBRank.size())
				continue;
			if (IS_ROBORT(CActiveCtrl::vecFBRank[j - 1].userid))
				continue;
//			if (j > MAX_FRIEND_BOSS_RANK_NUMBER)
//				continue;
			std::vector<uint16_t> vectype;
			std::vector<uint32_t> vecpara1;
			std::vector<uint32_t> vecpara2;
			if (CActiveCtrl::vecFBRankPrize[i].obj1.type != 0)
			{
				vectype.push_back(CActiveCtrl::vecFBRankPrize[i].obj1.type);
				vecpara1.push_back(CActiveCtrl::vecFBRankPrize[i].obj1.para1);
				vecpara2.push_back(CActiveCtrl::vecFBRankPrize[i].obj1.para2);
			}
			if (CActiveCtrl::vecFBRankPrize[i].obj2.type != 0)
			{
				vectype.push_back(CActiveCtrl::vecFBRankPrize[i].obj2.type);
				vecpara1.push_back(CActiveCtrl::vecFBRankPrize[i].obj2.para1);
				vecpara2.push_back(CActiveCtrl::vecFBRankPrize[i].obj2.para2);
			}
			if (CActiveCtrl::vecFBRankPrize[i].obj3.type != 0)
			{
				vectype.push_back(CActiveCtrl::vecFBRankPrize[i].obj3.type);
				vecpara1.push_back(CActiveCtrl::vecFBRankPrize[i].obj3.para1);
				vecpara2.push_back(CActiveCtrl::vecFBRankPrize[i].obj3.para2);
			}
			for (int k = 0; k < vectype.size(); k++)
			{
				uint16_t type = vectype[k];
				uint32_t para1 = vecpara1[k];
				uint32_t para2 = vecpara2[k];

				CMail::SBaseAttr tmpAttr = {0};

				uint32_t mail_id = 0;
				CDBCtrl::stuMailDetail tmp = {0};
				CMail::AddEmailSysDiy(CActiveCtrl::vecFBRank[j - 1].userid, mail_id, tmp, type, para1, para2, mailcontent, tmpAttr, mailtitle);   

				CRole *pPeerCRole = sWorld->FindUserRole(CActiveCtrl::vecFBRank[j - 1].userid);
				if (pPeerCRole != NULL)
				{
					CMail::EmailUpdate(pPeerCRole, CMail::ADD, &tmp, 0);
				}
			}
		}
	}

	CActiveCtrl::vecFBRankPrize.clear();
	CActiveCtrl::vecFBRankSec.clear();
	CActiveCtrl::mapPlayerLastRank = CActiveCtrl::mapPlayerRank;
	CActiveCtrl::vecFBLastRank = CActiveCtrl::vecFBRank;
	CActiveCtrl::mapPlayerRank.clear();
	CActiveCtrl::vecFBRank.clear();
	CActiveCtrl::FBContent = "";
	CActiveCtrl::FBRankFlushTime = time(NULL);
	CActiveCtrl::IsUpdate = 1;

//	CWorld::MapSessionByUserId::iterator it;
//	for (it = sWorld->GetUserMap().begin(); it != sWorld->GetUserMap().end(); it++)
//	{
//		it->second->GetRole()->GetCActive()->HandlerFriendBossRank();
//	}

	return true;
}

///////////////////////////////////
bool CommonChargeActive::Validate()
{
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 2);

	std::string content = GET_CMD_PARAM_STRING(1);
	activeindex = GET_CMD_PARAM_INT(0);
	if (activeindex != CHARGE_SINGLE_ACTIVE_ID && activeindex != CHARGE_ACCUMULATE_ACTIVE_ID && activeindex != CHARGE_CYCLE_ACTIVE_ID)
	{
		IME_ERROR("activeindex is unvalid %u", activeindex);
		return false;
	}
	map.clear();

	SChargePrize a;
	uint32_t key;
	CCsvReader csv;
	if (!csv.Init(content))
	{
		IME_ERROR("single charge fail");
		return false;
	}
	for (int i = 0; i < csv.GetLineCount(); i++)
	{
		int j = 0;
		READER_CSV_INT(key);
		READER_CSV_INT(a.obj1.type);
		READER_CSV_INT(a.obj1.para1);
		READER_CSV_INT(a.obj1.para2);
		READER_CSV_INT(a.obj2.type);
		READER_CSV_INT(a.obj2.para1);
		READER_CSV_INT(a.obj2.para2);
		READER_CSV_INT(a.obj3.type);
		READER_CSV_INT(a.obj3.para1);
		READER_CSV_INT(a.obj3.para2);
		READER_CSV_INT(a.obj4.type);
		READER_CSV_INT(a.obj4.para1);
		READER_CSV_INT(a.obj4.para2);
		READER_CSV_INT(a.prize);
		READER_CSV_INT(a.number);
		READER_CSV_STR(a.desc);
		IS_READER_CSV_ERROR;
		map[key] = a;
	}
	return true;
}

bool CommonChargeActive::Trigger()
{
	if (activeindex == CHARGE_SINGLE_ACTIVE_ID)
	{
		if (CActiveCtrl::SingleCharge)
		{
			IME_ERROR("singlecharge already start");
			return false;
		}
		CActiveCtrl::SingleCharge = 1;
		CActiveCtrl::mapSingleChargePrize = map;
	}
	else if (activeindex == CHARGE_ACCUMULATE_ACTIVE_ID)
	{
		if (CActiveCtrl::AccumulateCharge)
		{
			IME_ERROR("accumulatecharge already start");
			return false;
		}
		CActiveCtrl::AccumulateCharge = 1;
		CActiveCtrl::mapAccumulateChargePrize = map;
	}
	else
	{
		if (CActiveCtrl::CycleCharge)
		{
			IME_ERROR("cyclecharge already start");
			return false;
		}
		CActiveCtrl::CycleCharge = 1;
		CActiveCtrl::mapCycleChargePrize = map;
	}
	return true;
}

bool CommonChargeActive::Complete()
{
	if (activeindex == CHARGE_SINGLE_ACTIVE_ID)
	{
		CActiveCtrl::SingleCharge = 0;
		CActiveCtrl::SingleChargeFlushTime = time(NULL);
	}
	else if (activeindex == CHARGE_ACCUMULATE_ACTIVE_ID)
	{
		CActiveCtrl::AccumulateCharge = 0;
		CActiveCtrl::AccumulateChargeFlushTime = time(NULL);
	}
	else
	{
		CActiveCtrl::CycleCharge = 0;
		CActiveCtrl::CycleChargeFlushTime = time(NULL);
	}
	return true;
}

///////////////////////////////////
bool ExtraLoginActive::Validate()
{
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1);

	std::string content = GET_CMD_PARAM_STRING(0);
	map.clear();

	SExtraLogin a;
	uint32_t key;
	CCsvReader csv;
	if (!csv.Init(content))
	{
		IME_ERROR("extra login fail");
		return false;
	}
	for (int i = 0; i < csv.GetLineCount(); i++)
	{
		int j = 0;
		READER_CSV_INT(key);
		READER_CSV_INT(a.obj1.type);
		READER_CSV_INT(a.obj1.para1);
		READER_CSV_INT(a.obj1.para2);
		READER_CSV_INT(a.obj2.type);
		READER_CSV_INT(a.obj2.para1);
		READER_CSV_INT(a.obj2.para2);
		READER_CSV_INT(a.obj3.type);
		READER_CSV_INT(a.obj3.para1);
		READER_CSV_INT(a.obj3.para2);
		READER_CSV_INT(a.nextindex);
		READER_CSV_STR(a.desc);
		IS_READER_CSV_ERROR;
		map[key] = a;
	}
	return true;
}

bool ExtraLoginActive::Trigger()
{
	CActiveCtrl::ExtraLogin = 1;
	CActiveCtrl::mapExtraLogin = map;
	return true;
}

bool ExtraLoginActive::Complete()
{
	CActiveCtrl::ExtraLogin = 0;
	CActiveCtrl::mapExtraLogin.clear();
	CActiveCtrl::ExtraLoginFlushTime = time(NULL);
	return true;
}

///////////////////////////////////
bool FundActive::Validate()
{
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 3);

	std::string content = GET_CMD_PARAM_STRING(0);
	mailtitle = GET_CMD_PARAM_STRING(1);
	mailcontent = GET_CMD_PARAM_STRING(2);
//	IME_ERROR("param %s mail title %s, mail content %s", m_strParams.c_str(), mailtitle.c_str(), mailcontent.c_str());
	map.clear();

	SFundActive a;
	uint32_t key;
	CCsvReader csv;
	if (!csv.Init(content))
	{
		IME_ERROR("single charge fail");
		return false;
	}
	for (int i = 0; i < csv.GetLineCount(); i++)
	{
		int j = 0;
		READER_CSV_INT(key);
		READER_CSV_INT(a.obj1.type);
		READER_CSV_INT(a.obj1.para1);
		READER_CSV_INT(a.obj1.para2);
		READER_CSV_INT(a.obj2.type);
		READER_CSV_INT(a.obj2.para1);
		READER_CSV_INT(a.obj2.para2);
		READER_CSV_INT(a.obj3.type);
		READER_CSV_INT(a.obj3.para1);
		READER_CSV_INT(a.obj3.para2);
		READER_CSV_INT(a.CostCoin);
		READER_CSV_INT(a.CostDollar);
		READER_CSV_INT(a.Day);
		READER_CSV_STR(a.desc);
		READER_CSV_INT(a.ReturnCoin);
		READER_CSV_INT(a.ReturnDollar);
		IS_READER_CSV_ERROR;
		map[key] = a;
		if (a.Day <= 1)
		{
			IME_ERROR("fund day must big than one day now %u fundid %u", a.Day, key);
			return false;
		}
	}
	return true;
}

bool FundActive::Trigger()
{
	CActiveCtrl::Fund = 1;
	CActiveCtrl::mapFund = map;
	CActiveCtrl::FundMailTitle = mailtitle;
	CActiveCtrl::FundMailContent = mailcontent;
//	IME_ERROR("mail title %s, mail content %s", mailtitle.c_str(), mailcontent.c_str());
	return true;
}

bool FundActive::Complete()
{
	CActiveCtrl::Fund = 0;
	CActiveCtrl::mapFund.clear();
	CActiveCtrl::FundMailTitle = "";
	CActiveCtrl::FundMailContent = "";
	return true;
}

///////////////////////////////////
bool TaskActive::Validate()
{
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 2);

	std::string content = GET_CMD_PARAM_STRING(1);
	CUtil::StrSplit(content, ",", vec);
	for (int i = 0; i < vec.size(); i++)
	{
//		if (CActiveCtrl::TaskCheck(vec[i]))
//		{
//			IME_ERROR("task already start %u", vec[i]);
//			return false;
//		}
		STC_CONF_DURATION_MISSION *pConf = CConfDurationMission::Find(vec[i]);
		if (pConf == NULL)
		{
			IME_ERROR("task can not find in active duration mission %u", vec[i]);
			return false;
		}
		if (pConf->byType != E_MISSION_ACTIVE)
		{
			IME_ERROR("task is not active mission %u", vec[i]);
			return false;
		}
	}
	m_activeindex = GET_CMD_PARAM_INT(0);
	return true;
}

bool TaskActive::Trigger()
{
	CActiveCtrl::Task[m_activeindex - TASK1_ACTIVE_ID] = 1;
	CActiveCtrl::vecTask[m_activeindex - TASK1_ACTIVE_ID] = vec;
	return true;
}

bool TaskActive::Complete()
{
	CActiveCtrl::Task[m_activeindex - TASK1_ACTIVE_ID] = 0;
	CActiveCtrl::vecTask[m_activeindex - TASK1_ACTIVE_ID].clear();
	CActiveCtrl::TaskFlushTime[m_activeindex - TASK1_ACTIVE_ID] = time(NULL);
	return true;
}

CDataPush::CDataPush()
{
	m_byCharge = 0;
	m_dwChargeNumber = 0;
}

void
CDataPush::ClearData ()
{
	m_byCharge = 0;
	m_dwChargeNumber = 0;
	return;
}		/* -----  end of method CDataPush::ClearData  ----- */

void
CDataPush::HandlerDataPush (CRole *pCRole)
{
	WorldPacket info(CMD_SC_DATA_NOTICE_PUSH);
	info << m_byCharge;
	info << m_dwChargeNumber;
	pCRole->SendPacket(&info);	
	return ;
}		/* -----  end of method CDataPush::HandlerDataPush  ----- */

void 
CDataPush::HandlerDataPushVip(CRole *pCRole, uint32_t viplv)
{
	WorldPacket info(CMD_SC_DATA_NOTICE_PUSH);
	info << (uint8_t) PUSH_DATA_VIP;
	info << viplv;
	pCRole->SendPacket(&info);
	return;
}

void
CDataPush::HandlerDataPushLevelUp(CRole *pCRole, uint32_t prelv, uint32_t newlv)
{
	WorldPacket info(CMD_SC_DATA_NOTICE_PUSH);
	info << (uint8_t) PUSH_DATA_LVUP;
	info << prelv;
	info << newlv;
	pCRole->SendPacket(&info);
	return;
}

void
CDataPush::HandlerDataPushBossAttack(CRole *pCRole, std::string username, uint32_t userid, std::string bossname)
{
	WorldPacket info(CMD_SC_DATA_NOTICE_PUSH);
	info << (uint8_t) PUSH_DATA_BOSS;
	info << username;
	info << userid;
	info << bossname;
	pCRole->SendPacket(&info);
	return;
}

void
CDataPush::HandlerDataPushMissionComplete(CRole *pCRole, uint32_t index)
{
	WorldPacket info(CMD_SC_DATA_NOTICE_PUSH);
	info << (uint8_t) PUSH_DATA_ACHIEVE;
	info << index;
	pCRole->SendPacket(&info);
	return;
}
