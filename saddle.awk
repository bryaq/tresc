BEGIN{
	print "/* Generated automatically by saddle.awk; do not edit. */\n"
	print "const uchar saddle[] = {"
	pi4 = atan2(1, 1)	# pi/4
	for(i = 0; i <= 255; i++)
		if(i < 85)
			printf "	%d,\n", int(256 * sin(pi4 * (i + 0.5) / 32) - 0.5)
		else if(i < 170)
			printf "	%d,\n", int(-256 * sin(pi4 * (i + 0.5 - 170) / 32) - 0.5)
		else
			printf "	0,\n"
	print "};"
}
