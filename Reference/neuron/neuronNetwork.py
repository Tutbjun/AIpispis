import numpy as np
from ursina import *
import random
import time


class NeuronEnt():
    neuron = 0
    ent = 0
    def __init__(self,neuronArgs):
        self.neuron = Neuron(*neuronArgs)
        self.ent = Entity(model="sphere",color=color.red)
        self.ent.position = self.neuron.pos

class Neuron():
    pos = np.zeros(3)
    ODval = 1
    connects = []
    allPoses = []

    def __init__(self,allPoses,selfIndex,connects,OD):
        self.allPoses = allPoses
        self.pos = allPoses[selfIndex]
        self.connects = connects
        self.ODval = OD

def update():
    pass
    #time.sleep(1)

def startUp():
    poses = []
    for i in range(10):
        pos = []
        for j in range(3):
            pos.append(random.uniform(-1,1))
        poses.append(pos)
    
    neus = []
    for i in range(10):
        neus.append(NeuronEnt((poses,i,[],1)))

    

    #EditorCamera()
    app = Ursina()
    PointLight(parent=Entity(),x=5,y=5,z=-5,shadows=True)
    PointLight(parent=Entity(),x=-100,y=-100,z=-50,shadows=True)
    app.run()

if __name__ == "__main__":
    startUp()