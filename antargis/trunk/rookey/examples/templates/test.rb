#!/usr/bin/env ruby

require 'templates'
require 'pp'

def printFormat(len,str)
  str=str.to_s
  str=str.gsub("\n","")
  str=str[0..(len-1)]
  str+=" "*(len-str.length)
  print str
end

def showLine(line)
  filename,line=line.split(":")
  line=line.to_i
  printFormat 80, File.open(filename).readlines[line-1]
end

$failed=false
def assert(&p)
  showLine(caller[0])
  if p.call
    puts "  ok"
  else
    puts "  failed"
    $failed=true
  end
end

parent=Templates::Parent.new

children=parent.getChildren
assert { children.length==3 }
puts "...Reseting parent"
parent=nil
GC.start
assert { children.length==3 }
assert { children[0].is_a?(Templates::Child) }

raise "Failed" if $failed