require 'build/base_tools.rb'
module Testing


	def Testing.runCTest(file)
		puts file
		functions=extractFunctions(file)

		writeInterfaceFile(functions)
		files=[interfaceFile,file]
		files.each{|f|Build.compile(f)}
		objs=files.collect{|f|Build.cFileToObj(f)}

		Build.linkToLib(libName,objs,[])

	end
private
	def Testing.extractFunctions(file)
		content=File.open(file,"r").read

		lines=content.split("\n").select{|line|line=~/int test_[a-zA-Z_0-9]+\(\)/}
		functions=lines.collect{|line|line.gsub(/.* test/,"test").gsub(/\(.*/,"")}
		puts functions.join("\n")
		functions
	end

	def Testing.libName
		"anttesting.so"
	end

	def Testing.moduleName
		"anttesting"
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
		funcDeclarations=functions.collect{|f|"int #{f}();"}.join("\n")
		wrappers=functions.collect{|f|"VALUE wrap#{f}(ANYARGS)\n{\n  VALUE r=rb_intern(\"\");\n  #{f}();\n  return r;\n}"}.join("\n")
		funcDefinitions=functions.collect{|f|"rb_define_module_function(mModule,\"#{f}\",&wrap#{f},0);"}.join("\n")

		fileHandle.puts <<EOT

#include <ruby.h>

#{funcDeclarations}

#{wrappers}

void Init_#{moduleName}()
{
	VALUE mModule = rb_define_module("#{moduleName}");

	#{funcDefinitions}
}
EOT
	
	end
end