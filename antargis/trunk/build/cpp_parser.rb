#
# simple c(++) parser

class Parser
	def initialize
		@buf=""
		@lines=[]
	end

	def parse(content)
		@buf=content
		# first remove line-comments
		@buf.gsub!(/\/\/[^\n]*\n/,"")
		# remove multi-line-comments
		@buf.gsub!(/\/\*([^*]|\*[^\/])*\*\//,"")

		# remove preprocessor
		@buf=@buf.split("\n").collect{|l|l.gsub(/^#.*/,"")}.join("\n")

		@buf.gsub!("virtual","")
		@buf.gsub!(/(public|protected|private) *:/,"")

		@buf.gsub!(/ +/," ")
		@buf.gsub!(/\n+/,"\n")

# 		puts @buf
# 		puts "---"


 		#@buf.scan(/[^{;]*(\{[^}]*\})?;/) {|r|
 		@buf.scan(/(([^{;]|\n)*\{[^}]*\};|[^{;]*;)/) {|r|
 			puts "R:",r.join("||"),r.class
 		}

	end

	def output
		puts @buf
	end
end

f=File.open("ext/gui/ag_messageobject.h","r")
p=Parser.new
p.parse(f.read)
p.output