/*
 * Tower.cpp
 *
 *  Created on: Jan 8, 2014
 *      Author: long
 */

#include "Tower.h"
#include "DBCtrl.h"
#include "Role.h"
#include "WorldPacket.h"
#include "CmdDefine.h"
#include "Util.h"
#include "CardPacket.h"
#include "ErrorMsg.h"
#include<vector>
#include<map>
#include "CommonType.h"
#include "GlobalConfig.h"
#include "World.h"
#include "Card.h"
#include "EnumDefines.h"
#include "Shop.h"
#include<algorithm>
#include "Mission.h"

CTower::CTower() {
	this->m_pCRole = NULL;
	this->healthMax = 0;
	this->timestamp = 0;
}

void CTower::onTime5s() {
	if(!CUtil::IsTodayOffset(this->towerData.timestamp,60)){
		time_t timestamp = time(NULL);
		CDBCtrl::ResetUserTowerData(this->m_pCRole->GetdwAccountId(),timestamp);
		towerData.index = 0;
		towerData.breakBlockadeTimes = 0;
		towerData.threeAttack = 0;
		towerData.twoAttack = 0;
		towerData.buyTimes = 0;
		towerData.refreshAttribute = 0;
		towerData.dark = 0;
		towerData.hand = 0;
		towerData.stick = 0;
		towerData.sword = 0;
		towerData.internalForce = 0;
		towerData.health = 0;
		towerData.isMaxLayer = 0;
		towerData.timestamp = timestamp;
		randomBoss(1,&towerData,0);
		//在推一次塔
		handlerTowerInfo();
	}
}

void CTower::saveData() {
	//save data
	uint32 userId = this->m_pCRole->GetdwAccountId();
	STowerData *data = &towerData;
	//保存用户塔信息
	CDBCtrl::SaveUserTowerData(userId,data->index, data->twoAttack,
			data->threeAttack, data->refreshAttribute, data->sword, data->hand,
			data->internalForce, data->dark, data->stick,
			data->breakBlockadeTimes,data->buyTimes,data->health,data->isMaxLayer,data->timestamp);
	//保存用户闯关排行需要的信息
	uint32 level = this->m_pCRole->GetwLevel();
	uint32 power = this->m_pCRole->GetPower();
	std::string name = this->m_pCRole->GetstrUserName();
	STC_CONF_TOWER* configData = CConfTower::find(data->index);
	if (configData == NULL) {
		IME_ERROR("can not find record index %u in tower csv", data->index);
		return ;
	}
	uint32 layer = configData->layer - 1;
	uint32 breakTimes = data->breakBlockadeTimes;
	uint32 masterIndex;
	std::vector<uint32> cards;
	this->m_pCRole->GetclsCardPacket()->GetCardIndex(masterIndex,cards);
	CDBCtrl::SaveRank(userId,name,layer,breakTimes,level,power,masterIndex);
}

int CTower::init(CRole* pCRole) {
	this->m_pCRole = pCRole;
	STowerData data;
	uint32 userId = this->m_pCRole->GetdwAccountId();
	if (!CDBCtrl::GetUserTowerData(userId, data.index, data.twoAttack,
			data.threeAttack, data.refreshAttribute, data.sword, data.hand,
			data.internalForce, data.dark, data.stick,
			data.breakBlockadeTimes,data.buyTimes,data.health,data.isMaxLayer,data.timestamp)) {
		IME_ERROR("get tower data fail");
	}

	if(data.index == 0){
		this->randomBoss(1,&data,0);
	}

	//查找Boss的相关信息，主要使用血量 供前台展示
	STC_CONF_TOWER* configData = CConfTower::find(data.index);
	if (configData == NULL) {
		IME_ERROR("can not find record index %u in tower csv", data.index);
		return -1;
	}
	if(data.health == 0){
		data.health = this->healthMax = configData->health;
	}else{
		this->healthMax = configData->health;

	}
	if(data.dark == 0 && data.hand == 0 && data.internalForce == 0 && data.sword == 0 && data.sword == 0 && data.stick == 0){
		data.sword = configData->dwSwordEffect;
		data.hand = configData->dwHandEffect;
		data.internalForce = configData->dwInternalForceEffect;
		data.dark = configData->dwDarkEffect;
		data.stick = configData->dwStickEffect;
	}

	if(data.timestamp == 0){
		data.timestamp = time(NULL);
	}
	//data.timestamp = time(NULL) - 86400;
	this->towerData = data;
	return 0;
}
void CTower::handlerTowerInfo() {
	//取塔相关信息
	STowerData*data = &towerData;
	//uint32 index,uint32 health,uint32 healthMax,uint32 闯关次数,uint32 sword,uint32 hand,uint32 internalForce, uint32 dark, uint32 stick
	WorldPacket info(CMD_SC_TOWER_INFO);
	info << data->index;
	info << data->health;
	info << this->healthMax;
	info << -data->breakBlockadeTimes + data->buyTimes + GlobalConfig::TowerLimit;
	info << data->sword;
	info << data->hand;
	info << data->internalForce;
	info << data->dark;
	info << data->stick;
	info << data->threeAttack;
	info << data->twoAttack;
	info << data->isMaxLayer;
	uint32 remain;
	if(GlobalConfig::BuyBreakBlockadeTimesLimit >= data->buyTimes){
		remain = GlobalConfig::BuyBreakBlockadeTimesLimit - data->buyTimes;
	}else{
		remain = 0;
	}
	info << remain;
//	uint32 layer = CConfTower::find(data->index)->layer;
//	info << CConfTower::getNextDropLayer(layer)-layer + 1;
	this->m_pCRole->SendPacket(&info);

}

