import os, sys, operator

def sortMine(filename):
	pointFile = open(filename, 'r')
	newFile = open('processed.txt', 'w')
	newFile.write(pointFile.readline())
	toSort = []
	for line in pointFile:
		toSort.append(line.strip())
	newToSort = []
	toSort = list(set(toSort))
	for line in toSort:
		sanitized = line.strip().split("= ")
		leftOp = sanitized[0].strip()
		rightOp = sanitized[1].strip()
		countLevelLeft = leftOp.count('*')
		countLevelRight = rightOp.count('*') - rightOp.count('&')
		newToSort.append((line.strip(),countLevelLeft,countLevelRight))

	for newOrder in sorted(newToSort, key = lambda x : (x[1], x[2])):
		newFile.write(newOrder[0]+"\n")

	pointFile.close()
	newFile.close()

def findLayer(points_to, level, start):
	if(level == 0):
		return [start]
	overall = []
	for x in points_to[start]:
		overall += findLayer(points_to, level - 1, x)
	return list(set(overall))

def analyze(ops, points_to, constrained):
	leftOp = ops[0].strip()
	rightOp = ops[1].strip()
	countLevelLeft = leftOp.count('*')
	leftOp = leftOp.replace('*', '')
	countLevelRight = rightOp.count('*') - rightOp.count('&')
	rightOp = rightOp.replace('*', '')
	rightOp = rightOp.replace('&', '')
	toWorkWith = findLayer(points_to, countLevelLeft, leftOp)
	# print toWorkWith
	if countLevelRight < 0:
		for child in toWorkWith:
			if rightOp not in points_to[child]: points_to[child] += rightOp
			for temp in constrained[child]:
				if rightOp not in points_to[temp]: points_to[temp] += rightOp
		return True

	if countLevelLeft == 0 and countLevelRight == 0:
		for child in toWorkWith:
			for rStart in points_to[rightOp]:
				if rStart not in points_to[child]: points_to[child] += rStart
				for temp in constrained[child]:
					if rStart not in points_to[temp]: points_to[temp] += rStart
			if child not in constrained[rightOp]: constrained[rightOp] += child
		return True
	else:
		for child in toWorkWith:
			for rStart in findLayer(points_to, countLevelRight+1, rightOp):
				if rStart not in points_to[child]: points_to[child] += rStart
				for temp in constrained[child]:
					if rStart not in points_to[temp]: points_to[temp] += rStart
			for rStart in findLayer(points_to, countLevelRight, rightOp):
				if child not in constrained[rStart]: constrained[rStart] += child
		return True

def writeToGV(points_to):
	genDOTFile = open("analyses.gv", "a")
	genDOTFile.write("digraph analyses {\n");
	for k in points_to.keys():
		for v in points_to[k]:
			genDOTFile.write("\t"+k+" -> "+v+";\n");
	genDOTFile.write("}\n");
	genDOTFile.close()

def genAliasPairs(points_to):
	print "\n\n\nAlias Pairs: (Compact Representation)"
	print "-----------"
	for parent in points_to.keys():
		for child in points_to[parent]:
			print "(*"+parent+","+child+")",
		if len(points_to[parent]): print

def main():
	try:
		os.remove("analyses.gv")
		os.remove("processed.txt")
	except OSError:
		pass
	sortMine(sys.argv[1])
	a = open("processed.txt", "r")
	entities = a.readline().strip().split(", ")
	print "Entities: "+str(entities)
	points_to = dict([(x, []) for x in entities])
	constrained = dict([(x, []) for x in entities])
	print "\n\n"
	for line in a:
		sanitized = line.strip().split("= ")
		result = analyze(sanitized, points_to, constrained)
		if not result:
			print "Weird: "+line.strip()
		else:
			print line.strip()
			print "Points to: " +str(points_to)
			print "Constraint: " + str(constrained)
			print "\n"
		writeToGV(points_to)
	a.close()
	genAliasPairs(points_to)
	os.system("dot -Tpdf analyses.gv -o Summary.pdf")
	os.system("open Summary.pdf")

if __name__ == '__main__':
	if(len(sys.argv) != 2):
		print "\nUsage: python flowInsensitive.py <fileName>.txt\n"
		sys.exit(1)
	main()