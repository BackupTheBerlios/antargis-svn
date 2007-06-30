$config={
	"CXX"=>"$(g++)",
	"CC"=>"$(gcc)",
	"SWIG"=>"$(swig)",
	"RUBY"=>"$(ruby)",
	"CCACHE"=>"$(ccache)",

	"LINK"=>"$(CXX)",

	"COMPILE_PARAMS"=>" -c -o $(OUTPUT) $(INPUT)",
	"CC_CALL"=>"$(CCACHE) $(CC) $(COMPILE_PARAMS)",
	"CXX_CALL"=>"$(CCACHE) $(CXX) $(COMPILE_PARAMS)",
	"LINK_SHARED"=>"$(LINK) -shared -o $(OUTPUT) $(INPUT)",
	"SWIG_CALL"=>"$(SWIG) -v -Wall -ruby -c++ -o $(OUTPUT) $(INPUT)",
	"SWIGDEPS"=>"$(SWIG) -ruby -c++ -M $(INPUT)",
	"CREATEINTERFACE"=>"$(RUBY) build/create_interface.rb -d=$(OUTPUT) -i=$(INPUT)"
}
