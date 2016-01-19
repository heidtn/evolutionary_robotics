import matplotlib
import random
import copy


def MatrixCreate(rows, columns):
	mat = []
	for i in xrange(rows):
		mat.append([0]*columns)
	return mat

def MatrixRandomize(v):
	for i in xrange(len(v)):
		for j in xrange(len(v[0])):
			v[i][j] = random.random()
	return v

def Fitness(v):
	total = 0.0
	for i in xrange(len(v)):
		for j in xrange(len(v[0])):
			total += v[i][j]

	num = len(v)*len(v[0])
	return total/num

def MatrixPerturb(p, prob):
	c = copy.deepcopy(p)
	for i in xrange(len(c)):
		for j in xrange(len(c[0])):
			if prob > random.random():
				c[i][j] = random.random()
	return c

def RunGenerations(parent, gens, Genes = None):
	parentFitness = Fitness(parent) 

	fits = MatrixCreate(1, gens)

	for currentGeneration in range(gens):
		print currentGeneration, parentFitness 
		fits[0][currentGeneration] = parentFitness
		
		child = MatrixPerturb(parent, 0.05) 
		childFitness = Fitness(child) 
		if childFitness > parentFitness:
				parent = child 
				parentFitness = childFitness
		if Genes is not None:
			for i in xrange(len(parent[0])):
				Genes[i][currentGeneration] = parent[0][i]

	return fits, parent


