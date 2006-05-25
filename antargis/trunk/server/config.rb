#!/usr/bin/env ruby

require 'md5.rb'

$PORT = 11111

def myhash(x)
	MD5.md5(x.to_s).to_s
end