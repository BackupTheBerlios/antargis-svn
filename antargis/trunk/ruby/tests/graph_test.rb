#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

g=DecimatedGraph.new

n0=g.addNode(AGVector2.new(0,0))
n1=g.addNode(AGVector2.new(1,0))
n2=g.addNode(AGVector2.new(0,1))
n3=g.addNode(AGVector2.new(1,1))

e0=g.addEdge(n0,n1,2,2)
e1=g.addEdge(n1,n3,3,3)
e2=g.addEdge(n0,n2,2,2)
e2=g.addEdge(n2,n3,1,1)


g.print
puts "---"

g.decimate(0.25)
puts "---"

g.print

puts
puts "FIRST PART ok"




