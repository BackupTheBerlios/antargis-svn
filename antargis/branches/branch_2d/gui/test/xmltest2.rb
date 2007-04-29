#!/usr/bin/env ruby

require 'antargisgui.rb'

doc=Document.new
root=doc.root
root.setName("muh")
a=root.addChild("a")
b=root.addChild("b")
c=root.getChildren
puts c
puts doc.toString

c.each{|n|
	puts n
	n.set("x","x")
}

puts doc.toString


