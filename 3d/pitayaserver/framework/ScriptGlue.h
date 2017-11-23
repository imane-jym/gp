/*
 * =====================================================================================
 *
 *       Filename:  ScriptGlue.h
 *
 *    Description:  script glue for lua javascript and so on. convert some api to some simple api that suit script
 *
 *        Version:  1.0
 *        Created:  11/26/2014 06:29:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "lua/lua.hpp"

/*
 * =====================================================================================
 *        Class:  CScriptSupport
 *  Description:  
 * =====================================================================================
 */
class CScriptSupport
{
	public:
		enum EScriptType
		{
			CPP_SCRIPT = 0,
			LUA_SCRIPT = 1,
			JAVASCRIPT_SCRIPT	
		};
		/* ====================  LIFECYCLE     ======================================= */
		static CScriptSupport* GetInstance();
		~CScriptSupport ();
		bool Init(EScriptType e);
		void UnInit();
		bool LoadScript(const char *script);
		lua_State* GetLuaState() { return m_pL; }

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */
		CScriptSupport ();                             /* constructor */

		/* ====================  DATA MEMBERS  ======================================= */
		lua_State *m_pL;
		EScriptType m_Type;
		static CScriptSupport *m_pInstance;
}; /* -----  end of class CScriptSupport  ----- */

#define sSCript CScriptSupport::GetInstance()
