#!/usr/bin/env ruby

require 'pp'

def hasTabs(f)
  f=File.open(f)
  r=(f.read=~/\t/)
  f.close
  r
end

def removeTabs(fn)
  f=File.open(fn)
  c=f.read
  f.close
  f=File.open(fn,"w")
  f.puts c.gsub("\t","  ")
  f.close
end


Dir["ruby/**/*.rb"].each{|f|
  removeTabs(f) if hasTabs(f)
 # pp f,hasTabs(f)
}
