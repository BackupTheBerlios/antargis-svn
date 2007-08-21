def extendCommandLine(config,s)
  r=config[s]
  return r if r.nil?
  r.gsub(/\$\(([^\)]*)\)/) {|a|
		n=a[2..-2] # filter "$(xy..z)" to "xy..z"
		raise "Endless recursion in #{n}!" if n==s
		extendCommandLine(config,n)  # replace recursive values within this form: $(...)
	}
end

def extendCommand(config,name,map={})
	c=config.clone
	map.each{|k,v|
		c[k]=v
	}
	extendCommandLine(c,name)
end

# the getConfig function reads out compiler-information out of the mkmf-configuration variables - these are defined in a recursive manner
def getConfig(s)
	extendCommand(CONFIG,s)
end
