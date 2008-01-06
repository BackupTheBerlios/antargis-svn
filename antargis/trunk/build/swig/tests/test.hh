#ifndef TEST_HH
#define TEST_HH

#include <ag_string.h>

class A
{
	AGString m;
	public:
  A(const AGString &a);
	virtual ~A() {}
  void callTest(const AGString &a);
  void callTest2(AGString &a);
	void ptrTest(A *a);
	AGString returnTest() const;
  virtual AGString virtReturnTest();
  virtual const AGString &virtReturnTest2() const;
	void callVirt();
};

class C:public A
{
	public:
  C(const AGString &a);
  virtual AGString virtReturnTest();
};

#endif
