import random, copy

def CreateSynapses(numneurons):
	return [[random.uniform(1, -1) for i in xrange(numneurons)] for j in xrange(numneurons)]

def Fitness(numneurons, numiters, synapses, desiredneurons):
	neuronValues = [[0]*numneurons for i in xrange(numiters)]
	for i in xrange(len(neuronValues[0])):
		neuronValues[0][i] = .5
	for j in xrange(1, len(neuronValues)):
		neuronValues = Update(neuronValues, synapses, j)
	finalNueronValues = neuronValues[-1]

	return MeanDistance(finalNueronValues, desiredneurons)

def Fitness2(numneurons, numiters, synapses, desiredneurons):
	neuronValues = [[0]*numneurons for i in xrange(numiters)]
	for i in xrange(len(neuronValues[0])):
		neuronValues[0][i] = .5
	for j in xrange(1, len(neuronValues)):
		neuronValues = Update(neuronValues, synapses, j)

	return MeanChange(neuronValues)

def MeanChange(nueronValues):
	diff=0.0

	for i in range(1,9): 

	      for j in range(0,9):

	           diff=diff + abs(neuronValues[i][j]-neuronValues[i][j+1])

	           diff=diff + abs(neuronValues[i+1][j]-neuronValues[i][j]) 

	diff=diff/(2*8*9)

def Update(neuronValues, synapses, i):

	for j in xrange(len(neuronValues[i])):
		for k in xrange(len(neuronValues[i - 1])):
			neuronValues[i][j] += neuronValues[i - 1][k]*synapses[j][k]
		if neuronValues[i][j] > 1.0:
			neuronValues[i][j] = 1.0
		elif neuronValues[i][j] < 0:
			neuronValues[i][j] = 0.0

	return neuronValues

def MatrixPerturb(p, prob):
	c = copy.deepcopy(p)
	for i in xrange(len(c)):
		for j in xrange(len(c[0])):
			if prob > random.random():
				c[i][j] = random.uniform(1, -1)
	return c

def MeanDistance(curneurons, desiredneurons):
	return 1 - sum([(curneurons[i] - desiredneurons[i])**2 for i in xrange(len(curneurons))])/len(curneurons)