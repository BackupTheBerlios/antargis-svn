#!/usr/bin/env ruby

# tests xml-parser


require 'antargisgui.rb'

document=<<EOT
<?xml version="1.0"?>
<test>
	<a bla="x\\\" \\x">
		<b/>huhu
	</a>
</test>
EOT

p=Parser.new
p.parse(document)

# theme.xml

t=File.open("../data/theme.xml").read
#p.parse(t)

dp=DomParser.new

d=dp.parse(t) #document)

puts d.toString