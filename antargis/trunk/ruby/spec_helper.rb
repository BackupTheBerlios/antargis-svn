Dir.chdir(File.split(File.split(File.expand_path(__FILE__))[0])[0])

require 'pp'
require 'ruby/antargislib.rb'

#if Object.respond_to?(:define_cmethod)
	
	class Object
		def define_cmethod(method_name, method = nil, &proc)
			if proc.nil?
				raise ArgumentError
			else
				method = proc
			end if method.nil?
		
			(class << self; self; end).module_eval do
				define_method( method_name.to_sym, method )
			end
		end
	end
	
	$MY_KERNEL=self
	def kernel
		$MY_KERNEL
	end
	class Cross
		@@called={}
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
			"expected #{@proc.inspect} not to call #{@expected}"
		end
		def Cross.symCall(name)
			@@called[name]+=1
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