void CTower::handlerTowerCombat(WorldPacket &wpk) {

	uint32 userId = this->m_pCRole->GetdwAccountId();

	STowerData* towerData = &(this->towerData);
	if(towerData->breakBlockadeTimes >= GlobalConfig::TowerLimit + towerData->buyTimes){
		IME_ERROR("you have no break times");
		this->m_pCRole->SendErrorMsg(ERRNO_MSG_NO_BREAK_TIMES,CMD_SC_TOWER_COMBAT_RESULT);
		return ;
	}
	uint32 index;
	wpk >> index;
	if(index != towerData->index){
		IME_ERROR("request data error");
		this->m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL,CMD_SC_TOWER_COMBAT_RESULT);
		return;
	}
	STC_CONF_TOWER* configData = CConfTower::find(towerData->index);
	if (configData == NULL) {
		IME_ERROR("can not find record index %u in tower csv",towerData->index);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_TOWER_COMBAT_RESULT);
		return;
	}
	IME_CHAR_LOG("action [userid %u enter tower %u ]", userId, configData->layer);
	//验证奖励是否合法
	std::vector<SProduct> vect;
	if(configData->cardReward != 0){
		SProduct sp;
		sp.type = E_OBJ_COL_CARD;
		sp.para1 = configData->cardReward;
		sp.para2 = 1;
		vect.push_back(sp);
	}
	if(configData->propReward != 0){
		SProduct sp;
		sp.type = E_OBJ_COL_SHOP;
		sp.para1 = configData->propReward;
		sp.para2 = 1;
		vect.push_back(sp);
	}
