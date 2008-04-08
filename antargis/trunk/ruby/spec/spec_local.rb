require "ruby/spec/level_testing.rb"

describe "Localizer" do
  it "should work in Ruby" do
    _("Quit").should == "Beenden" if getLanguage=="de_DE"
  end
  it "a ruby one should work in C++,too"
end
