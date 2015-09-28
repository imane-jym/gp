/*
 * =====================================================================================
 *
 *       Filename:  Group.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月29日 14时16分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _GROUP_H_
#define _GROUP_H_

#include <stdint.h>
#include <vector>
#include <map>
#include "DynamicStruct.h"
#include "DBCtrl.h"
#include "WorldPacket.h"

#define PROPERTY_GROUP_DEF(T,name) \
	T Get##name() { return m_##name;}; \
	void Set##name(T name)\
	{ m_##name = name; \
	setProperty(E_GROUP_##name, name); \
	;}; 

#define PROPERTY_GROUP_DEF_OBJECT(T,name) \
	T& Get##name() { return m_##name;}; \
	void Set##name(const T& name) \
	{ m_##name = name; \
	setProperty(E_GROUP_##name, AnyType(m_##name, true)); \
	}; 

class CRole;

typedef struct
{
	uint32_t dwUserId;
	uint32_t dwCredit;
	uint8_t byPosion;
	std::map<uint32_t, uint8_t> mapMission;
	uint32_t dwCreditSupport;
} SGroupMember;

/*
 * =====================================================================================
 *        Class:  CGroup
 *  Description:  
 * =====================================================================================
 */
class CGroup: public CDynamicStruct
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		typedef enum _E_PROPERTY_GROUP
		{
			E_GROUP_dwId,
			E_GROUP_strName,
			E_GROUP_dwCreateId,
			E_GROUP_strCreateName,
			E_GROUP_dwCredit,
			E_GROUP_wLevel,
			E_GROUP_wPersionLimit,
			E_GROUP_byPosion1Limit,
			E_GROUP_byPosion2Limit,
			E_GROUP_byPosion3Limit,
			E_GROUP_byPosion4Limit,
			E_GROUP_vecPosion1Persion,
			E_GROUP_vecPosion1PersionStr,
			E_GROUP_vecPosion2Persion,
			E_GROUP_vecPosion3Persion,
			E_GROUP_vecPosion4Persion,
			E_GROUP_vecPosion5Persion,
			E_GROUP_wPersionNumber,
			E_GROUP_strNotice,
			E_GROUP_dwNeedCredit,
			E_GROUP_dwCreditLimit,
			E_GROUP_dwMissionFlushTime,
			E_GROUP_byMissionFlushNumber,
			E_GROUP_dwMissionFlushPoint,
		}E_PROPERTY_GROUP;

		CGroup ();                             /* constructor */
		bool Init(const CDBCtrl::SGroup &group);

