/*
 * =====================================================================================
 *
 *       Filename:  Arena.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月16日 20时33分52秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _ARENA_H_
#define _ARENA_H_

class CRole;
class WorldPacket;

/*
 * =====================================================================================
 *        Class:  CArena
 *  Description:  
 * =====================================================================================
 */
class CArena
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CArena ();                             /* constructor */
		int CreateInitData(CRole *pCRole);				
		int Init(CRole *pCRole);

		int SaveData();
		/* ====================  OPERATORS     ======================================= */
		
		void HandlerArenaTeam(WorldPacket &pkg);
		void HandlerArenaChallenge(WorldPacket &pkg);

		void HandlerArenaRequestPrestigeProduct(WorldPacket &pkg);
//		void HandlerArenaRequestRankProduct(WorldPacket &pkg);

		void HandlerTopUpdate();
//		void HandlerLoseUpdate();
		void HandlerChallengeUpdate();

		void HandlerProUpdate();	
//		void HandlerArenaPrestigeProduct();

//		void HandlerArenaRankFlush();           /* flush some value for purchase */
//		void HandlerArenaRankProduct();

		static uint32_t GetPrizePrestige(int rank);

		void OnTimer5s();
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  CArena
		 *      Method:  get_dwRank
		 *--------------------------------------------------------------------------------------
		 */
		inline uint32_t
			GetdwRank (  ) const
			{
				return m_dwRank;
			}		/* -----  end of method CArena::get_dwRank  ----- */

		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  CArena
		 *      Method:  set_dwRank
		 *--------------------------------------------------------------------------------------
		 */
		inline void
			SetdwRank ( uint32_t value )
			{
				m_dwRank	= value;
				return ;
			}		/* -----  end of method CArena::set_dwRank  ----- */
			
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  CArena
		 *      Method:  get_m_dwPrestige
		 *--------------------------------------------------------------------------------------
		 */
		inline uint32_t
			GetdwPrestige (  ) const
			{
				return m_dwPrestige;
			}		/* -----  end of method CArena::get_m_dwPrestige  ----- */

		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  CArena
		 *      Method:  set_m_dwPrestige
		 *--------------------------------------------------------------------------------------
		 */
		inline void
			SetdwPrestige ( uint32_t value )
			{
				m_dwPrestige	= value;
				return ;
			}		/* -----  end of method CArena::set_m_dwPrestige  ----- */
			
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  CArena
		 *      Method:  get_dwLastTime
		 *--------------------------------------------------------------------------------------
		 */
		inline uint32_t
			GetdwLastTime (  ) const
			{
				return m_dwLastTime;
			}		/* -----  end of method CArena::get_m_dwLastTime  ----- */

		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  CArena
		 *      Method:  set_dwLastTime
		 *--------------------------------------------------------------------------------------
		 */
		inline void
			SetdwLastTime ( uint32_t value )
			{
				m_dwLastTime	= value;
				return ;
			}		/* -----  end of method CArena::set_m_dwLastTime  ----- */
			
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  CArena
		 *      Method:  get_dwChallenge_number
		 *--------------------------------------------------------------------------------------
		 */
		inline uint32_t
			GetdwChallengeNumber (  ) const
			{
				return m_dwChallengeNumber;
			}		/* -----  end of method CArena::get_m_dwChallenge_number  ----- */

		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  CArena
		 *      Method:  set_dwChallenge_number
		 *--------------------------------------------------------------------------------------
		 */
		inline void
			SetdwChallengeNumber ( uint32_t value )
			{
				m_dwChallengeNumber	= value;
				return ;
			}		/* -----  end of method CArena::set_m_dwChallenge_number  ----- */
		
//		inline void SetdwDelCD(uint32_t val)
//		{
//			m_dwDelCD = val;
//		}
//		
//		inline uint32_t GetdwDelCD()
//		{
//			return m_dwDelCD; 
//		}
		
		inline void SetdwRanked(uint32_t ranked)
		{
			m_dwRanked = ranked;
		}

		inline void SetdwGetPrestige(uint32_t prestige)
		{
			m_dwGetPrestige = prestige;
		}

	protected:
		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		bool m_IsData;
		CRole *m_pCRole;
		uint32_t m_dwUserid;
		uint32_t m_dwPrestige;
		uint32_t m_dwRank;
		uint32_t m_dwLastTime;
		uint32_t m_dwChallengeNumber;
		uint32_t m_dwRanked;
		uint32_t m_dwGetPrestige;

//		uint32_t m_dwGetPrizeTime;
//		uint32_t m_dwDelCD;

}; /* -----  end of class CArena  ----- */

#endif
