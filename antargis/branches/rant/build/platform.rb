class Dir
    @@dirSep="/"    
    @@dirSep="\\" if ENV['WINDIR']
	def Dir.separator
		@@dirSep
	end
end

if ENV['WINDIR']
    CONFIG={"archdir"=>Dir.pwd+"/build/win32/usr/lib"}
    
else
    require 'mkmf'
end