#
# This file runs all tests in "ruby/tests" and gives results on this
#
#

def runScript(s)
	r=`ruby #{s} 2>/dev/null`	
	status=$?
	#puts r
	#puts status
	if status!=0
		#raise "sth wrong"
	end
	status==0
end

tests=Dir["ruby/tests/*.rb"]

failed=[]

tests.each{|t|
	s=" "*(40-t.length)
	print "Running: "+t+s
	if not runScript(t)
		failed.push(t)
		puts "FAILED"
	else
		puts "OK"
	end

}

puts
puts "Successfully completed: #{tests.length-failed.length}"
puts "Failed tests          : #{failed.length}"