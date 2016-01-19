import hill_climber as hc
import matplotlib.pyplot as plt
import numpy as np


def PlotVectorAsLine(fits):
	#parent = hc.MatrixCreate(1, 50) 
	#parent = hc.MatrixRandomize(parent) 

	#fits, p = hc.RunGenerations(parent, 5000)

	plt.plot(fits[0])
	plt.show()


Genes = hc.MatrixCreate(50, 5000)

for i in xrange(5):
	parent = hc.MatrixCreate(1, 50) 
	parent = hc.MatrixRandomize(parent) 
	fts, p = hc.RunGenerations(parent, 5000, Genes)
	plt.plot(fts[0])

plt.show()

plt.imshow(Genes, cmap=plt.cm.gray, aspect='auto', interpolation='nearest')
plt.show()


#plt.barh(np.arange(50), p[0])
#plt.show()


