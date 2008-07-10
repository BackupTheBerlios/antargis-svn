def requireRelative(file,current)
  require File.join(File.split(current)[0],file)
end

class FalseClass
  def <=>(o)
    if o==true
      -1
    else
      0
    end
  end
end
class TrueClass
  def <=>(o)
    if o==false
      1
    else
      0
    end
  end
end

requireRelative("ruby_layouts.rb",__FILE__)