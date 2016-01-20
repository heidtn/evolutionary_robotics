import evolved_ann as ea
import sys
import matplotlib.pyplot as plt
#sys.path.append("../hill_climber")
#import hill_climber as hc

numneurons = 10

#create random synapses
synapses = ea.CreateSynapses(10)

#create desired neurons
desiredNeurons = [0.0]*10
for i in xrange(0, 10, 2):
	desiredNeurons[i] = 1



parent = synapses
parentFitness = ea.Fitness(10, 10, parent, desiredNeurons) 

#parent before optimization
neuronValues = [[0]*10 for i in xrange(10)]
for i in xrange(len(neuronValues[0])):
	neuronValues[0][i] = .5
for i in xrange(1, 10):
	ea.Update(neuronValues, parent, i)

plt.imshow(neuronValues, cmap=plt.cm.gray, aspect='auto', interpolation='nearest')
plt.show()

#find the optimal synapses
fitnessvec = []
for currentGeneration in range(0,5000):
	print currentGeneration, parentFitness 
	fitnessvec.append(parentFitness)
	child = ea.MatrixPerturb(parent,0.05) 
	childFitness = ea.Fitness(10, 10, child, desiredNeurons) 

	if ( childFitness > parentFitness ):
		parent = child 
		parentFitness = childFitness

#parent after optimization
neuronValues = [[0]*10 for i in xrange(10)]
for i in xrange(len(neuronValues[0])):
	neuronValues[0][i] = .5
for i in xrange(1, 10):
	neuronValues = ea.Update(neuronValues, parent, i)

plt.imshow(neuronValues, cmap=plt.cm.gray, aspect='auto', interpolation='nearest')
plt.show()

plt.plot(fitnessvec)
plt.show()



#same as before, but with a new fitness function
parent = ea.CreateSynapses(10)
parentFitness = ea.Fitness(10, 10, parent, desiredNeurons) 

neuronValues = [[0]*10 for i in xrange(10)]
for i in xrange(len(neuronValues[0])):
	neuronValues[0][i] = .5
for i in xrange(1, 10):
	ea.Update(neuronValues, parent, i)

plt.title("fitness2 before")
plt.imshow(neuronValues, cmap=plt.cm.gray, aspect='auto', interpolation='nearest')
plt.show()


fitnessvec = []
for currentGeneration in range(0,5000):
	print currentGeneration, parentFitness 
	fitnessvec.append(parentFitness)
	child = ea.MatrixPerturb(parent,0.05) 
	childFitness = ea.Fitness(10, 10, child, desiredNeurons) 

	if ( childFitness > parentFitness ):
		parent = child 
		parentFitness = childFitness

#parent after optimization
neuronValues = [[0]*10 for i in xrange(10)]
for i in xrange(len(neuronValues[0])):
	neuronValues[0][i] = .5
for i in xrange(1, 10):
	neuronValues = ea.Update(neuronValues, parent, i)

plt.imshow(neuronValues, cmap=plt.cm.gray, aspect='auto', interpolation='nearest')
plt.title("fitness2 after")
plt.show()

plt.plot(fitnessvec)
plt.title("fitness2 evolution")
plt.show()