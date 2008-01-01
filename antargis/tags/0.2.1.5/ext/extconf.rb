Dir.chdir(File.split(__FILE__)[0])

puts "running rake..."
require 'rake'
ARGV=[]
Rake.application.run
puts "creating makefile"

file=File.open("ext/Makefile","w")

file.puts "install:\n\techo 'ready.'\n"

file.close

exit 0
