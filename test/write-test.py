#!/usr/bin/env python

import os
import time

if not os.path.exists("generated"):
    os.makedirs("generated")
# end if

print("Writing output...")

entry = 0
with open("generated/test.log", "w") as text_file:
	while True:
		entry += 1
		if entry % 8 != 0:
			text_file.write("This is entry %d!\n" % entry)
			text_file.write("  some more info\n")
		else:
			text_file.write("Some warning happened here\n")
			text_file.write("Some error happened here\n")
		# end if
		text_file.flush()
		time.sleep(1)
	# end while
# end with

