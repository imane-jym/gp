
#ifndef _WORLD_PACKET_H
#define _WORLD_PACKET_H

//#include "Common.h"
#include "ByteBuffer.h"

class WorldPacket : public ByteBuffer
{
    public:
                                                            // just container for later use
        WorldPacket()                                       : ByteBuffer(1000), m_opcode(0)
        {
        }
        explicit WorldPacket(uint32 opcode, size_t res) : ByteBuffer(res), m_opcode(opcode) { }
                                                            // copy constructor
        WorldPacket(const WorldPacket &packet)              : ByteBuffer(packet), m_opcode(packet.m_opcode)
        {
        }

		explicit WorldPacket(uint32_t opcode): ByteBuffer(1000), m_opcode(opcode)
		{

		}

        void Initialize(uint32 opcode, size_t newres=200)
        {
            clear();
            _storage.reserve(newres);
            m_opcode = opcode;
        }

        uint32 GetOpcode() const { return m_opcode; }
        void SetOpcode(uint32 opcode) { m_opcode = opcode; }

    protected:
        uint32 m_opcode;
};
#endif
