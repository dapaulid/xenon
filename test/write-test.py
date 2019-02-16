import time

print("Writing output...")

line = 0
with open("test.log", "w") as text_file:
	while True:
		line += 1
		text_file.write("This is line %d!\n" % line)
		text_file.flush()
		time.sleep(1)
