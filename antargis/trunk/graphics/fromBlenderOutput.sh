#!/bin/bash
for i in 1 2 3 4 5 6 7 8 ; {
	s=$(echo "/tmp/direction${i}_000$i.png")
	d=$(echo "man$i.png")
	echo "$s $d"
gimp --verbose -i -b "(fromBlender \"$s\" \"$d\")" '(gimp-quit 0)'
}
#convert -blue-primary 191,191 input.png output.png
