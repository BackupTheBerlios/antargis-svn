require 'ruby/ant_tools.rb'
require 'ruby/antargislib.rb'
require 'ruby/entities/entities.rb'
require 'ruby/map.rb'

(AntRubyEntity.descendants-[AntRubyEntity]).each{|aClass|
	describe aClass do
		it "should call AntRubyEntity.resourceChanged when resourceChanges is called on a descendant" do
			puts "TESTING #{aClass}"
			class AntRubyEntity
				alias :resourceChangedOld :resourceChanged
				def resourceChanged
					$ok=true
				end
			end
			$ok=false
			map=AntRubyMap.new(nil,nil,32,32)
			object=aClass.new #(AGVector2.new(0,0))
			object.setPos(AGVector2.new(1,1))
			object.resourceChanged
			class AntRubyEntity
				alias :resourceChanged :resourceChangedOld
			end
			$ok.should == true
			
		end
		it "should run without a scene-object defined"
	end
}