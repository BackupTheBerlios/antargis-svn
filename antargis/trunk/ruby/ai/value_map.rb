def mapValue01(v,min,max=1000.0)
  factor=1000
  n=Math.atan((v-min)/(max-min)*factor+0.0000001)/(Math::PI/2)
  n=[0,n].max
  #puts "#{v}:#{n}"
  n
end

def mapValueLinear(v,min,max)
  n=((v-min).to_f)/(max-min)
  puts "linearmPa:#{v} #{n} (#{min} #{max}"
  [0,n,1].sort[1]
end


def mapMinValues(values)
  count=values.length
  values2=values.map{|v|mapValue01(v,0)}
  r=values2.map{|v|v}.inject(0){|a,b|a+b*b}
  res=Math.sqrt(r)/Math.sqrt(count)
  res
end