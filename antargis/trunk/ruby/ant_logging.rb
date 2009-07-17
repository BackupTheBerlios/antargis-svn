class Log
  @@names=[:err,:debug,:warn,:out,:gc,:info,:trace]

  def self.method_missing(name,*a)

    name=:err unless @@names.member?(name)

    chan=::Antargis.send(name)
    chan<<caller[0]<<" "
    chan<<a.map{|x|x.to_s}.join(" ")<<Antargis.endl
  end
end