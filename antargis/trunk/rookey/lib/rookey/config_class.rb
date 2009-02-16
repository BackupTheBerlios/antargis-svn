module Rookey
  
  # Rookeys Config-hash. 
  class Config
    def initialize(hash={})
      @hash=hash
    end
    def [](name)
      @hash[name]||""
    end
    def []=(name,value)
      @hash[name]=value
    end
    def add(name,value)
      @hash[name]||=""
      @hash[name]+=" "+value
    end
  end
  @@config=nil
  
  def Rookey.getConfig
    if @@config.nil?
      if File.exists?(Rookey::CONFIG_FILENAME)
        load Rookey::CONFIG_FILENAME
      else 
        runConfigure
        hibe(@@config,Rookey::CONFIG_FILENAME)
      end
      CLEAN << Rookey::CONFIG_FILENAME
    end
    @@config
  end 
end