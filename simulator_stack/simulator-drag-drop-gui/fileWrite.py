fin = open("input.xml", "r")
fout = open("output.xml", "w")

robot = open("robot-tag.txt", "r")
fout.write(robot.read())
fout.write("\n")

counter = 1
for line in fin:
	if counter:
		counter = counter - 1
		continue
	else:
		fout.write(line)

fout.close()
fin.close()
robot.close()