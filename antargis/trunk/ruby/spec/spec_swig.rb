require 'ruby/spec/level_testing.rb'


describe "swig-mapping" do
  include LevelTesting
  it "should work correctly on meshlists" do
    getTestApp(:tutorial0)
#    map=AntRubyMap.new
    tower=AntTower.new(map)
    puts "::GETMESH"
    l=tower.getMesh
    l.should be_an_instance_of(Array)
    l.length.should == 3
    l.each{|node|
      #pp node,node.class,node.class.ancestors
      node.should_not be_an_instance_of(SceneNode)
      node.should be_a_kind_of(SceneNode)
    }
  end
end