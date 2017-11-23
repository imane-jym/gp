/*
 * CmdInnerDefine.h
 *
 *  Created on: 2011-7-13
 *      Author: yq
 */

#ifndef CMDINNERDEFINE_H_
#define CMDINNERDEFINE_H_

//服务器组内部协议定义
//命令定义规范
//奇数为 RelayClient 发送到 RelayServer
//偶数为 RelayServer 发送到 RelayClient
#define CMD_MAKE_RELAYCLIENT(v)			(((v)<<1)|0x0001)
#define	CMD_MAKE_RELAYSERVER(v)			((v)<<1)
#define CMD_IS_RELAYCLIENT(v)			(((v)&0x0001)==0x0001)
#define CMD_IS_RELAYSERVER(v)			(((v)&0x0001)==0x0000)

//RelayClient(ex: GameServer) 发送到 RelayServer的命令
#define CMD_RELAY_CS_TEST					CMD_MAKE_RELAYCLIENT(0x0000) //测试用 echo{string发送字符串}

#define CMD_RELAY_CS_LOGIN					CMD_MAKE_RELAYCLIENT(0x0001)	// 玩家游戏上线 {uint32 dwAccountId }
#define CMD_RELAY_CS_LOGOUT					CMD_MAKE_RELAYCLIENT(0x0002)	// 玩家游戏下线 {uint32 dwAccountId }


#define CMD_RELAY_CS_BROADCAST				CMD_MAKE_RELAYCLIENT(0x0009)	// 广播
#define CMD_RELAY_CS_RELAY_PACKET			CMD_MAKE_RELAYCLIENT(0x0010)	// 转发
#define CMD_RELAY_CS_RELOAD_CONFIG			CMD_MAKE_RELAYCLIENT(0x001A)	// 重载配置文件 {string 配置文件名字}

#define CMD_RELAY_CS_APPLE_PAY				CMD_MAKE_RELAYCLIENT(0x001B)	// {uint32 + string + string}



//RelayServer 发送到 RelayClient的命令
#define CMD_RELAY_SC_TEST					CMD_MAKE_RELAYSERVER(0x0000) //测试用 echo 服务器返回{string返回字符串;}

#define CMD_RELAY_SC_BROADCAST				CMD_MAKE_RELAYSERVER(0x0009)	// 广播
#define CMD_RELAY_SC_RELAY_PACKET			CMD_MAKE_RELAYSERVER(0x0010)	// 转发

#define CMD_RELAY_SC_RELOAD_CONFIG			CMD_MAKE_RELAYSERVER(0x001A)	// 重载配置文件 {string 配置文件名字}

#define CMD_RELAY_SC_APPLE_PAY				CMD_MAKE_RELAYSERVER(0x001B)

typedef enum _E_RELAY_PACKET_CMD
{
	E_RELAY_CMD_FRIEND_UPDATE,		// 好友信息更新
	E_RELAY_CMD_MAIL_UPDATE,		// 邮件信息更新
	E_RELAY_CMD_FRIEND_SPEAKER,		// 好友消息
	E_RELAY_CMD_FRIEND_DEL,			// 好友删除 { 主动删除的用户id}
}E_RELAY_PACKET_CMD;

#endif /* CMDINNERDEFINE_H_ */

