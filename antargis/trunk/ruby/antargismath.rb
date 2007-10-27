@@programDir=Dir.pwd+"/ruby"
@@extDir=Dir.pwd+"/ext"
# add programdir to path
$:.push(@@programDir)
$:.push(@@extDir)


require 'ext/antargismath.so'
include Antargisbasic
include Antargismath

