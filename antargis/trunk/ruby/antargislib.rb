
if File.exists?("src/.libs/libantargis.so")
	require 'src/.libs/libantargis'
elsif File.exists?("../src/.libs/libantargis.so")
	require '../src/.libs/libantargis'
else
	require 'libantargis'
end
include Libantargis

require 'ag_tools'