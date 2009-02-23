ALLCLEAN=[]

require File.join(File.split(File.expand_path(__FILE__))[0],'rookey','tools.rb')
require './rookey/tasks.rb'


task :allclean=>:clean do
  ALLCLEAN.each{|f|
    begin
      File.delete(f)
    rescue
    end
  }
end