//		int LoadData();
		int SaveData();
		/* ====================  ACCESSORS     ======================================= */
		PROPERTY_GROUP_DEF(uint32_t, dwId);
		PROPERTY_GROUP_DEF_OBJECT(std::string, strName);
		PROPERTY_GROUP_DEF(uint32_t, dwCreateId);
		PROPERTY_GROUP_DEF_OBJECT(std::string, strCreateName);
		PROPERTY_GROUP_DEF(uint32_t, dwCredit);
		PROPERTY_GROUP_DEF(uint16_t, wLevel);
		PROPERTY_GROUP_DEF(uint16_t, wPersionLimit);
		PROPERTY_GROUP_DEF(uint8_t, byPosion1Limit);
		PROPERTY_GROUP_DEF(uint8_t, byPosion2Limit);
		PROPERTY_GROUP_DEF(uint8_t, byPosion3Limit);
		PROPERTY_GROUP_DEF(uint8_t, byPosion4Limit);
		PROPERTY_GROUP_DEF_OBJECT(std::vector<AnyType>, vecPosion1Persion);
		PROPERTY_GROUP_DEF_OBJECT(std::vector<AnyType>, vecPosion1PersionStr);
		PROPERTY_GROUP_DEF_OBJECT(std::vector<AnyType>, vecPosion2Persion);
		PROPERTY_GROUP_DEF_OBJECT(std::vector<AnyType>, vecPosion3Persion);
		PROPERTY_GROUP_DEF_OBJECT(std::vector<AnyType>, vecPosion4Persion);
		PROPERTY_GROUP_DEF_OBJECT(std::vector<AnyType>, vecPosion5Persion);
		PROPERTY_GROUP_DEF(uint16_t, wPersionNumber);
		PROPERTY_GROUP_DEF_OBJECT(std::string, strNotice);
		PROPERTY_GROUP_DEF(uint32_t, dwNeedCredit);
		PROPERTY_GROUP_DEF(uint32_t, dwCreditLimit);
		PROPERTY_GROUP_DEF(uint32_t, dwMissionFlushTime);
		PROPERTY_GROUP_DEF(uint32_t, byMissionFlushNumber);
		PROPERTY_GROUP_DEF(uint32_t, dwMissionFlushPoint);

		/* ====================  OPERATORS     ======================================= */
		int SerializeDB(ByteBuffer &pkg, ByteBuffer &member);
		int DeserializeDB(ByteBuffer &pkg, ByteBuffer &member);
		void PackProUpdate(WorldPacket &pkg);
		void PackGroupInfo(WorldPacket &pkg);
		int Calculate();		

		int LoadConf();

		void SendAllGroupData(CRole *pCRole);
		void PackGroupMemberInfo(uint32_t userid, WorldPacket &pkg);
		SGroupMember *GetGroupMember(uint32_t userid)
		{
			std::map<uint32_t, SGroupMember>::iterator it;
			it = m_mapUserGroup.find(userid);
			if (it != m_mapUserGroup.end())
			{
				return &(it->second);
			}
			else
			{
				return NULL;
			}
		}
		void SendDataToAllMember(WorldPacket *pkg);
		void HandlerLevelUp(CRole *pCRole, WorldPacket &pkg);
		int ChangeCredit(int32 credit);
		void HandlerMailInfo(CRole *pCRole, WorldPacket &pkg);
		void CreateSGroupMemberNormal(SGroupMember &SGMtmp, uint32 userid);
		void HandlerMailOpt(CRole *pCRole, WorldPacket &pkg);
		void OnTimer1s();
		void MissionFlush();
		void HandlerAcceptMission(CRole *pCRole, WorldPacket &pkg);
		void HandlerRequestPrize(CRole *pCRole, WorldPacket &pkg);
		void HandlerTechUpdate(CRole *pCRole, WorldPacket &pkg);
		void HandlerMemberInfo(CRole *pCRole, WorldPacket &pkg);
		void HandlerChat(CRole *pCRole, WorldPacket &pkg);
		void HandlerAppoint(CRole *pCRole, WorldPacket &pkg);
		void HandlerUnAppoint(CRole *pCRole, WorldPacket &pkg);
		void HandlerMemberSearchInfo(CRole *pCRole, WorldPacket &pkg);
		void HandlerModifyInfo(CRole *pCRole, WorldPacket &pkg);
		int SendMailRequestEnter(CRole *pCRole, std::string str);
		int SendMailAgreeEnter(CRole *pCRole, std::string str);
		int SendMailCancel(CRole *pCRole);
		int SendMailDelete(CRole *pCRole, std::string PeerUserName);
		void HandlerExit(CRole *pCRole, WorldPacket &pkg);
		void HandlerDeleteMember(CRole *pCRole, WorldPacket &pkg);
		void HandlerUnAppointMemberInfo(CRole *pCRole, WorldPacket &pkg);
		int GroupCancel(CRole *pCRole, WorldPacket &pkg);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		uint32_t m_dwId;
		std::string m_strName;
		uint32_t m_dwCreateId;
		std::string m_strCreateName;	
		uint32_t m_dwCredit;
		uint16_t m_wLevel;
		uint16_t m_wPersionLimit;
		uint8_t m_byPosion1Limit;
		uint8_t m_byPosion2Limit;
		uint8_t m_byPosion3Limit;
		uint8_t m_byPosion4Limit;
		std::vector<AnyType> m_vecPosion1Persion;
		std::vector<AnyType> m_vecPosion1PersionStr;
		std::vector<AnyType> m_vecPosion2Persion;
		std::vector<AnyType> m_vecPosion3Persion;
		std::vector<AnyType> m_vecPosion4Persion;
		std::vector<AnyType> m_vecPosion5Persion;
		uint16_t m_wPersionNumber;
		std::string m_strNotice;
		uint32_t m_dwNeedCredit;
		uint32_t m_dwCreditLimit;
		uint32_t m_dwMissionFlushTime;
		uint32_t m_byMissionFlushNumber;
		uint32_t m_dwMissionFlushPoint;

		uint8_t m_byFlag;
		std::map<uint32_t, SGroupMember> m_mapUserGroup;

}; /* -----  end of class CGroup  ----- */


#endif