//	if(configData->successCardReward != 0){
//		SProduct sp;
//		sp.type = E_OBJ_COL_CARD;
//		sp.para1 = configData->propReward;
//		sp.para2 = 1;
//		vect.push_back(sp);
//	}
//
//	if (configData->successPropReward != 0) {
//		SProduct sp;
//		sp.type = E_OBJ_COL_SHOP;
//		sp.para1 = configData->propReward;
//		sp.para2 = 1;
//		vect.push_back(sp);
//	}

	if (CShopEffect::IsObjValid(vect)) {
		IME_ERROR("prize product type para fail");
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_TOWER_COMBAT_RESULT);
		return;
	}
	//判断背包是不是已满
	if (this->m_pCRole->GetclsCardPacket()->IsFullSpecial(vect)) {
		IME_ERROR("user %u card packet already full", m_pCRole->GetdwAccountId());
		m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_TOWER_COMBAT_RESULT);
		return;
	}

	//判断是不是今天第一次闯塔
	if(judgeFirstTowerDaily(userId)){
		uint32 level = this->m_pCRole->GetwLevel();
		uint32 power = this->m_pCRole->GetPower();
		std::string name = this->m_pCRole->GetstrUserName();
		STC_CONF_TOWER* configData = CConfTower::find(towerData->index);
		if (configData == NULL) {
			IME_ERROR("can not find record index %u in tower csv", towerData->index);
			m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_TOWER_COMBAT_RESULT);
			return;
		}
		uint32 layer = configData->layer;
		uint32 breakTimes = towerData->breakBlockadeTimes;
		uint32 masterIndex;
		std::vector<uint32> cards;
		this->m_pCRole->GetclsCardPacket()->GetCardIndex(masterIndex,cards);
		CDBCtrl::InsertUserRank(userId,name,layer,breakTimes,level,power,masterIndex);
	}

	//初始化boss属性
	SDurationIn in;
	if(towerData->refreshAttribute == 1){//否刷新属性
		//属性刷新生效
		in.dark = towerData->dark;
		in.hand = towerData->hand;
		in.internalforce = towerData->internalForce;
		in.stick = towerData->stick;
		in.sword = towerData->sword;
	}else{
		in.dark = configData->dwDarkEffect;
		in.hand = configData->dwHandEffect;
		in.internalforce = configData->dwInternalForceEffect;
		in.stick = configData->dwStickEffect;
		in.sword = configData->dwSwordEffect;
	}
	//初始化boss战斗数据
	TowerData bossData;
	if(bossData.init(configData,towerData->health)){
		IME_ERROR("NPC init fail %u",configData->objectId);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_TOWER_COMBAT_RESULT);
		return;
	}

	//提升攻击力
	uint32 times = one;
	if(towerData->twoAttack == 1){
		times = two;
	}
	if(towerData->threeAttack == 1){
		times = three;
	}
	this->m_pCRole->SetAttackTimes(times);


	//战斗
	WorldPacket Dpkg(CMD_SC_DURATION_PKG);
	uint8 eva;
	//0是战斗成功，1是失败，-1是系统错误
	int ret = DurationJudgeWin(m_pCRole, &bossData, Dpkg, eva, in);

	//发送战报
	uint32_t len = Dpkg.size();
	const char *pStart = (const char *) Dpkg.contents();
	uint32_t number;
	while (len) {
		WorldPacket DpkgSplit(CMD_SC_DURATION_PKG);
		if (len > PACKET_SPLIT_NUMBER) {
			number = PACKET_SPLIT_NUMBER;
			len -= PACKET_SPLIT_NUMBER;
		} else {
			number = len;
			len = 0;
		}
		DpkgSplit.append(pStart, number);
		pStart += number;
		m_pCRole->SendPacket(&DpkgSplit);
	}

	//战斗结束后各种购买点都清零
	towerData->refreshAttribute = 0;
	towerData->twoAttack = 0;
	towerData->threeAttack = 0;

	//将攻击力还原
	this->m_pCRole->SetAttackTimes(1);

	uint32 rankBreakTimes = towerData->breakBlockadeTimes;
	//战斗失败
	if(ret == 1){
		//需要将战斗属性刷成当前塔boss的属性
		//战斗失败需要将闯关次数+1
		towerData->breakBlockadeTimes ++;
		towerData->sword = configData->dwSwordEffect;
		towerData->hand = configData->dwHandEffect;
		towerData->internalForce = configData->dwInternalForceEffect;
		towerData->dark = configData->dwDarkEffect;
		towerData->stick = configData->dwStickEffect;

		//失败物品掉落
		std::vector<CShopEffect::SGiveProduct> vect;
		CShopEffect::SGiveProduct ObjEffect;
		//coin
		if (configData->failureCoinReward != 0) {
			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_COIN, 0,	configData->failureCoinReward, ObjEffect, SOURCE_TOWER);
			vect.push_back(ObjEffect);
		}
		//dollar
		if (configData->failureDollarReward != 0) {
			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_DOLLAR, 0,configData->failureDollarReward, ObjEffect, SOURCE_TOWER);
			vect.push_back(ObjEffect);
		}

		//处理道具包掉落
		if (configData->failurePropReward != 0) {
			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_COL_SHOP,configData->failurePropReward, 0, ObjEffect, SOURCE_TOWER);
			vect.push_back(ObjEffect);
		}

		//卡牌包
		if (configData->failureCardReward != 0) {
			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_COL_CARD,configData->failureCardReward, 0, ObjEffect, SOURCE_TOWER);
			vect.push_back(ObjEffect);
		}
		//江湖阅历
		if(configData->failureWorldExpReward != 0){
			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_WORLD_EXP,0, configData->failureWorldExpReward, ObjEffect, SOURCE_TOWER);
			vect.push_back(ObjEffect);
		}
		//发送物品掉落客户端
		m_pCRole->SendObjEffect(vect);

		//失败将闯关次数+1更新排行榜
		uint32 rankBreakTimes = towerData->breakBlockadeTimes;

		//计算boss剩余的血量
		if(towerData->health < this->m_pCRole->GetTotalHarm()){
			towerData->health = 0;
		}else{
			towerData->health = towerData->health - this->m_pCRole->GetTotalHarm();
		}

		//推塔
		handlerTowerInfo();
		//发送战斗结果
		WorldPacket info(CMD_SC_TOWER_COMBAT_RESULT);
		info << (uint16)0;
		info << (uint8)1;
		this->m_pCRole->SendPacket(&info);
	}
	//战斗胜利，推送下一个塔的信息
	else if(ret == 0){
		//生成下一层塔的信息，将塔的信息定时刷入tower表中

		//如果nextLayer是0，表示到达顶层了
		if (configData->nextLayer == 0) {
			towerData->isMaxLayer = 1;
			towerData->health = 0;
			handlerTowerInfo();
		}else{

			//随机生成下一层塔的boss
			this->randomBoss(configData->nextLayer,towerData,0);
			handlerTowerInfo();
		}

		std::vector<CShopEffect::SGiveProduct> vect;
		CShopEffect::SGiveProduct ObjEffect;
		//金钱元宝掉落
		if (configData->coinReward != 0) {
			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_COIN, 0,configData->coinReward, ObjEffect, SOURCE_TOWER);
			vect.push_back(ObjEffect);
		}
		//dollar
		if (configData->dollarReward != 0) {
			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_DOLLAR, 0, configData->dollarReward, ObjEffect, SOURCE_TOWER);
			vect.push_back(ObjEffect);
		}

		//处理道具包掉落
		if(configData->propReward != 0){
			CShopEffect::GiveGameProduct(this->m_pCRole,E_OBJ_COL_SHOP,configData->propReward,0,ObjEffect,SOURCE_TOWER);
			vect.push_back(ObjEffect);
		}

		//卡牌包
		if (configData->cardReward != 0) {
			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_COL_CARD,configData->cardReward, 0, ObjEffect, SOURCE_TOWER);
			vect.push_back(ObjEffect);
		}
