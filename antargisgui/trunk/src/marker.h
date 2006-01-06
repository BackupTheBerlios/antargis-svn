%exception AGRubyObject::AGRubyObject {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
}
%markfunc AGRubyObject "AGWidget_markfunc"
