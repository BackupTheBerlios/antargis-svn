# DOES NOT WORK !!!!

if false
  require 'ruby/antargislib.rb'



def doapp
	address=nil
	Module.new.module_eval{
		app=AGApplication.new #AGMessageObject.new #(nil,AGRect2.new(0,0,100,100)) #AGApplication.new
		address=getAddressOfRubyObject(app)
		app=nil
	}
    address
	#nil
end

describe "Garbage Collection" do
	it "should handle AGApplication (alone) correctly" do
		#setDebugLevel(0)
		address=0
		begin
			address=doapp
		end
		GC.enable
	    # allocate much mem (200 MB)
		text="a"*1000*1000*300
		GC.start
		rubyObjectExists(address).should be_false
		rubyObjectExists(getAddressOfRubyObject(getMain)).should be_true
	end
end

end