//		//coin
//		if (configData->successCoinReward != 0) {
//			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_COIN, 0,	configData->successCoinReward, ObjEffect, SOURCE_TOWER);
//			vect.push_back(ObjEffect);
//		}
//		//dollar
//		if (configData->successDollarReward != 0) {
//			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_DOLLAR, 0,configData->successDollarReward, ObjEffect, SOURCE_TOWER);
//			vect.push_back(ObjEffect);
//		}
//
//		//处理道具包掉落
//		if (configData->successPropReward != 0) {
//			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_COL_SHOP,configData->successPropReward, 0, ObjEffect, SOURCE_TOWER);
//			vect.push_back(ObjEffect);
//		}E_TOWER_PURCHASE_BREAK_TIMES
//
//		//卡牌包
//		if (configData->successCardReward != 0) {
//			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_COL_CARD,configData->successCardReward, 0, ObjEffect, SOURCE_TOWER);
//			vect.push_back(ObjEffect);
//		}
//		//江湖阅历
//		if(configData->successWorldExpReward != 0){
//			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_WORLD_EXP,0, configData->successWorldExpReward, ObjEffect, SOURCE_TOWER);
//			vect.push_back(ObjEffect);
//		}
		//江湖阅历
		if(configData->worldExpReward != 0){
			CShopEffect::GiveGameProduct(this->m_pCRole, E_OBJ_WORLD_EXP,0, configData->worldExpReward, ObjEffect, SOURCE_TOWER);
			vect.push_back(ObjEffect);
		}

		//发送物品掉落客户端
		m_pCRole->SendObjEffect(vect);

		//发送战斗结果
		WorldPacket info(CMD_SC_TOWER_COMBAT_RESULT);
		info << (uint16)0;
		info << (uint8)0;
		this->m_pCRole->SendPacket(&info);
		//添加活跃度
		this->m_pCRole->GetCMission()->BreakTower(0,configData->layer);
	}
}

