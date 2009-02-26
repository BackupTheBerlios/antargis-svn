require File.expand_path('../../ant_tools.rb',__FILE__)

def checkTabs(filename)
  file=File.open(filename)
  c=file.read
  file.close
  okfilename="#{filename} contains no tabs"
  result=okfilename
  result="#{filename} contains tabs" if c =~/\t/
  
  linenr=0
  numbers=c.split("\n").map{|line|[line=~/\t/,linenr+=1]}.select{|l|l[0]}.map{|l|l[1]}
  puts "failed line-numbers (#{filename}): #{numbers.join(",")}" if result!=okfilename
  
  result.should == okfilename
end

def checkDeletes(filename)
  c=File.load(filename)
  linenr=0
  ok="#{filename} contains no unreviewed deletes"
  result=ok
  c.split("\n").each{|line|
    linenr+=1
    #unless line=~/checked - no agrubyobject/
      line.gsub!(/\/\/.*/,"")
      if line=~/delete /
        puts "failed line-number (#{filename}) #{linenr}"
        result="#{filename} contains unreviewed deletes"
      end
    #end
  }
  result.should == ok
end

describe "Coding style C/++" do
  it "shouldn't include any TABs anymore" do
    (Dir["ext/**/*.cc"]+Dir["ext/**/*.h"]).select{|filename|not filename=~/swig/}.each {|filename|
      checkTabs(filename)
    }
  end
  it "should not have senseless comments (outcommenting)"
  it "should include decent documentation"
  it "should include author/modification date"
  it "should include GPL in all files"
  it "should not have any TODOs left"

  it "should not include any unreviewed delete-statements" do
    (Dir["ext/**/*.cc"]+Dir["ext/**/*.h"]).select{|filename|not filename=~/swig/}.each {|filename|
      checkDeletes(filename)
    }
  end
end

describe "Coding style Ruby" do
  it "shouldn't include any TABs anymore" do
    Dir["ruby/**/*.rb"].each{|filename|
      checkTabs(filename)
    }
  end
  it "should include decent documentation"
  it "should include author/modification date"
  it "should include GPL in all files"
end