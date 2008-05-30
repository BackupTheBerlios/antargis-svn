Dir.chdir(File.split(File.split(File.expand_path(__FILE__))[0])[0])

require 'pp'
require 'ruby/antargislib.rb'

#setDebugLevel(0)

#if Object.respond_to?(:define_cmethod)
  
  
  $MY_KERNEL=self
  def kernel
    $MY_KERNEL
  end
  class Cross
    @@called={}
    @@backtrace={}
    def initialize(target,function)
      @target=target
      @function=function
      @expected="#{target}.#{function}(.)"
    end
    def matches?(proc)      
      @proc = proc
      callName=@expected
      @@called[callName]=0
      
      if @target.is_a?(Class) and @target.singleton_methods.member?(@function.to_s)
        mclass=@target
        
        function=@function
        oldFunction=@target.method(@function)
        mclass.define_cmethod(@function) do 
          Cross.symCall(callName)
        end
        
        @proc.call
        mclass.define_cmethod(@function,oldFunction)
        
        return @@called[callName]>0
  
      end
      @callName=callName
      mclass=@target
      mclass=@target.class unless @target.is_a?(Class)
  
      oldFunction=mclass.instance_method(@function)
      mclass.send(:define_method,@function) do 
        Cross.symCall(callName) 
      end
      @proc.call
      mclass.send(:define_method,@function,oldFunction)
      @@called[callName]>0
    end
    def failure_message
      "expected #{@proc.inspect} to call #{@expected}"
    end
    def negative_failure_message
      bt=""
      bt=@@backtrace[@callName].join("\n") if @@backtrace[@callName] 
      "expected #{@proc.inspect} not to call #{@expected} BT:#{bt}"
    end
    def Cross.symCall(name)
      @@called[name]+=1
      @@backtrace[name]=caller
      nil
    end
  end
  
  def cross(target,function=nil)
      if function.nil? and target.is_a?(Symbol)
        function=target
        target=kernel
      end
  
    Cross.new(target,function)
  end
#end
  
  
class Observer
  attr_accessor :ok
  def initialize(object,methodName)
    @methodName=methodName
    @object=object
    @ok=false
  end
  def run
    method=@object.method(@methodName)
    this=self
    object=@object
    @object.class.send(:define_method,@methodName) {|*s|
      if self==object
        this.ok=true
        puts "MUH"
      end
      method.call(*s)
    } 
    yield
    @object.class.send(:define_method,@methodName,method)
    @ok
  end
  def isNotCalled
     @ok==false
  end
  def isCalled
    @ok==true
  end
end  

def observe(object,method,&block)
  observer=Observer.new(object,method)
  
  observer.run {block.call(observer)}
end

class Object
  def method_missing(name,*s)
    alt=name.to_s.gsub(/\?$/,"")
    if respond_to?(alt) and alt!=name
      self.send(alt,*s)
    else
      super
    end
    
  end
end

#class A
#  def b
#    puts "B"
#  end
#end
#
#a=A.new
#call=CalledIn.new(a,:b)
#call.run {}
