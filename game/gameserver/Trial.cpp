#include "Trial.h"
#include "DBCtrl.h"
#include "EnumDefines.h"
#include "ErrorMsg.h"
#include "Role.h"
#include "CmdDefine.h"
#include "CardPacket.h"
#include "WorldBoss.h"

CTrial::CTrial()
{
	m_dwCurrentdwIndex = 0;
	m_pCRole = NULL;
	m_dwCurrentTrigger = 0;
}

int CTrial::Init(CRole *pCRole)
{
	m_pCRole = pCRole;
	std::string temp;
	if (!CDBCtrl::GetUserTrialData(temp, pCRole->GetdwAccountId()))
	{
		IME_ERROR("get trial data fail");
		return -1;
	}
	ByteBuffer by;
	by.append(temp.data(), temp.size());	
	DeSeriliazeDB(by);

	// init trial data
	if (m_dwCurrentdwIndex == 0)
	{
		STC_CONF_TRIAL *pConf = CConfTrial::FindOfFirstTrial();
		if (pConf == NULL)
		{
			IME_ERROR("can not find first record in trial");
			return -1;
		}
		m_dwCurrentdwIndex = pConf->dwIndex;
		uint32_t now = time(NULL);
		STrialData s;
		s.ChallengeCnt = 0;
		s.FlushTime = now;
		s.StartTime = now;
		m_mapTrial[m_dwCurrentdwIndex] = s;
	}

	OnTimer5s();
	return 0;
}

void CTrial::OnTimer5s()
{
	std::map<uint32_t, STrialData>::iterator it;
	uint32_t now = time(NULL);
	for (it = m_mapTrial.begin(); it != m_mapTrial.end(); it++)
	{
		if (!CUtil::IsToday(it->second.FlushTime))
		{
			it->second.ChallengeCnt = 0;
			it->second.FlushTime = now;
		}
	}
	STC_CONF_TRIAL *pFirstConf = CConfTrial::FindOfFirstTrial();
	if (pFirstConf == NULL)
	{
		IME_ERROR("can not find first record in trial");
		return;
	}
	if (m_dwCurrentdwIndex != pFirstConf->dwIndex)
	{
		STC_CONF_TRIAL *pConf = CConfTrial::Find(m_dwCurrentdwIndex);
		if (pConf == NULL)
		{
			IME_ERROR("can not find current trial index %u in trial csv", m_dwCurrentdwIndex);
			return;
		}
		it = m_mapTrial.find(m_dwCurrentdwIndex);
		if (it == m_mapTrial.end())
		{
			IME_ERROR("can not find current trial index in trial map");
			return;
		}
		if (m_mapTrial.find(pFirstConf->dwIndex) == m_mapTrial.end())
		{
			IME_ERROR("can not find first trial index %u in trial map", pFirstConf->dwIndex);
			return;
		}

		if (now >= it->second.StartTime + pConf->dwTrieTime)
		{
			m_dwCurrentdwIndex = pFirstConf->dwIndex;
			HandlerDataInfo();
		}
	}
}

void CTrial::SaveData()
{
	ByteBuffer by;
	SeriliazeDB(by);
	std::string temp;
	temp.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveUserTrialData(temp, m_pCRole->GetdwAccountId());
	return;
}

void CTrial::SeriliazeDB(ByteBuffer &by)
{
	by << m_dwCurrentTrigger;
	by << m_dwCurrentdwIndex;
	by << (uint32_t) m_mapTrial.size();
	std::map<uint32_t, STrialData>::iterator it;
	for (it = m_mapTrial.begin(); it != m_mapTrial.end(); it++)
	{
		by << it->first;
		by << it->second.ChallengeCnt;
		by << it->second.FlushTime;
		by << it->second.StartTime;
	}
}

void CTrial::DeSeriliazeDB(ByteBuffer &by)
{
	m_mapTrial.clear();
	by >> m_dwCurrentTrigger;
	by >> m_dwCurrentdwIndex;
	uint32_t number;
	by >> number;
	for (int i = 0; i < number; i++)
	{
		STrialData s;
		uint32_t key;
		by >> key;
		by >> s.ChallengeCnt;
		by >> s.FlushTime;
		by >> s.StartTime;
		m_mapTrial[key] = s;
	}
}

