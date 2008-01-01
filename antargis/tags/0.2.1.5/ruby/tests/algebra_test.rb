#!/usr/bin/env ruby

require 'ruby/antargisgui.rb'
# libantargis.so'
# include Libantargis

setDebugLevel(0)

def inverseTest
	m=AGMatrixN.new(4,4)
	
	a=
	[[1,-1,0,0],
	[0,1,-1,0],
	[0,0,1,-1],
	[1,0,0,1]]
	
	(0..3).each{|x|
		(0..3).each{|y|
			m.set(x,y,a[y][x])
		}
	}
	puts "M:"
	m.output
	
	x=m.inverse
	
	y=m*x
	puts "Y:"
	y.output
end

def multTest
	a=AGMatrixN.new(2,2)
	b=AGMatrixN.new(2,2)
	a.makeUnitMatrix
	b.makeUnitMatrix
	a.set(0,1,10)
	b.set(0,1,10)
	puts "a:"	
	a.output
	puts "b:"	
	b.output
	c=a*b
	puts "c:"	
	c.output
end

def invTest2
	a=AGMatrixN.new(3,3)
	a.set(0,0,1)
	a.set(0,1,1)
	a.set(1,0,1)
	a.set(2,2,2)
	puts "a:"
	a.output
	i=a.inverse
	puts "i:"
	i.output

	t=a*i
	puts "t:"
	t.output
end

#multTest
#invTest2
inverseTest