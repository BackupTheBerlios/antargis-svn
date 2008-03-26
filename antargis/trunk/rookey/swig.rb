module Rookey
  class Swig
    def initialize(config,target=:ruby,cpp=true)
      @config=config
      @target=target
      @cpp=cpp
    end
    def swig(t,interface)
      options=[]
      program=@config["SWIG"]
      program||="swig"
      program="swig" if program==""
      options << "-#{@target}"
      options << "-c++" if @cpp
      swigOptions = @config["SWIG_OPTIONS"].split(" ")
      swigOptions||=[]
      includeDirs = @config["INCLUDEDIRS"].split(" ")
      includeDirs||=[]
      includes=includeDirs.map{|d|"-I"+d}
      options+=swigOptions+includes
      options << "-o "+t.name
      options << interface
      call=program+" "+options.join(" ")
      begin
        sh call
      rescue
        begin
          File.delete(t.name)
        rescue
        end
        raise
      end
      
      # write deps
      # rmove -o
      options=options.select{|op|not op=~/^-o/}
      depsfile=File.join(".deps",interface)
      depsdir=File.split(depsfile)[0]
      begin
        Dir.mkdir(depsdir)
      rescue
      end
      options=options[0..2]+["-o "+depsfile,"-M"]+options[3..-1]
      call=program+" "+options.join(" ")
      sh call
      
      puts    
    end
    
    def Swig.getDeps(interface)
      interface=[interface].flatten[0]
      Compiler.getDeps(interface.sub(/\.d$/,".i"))
    end
  end
end