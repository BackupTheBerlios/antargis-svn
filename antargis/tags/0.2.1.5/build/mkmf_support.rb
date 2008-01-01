def checkMkmf(value,config)
	changed=false
		value=value.gsub(/\$\(([^)]*)\)/) do |str|
		inner=str[2..-2]
		changed=true
		config[inner]
	end
	return checkMkmf(value,config) if changed
	value
end
