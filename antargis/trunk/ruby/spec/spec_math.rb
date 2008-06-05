require 'ruby/spec_helper.rb'

class AGMatrix3
  def toString
    (0..2).map{|y|"("+(0..2).map{|x|get(x,y).to_s}.join(";")+")"}.join(",")
  end
end

def singleMatrix
  m=AGMatrix3.new
  m.set(0,0,1)
  m.set(1,0,0)
  m.set(2,0,0)
  m.set(0,1,0)
  m.set(1,1,1)
  m.set(2,1,0)
  m.set(0,2,0)
  m.set(1,2,0)
  m.set(2,2,1)
  m 
end

describe "Determinant" do
  def makeQuadMatrix(*s)
    len=s.length
    w=Math::sqrt(len)
    case w
      when 3
        m=AGMatrix3.new
      when 4
        m=AGMatrix4.new
      else
        m=AGMatrixN.new(w,w)
    end
    0.upto(w-1) {|x|
      0.upto(w-1) {|y|
        m.set(x,y,s[x+y*w])
      }
    }   
    m
  end
  it "should return 0 if matrix is lin.dep" do
    m=makeQuadMatrix(1,0,0,1,1,0,0,1,0)
    determinantAGMatrix3(m).should == 0
  end
  
  it "should work on a 3-dim matrix" do
    m=AGMatrix3.new
    m.set(0,0,2)
    m.set(1,0,1)
    m.set(2,0,1)
    m.set(0,1,1)
    m.set(1,1,2)
    m.set(2,1,1)
    m.set(0,2,1)
    m.set(1,2,1)
    m.set(2,2,2)
    result=determinantAGMatrix3(m)
    p result
    result.should>0
  end
end

describe "Gauss should work correctly" do
  def makeMatrix(str)
    y=0
    m=AGMatrix3.new
    str.gsub("(","").gsub(")","").split(",").each{|a|
      x=0
      a.split(";").each{|v|
        puts "#{x},#{y}:#{v}"
        m.set(x,y,v.to_f)
        x+=1
      }
      y+=1
      
    }
    m
  end
  def self.check(s)
    it "should work on #{s}" do
      m=makeMatrix(s)
      pp s
      pp m.toString
      i=m.inverted
      pp i.toString
      (m*i).should==singleMatrix
    end
  end
  
  check("((1;0;-9),(0;1;0),(0;0;1),)")
#  check("(1;0;-2),(0;1;0),(0;0;1)")
end


describe "AGMatrix3D(one with 0s on the main axis)" do
  it "should be invertable" do
    m=AGMatrix3.new
    m.set(0,0,0)
    m.set(1,0,1)
    m.set(2,0,0)
    m.set(0,1,0)
    m.set(1,1,0)
    m.set(2,1,1)
    m.set(0,2,1)
    m.set(1,2,0)
    m.set(2,2,0)
    (m.inverted*m).should == singleMatrix 
  end
end
