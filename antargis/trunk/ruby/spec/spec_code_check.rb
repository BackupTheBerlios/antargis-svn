describe "The source code" do
  it "should have no mentioning of rant or Rantfile" do
    extensions=["rb","cc","c","h"]
    okdirs=["ruby","ext","starter"]
    (extensions.map{|e|Dir["**/*."+e]}+Dir["**/README"]).flatten.select{|filename|okdirs.inject(true){|a,b|a and filename=~/^#{b}/} and not filename=~/spec/}.each{|filename|
      file=File.open(filename)
      content=file.read
      file.close
      wanted=filename+" should not include rant/Rantfile"
      error=wanted
      if content=~/[^a-zA-Z][rR]ant(file)?[^a-zA-Z]/
        error=filename+" includes rant/Rantfile"
      end
      
      error.should == wanted
      #content.should_not =~/[rR]ant(file)?/
    }
  end
end