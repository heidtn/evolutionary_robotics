import sys
sys.path.append("../hill_climber")
import hill_climber as hc
import math
import matplotlib.pyplot as plt
import random
import ANN

num_neurons = 10



neuronPositions = hc.MatrixCreate(2,num_neurons)


angle = 0.0 
angleUpdate = 2 * math.pi / num_neurons 

for i in range(0, num_neurons):
	x = math.sin(angle)
	y = math.cos(angle)

	angle = angle + angleUpdate

	neuronPositions[0][i] = x
	neuronPositions[1][i] = y



#plt.show()

synapses = [[random.uniform(1, -1) for i in xrange(num_neurons)] for j in xrange(num_neurons)]

for i in xrange(num_neurons):
	for j in xrange(num_neurons):
		if synapses[i][j] < 0:
			color = [0.8,0.8,0.8]
		else:
			color = [0,0,0]

		w = int(10 * abs(synapses[i][j])) + 1
		#plt.plot([neuronPositions[0][i], neuronPositions[0][j]], [neuronPositions[1][i], neuronPositions[1][j]], color=color, linewidth=w)

#plt.plot(neuronPositions[0], neuronPositions[1], 'ko',markerfacecolor=[1,1,1], markersize=18)
#plt.show()


neuronValues = hc.MatrixCreate(50, num_neurons)
for i in xrange(num_neurons):
	neuronValues[0][i] = random.random()
print neuronValues[0]

for i in xrange(1, 50):
	neuronValues = ANN.Update(neuronValues, synapses, i)

plt.imshow(neuronValues, cmap=plt.cm.gray, aspect='auto', interpolation='nearest')
plt.show()







