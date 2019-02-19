#!/usr/bin/env python

import time

print("Writing output...")

entry = 0
with open("test.log", "w") as text_file:
	while True:
		entry += 1
		text_file.write("This is entry %d!\n" % entry)
		text_file.write("  some more info")
		text_file.flush()
		time.sleep(1)
