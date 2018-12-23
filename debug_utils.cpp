#include<iostream>
using namespace std;

#include "utils.hpp"

void test_isDN(){
	char *s = "www.google.com";
	printf("%s %d\n",s,isDomainName(s));
	s = ".www.google.com";
	printf("%s %d\n",s,isDomainName(s));
	s = "www.google.com.";
	printf("%s %d\n",s,isDomainName(s));
	s = "www..asd.";
	printf("%s %d\n",s,isDomainName(s));
	s = "www.google.com..";
	printf("%s %d\n",s,isDomainName(s));
	s = "-www.google.com.";
	printf("%s %d\n",s,isDomainName(s));
	s = "www.google.com-";
	printf("%s %d\n",s,isDomainName(s));
	s = "www.a1234567b1234567c1234567d1234567e1234567f1234567g1234567h1234567.com";
	printf("%s %d\n",s,isDomainName(s));
}
void test_dn2msg(){
	char *s, *t;
	s = "www.google.com";
	t = dn2msg(s);
	printf("%s#\n%s#\n\n",s,t);
	s = "www.google.com.";
	t = dn2msg(s);
	printf("%s#\n%s#\n\n",s,t);
}
int main(){
	test_isDN();
	test_dn2msg();
}