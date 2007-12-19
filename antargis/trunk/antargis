#!/usr/bin/env ruby
#
# This is a starter-program. It's run in a ruby-interpreter, because that's easiest.
# Because we have to load DLLs which may not be in the standard-search-path, we'll
# start another ruby-interpreter in a sub-process and before we do so, we set
# the LD_LIBRARY_PATH environment variable, that will add the extension-directory to
# the path
#

# the path, where this program lies (fully expanded)
base=File.expand_path(File.split(__FILE__)[0])

# for Debugging
require 'pp'
# for parsing the mkmf-CONFIG-global
require File.join(base,'build','mkmf_support.rb')
# get the actual mkmf-CONFIG
require 'mkmf'

# read-out where the ruby-interpreter is
rubyExe=checkMkmf("$(bindir)/$(ruby_install_name)",CONFIG)

# add the extension-path (for the non-extension dlls) to the ld-search-path
path=File.join(base,"ext")
seperator=":"
separator=";" if File.join("a","b")!="a/b"
old_path=ENV['LD_LIBRARY_PATH']||""
ENV['LD_LIBRARY_PATH']=(old_path.split(separator).map{|dir|File.expand_path(dir)}+[path]).uniq.join(separator)

# run ruby with path as an extension dir
call=rubyExe+" -I#{path} "+File.join(File.split(__FILE__)[0],'runAntargis.rb')

pp call,base,path,ENV
system call
