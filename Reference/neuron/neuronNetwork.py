import numpy as np
from ursina import *
from ursina import shaders
import random
import time


class NeuronEnt():
    neuron = 0
    ent = 0
    def __init__(self,neuronArgs):
        self.neuron = Neuron(*neuronArgs)
        self.ent = Entity(model="sphere",color=color.red,shader=shaders.basic_lighting_shader)
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
    fov = 40
    camera.orthographic =  True
    EditorCamera()
    app = Ursina()
    app.run()

if __name__ == "__main__":
    startUp()