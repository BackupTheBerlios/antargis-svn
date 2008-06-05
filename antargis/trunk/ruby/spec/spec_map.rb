require "ruby/spec/level_testing.rb"

describe AntRubyMap,"A BoA-Ruby-map" do
  it "should run without UI at all" do
    map=AntRubyMap.new(nil,nil,64,64)
    map.move(1)
  end
  it "should not include ant_mock.rb when calling without GUI"

  it "should be possible to create two maps concurrently (low prio)"
end