void CTrial::HandlerDataInfo()
{
	STC_CONF_TRIAL *pConf = CConfTrial::Find(m_dwCurrentdwIndex);
	if (pConf == NULL)
	{
		IME_ERROR("can not find record index %u in trial csv", m_dwCurrentdwIndex);
		return;
	}
	if (m_mapTrial.find(m_dwCurrentdwIndex) == m_mapTrial.end())
	{
		IME_ERROR("can not find record index %u in trial map data", m_dwCurrentdwIndex);
		return;
	}
	STC_CONF_ROLE *pConfExp = CConfRole::Find(m_pCRole->GetwLevel());
	if (pConfExp == NULL)
	{
		IME_ERROR("can not find record index %u in role exp csv", m_pCRole->GetwLevel());
		return;
	}

	WorldPacket info(CMD_SC_TRIAL_INFO);
	info << m_dwCurrentdwIndex;	
	int remain = -1;
	if (pConf->dwChallengeNumber != 0)
	{
		remain = pConf->dwChallengeNumber - m_mapTrial[m_dwCurrentdwIndex].ChallengeCnt;
		remain = remain < 0 ? 0 : remain;
	}
	info << remain;
	info << pConf->wConsume;
	info << m_pCRole->GetdwStamina();
	uint32_t roleexpneed, coinneed, cardexpneed;
	roleexpneed = pConf->dwRoleExpNeed / 100.0 * pConfExp->dwRoleExp;
	coinneed = pConf->dwCoinNeed / 100.0 * pConfExp->dwCoin;
	cardexpneed = pConf->dwCardExpNeed / 100.0 *pConfExp->dwCardExp;

	info << roleexpneed;
	info << coinneed;
	info << cardexpneed;
	int cd = -1;
	if (pConf->dwTrieTime != 0)
	{
		cd = m_mapTrial[m_dwCurrentdwIndex].StartTime + pConf->dwTrieTime - time(NULL);
		if (cd < 0)
			cd = 0;
	}
	info << cd;
	info << m_dwCurrentTrigger;
	m_pCRole->SendPacket(&info);
}

