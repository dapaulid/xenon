#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import codecs
import datetime

print("Writing output...")

total_lines = int(sys.argv[1]) if len(sys.argv) > 1 else 10
lines_per_sec = int(sys.argv[2]) if len(sys.argv) > 2 else 1

ts = datetime.datetime.now()
with codecs.open("static.log", "w", encoding='utf8') as text_file:
	for line in range(total_lines):
		text_file.write(u"%s\tThis is line number %d. Some utf-8: äöü\n" % (ts.isoformat(), line+1))
		ts = ts + datetime.timedelta(seconds=1.0/lines_per_sec)
	# end while
# end with