void CTower::handlerTowerqueryRank() {

	std::vector<STowerRank> *rank = CWorld::GetInstance()->getRank();
	//封装消息
	WorldPacket info(CMD_SC_TOWER_RANK);
	info << (uint32) (rank->size());
	for (std::vector<STowerRank>::iterator it = rank->begin(); it != rank->end();it++) {
		info <<  (*it).userId;
		info <<  ((*it).userName).c_str();
		info <<  (*it).userLevel;
		info <<  (*it).layer;
		info <<  (*it).masterIndex;
	}
	this->m_pCRole->SendPacket(&info);
}

CTower::~CTower() {
}

uint32 CTower::getTwoAttack(){
	return this->towerData.twoAttack;
}

uint32 CTower::getThreeAttack(){
	return this->towerData.threeAttack;
}

void CTower::handlerTowerThreeAttack() {
	this->towerData.threeAttack = 1;
	handlerTowerInfo();
}

void CTower::handlerTowerTwoAttack() {
	this->towerData.twoAttack = 1;
	handlerTowerInfo();
}

void CTower::handlerTowerRefreshAttribute() {
	STowerData *towerData = &(this->towerData);

//	towerData->refreshAttribute = 1;
//
	STC_CONF_TOWER* configData = CConfTower::find(towerData->index);
	if (configData == NULL) {
		IME_ERROR("can not find record index %u in tower csv",towerData->refreshAttribute);
		return;
	}
//
//	std::vector<uint32> temp;
//	temp.push_back(configData->dwDarkEffect);
//	temp.push_back(configData->dwHandEffect);
//	temp.push_back(configData->dwInternalForceEffect);
//	temp.push_back(configData->dwSwordEffect);
//	temp.push_back(configData->dwStickEffect);
//
//	//随机生成他们的顺序,洗牌函数
//	std::random_shuffle(temp.begin(),temp.end());
//	//刷新后的塔的属性
//	towerData->dark = temp[0];
//	towerData->hand = temp[1];
//	towerData->internalForce = temp[2];
//	towerData->sword = temp[3];
//	towerData->stick = temp[4];

	randomBoss(configData->layer,towerData,1);
	//推送塔
	handlerTowerInfo();
}

void CTower::handlerTowerBreakBlockadeTimes(uint32 times) {
	STowerData *data = &(this->towerData);
	data->buyTimes += times;
	handlerTowerInfo();
//	std::vector<STowerRank> *rank = CWorld::GetInstance()->getRank();
//	//封装消息
//	WorldPacket info(CMD_SC_TOWER_RANK);
//	info << (uint32) (rank->size());
//	for (std::vector<STowerRank>::iterator it = rank->begin(); it != rank->end();it++) {
//		info <<  (*it).userId;
//		info <<  ((*it).userName).c_str();
//		info <<  (*it).userLevel;
//		info <<  (*it).layer;
//		info <<  (*it).masterIndex;
//	}
//	this->m_pCRole->SendPacket(&info);
}

uint32 CTower::getBuytimes() {
	STowerData *data = &(this->towerData);
	return data->buyTimes;
}
/**
 * type 0表示推下一层塔boss，1表示刷新该层塔的boss
 */
void CTower::randomBoss(uint32 nextLayer,STowerData *towerData,uint32 type) {
	std::vector<uint32>* objects;
	objects = CConfTower::findLayerObjects(nextLayer);
	if (objects == NULL) {
		IME_ERROR("can not find record index %u in tower csv", nextLayer);
	}
	uint32 randomIndex = (*objects)[CUtil::RandByRange(0, (*objects).size())];
	STC_CONF_TOWER* randomData = CConfTower::find(randomIndex);
	if (randomData == NULL) {
		IME_ERROR("can not find record index %u in tower csv", randomIndex);
		return;
	}

	//获取下一个塔的信息
	towerData->index = randomData->dwIndex;
	towerData->dark = randomData->dwDarkEffect;
	towerData->hand = randomData->dwHandEffect;
	towerData->stick = randomData->dwStickEffect;
	towerData->sword = randomData->dwSwordEffect;
	towerData->internalForce = randomData->dwInternalForceEffect;
	if(type == 0){
		towerData->health = randomData->health;
	}else{
		towerData->health = towerData->health * 1.0/ this->healthMax * randomData->health ;
	}
	this->healthMax = randomData->health;
}


