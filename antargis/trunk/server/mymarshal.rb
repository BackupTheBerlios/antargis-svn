
def getDescendantsOfClass(p)
	c=[]
	ObjectSpace.each_object(Class){|a|c.push(a) if a.ancestors.member?(p)}
	return c
end

def addMessageTypes
	desc=getDescendantsOfClass(Message).sort{|a,b|a.to_s<=>b.to_s}
	$messageType2ID={}
	$messageID2Type={}
	(0..(desc.length-1)).each{|id|
		d=desc[id]
		$messageType2ID[d]=id
		$messageID2Type[id]=d
		#puts "INIT:#{$messageID2Type[id]} #{id}"
	}
end

def parse(m)
	t=m.unpack("n")[0]
	c=$messageID2Type[t]
	c.parse(m)
end

addMessageTypes

def test
	m=LogonMessage.new("david","muh")
	d=m.to_m
	puts d
	d.each_byte{|b|
		print b,";"
	}
	puts
	m2=parse(d)
	puts m2
	puts m2.to_m
	
	
	o=parse(AMessage.new.to_m)
	puts o.class
end