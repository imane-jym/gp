#ifndef __GM_COMMAND_EXT_H__
#define __GM_COMMAND_EXT_H__

#include "GmCommand.h"

using namespace CenterDB;

GmCommand* Create(
			uint32				dwAutoId,
			std::string&		strOpr,
			GmCommandTargetType	eTargetType,
			uint64				dwTargetId,
			uint32				dwStartTime,
			uint32				dwEndTime,
			std::string&		strBackup,
			GmCommandStatus		eStatus,
			std::string&		strParams );
bool CheckTargetType( uint8 byTargetType, uint64 dwTargetId );

class SetGmAuthCommand : public GmCommand
{
public:
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();

private:

	uint8	m_byGmAuth;

};

class CancelCommand : public GmCommand
{
public:
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();

private:
	uint32	m_dwGmAutoId;
};


#endif
