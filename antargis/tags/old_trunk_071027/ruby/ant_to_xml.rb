require 'libantargis.so'
include Libantargis

# def addToXmlMarshal
# 	puts caller
# 	exit
# 	XmlMarshal.addClass(caller)
# end
# 
# class Integer
# 	addToXmlMarshal
# 
# 	def to_xml(parentNode,myName)
# 		n=parentNode.addChild("Integer")
# 		n.set("name",myName)
# 		n.set("value",to_s)
# 		n
# 	end
# 	def from_xml(node)
# 		self=node.get("value").to_i
# 	end
# end
# 
# class Float
# 	def to_xml(parentNode,myName)
# 		n=parentNode.addChild("Integer")
# 		n.set("name",myName)
# 		n.set("value",to_s)
# 		n
# 	end
# 	def from_xml(node)
# 		self=node.get("value").to_f
# 	end
# end
# 
# 
# class String
# 	def to_xml(parentNode,myName)
# 		n=parentNode.addChild("String")
# 		n.set("name",myName)
# 		n.set("value",self)
# 		n
# 	end
# 
# 	def from_xml(node)
# 		self=node.get("value")
# 	end
# end

# class Array
# 	def to_xml(parentNode,myName)
# 		n=parentNode.addChild("String")
# 		n.set("name",myName)
# 		n.set("value",self)
# 		n
# 	end
# 
# 	def from_xml(node)
# 		self=node.get("value")
# 	end
# end



class XmlMarshal
	@@classes=[]
	
	def XmlMarshal.dump(v,node)
		node.set("type",v.class.to_s)
		if v.class.method_defined?(:to_xml)
			v.to_xml(node,"")
		else
			case v
				when Numeric,String
					node.set("value",v.to_s)
				when Array
					v.each{|i|
						n=node.addChild("element")
						XmlMarshal.dump(i,n)
					}
				when Hash
					v.each{|a,b|
						p=node.addChild("pair")
						k=p.addChild("key")
						m=p.addChild("value")
						XmlMarshal.dump(a,k)
						XmlMarshal.dump(b,m)
					}
			end
		end
	end
	def XmlMarshal.load(node)
		v=nil
		type=node.get("type")
		cl=ObjectSpace.const_get(type)
		if cl.method_defined?(:from_xml)
			v=cl.from_xml(node)
		else
			case type
				when "Fixnum","Bignum"
					v=node.get("value").to_i
				when "Float"
					v=node.get("value").to_f
				when "String"
					v=node.get("value").to_s
				when "Hash"
					puts ":"+type
					v={}
					node.getChildren("pair").each{|p|
						ks=p.getChildren("key")
						vs=p.getChildren("value")
						if ks.length==1 and vs.length==1
							k=XmlMarshal.load(ks[0])
							m=XmlMarshal.load(vs[0])
							v[k]=m
						end
						
					}
				when "Array"
					v=[]
					node.getChildren("element").each{|e|
						m=XmlMarshal.load(e)
						v.push(m)
					}
			end
		end
		v
	end
end


def printDbgMy(v)
	case v
		when String
			'"'+v+'"'
		when Numeric
			v.to_s
		when Array
			"["+v.collect{|i|printDbgMy(i)}.join(",")+"]"
		when Hash
			"{"+v.collect{|k,v|printDbgMy(k)+"=>"+printDbgMy(v)}.join(",")+"}"
	end
end

def printDbg(v)
	puts printDbgMy(v)
end

def test
	doc=Document.new

	root=doc.root
	
	root.setName("root_a")
	a={"a"=>[1,2,3],"huhu"=>["hallo",2.222]}

	XmlMarshal.dump(a,root)


	b=XmlMarshal.load(root)


	puts root.toString


	#puts b
	printDbg(a)
	printDbg(b)

end

#test