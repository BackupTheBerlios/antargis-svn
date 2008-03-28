require 'pp'

def checkExe(exe)
  puts `#{exe}`
  $?.should == 0
end

def local(var)
  File.expand_path(var.my)
end

class FileInspector
  def initialize(file)
    @filename=file
    @mtime=File.mtime(file)
    sleep(2)
  end
  
  def touched?
    curtime=File.mtime(@filename)
    puts "#{@mtime}!=#{curtime}"
    @mtime!=curtime
  end
  def touch(file)
    checkExe("touch #{file}")
  end
end

def inspectFile(filename,&block)
  ins=FileInspector.new(filename)
  ins.instance_eval(&block)
  ins
end

class Rakefile
  def method_missing(name)
    checkExe("rake #{name}")
  end
end

class RakefileConfig
  
  class Var
    attr_reader :my
    def initialize(v,m=nil)
      @s=v.to_s
      @my=m
    end
    def to_s
      @s
    end
  end
  
  def initialize
    @tasks=[]
    @varcounter=0
  end
  
  def link_dll(what)
    var=makeVar
    swigname="File.split(#{what}.flatten.select{|f|f=~/swig/}[0])[1].sub(/_swig.*/,'')"
    @tasks << "#{var}=Rookey.link_dll("+swigname+",#{what})"
    Var.new(var)    
  end
  def link_exe(what)
    var=makeVar
    myFile=dummyFile
    @tasks << "#{var}=Rookey.link_exe('"+myFile+"',#{what})"
    Var.new(var,myFile)
  end
  
  def compile(files)
    var=makeVar
    @tasks << var+"=Rookey.compile(["+files.map{|f|f.is_a?(String)?"'#{f}'":f}.join(",")+"])"
    Var.new(var)
  end
  
  def swig(inputs)
    var=makeVar
    var2=makeVar
    swigName=dummyFile
    @tasks << var +"=Rookey.swig_interface('#{swigName}'=>["+inputs.map{|f|f.is_a?(String)?"'#{f}'":f}.join(",")+"])"
    @tasks << var2 +"=Rookey.swig('#{swigName}_swig.cc'=>["+var+"])"
    Var.new(var2)
  end
  
  def write
    puts "RUN"
    content=@tasks.join("\n")
    makeRakefile(content)
  end
  def default(r)
    @tasks << "task :default=>#{r}"
  end

private
  def dummyFile
    @varcounter+=1
    f="file#{@varcounter}"
    f
  end

  def makeVar
    @varcounter+=1
    "var#{@varcounter}"
  end
  
  def makeRakefile(content)
    fd=File.open("Rakefile","w")
    fd.puts "require '../tasks.rb'"
    fd.puts "require 'rake/clean.rb'"
    fd.puts content
    fd.close
  end
end

def rake(&block)
  rakefile=RakefileConfig.new
  #yield rakefile
  r=rakefile.instance_eval(&block)
  
  rakefile.default(r)
  rakefile.write
  Rakefile.new
end
