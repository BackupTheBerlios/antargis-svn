class Property
	attr_accessor :type,:name,:value
	def initialize(type,name,value)
		@type=type
		@name=name
		@value=value
	end
	def merge!(p)
		case @type
			when :optionList
				@value+=" "+p.value
			else
				@value=p.value
		end
	end

	def toRubyPriv
		case @value
			when Array
				"'#{@name}'=>['#{@value.join("','")}']"
			when Hash
				"'#{@name}'=>{#{@value.keys.map{|k|"'#{k}'=>'#{@value[k]}'"}.join(",")}}"
			else String
				"'#{@name}'=>'#{@value}'"
			
		end
	end
end

class Properties
	OPTION_LIST=["LDFLAGS","CFLAGS","INCLUDEPATH","LIBPATH","LIBS"]
	attr_reader :properties

	def initialize(config={})
		@properties={}
		config.each{|k,v|
			if OPTION_LIST.member?(k)
				@properties[k]=Property.new(:optionList,k,v)
			else
				@properties[k]=Property.new(:string,k,v)
			end
		}
	end
	def [](key)
		@properties[key]
	end

	def []=(a,b)
		b=Property.new(getType(a),a,b) unless b.is_a?(Property)
		if @properties[a]
			@properties[a].merge!(b)
		else
			@properties[a]=b
		end
	end

	def set!(a,b)
		b=Property.new(getType(a),a,b) unless b.is_a?(Property)
		@properties[a]=b
	end

	def merge!(b)
		if b.is_a?(Properties)
			b=b.properties
			b.each{|k,v|
				if @properties[k].nil?
					@properties[k]=v
				else
					@properties[k].merge!(v)
				end
			}
		else
			b.each{|k,v|
				prop=Property.new(getType(k),k,v)
				if @properties[k].nil?
					@properties[k]=prop
				else
					@properties[k].merge!(prop)
				end
			}
		end
		self
	end

	def getType(k)
		return :optionList if OPTION_LIST.member?(k)
		:string
	end

	def toRuby
		str=""
		str << "$CONFIG={\n  "
		str << @properties.values.map{|prop|prop.toRubyPriv}.join(",\n  ")

		str << "\n}"
		#str << "\nrequire 'pp'\npp config"
		str
	end
end
