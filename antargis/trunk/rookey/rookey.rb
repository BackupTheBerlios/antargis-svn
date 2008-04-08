ALLCLEAN=[]

require File.join(File.split(__FILE__)[0],'tools.rb')

task :allclean=>:clean do
  ALLCLEAN.each{|f|
    begin
      File.delete(f)
    rescue
    end
  }
end
