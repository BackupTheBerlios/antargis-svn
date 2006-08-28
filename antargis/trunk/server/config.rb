
require 'md5.rb'

# some default port for the game
$PORT = 11112

def myhash(x)
	MD5.md5(x.to_s).to_s
end


# if some thread aborts, everything should be aborted!
Thread.abort_on_exception=true