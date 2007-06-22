#!/usr/bin/env ruby

require 'ruby/antargismath.rb'

setDebugLevel(0)

def makeBasis
	m=AGMatrixN.new(2,8)
	
	a=[
	[1,1,1,1],
	[1,1,-1,-1],
	[1,-1,0,0],
	[0,0,1,-1],
	]

	a=[
	[1,1,1,1,1,1,1,1],
	[1,1,1,1,-1,-1,-1,-1],
	[1,1,-1,-1,0,0,0,0],
	[0,0,0,0,1,1,-1,-1],
	[-1,1,0,0,0,0,0,0],
	[0,0,-1,1,0,0,0,0],
	[0,0,0,0,-1,1,0,0],
	]

	
	(0..7).each{|x|
		(0..1).each{|y|
			m.set(y,x,a[y][x])
		}
	}
	m
end

def makeSample(w)
	v=AGMatrixN.new(1,w)
	(0..(w-1)).each{|c|
		v.set(0,c,rand)
	}
	v
end

def encode(basePair,v)
	inv=basePair[1]
	inv*v
end

def decode(basePair,encodedV)
	m=basePair[0]
	m*encodedV
end

basis=makeBasis

basis.output

inv=basis.pseudoInverse

basePair=[basis,inv]

inv.output

puts "CHECK unit-matrix:"
(basis*inv).output
#exit

sample=makeSample(8)

puts "sample:"
sample.output
puts "encoded:"
enc=encode(basePair,sample)
enc.output
puts "decoded:"
dec=decode(basePair,enc)
dec.output

puts "ERR:"
puts (dec-sample).scalar


err=0
err2=0
count=10000
(0..count).each{|i|
	sample=makeSample(8)
	sample2=makeSample(8)
	err2+=(sample-sample2).scalar
	enc=encode(basePair,sample)
	dec=decode(basePair,enc)
	c=(dec-sample).scalar
	#puts c
	err+=c
	
}

puts err/count
puts err2/count