int CTower::TowerData::init(STC_CONF_TOWER* configData,uint32 health ){
	//初始化vec 数组
	STC_CONF_CARD_BASE *pConfCard = CConfCardBase::Find(configData->objectId);
	if (pConfCard == NULL){
		IME_ERROR("can not find master card record index %u", configData->objectId);
		return -1;
	}

	masterid = 0;
	SBossCard temp;
	uint32_t initid = 1;
	if (configData->dwIndex != 0) {
		if (configData->dwLevel == 0)
			configData->dwLevel = 1;

		CCard Card;
		if (Card.CreateCard(configData->objectId, configData->dwLevel, initid++)) {
			IME_ERROR("create card fail index %u", configData->objectId);
			return -1;
		}

		temp.dwObjId = Card.GetdwObjId();
		temp.dwIndex = Card.GetdwIndex();
		temp.byAttribute = Card.GetbyAttribute();
		temp.dwAttack = configData->attack;	//pConf->dwMasterAttackBase + (level - 1) * pConf->dwMasterAttackGrow;
		temp.dwDefence = configData->defend;//pConf->dwMasterDefenceBase + (level - 1) * pConf->dwMasterDefenceGrow;
		temp.dwHealth = health;	//pConf->dwMasterHealthBase + (level - 1) * pConf->dwMasterHealthGrow;
		temp.dwHealthMax = configData->health;//pConf->dwMasterHealthBase + (level - 1) * pConf->dwMasterHealthGrow;
		temp.dwCritRate = Card.GetdwCritRate();
		temp.dwBigSkill = configData->dwSkill;		//pConf->dwMasterSkillIndex;
		temp.dwBigSkillLv = configData->dwSkillLevel;//pConf->wMasterSkillLevel;
		temp.dwCardLv = configData->dwLevel;
		temp.byCardType = E_ITEM_MASTER_CARD;
		temp.byRare = E_CR_NORMAL;
		temp.byBreakCount = 0;

		IME_DEBUG("init fellower card objid %u index %u attribute %u attack %u defence %u health %u critrate %u BigSkill %u BigSkillLv %u cardlv %u cardtype %u",temp.dwObjId, temp.dwIndex, temp.byAttribute, temp.dwAttack,temp.dwDefence, temp.dwHealth, temp.dwCritRate, temp.dwBigSkill,temp.dwBigSkillLv, temp.dwCardLv, temp.byCardType);
		vec.push_back(temp);
		masterid = Card.GetdwObjId();
	}
	SBossCard temp1;
	vec.resize(MAX_CARD_CAPACITY + 1, temp1);
	bossName = pConfCard->strName;
	return 0;

}

int CTower::TowerData::PrepareDurationData(std::vector<cardforduration_t>& vec,
		std::vector<uint32_t>& vecline) {
	//初始化弟子，由于tower中boss没有弟子，所以全部初始化为0
	vecline.resize(MAX_CARD_CAPACITY, 0);
	for(int i = 0; i < vecline.size(); i++){
		vecline[i] = 0;
	}
	vec.clear();
	vec.resize(MAX_CARD_CAPACITY + 1);
	for (int i = 0; i < this->vec.size(); i++) {
		vec[i].CardObjId = this->vec[i].dwObjId;
		vec[i].CardIndex = this->vec[i].dwIndex;
		vec[i].BigSkill = this->vec[i].dwBigSkill;
		vec[i].BigSkillLv = this->vec[i].dwBigSkillLv;
		vec[i].AttackM = this->vec[i].dwAttack;
		vec[i].AttackE = 0;
		vec[i].DefenceM = this->vec[i].dwDefence;
		vec[i].DefenceE = 0;
		vec[i].Health = this->vec[i].dwHealth;
		vec[i].HealthMaxM = this->vec[i].dwHealthMax;
		vec[i].HealthMaxE = 0;
		vec[i].Crit = this->vec[i].dwCritRate;
		vec[i].Attribute = this->vec[i].byAttribute;
		vec[i].SkillPro = 0;
		vec[i].SkillPower = 0;
		vec[i].CardLv = this->vec[i].dwCardLv;
		vec[i].CardType = this->vec[i].byCardType;
		vec[i].Rare = E_CR_NORMAL;
		vec[i].BreakCount = this->vec[i].byBreakCount;
		IME_DEBUG("objid %u attack %u", this->vec[i].dwObjId,
				this->vec[i].dwAttack);
	}
		CDuration::SetStrUserName(bossName);
	vecline.insert(vecline.begin(), masterid);		
	return 0;
}
bool CTower::judgeFirstTowerDaily(uint32 userId){
	if(CDBCtrl::GetRankByUserId(userId)){
		return false;
	}
	return true;
}


