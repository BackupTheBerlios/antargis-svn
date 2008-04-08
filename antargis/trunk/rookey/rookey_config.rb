require 'pp'
require 'optparse'

module Rookey
  class Configure
    attr_accessor :programs, :input, :filename
    
    FILENAME="rookey_configuration.rb"
    
    attr_reader :options
    
    def self.load(filename=nil)
      filename||=FILENAME
      unless File.exists?(filename)
        if File.exists?("configure")
          ruby("configure")
        end
        unless File.exists?(filename)
          self.new.run
        end
      end 
      Kernel.load(filename)
    end
    def self.cleanTask
      ALLCLEAN << FILENAME
    end
    
    def initialize
      @programs=[]
      @input=nil
      @ons=[]
      @options={}
      @filename=FILENAME
    end
        
    def set(k,v)
      @options[k]=v
    end
    
	  def run
	    @input||=ARGV
	    options={}
	    opts=OptionParser.new do |opts|
	      opts.banner = "Usage: configure [options]"
	
	      opts.separator ""
	      opts.separator "Specific options:"
	
	      opts.on("-d", "--enable-debug",
	              "enable debugging") do 
	        @options[:debug]=true
	      end
	      opts.on("-g","--enable-gc-debug",
	              "enable debugging of garbage collector") do
	        @options[:gcdebug]=true
	      end
	
	      opts.on("-p","--enable-profile",
	              "enable c++ profiling") do 
	        @options[:profile]=true
	      end
	
	      opts.on("--disable-sdlconfig",
	              "do not use sdl-config for configuration") do 
	        @options[:use_sdlconfig]=false
	      end
	      opts.on("--disable-fallbacks",
	              "do not use fallback dirs for X11 and GL") do 
	        @options[:use_fallbacks]=false
	      end
	
	      opts.on("--prefix PREFIXDIR",
	              "set prefix directory PREFIXDIR") do |dir|
	        @options[:prefix]=dir
	      end

	      
	      opts.on("-v","--version",
	        "set release version of BoA") do |v|
	          @options[:version]=v
	      end
	
	      opts.on("--ext-config CONFIG",
	        "use user-defined config instead of ruby's mkmf","CONFIG maybe in mingw32 or unix") do |extconfig|
	          @options[:extconfig]=extconfig
	          @options[:use_mkmf]=false
	      end
        
	
	      @programs.each{|program|
	        opts.on("--path-"+program+" PATH",
	          "set path to program '#{program}'") do |d|
	          @options[program]=d
	        end
	      }
        if block_given?
        yield opts
        end

	    
		  end	        
      opts.parse!(@input)
      save
      @options
	  end
    private
    def save
      fd=File.open(@filename,"w")
      fd.puts "ROOKEY_CONFIG={"
      fd.puts @options.map{|k,v|
        ":#{k}=>'#{v}'"
      }.join(",\n")
      fd.puts "}"
      fd.close
      begin
        File.delete("config_cache.rb")
      rescue
      end
    end
  end
end