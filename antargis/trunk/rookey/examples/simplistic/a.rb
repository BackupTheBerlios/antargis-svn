#!/usr/bin/env ruby

require 'test'

a=Test::A.new
if a.x=="A::x()"
  puts "Everythings fine"
else
  raise "Somethings gone wrong in creation of Test::A"
end

class B<Test::A
  def x
    super+"new"
  end
end

b=B.new
if b.x=="A::x()new"
  puts "Everythings fine"
else
  raise "Somethings gone wrong in creation of B"
end
