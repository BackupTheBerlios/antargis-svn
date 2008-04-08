$swigVersion="1.3.34"
$swigDir=File.expand_path(File.join(File.split(__FILE__)[0],"swig","swig-#{$swigVersion}"))
$swigTarGz="#{$swigDir}.tar.gz"
$swigUrl="http://downloads.sourceforge.net/swig/"+$swigTarGz

$swigTarget=File.expand_path(File.join(File.split(__FILE__)[0],"built","swig"))