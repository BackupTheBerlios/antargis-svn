require 'ruby/antargislib.rb'
require 'ruby/gui/ag_tools.rb'

describe "Array.shuffle" do
  it "should really shuffle an array (in a 100 trials)" do
    tries=100
    a=[1,2,3,4,5]
    while tries>0
      break if a!=a.shuffle
      tries-=1
    end
    tries.should >0
  end
end
