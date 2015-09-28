/*
 * =====================================================================================
 *
 *       Filename:  shop.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年12月09日 16时35分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _SHOP_H_
#define _SHOP_H_

#include <map>
#include <stdint.h>
#include "WorldPacket.h"

struct STC_CONF_GACHA;
struct SProduct;
class CRole;
/*
 * =====================================================================================
 *        Class:  CShop
 *  Description:  
 * =====================================================================================
 */
class CShop
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CShop ();                             /* constructor */
		~CShop ();
		int Init(CRole *pCRole);

		/* ====================  OPERATORS     ======================================= */
		bool LoadData();
		int SaveData();
		int SerilizeDB(ByteBuffer &byte);
		int DeSerilizeDB(ByteBuffer &byte);
		void AddItem(uint32_t index, int source, uint32_t number = 1);
		int DelItem(uint32_t index, int source, uint32_t number = 1);
		bool IsHave(uint32_t index, uint32_t number = 1);

		bool IsHaveBoxAndKey(uint32_t CombineId);
		int DelItemByFuncType(uint32_t Functype, uint32_t CombineId, uint32_t &shopid);

		void PreDataForUpgrade(std::map<uint32_t, uint32_t> &map) {map = m_mapShop;};

		void HandlerStoreData(WorldPacket &pkg);
		void HandlerShopInfo(uint32_t index = 0);
		void HandlerBuy (WorldPacket &pkg);
		void HandlerShopUse(WorldPacket &pkg);
		void HandlerBuyDelCD(WorldPacket &pkg);
		void HandlerShopUseCancel(WorldPacket &pkg);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */
		CRole *m_pCRole;
		bool m_IsInit;
		std::map<uint32_t, uint32_t> m_mapShop;

		/* ====================  DATA MEMBERS  ======================================= */
		uint32_t m_kind;

}; /* -----  end of class CShop  ----- */


/*
 * =====================================================================================
 *        Class:  CShopEffect
 *  Description:  
 * =====================================================================================
 */
class CShopEffect
{
	public:
		/* ====================  LIFECYCLE     ======================================= */

//		typedef enum
//		{
//			E_COMBINE_LOW,
//			E_COMBINE_HIGH
//		}ECombine;

		typedef struct SGiveProduct
		{
			uint32_t effect;
			int32_t para1;
			int32_t para2;
		}SGiveProduct;

		typedef struct SEffect
		{
			int32_t para1;
			int32_t para2;
			int32_t para3;
			int32_t para4;
			std::vector<SGiveProduct> out;
		}SEffect;

		typedef int (*ItemUseEffectFunc)(CRole *pCRole, SEffect &data);
		/* ====================  OPERATORS     ======================================= */
		static void Init();
		
		static int UniqueGiftEffect(CRole *pCRole, SEffect &data); /* in para1 randomindex out 和E_OBJ_EFFECT一样*/
		static int GiftEffect(CRole *pCRole, SEffect &data); /* in para1 gift index out 无 */
		static int EnergyItemEffect(CRole *pCRole, SEffect &data); /* in para1 rate out 无 */
		static int StrengthItemEffect(CRole *pCRole, SEffect &data); /* in para1 rate out 无*/
		static int MissDurationEffect(CRole *pCRole, SEffect &data); /* in para1 time out 无*/
		static int CoinPacketEffect(CRole *pCRole, SEffect &data); /* in para1 coin number out 无 */
		static int CoinBoxEffect(CRole *pCRole, SEffect &data); /* in para1 coin number out 无*/
		static int BoxEffect(CRole *pCRole, SEffect &data); /* in para1 combine id para2 egg randomindex out 和E_OBJ_EFFECT一样*/
		static int KeyEffect(CRole *pCRole, SEffect &data); /* in para1 combine id para2 egg randomindex out 和E_OBJ_EFFECT一样*/
		static int MasterEffect(CRole *pCRole, SEffect &data); /* in para1 level out para1 选项, para2 masterindex, para3 flag 1:增长经验 0:无 */
		static int ChipEffect(CRole *pCRole, SEffect &data); /* in para1 need number para2-4 (参见E_OBJ_TYPE) out para2-4(参见E_OBJ_EFFECT) */
		static int VipEffect(CRole *pCRole, SEffect &data); /* in para1 vipexp out para1 vipexp  */
		static int EnergyItemCountEffect(CRole *pCRole, SEffect &data); /* in para1 number out 无 */
		static int StrengthItemCountEffect(CRole *pCRole, SEffect &data); /* in para1 number out 无 */
		static int StaminaItemEffect(CRole *pCRole, SEffect &data); /* in para1 rate out 无 */
		static int StaminaItemCountEffect(CRole *pCRole, SEffect &data); /* in para1 number out  */
		static int TrialEnterEffect(CRole *pCRole, SEffect &data); /* in para1 index out para1 index */
		static int RoleExpEffect(CRole *pCRole, SEffect &data); /* in para1 roleexp count out 和E_OBJ_EFFECT一样*/
		static int CardExpEffect(CRole *pCRole, SEffect &data);

		static ItemUseEffectFunc Find(uint16_t fun)
		{
			return (m_mapFunc.find(fun) == m_mapFunc.end() ? NULL : m_mapFunc[fun]);
		}

		static int ExecEffect(CRole *pCRole, SEffect &data, uint16_t cmd)
		{
			ItemUseEffectFunc fn = Find(cmd);
			if (fn != NULL)
			{
				return fn(pCRole, data);
			}
			else
			{
				return -1;
			}
		}

//		static int SkillCombineBookEffect(CRole *pCRole, uint32_t objid, uint8_t position, SEffect &data); /* in para1 skill index, para2 card obid, para3, position   */
		static bool IsFullCardPacketGift(CRole *pCRole, uint32_t giftid);
		// look in: E_OBJ_TYPE  out: E_OBJ_EFFECT
		static int GiveGameProduct(CRole *pCRole, uint16_t type, uint32_t index, uint32_t number, SGiveProduct &Rel, int source);
		static int GiveGameProductSpecial(CRole *pCRole, uint16_t type, uint32_t index, uint32_t number, SGiveProduct &Rel, int source);
		static int IsObjValid(std::vector<SProduct> vproduct);
		static bool IsBoxAndKeyOK(CRole *pCRole, uint32_t CombineId);

		static void HandlerGacha(CRole *pCRole, WorldPacket &pkg);
		static void HandlerTenGacha(CRole *pCRole, WorldPacket &pkg);
		static STC_CONF_GACHA* SelectGacha(CRole *pCRole, uint8_t &choose);
		static void SelectGachaOpt(CRole *pCRole, STC_CONF_GACHA *pConf, uint8_t choose);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		static std::map<uint16_t, ItemUseEffectFunc> m_mapFunc;
}; /* -----  end of class CShopEffect  ----- */


#endif
