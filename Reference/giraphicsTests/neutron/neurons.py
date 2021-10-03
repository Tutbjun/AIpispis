
import numpy as np
from ursina import *

app = Ursina()

#Mangler at definere en transformationsmatrice. Signalet skal også fungerer som en værdiændrer, derfor består den af en transformation af værdien i sig selv.
#Jeg fandt et program som gjorde noget lignende hvad vi ville have vores til(https://www.kdnuggets.com/2018/10/simple-neural-network-python.html), heri er der dog ikke eksistens af neuroner som sådan. 

def createGrid(count = 1000):
    #find singleDimLen = int(qubicRoot(count))
    #find spacing = (singleDimLen-1)^-1
    #create array of coordinates to fill []
    #for x dim
        #for y dim
            #for z dim
                #cordsToFill.append([x*spacing,y*spacing,z*spacing])
    #gridArray = cordsToFill
    return gridArray

def normalizeGrid2MeshSize(mesh,grid):
    #origoShift = [min(mesh.x's),min(mesh.y's),min(mesh.z's)]
    #maxCords = [max(mesh.x's),max(mesh.y's),max(mesh.z's)]
    #dimSpans = maxCords - origoShift
    #newGrid = grid*dimSpanse
    return newGrid

def checkWhetherPointIsInGrid(point, mesh):
    #TODO: find a method to determine whether a point is within the mesh
    return boolVal

def isolatePointsByObject(mesh,gridArray):
    #for point in gridArray #protip: reverse the order of this for-loop, cause then potentially removed values wont make issues ;)
        #if checkWhetherPointIsInGrid(point, mesh) != True:
            #remove point from gridArray
    return newGridArray

def chooseAtRandom(gridArray):
    #pick an ammount of neurons
    return neuronPoints

grid = createGrid()
grid = normalizeGrid2MeshSize(mesh,grid)
isolatedGrid = isolatePointsByObject(mesh,grid)
neuronPoints = chooseAtRandom(isolatedGrid)


#def update(): #En vigtig funktion som automatisk bliver kaldt


app.run()
