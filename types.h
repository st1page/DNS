#ifndef _DNSTYPES_H_
#define _DNSTYPES_H_
#include <stdint.h>
#include <asm/byteorder.h>

#pragma pack(push)
#pragma pack(1)
struct DNSHeader{
#ifdef __LITTLE_ENDIAN
	uint16_t transID;
	uint8_t RD : 1;
	uint8_t TC : 1;
	uint8_t AA : 1;
	uint8_t opcode : 4;
	uint8_t QR : 1;
	uint8_t rcode : 4;
	uint8_t zero : 3;
	uint8_t RA : 1;
	uint16_t Questions;
	uint16_t AnswerRRs;
	uint16_t AuthorityRRs;
	uint16_t AdditionalRRs;
#else
	uint16_t transID;
	uint8_t QR : 1;
	uint8_t opcode : 4;
	uint8_t AA : 1;
	uint8_t TC : 1;
	uint8_t RD : 1;
	uint8_t RA : 1;
	uint8_t zero : 3;
	uint8_t rcode : 4;
	uint16_t Questions;
	uint16_t AnswerRRs;
	uint16_t AuthorityRRs;
	uint16_t AdditionalRRs;
#endif 
};
#pragma pack(pop)
#endif 