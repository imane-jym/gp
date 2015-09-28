#ifndef __LOGIC_PACKETDISPATCHER_H__
#define __LOGIC_PACKETDISPATCHER_H__

#include "Defines.h"
#include "server_eventdispatcher.h"
#include "WorldPacket.h"

template<typename TEvent>
class PacketDispatcher_T : public EventDispatcher<TEvent>
{
public:

	PacketDispatcher_T(){}

	virtual ~PacketDispatcher_T(void){}

	struct WrapperBase
	{
	public:
		WrapperBase(){}
		virtual ~WrapperBase(){}
		virtual void handle(uint32 id, TEvent& b) = 0;
		
	};

	template<typename HandlerObject, typename M>
	class Wrapper : public WrapperBase
	{
	public:
		typedef void (HandlerObject::* _pF_t)(M& b);
		void set(HandlerObject* ph, _pF_t pF)
		{
			m_ph = ph;
			m_pF = pF;
		}
		void handle(uint32 id, TEvent& b)
		{
			M m;

			int remain = (int)b.size();
			if (m.ParseFromArray((char*)b.contents(), remain) == false)
			{
				IME_ERROR("parse data fail type %s error content %s", m.GetTypeName().c_str(), m.InitializationErrorString().c_str());
				return;
			}
			std::string strPacket = m.Utf8DebugString();
			IME_DEBUG("recv pkg %s", strPacket.c_str());
			if (!m.IsInitialized())
				IME_ERROR("recv message of type %s error content %s", m.GetTypeName().c_str(), m.InitializationErrorString().c_str());
			(m_ph->*m_pF)(m);
		}
	private:
		HandlerObject* m_ph;
		_pF_t m_pF;
	};
	template<typename HandlerObject, typename M>
	void																				AddHandler_Packet(HandlerObject* pObj, uint32 id, void (HandlerObject::* m)(M&))
	{
//		boost::shared_ptr<WrapperBase> ptrBase;
		boost::shared_ptr<Wrapper<HandlerObject, M> > ptrProtocal(new Wrapper<HandlerObject, M>());

		ptrProtocal->set(pObj, m);
//		ptrBase = ptrProtocal;
//		AddEventHandler(id, boost::bind(&WrapperBase::handle, ptrBase, _1, _2));
		AddEventHandler(id, boost::bind(&WrapperBase::handle, ptrProtocal, _1, _2));
	}

	bool ProtocolID(const WorldPacket& buffer, uint32& id)
	{
		id = buffer.GetOpcode();
		return true;
	}

	bool DispatchPacket(TEvent& packet)
	{
		uint32 id = -1;
		if(ProtocolID(packet, id) == false)
			return false;

		return DispatchEvent(id, packet);
	}
};

#define REG_PACKET_HANDLERS(DispType__, pDisp__, HandlerObject__, pThis__) \
		{	\
			DispType__* __pDisp					= pDisp__;\
			HandlerObject__* __pThis			= pThis__;\
			typedef HandlerObject__				__HandlerObject;\
			{\
				using namespace TgsProtocol;

#define REG_PACKET_HANDLERS_END	}}

#define PACKET_HANDLER(id, handler, PacketType)			__pDisp->AddHandler_Packet<__HandlerObject, PacketType>(__pThis, id, &__HandlerObject::handler)

#endif
