require 'ruby/spec_helper.rb'

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