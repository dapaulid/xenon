#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import codecs
import datetime

if not os.path.exists("generated"):
    os.makedirs("generated")
# end if

print("Writing output...")

total_lines = int(sys.argv[1]) if len(sys.argv) > 1 else 10
lines_per_sec = int(sys.argv[2]) if len(sys.argv) > 2 else 1

ts = datetime.datetime.now()
with codecs.open("generated/static.log", "w", encoding='utf8') as text_file:
	line = 0
	while line < total_lines:
		for i in range(lines_per_sec):
			text_file.write(u"%s\tThis is line number %d. Some utf-8: äöü\n" % ((ts + datetime.timedelta(milliseconds=i)).isoformat(), line+1))
			line += 1
		# end for
		ts += datetime.timedelta(seconds=1)
	# end while
# end with