void CTrial::HandlerTrial(WorldPacket &pkg)
{
	IME_CHAR_LOG("action [userid %u trial]", m_pCRole->GetdwAccountId());
	STC_CONF_TRIAL *pConf = CConfTrial::Find(m_dwCurrentdwIndex);
	if (pConf == NULL)
	{
		IME_ERROR("can not find record index %u in trial csv", m_dwCurrentdwIndex);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_TRIAL_RESULT);
		return;
	}
	std::map<uint32_t, STrialData>::iterator it = m_mapTrial.find(m_dwCurrentdwIndex);
	if (it == m_mapTrial.end())
	{
		IME_ERROR("can not find record index %u in trial map data", m_dwCurrentdwIndex);
		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_TRIAL_RESULT);
		return;
	}

	if (m_pCRole->GetdwStamina() < pConf->wConsume)
	{
		m_pCRole->SendStaminaNotEnough(CMD_SC_TRIAL_RESULT);
		return;
	}

	if (m_pCRole->GetwLevel() < pConf->wLevelLimit)
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_TRIAL_LEVEL, CMD_SC_TRIAL_RESULT);
		return;
	}

	STC_CONF_ROLE *pConfExp = CConfRole::Find(m_pCRole->GetwLevel());
	if (pConfExp == NULL)
	{
		IME_ERROR("can not find record index %u in role exp csv", m_pCRole->GetwLevel());
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_TRIAL_RESULT);
		return;
	}


	if (pConf->dwChallengeNumber != 0 && m_mapTrial[m_dwCurrentdwIndex].ChallengeCnt >= pConf->dwChallengeNumber)
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_TRIAL_CHALLENGE, CMD_SC_TRIAL_RESULT);
		return;
	}

	if (m_pCRole->GetclsCardPacket()->IsFull(1, 0))
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_TRIAL_RESULT);
		return;
	}

	std::vector<CCard *> vec;
	if (m_pCRole->GetclsCardPacket()->GetEquippCards(vec))
	{
		IME_ERROR("get card property fail, userid %u", m_pCRole->GetdwAccountId());
		m_pCRole->SendErrorMsg(ERRNO_MSG_SYS, CMD_SC_TRIAL_RESULT);
		return;
	}

	std::vector<int> vecpro;
	vecpro.push_back(pConf->dwPro);
	vecpro.push_back(pConf->wPro1);
	vecpro.push_back(pConf->wPro2);
	vecpro.push_back(pConf->wPro3);
	vecpro.push_back(pConf->wPro4);
	vecpro.push_back(pConf->wPro5);
	vecpro.push_back(pConf->wPro6);
	vecpro.push_back(pConf->wPro7);
	vecpro.push_back(pConf->wPro8);
	vecpro.push_back(pConf->wPro9);
	vecpro.push_back(pConf->wPro10);
	vecpro.push_back(pConf->wPro11);
	vecpro.push_back(pConf->wPro12);
	vecpro.push_back(pConf->wPro13);
	vecpro.push_back(pConf->wPro14);
	vecpro.push_back(pConf->wPro15);
	IME_DEBUG("total pro %u boss pro %u, %u %u %u %u", pConf->wPro, pConf->wPro1, pConf->wPro2, pConf->wPro3, pConf->wPro4, pConf->wPro5, pConf->wPro6, pConf->wPro7, pConf->wPro8);

	if (CUtil::RandEither(pConf->wPro, 100))
	{
		int ret = CUtil::RandFactor(vecpro);
		IME_DEBUG("choose %u", ret);
		SProduct duct = {0};
		if (ret == BOSS_HAPPEN)
		{
			CFriendBoss *pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(m_pCRole->GetdwAccountId()));
			if (pFb == NULL)
			{
				IME_ERROR("can not find this user %u friend boss data", m_pCRole->GetdwAccountId());
				m_pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_TRIAL_RESULT);
				return;
			}
			if (pFb->GetBossId() == 0)
			{
				pFb->HandlerHappenBoss(m_pCRole, pConf->dwBossId);
				m_pCRole->SendObjEffectByHappenBoss();
			}
		}
		else if (ret == NEXT_ENTER)
		{
			if (pConf->dwNextTrial != 0)
			{
				m_dwCurrentTrigger = pConf->dwNextTrial;
			}
		}
		else if (ret == HAPPEN_ROLE)
		{
			m_pCRole->SendObjEffectByHappenRole();
			if (m_pCRole->GetUJoinCount() != 0)
			{
				duct.type = E_OBJ_WORLD_EXP;
				duct.para1 = 0;
				duct.para2 = GlobalConfig::TrialFriendWorldHistory;
			}
		}
		else
		{
			switch(ret)
			{
				case COIN_GET:
					duct.type = E_OBJ_COIN;
					duct.para1 = 0;
					duct.para2 = pConf->dwCoin;
					break;
				case ROLE_EXP:
					duct.type = E_OBJ_ROLE_EXP;
					duct.para1 = 0;
					duct.para2 = pConf->dwRoleExp;
					break;
				case CARD_EXP:
					duct.type = E_OBJ_CARD_EXP;
					duct.para1 = 0;
					duct.para2 = pConf->dwCardExp;
					break;
				case WORLD_EXP:
					duct.type = E_OBJ_WORLD_EXP;
					duct.para1 = 0;
					duct.para2 = pConf->dwWorldExp;
					break;
				case ITEM_DROP:
					duct.type = E_OBJ_COL_SHOP;
					duct.para1 = pConf->dwItemCollection;
					duct.para2 = 0;
					break;
				case PIECES_DROP:
					duct.type = E_OBJ_COL_ITEM;
					duct.para1 = pConf->dwPiecesCollection;
					duct.para2 = 0;
					break;
				case CARD_DROP:
					duct.type = E_OBJ_COL_CARD_PLUS;
					duct.para1 = pConf->dwCardCollection;
					duct.para2 = pConf->wPlusPro;
					break;
				case ENERGY_RECOVER:
					duct.type = E_OBJ_ENERGY;
					duct.para1 = 0;
					duct.para2 = pConf->dwEnergy;
					break;
				case LOT_ENERGY_RECOVER:
					duct.type = E_OBJ_ENERGY;
					duct.para1 = 0;
					duct.para2 = pConf->dwLotEnergy;
					break;
				case STRENGTH_RECOVER:
					duct.type = E_OBJ_STRENGTH;
					duct.para1 = 0;
					duct.para2 = pConf->dwEnergy;
					break;
				case LOT_STRENGTH_RECOVER:
					duct.type = E_OBJ_STRENGTH;
					duct.para1 = 0;
					duct.para2 = pConf->dwLotEnergy;
					break;
				case STAMINA_RECOVER:
					duct.type = E_OBJ_STAMINA;
					duct.para1 = 0;
					duct.para2 = pConf->dwStamina;
					break;
				case LOT_STAMINA_RECOVER:
					duct.type = E_OBJ_STAMINA;
					duct.para1 = 0;
					duct.para2 = pConf->dwLotStamina;
					break;
				default:
					break;
			}
		}
		if (duct.type != 0)
		{
			CShopEffect::SGiveProduct ObjEffect = {0};
			CShopEffect::GiveGameProductSpecial(m_pCRole, duct.type, duct.para1, duct.para2, ObjEffect, SOURCE_TRIAL);
			std::vector<CShopEffect::SGiveProduct> vect;
			vect.push_back(ObjEffect);
			m_pCRole->SendObjEffect(vect);
		}
	}

	m_pCRole->ChangeStamina(-pConf->wConsume);
	it->second.ChallengeCnt++;

	uint32_t roleexpneed, coinneed, cardexpneed;
	roleexpneed = pConf->dwRoleExpNeed / 100.0 * pConfExp->dwRoleExp * pConf->wConsume;
	coinneed = pConf->dwCoinNeed / 100.0 * pConfExp->dwCoin * pConf->wConsume;
	cardexpneed = pConf->dwCardExpNeed / 100.0 *pConfExp->dwCardExp * pConf->wConsume;

	m_pCRole->AddExp(roleexpneed);
	m_pCRole->ChangeCoin(coinneed, SOURCE_TRIAL);
	for (int i = 0; i < vec.size(); i++)
	{
		if (m_pCRole->GetwLevel() * STRENGTH_LEVEL_LIMIT_PARA > vec[i]->GetwLevel())
		{
			vec[i]->AddExp(cardexpneed);
			vec[i]->Calculate(true);
		}
	}
	m_pCRole->Calculate(true);
	m_pCRole->HandlerInfoOpt();

	HandlerDataInfo();

	WorldPacket info(CMD_SC_TRIAL_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) roleexpneed;
	info << (uint32_t) coinneed;
	info << (uint32_t) cardexpneed;
	m_pCRole->SendPacket(&info);
}

