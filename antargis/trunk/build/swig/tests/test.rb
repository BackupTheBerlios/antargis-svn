#!/usr/bin/env ruby

require 'test/unit'
require 'test.so'
require 'pp'
include Test

class B<C
	def virtReturnTest
		"muh"
	end
end

class MTest<Test::Unit::TestCase

	def test_main
		a=A.new("Hallo")
		a.callTest("111")
		assert(a.returnTest=="returnTest") 
		assert(a.virtReturnTest=="virtReturnTest")
		assert(a.virtReturnTest2=="Hallo")
		pp a.virtReturnTest
		
		b=B.new("Hallo")
		b.callTest("111")
		assert(b.virtReturnTest=="muh")
		pp b.returnTest
		pp b.virtReturnTest

		puts "a.callVirt:"
		a.callVirt
		puts "b.callVirt:"
		b.callVirt

		a.ptrTest(a)
	
	end
end