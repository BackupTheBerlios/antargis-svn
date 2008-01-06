#include "test.hh"
#include <iostream>

A::A(const AGString &a):m(a)
{
	std::cout<<"A::A:"<<a<<std::endl;
}
void A::callTest(const AGString &a)
{
	std::cout<<"A::callTest:"<<a<<std::endl;
}

AGString A::returnTest() const
{
	return "returnTest";
}

AGString A::virtReturnTest()
{
	return "virtReturnTest";
}

const AGString &A::virtReturnTest2() const
{
	return m;
}

void A::callVirt()
{
	std::cout<<virtReturnTest()<<std::endl;
}

void A::callTest2(AGString &a)
{
	a="ksjklkjdslkj";
}

void A::ptrTest(A *a)
{
	std::cout<<a->m<<std::endl;
}



C::C(const AGString &a):A(a)
{
	std::cout<<"C::C:"<<a<<std::endl;
}



AGString C::virtReturnTest()
{
	return "virtReturnTest2";
}
