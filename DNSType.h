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
	uint16_t ID;
	uint8_t RD : 1;
	uint8_t TC : 1;
	uint8_t AA : 1;
	uint8_t OPCODE : 4;
	uint8_t QR : 1;
	uint8_t RCODE : 4;
	uint8_t Z : 3;
	uint8_t RA : 1;
	uint16_t QDCOUNT;
	uint16_t ANCOUNT;
	uint16_t NSCOUNT;
	uint16_t ARCOUNT;
#else
	uint16_t ID;
	uint8_t QR : 1;
	uint8_t OPCODE : 4;
	uint8_t AA : 1;
	uint8_t TC : 1;
	uint8_t RD : 1;
	uint8_t RA : 1;
	uint8_t Z : 3;
	uint8_t RCODE : 4;
	uint16_t QDCOUNT;
	uint16_t ANCOUNT;
	uint16_t NSCOUNT;
	uint16_t ARCOUNT;
#endif 
	size_t len(){
		return 12;
	}
	DNSHeader(const char *s){
		strncpy((char*)this, s, 12);
		n2h();
	}
	void toString(char *s){
		strncpy(s, (char*)this, 12);
		h2n();
	}
private:
	void n2h(){
		ID = ntohs(ID);
		QDCOUNT = ntohs(QDCOUNT);
		ANCOUNT = ntohs(ANCOUNT);
		NSCOUNT = ntohs(NSCOUNT);
		ARCOUNT = ntohs(ARCOUNT);
	}
	void h2n(){
		ID = htons(ID);
		QDCOUNT = htons(QDCOUNT);
		ANCOUNT = htons(ANCOUNT);
		NSCOUNT = htons(NSCOUNT);
		ARCOUNT = htons(ARCOUNT);
	}
};
#pragma pack(pop)
struct DNSQuestion{
	char *Name;
	uint8_t Type;
	uint8_t Class;

	uint8_t nameLen;

	size_t len(){
		return nameLen + 2;
	}
	~DNSQuestion(){
		free(Name);
	}
	DNSQuestion(const char *s){
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
struct DNSRR{
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
	~DNSRR(){
		free(Name);
		free(Data);
	}
	DNSRR(const char *s,const char *frS){
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