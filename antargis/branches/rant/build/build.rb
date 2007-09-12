def makeDepName(output)
	dep=".deps/"+output.gsub("/","_")
end


# build a command out of templates in config.rb
def makeCommand(cmd,output,input)
	#cmd.sub("§OUTPUT§",output).sub("§INPUT§",input)
	begin
		Dir.mkdir(".deps")
	rescue
	end
	dep=makeDepName(output)
	extendCommand($config,cmd,{"OUTPUT"=>output,"INPUT"=>input,"DEP"=>dep})
end

module Cmd
	@@quiet=false 
	def Cmd.sys(cmd)
		puts cmd if not @@quiet
		res=system(cmd)
		raise 1 unless res
	end

	def Cmd.setQuiet(flag)
		@@quiet=flag
	end
end


module Build
	include Cmd
	@@silent=false

	def self.silent=(flag)
		@@silent=flag
	end

	def Build.includes
		includes=Dir.glob(File.join("ext","**","*.h")).collect{|f|f.sub(/\/[^\/]*$/,"")}.uniq+[getConfig("archdir")]
		includes.collect{|i|"-I#{i}"}.join(" ")
	end

	def Build.cflags
		includes+" "+getConfig("CFLAGS")+" "+$config["CFLAGS"].to_s
	end

	def Build.compile(cFile)
		cObj=cFileToObj(cFile)
		cmd=makeCommand("CXX_CALL",cObj,cflags+" "+cFile)
		call(cmd)
	end

	def Build.link(target,objs,libs)
		cmd=makeCommand("LINK_EXE",target,(objs+libs).join(" "))
		call(cmd)
	end
	
	def Build.linkToLib(target,objs,libs)
		cmd=makeCommand("LINK_SHARED",target,(objs+libs).join(" "))
		call(cmd)
	end

	def Build.cFileToObj(filename)
		filename.sub(/\.cc$/,".oo").sub(/\.c$/,".o")
	end
private	
	def self.call(cmd)
		if @@silent
			cmd+=" 2>/dev/null"
			#puts cmd
			system(cmd)
		else
			Cmd.sys(cmd)
		end
	end
end

