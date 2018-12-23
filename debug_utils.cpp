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
int main(){
	test_isDN();
}