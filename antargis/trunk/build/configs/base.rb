
#
# this is the reference implementation of a base-configuration file.
# it's meant to configure all needed build-processes on a unix-os for BoA.
# for other operation systems or compiler-systems please copy this file and
# change the calls.
# 

$config={

	"LINK"=>"$(CXX)",

  # call the compiler using the standard unix-style mechanism <CC> -c -o <outputname> <input0> [<input1> ...]
	"COMPILE_PARAMS"=>" -c -o $(OUTPUT) $(INPUT) -I#{Dir.pwd}/build/win32/usr/include",
	# an ansi-c compiler call with parameters (using ccache if available)
	"CC_CALL"=>"$(CCACHE) $(CC) $(COMPILE_PARAMS)",
	# a c++ compiler call with parameters (using ccache if available)
	"CXX_CALL"=>"$(CCACHE) $(CXX) $(COMPILE_PARAMS)",
	# build a shared library (.so or .dll)
	"LINK_SHARED"=>"$(LINK) -shared -o $(OUTPUT) -Wl,--enable-auto-image-base -Xlinker --out-implib -Xlinker $(OUTPUT).a  $(INPUT) $(LIBDIR) -Lext",

	# generate c++-code from a swig-interface file
	"SWIG_CALL"=>"$(SWIG) -v -Wall -ruby -c++ -o $(OUTPUT) -Ic:/antargis/rant/build/win32/usr/include $(INPUT) ",
	# generate dependencies for swig-interface files
	"SWIGDEPS"=>"$(SWIG) -ruby -c++ -M $(INPUT)",

	# generate a swig-interface-file from the c++-sources and some other pre-defined interface-files (.i)
	# go to build/*.i or ext/*/*.i for more information
	"CREATEINTERFACE"=>"$(RUBY) build/create_interface.rb -d=$(OUTPUT) -i=$(INPUT)"
}
