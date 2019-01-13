#include "DNSHeader.h"
#include<stdint.h>
#include<iostream>
using namespace std;
void outchar(char ch){
	uint8_t x = (1<<7);
	while(x){
		printf("%d",(bool)(ch&x));
		x>>=1;
	}
	putchar(' ');
}
void out(char *addr,size_t size){
	char *p = addr;
	for(size_t i=0;i<size;i++){
		outchar(*p);
		p++;
	}
	putchar('\n');
}
void testHeader(){
	DNSHeader x;
	cout<<sizeof(x)<<endl;
	cout<<&x<<endl;
	cout<<(&x.transID)<<endl;
	cout<<(&x.Questions)<<endl;
	cout<<(&x.AnswerRRs)<<endl;
	cout<<(&x.AuthorityRRs)<<endl;
	cout<<(&x.AdditionalRRs)<<endl;
	char *p = (char *)&x.transID;
	p+=2;
	out(p,2);
	x.QR = 1;
	out(p,2);
	x.opcode = 0xf;
	out(p,2);
	x.AA = 1;
	out(p,2);
	x.TC = 1;
	out(p,2);
	x.RD = 1;
	out(p,2);
	x.RA = 1;
	out(p,2);
	x.rcode = 0xf;
	out(p,2);
}
int main(){
	testHeader();
	
}