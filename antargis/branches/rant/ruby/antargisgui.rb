@@programDir=Dir.pwd+"/ruby"
@@extDir=Dir.pwd+"/ext"
# add programdir to path
$:.push(@@programDir)
$:.push(@@extDir)


require 'ext/antargisgui.so'
include Antargisgui
#require 'ext/antargisbasic.so'
include Antargisbasic
include Antargismath
include Antargisvideo

