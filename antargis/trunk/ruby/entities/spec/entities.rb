require 'ruby/ant_tools.rb'
require 'ruby/antargislib.rb'
require 'ruby/entities/entities.rb'
require 'ruby/map.rb'

class AntRubyEntity
  alias :resourceChangedOld :resourceChanged
  def resourceChanged
    $ok=true
  end
end

(AntRubyEntity.descendants-[AntRubyEntity]).each{|aClass|
  describe aClass do
    it "(#{aClass}) should call AntRubyEntity.resourceChanged when resourceChanges is called on a descendant" do
      #puts "TESTING #{aClass}"
      $ok=false
      app=GLApp.new(800,600)
      scene=app.getScene
      map=AntRubyMap.new(app,scene,32,32)
      object=aClass.new(map)
      object.setPos(AGVector2.new(1,1))
      object.resourceChanged
      $ok.should == true
      
    end
    it "should run without a scene-object defined"
  end
}
