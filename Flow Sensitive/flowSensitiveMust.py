import os, sys, operator
from pprint import pprint

def findLayer(points_to, level, start):
	if(level == 0):
		return [start]
	overall = []
	for x in points_to[start]:
		overall += findLayer(points_to, level - 1, x)
	return list(set(overall))

def analyze(ops, points_to):
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
			points_to[child] = [rightOp]
		return True

	if countLevelLeft == 0 and countLevelRight == 0:
		for child in toWorkWith:
			points_to[child] = []
			for rStart in points_to[rightOp]:
				points_to[child] += rStart
		return True
	else:
		for child in toWorkWith:
			points_to[child] = []
			for rStart in findLayer(points_to, countLevelRight+1, rightOp):
				points_to[child] += rStart
		return True

def writeToGV(points_to):
	genDOTFile = open("analyses.gv", "a")
	genDOTFile.write("digraph analyses {\n");
	for k in points_to.keys():
		for v in points_to[k]:
			if(v != ""): genDOTFile.write("\t"+str(k)+" -> "+str(v)+";\n");
	genDOTFile.write("}\n");
	genDOTFile.close()

def genAliasPairs(points_to):
	for parent in points_to.keys():
		for child in points_to[parent]:
			print "(*"+parent+","+child+")",
		if(len(points_to[parent])): print
	print

def addEdge(cfg, i, j):
	if(i in cfg):
		cfg[i].append(j)
	else:
		cfg[i] = [j]

def getPointsToGraph(data, points_to):
	print "Contents\n~~~~~~~~"
	for line in data:
		sanitized = line.strip().split("= ")
		result = analyze(sanitized, points_to)
		if not result:
			print "Weird: "+line.strip()
		else:
			print line.strip()
	print "\nPoints-to Representation:\n~~~~~~~~"
	pprint(points_to)
	print "\nAlias Pairs: (Compact Representation)\n~~~~~~~~"
	genAliasPairs(points_to)
	return points_to

def merge(uptilNow, childSet):
	for x in childSet.keys():
		if(x not in uptilNow): uptilNow[x] = childSet[x]
		else: 
			temp = []
			for y in childSet[x]:
				if y in uptilNow[x]: temp.append(y)
			uptilNow[x] = temp

def analyzeLink(link, buildCFG, contents):
	if((link[link.index(':')+1:].strip().split(", "))[0] != ''): buildCFG[link[:link.index(':')]] = link[link.index(':')+1:].strip().split(", ")
	else: buildCFG[link[:link.index(':')]] = []
	contents[link[:link.index(':')]] = []

def main():
	try:
		os.remove("analyses.gv")
	except OSError:
		pass
	k = open(sys.argv[1], "r")
	entities = k.readline().strip().split(", ")
	print "Entities: "+str(entities)
	
	succs = {}
	preds = {}
	contents = {}

	assert k.readline().strip() == "startGraph"
	readFrontier = 0
	for link in k:
		if(readFrontier == 1): frontier = [link.strip()]; break
		if(link.strip() == "endGraph"): readFrontier = 1; continue
		analyzeLink(link, succs, contents)


	preds = dict([(x, []) for x in succs.keys()])

	for x in succs.keys():
		for y in succs[x]:
				preds[y].append(x)

	blockName = ""
	readBlockFlag = 0

	for line in k:
		if(readBlockFlag): blockName = line.strip(); readBlockFlag = 0; continue
		if(line.strip() == "startBlock"): readBlockFlag = 1; continue
		if(line.strip() == "endBlock"): continue
		contents[blockName].append(line.strip())

	print "\nSuccessors:\n-------"
	pprint(succs)
	print "\nPredecessors:\n-------"
	pprint(preds)
	print "\nContents:\n-------"
	pprint(contents)
	print "\n\nAnalysing Blocks:\n-------\n"

	level0 = dict([(x, dict([(y, []) for y in entities])) for x in succs.keys()])
	points_to = dict([(x, []) for x in entities])
	visited = dict([(x, 0) for x in succs.keys()])

	while(len(frontier)):
		print frontier
		current = frontier[0]
		frontier.remove(frontier[0])
		visited[current] += 1
		print "Analysing Block "+current+"...\n======\n"
		for kids in succs[current]:
			if kids not in frontier and visited[kids] <= 2: frontier.append(kids)
		if(len(preds[current]) == 0): level0[current] = getPointsToGraph(contents[current], points_to)
		else:
			uptilNow = {}
			for child in preds[current]:
				if(len(level0[child])): merge(uptilNow, level0[child])
				else: uptilNow = dict([(x, []) for x in entities]); break
			level0[current] = getPointsToGraph(contents[current], uptilNow)
		print 
		writeToGV(level0[current])

	os.system("dot -Tpdf analyses.gv -o Summary.pdf")
	os.system("open Summary.pdf")

if __name__ == '__main__':
	if(len(sys.argv) != 2):
		print "\nUsage: python flowInsensitive.py <fileName>.txt\n"
		sys.exit(1)
	main()