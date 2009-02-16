class GenericLayoutCreator<AGLayoutCreator
  def initialize(c)
    super()
    @c=c
  end
  def create(p,r,node)
    options=node.getAttributes
    pp @c
    setResult @c.new(p,r,options)
  end
end

def standardLayoutCreator(pClass)
  name=pClass.to_s
  name=name[0..0].downcase+name[1..-1]
  creator=GenericLayoutCreator.new(pClass)
  getLayoutFactory.addCreator(name,creator)
end

require "./effect.rb"