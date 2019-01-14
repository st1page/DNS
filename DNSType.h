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
	DNSHeader(){
		memset((char*)this, 0, 12);
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
	uint16_t Type;
	uint16_t Class;

	uint8_t nameLen;

	size_t len(){
		return nameLen + 4;
	}
	~DNSQuestion(){
		free(Name);
	}
	DNSQuestion(const uint16_t type, const uint16_t _class, const char *name){
		Type = type;
		Class = _class;
		nameLen = strlen(name) + 1;
		Name = (char*)malloc(nameLen);
		strncpy(Name, name, nameLen);
	}
	DNSQuestion(const char *s){
		nameLen = strlen(s) + 1;
		Name = (char*)malloc(nameLen);
		strncpy(Name, s, nameLen);
		uint16_t *p = (uint16_t*)(s+nameLen);
		Type = ntohs(*p++);
		Class = ntohs(*p);
	}
	void toString(char *s){
		strncpy(s, Name, nameLen);
		s[nameLen] = Type;
		s[nameLen+1] = Class;
		uint16_t *p = (uint16_t*)(s+nameLen);
		*p = htons(Type); p++;
		*p = htons(Class);
	}
};
struct DNSRR{
	char *Name;
	uint16_t Type;
	uint16_t Class;
	uint32_t TTL;
	uint16_t DataLen;
	char *Data;

	uint8_t nameLen;
	

	size_t len(){
		return nameLen + DataLen + 10;
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
		const uint16_t *p16 = (const uint16_t *) p;
		Type = ntohs(*p16++);
		Class = htons(*p16++);
		const uint32_t *p32 = (const uint32_t *) p16;
		TTL = ntohl(*p32++);
		p = (char*)p32;
		DataLen = *p++;
		Data = (char*)malloc(DataLen);
		strncpy(Data, p, DataLen);
	}
	void toString(char *s){
		strncpy(s, Name, nameLen);
		char *p = s + nameLen;
		uint16_t *p16 = (uint16_t *)(s+nameLen);
		*p16++ = htons(Type);
		*p16++ = htons(Class);
		uint32_t *p32 = (uint32_t *)p16;
		*p32++ = htonl(TTL);
		p = (char*)p32;
		*p++ = DataLen;
		strncpy(p, Data, DataLen);
	}
};
//TODO:refactor with new & delete
struct DNSPackage{
	DNSHeader header;
	DNSQuestion *questions;
	DNSRR *answers;
	DNSRR *authoritys;
	DNSRR *additionals;

	uint16_t _len;
	uint16_t len(){
		return _len;
	}
	~DNSPackage(){
		for(int i=0;i<header.QDCOUNT;i++) questions[i].~DNSQuestion();
		for(int i=0;i<header.ANCOUNT;i++) answers[i].~DNSRR();
		for(int i=0;i<header.NSCOUNT;i++) authoritys[i].~DNSRR();
		for(int i=0;i<header.ARCOUNT;i++) additionals[i].~DNSRR();
		free(questions);
		free(answers);
		free(authoritys);
		free(additionals);
	}
	DNSPackage(uint16_t id, const char *qdom){
		memset((char*)this, 0, sizeof(DNSPackage));
		header.ID = 0x1;
		header.RD = 0x1;
		header.QDCOUNT = 0x1;
		questions = (DNSQuestion *)malloc(sizeof(DNSQuestion));
		questions[0] = DNSQuestion(0x1, 0x1, qdom);
		_len = header.len() + questions[0].len();
	}
	DNSPackage(const char *msg){
		const char *p = msg;
		header = DNSHeader(p);
		p += header.len();
		_len += header.len();

		questions = (DNSQuestion *)malloc(sizeof(DNSQuestion)*header.QDCOUNT);
		for(int i=0;i<header.QDCOUNT;i++) {
			questions[i] = DNSQuestion(p);
			p += questions[i].len();
			_len += questions[i].len();
		}

		answers = (DNSRR *)malloc(sizeof(DNSRR)*header.ANCOUNT);
		for(int i=0;i<header.ANCOUNT;i++) {
			answers[i] = DNSRR(p, msg);
			p += answers[i].len();
			_len += answers[i].len();
		}

		authoritys = (DNSRR *)malloc(sizeof(DNSRR)*header.NSCOUNT);
		for(int i=0;i<header.NSCOUNT;i++) {
			authoritys[i] = DNSRR(p, msg);
			p += authoritys[i].len();
			_len += authoritys[i].len();
		}

		additionals = (DNSRR *)malloc(sizeof(DNSRR)*header.ARCOUNT);
		for(int i=0;i<header.ARCOUNT;i++) {
			additionals[i] = DNSRR(p, msg);
			p += additionals[i].len();
			_len += additionals[i].len();
		}
	}
	void toString(char *s){
		compress();
		char *p = s;
		for(int i=0;i<header.QDCOUNT;i++) {
			questions[i].toString(p);
			p += questions[i].len();	
		}		
		for(int i=0;i<header.ANCOUNT;i++) {
			answers[i].toString(p);
			p += answers[i].len();	
		}
		for(int i=0;i<header.NSCOUNT;i++) {
			authoritys[i].toString(p);
			p += authoritys[i].len();
			
		}
		for(int i=0;i<header.ARCOUNT;i++) {
			additionals[i].toString(p);
			p += additionals[i].len();
		}
	}

private:
	//TODO
	void compress(){

	}
};
#endif 