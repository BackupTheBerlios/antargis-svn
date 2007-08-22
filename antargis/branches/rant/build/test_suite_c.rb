require 'build/base_tools.rb'
module Testing
	@@stats={}
	def Testing.runCTest(file)
		@@stats={}

		puts "testing #{file}"
		functions=extractFunctions(file)

		writeInterfaceFile(functions)
		files=[interfaceFile,file]
		files.each{|f|Build.compile(f)}
		objs=files.collect{|f|Build.cFileToObj(f)}

		Build.linkToLib(libName,objs,extLibs)
		runTest(functions)

	end

	def Testing.getStats
		@@stats
	end

private

	def Testing.initRunEnvironment
		if ENV["WINDIR"] and false
      ENV["PATH"]+=";.\\ext"
    else
      ENV["PATH"]+=":./ext"
		end
		$:.push("ext")
		ENV["LD_LIBRARY_PATH"]||=""
		ENV["LD_LIBRARY_PATH"]+="./ext/"
	end
	def Testing.runTest(functions)
		initRunEnvironment

		funcCalls=functions.map{|f|"result[\"#{f}\"]=#{f}"}.join("\n")

		script=<<EOT
require '#{libName}'
include #{moduleName}
result={}
#{funcCalls}
result.each{|k,v|
	puts "TESTING:"+k.to_s+":"+v.to_s
}
EOT
		f=File.open("curtest.rb","w")
		f.puts script
		f.close
		output=`ruby curtest.rb`

		result=output.split("\n").select{|l|l=~/^TESTING/}.map{|l|l.gsub("TESTING:","").split(":")}

		result.each{|a|
			puts a[0]+":::"+a[1]
			k=a[0]
			v=a[1]=="true"
	
			@@stats[v]||=0
			@@stats[v]+=1
		}
		
		puts output
	end

	def Testing.extractFunctions(file)
		content=File.open(file,"r").read

		lines=content.split("\n").select{|line|line=~/bool +test_[a-zA-Z_0-9]+\(\)/}
		functions=lines.collect{|line|line.gsub(/.* test/,"test").gsub(/\(.*/,"")}
		functions
	end

	def Testing.extLibs
		#return []
		Dir["ext/*.so"].select{|f|not f=~/lib/}.collect{|f|f.gsub(/^ext\/(.*)\.so/,'-l\1')}-["-lanttesting"]
	end

	def Testing.libName
		"ext/anttesting.so"
	end

	def Testing.moduleName
		"Anttesting"
	end

	def Testing.interfaceFile
		"tmpInterface.cc"
	end

	def Testing.writeInterfaceFile(functions)
		f=File.open(interfaceFile,"w")
		writeInterface(functions,f)
		f.close
	end

	def Testing.writeInterface(functions,fileHandle)
		funcDeclarations=functions.collect{|f|"bool #{f}();"}.join("\n")
		wrappers=functions.collect{|f|"VALUE wrap#{f}(ANYARGS)\n{\n  bool r=#{f}();\n  return (r?Qtrue:Qfalse);\n}"}.join("\n")
		funcDefinitions=functions.collect{|f|"rb_define_module_function(mModule,\"#{f}\",&wrap#{f},0);"}.join("\n")

		fileHandle.puts <<EOT

#include <ruby.h>
#include <stdio.h>

#{funcDeclarations}

#{wrappers}
extern "C" {
void Init_#{moduleName.downcase}()
{
	VALUE mModule = rb_define_module("#{moduleName}");

	#{funcDefinitions}
}
}
EOT
	
	end
end