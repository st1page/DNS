#ifndef _DNSTYPES_H_
#define _DNSTYPES_H_
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <asm/byteorder.h>
#include <arpa/inet.h>
#pragma pack(push)
#pragma pack(1)
//字长为4 不然GG
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
	size_t len(){
		return 12;
	}
	DNSHeader(const char *s){
		strncpy((char*)this, s, 12);
	}
	void toString(char *s){
		strncpy(s, (char*)this, 12);
	}
};
#pragma pack(pop)
struct Query{
	char *Name;
	uint8_t Type;
	uint8_t Class;

	uint8_t nameLen;

	size_t len(){
		return nameLen + 2;
	}
	Query(const char *s){
		nameLen = strlen(s) + 1;
		Name = (char*)malloc(nameLen);
		strncpy(Name, s, nameLen);
		Type = s[nameLen];
		Class = s[nameLen+1];
	}
	void toString(char *s){
		strncpy(s, Name, nameLen);
		s[nameLen] = Type;
		s[nameLen+1] = Class;
	}
};
struct RR{
	char *Name;
	uint8_t Type;
	uint8_t Class;
	uint16_t TTL;
	uint8_t DataLen;
	char *Data;

	uint8_t nameLen;
	

	size_t len(){
		return nameLen + DataLen + 5;
	}
	RR(const char *s,const char *frS){
		uint8_t flag = s[0];
		const char *p;
		if(flag>>2==0xC){
			const char *p = frS + ((uint)(s[0]&0xf)<<8) + uint(s[1]);
			nameLen = strlen(p) + 1;
			Name = (char*)malloc(nameLen);
			strncpy(Name, p, nameLen);
			p = s + 2;
		} else {
			nameLen = strlen(s) + 1;
			Name = (char*)malloc(nameLen);
			strncpy(Name, s, nameLen);
			p = s + nameLen;
		}
		Type = *p++;
		Class = *p++;
		TTL = ntohs(*(uint16_t*)p);
		p+=2;
		DataLen = *p++;
		Data = (char*)malloc(DataLen);
		strncpy(Data, p, DataLen);
	}
	void toString(char *s){
		strncpy(s, Name, nameLen);
		char *p = s + nameLen;
		*p++ = Type;
		*p++ = Class;
		*(uint16_t*)p = htons(TTL);
		p+=2;
		*p++ = DataLen;
		strncpy(p, Data, DataLen);
	}
};

#endif 