describe "Coding style C/++" do
  it "shouldn't include any TABs anymore" do
    (Dir["ext/**/*.cc"]+Dir["ext/**/*.h"]).select{|filename|not filename=~/swig/}.each {|filename|
      file=File.open(filename)
      c=file.read
      file.close
      okfilename="#{filename} contains no tabs"
      result=okfilename
      result="#{filename} contains tabs" if c =~/\t/
      
      linenr=0
      numbers=c.split("\n").map{|line|[line=~/\t/,linenr+=1]}.select{|l|l[0]}.map{|l|l[1]}
      puts "failed line-numbers (#{filename}): #{numbers.join(",")}"
      
      result.should == okfilename
    }
  end
  it "should not have senseless comments (outcommenting)"
  it "should include decent documentation"
  it "should include author/modification date"
  it "should include GPL in all files"
  it "should not have any TODOs left"
end

describe "Coding style Ruby" do
  it "shouldn't include any TABs anymore"
  it "should include decent documentation"
  it "should include author/modification date"
  it "should include GPL in all files"
end