void CTrial::HandlerEnterNext(WorldPacket &pkg)
{
	uint8_t isenter;
	pkg >> isenter;
	IME_CHAR_LOG("action [userid %u enter next trial isenter %u]", m_pCRole->GetdwAccountId(), isenter);
	if (m_dwCurrentTrigger == 0)
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_TRIAL_NEXT, CMD_SC_TRIAL_NEXT_RESULT);
		return;
	}
	STC_CONF_TRIAL *pConf = CConfTrial::Find(m_dwCurrentTrigger);
	if (pConf == NULL)
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_TRIAL_NEXT_RESULT);
		return;
	}
	if (m_pCRole->GetwLevel() < pConf->wLevelLimit)
	{
		m_dwCurrentTrigger = 0;
		HandlerDataInfo();
		WorldPacket info(CMD_SC_TRIAL_NEXT_RESULT);
		info << (uint16_t) ERRNO_MSG_TRIAL_LEVEL;
		info << pConf->wLevelLimit;
		m_pCRole->SendPacket(&info);
		return;
	}


	if (isenter > 0)
	{
		m_dwCurrentdwIndex = m_dwCurrentTrigger;
		uint32_t now = time(NULL);
		if (m_mapTrial.find(m_dwCurrentdwIndex) != m_mapTrial.end())
		{
			m_mapTrial[m_dwCurrentdwIndex].StartTime = now;
		}
		else
		{
			m_mapTrial[m_dwCurrentdwIndex].ChallengeCnt = 0;
			m_mapTrial[m_dwCurrentdwIndex].FlushTime = now;
			m_mapTrial[m_dwCurrentdwIndex].StartTime = now;
		}
	}
	m_dwCurrentTrigger = 0;
	HandlerDataInfo();
	WorldPacket info(CMD_SC_TRIAL_NEXT_RESULT);
	info << (uint16_t) 0;
	m_pCRole->SendPacket(&info);
}

void CTrial::ChangeTrial(uint32_t index)
{
	STC_CONF_TRIAL *pConf = CConfTrial::Find(index);
	if (pConf == NULL)
	{
		IME_ERROR("can not find record index %u in trial csv", index);
		return;
	}
	if (m_pCRole->GetwLevel() < pConf->wLevelLimit)
	{
		IME_ERROR("role level is not enough, level limit %u", pConf->wLevelLimit);
		return;
	}

	uint32_t current = m_dwCurrentdwIndex;
	m_dwCurrentdwIndex = index;
	uint32_t now = time(NULL);
	if (m_mapTrial.find(m_dwCurrentdwIndex) != m_mapTrial.end())
	{
		if (current != m_dwCurrentdwIndex)
		{
			m_mapTrial[m_dwCurrentdwIndex].StartTime = now;
		}
		else
		{
			m_mapTrial[m_dwCurrentdwIndex].StartTime -= pConf->dwTrieTime;
		}
	}
	else
	{
		m_mapTrial[m_dwCurrentdwIndex].ChallengeCnt = 0;
		m_mapTrial[m_dwCurrentdwIndex].FlushTime = now;
		m_mapTrial[m_dwCurrentdwIndex].StartTime = now;
	}
	HandlerDataInfo();
	return;
}

bool CTrial::ValidChangeTrial(uint32_t index)
{
	STC_CONF_TRIAL *pConf = CConfTrial::Find(index);
	if (pConf == NULL)
	{
		IME_ERROR("can not find record index %u in trial csv", index);
		return false;
	}
	if (m_pCRole->GetwLevel() < pConf->wLevelLimit)
	{
		IME_ERROR("role level is not enough, level limit %u", pConf->wLevelLimit);
		return false;
	}
	return true;
}
