SUBDIRS = include src bindings test

count:
	{ { find -name "*.cc" ; find -name "*.h"  ; } |grep -v swig|xargs cat ; } | wc
