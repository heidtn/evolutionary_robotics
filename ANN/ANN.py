import sys
sys.path.append("../hill_climber")
import hill_climber as hc
import math
import matplotlib.pyplot as plt
import random

def Update(neuronValues, synapses, i):

	for j in xrange(len(neuronValues[i])):
		for k in xrange(len(neuronValues[i - 1])):
			neuronValues[i][j] += neuronValues[i - 1][k]*synapses[j][k]
		if neuronValues[i][j] > 1.0:
			neuronValues[i][j] = 1.0
		elif neuronValues[i][j] < 0:
			neuronValues[i][j] = 0.0

	return neuronValues


