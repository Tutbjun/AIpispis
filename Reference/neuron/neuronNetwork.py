import numpy as np
from ursina import *
import random
import time

from ursina.lights import AmbientLight


class Network():
    neuronEnts = []
    poses = []
    def __init__(self):
        pass

    def initConnects(self):
        for i in range(len(self.neuronEnts)):
            for j in range(len(self.neuronEnts[i].neuron.connects)):
                self.neuronEnts[i].conLines.append(Entity(model=Mesh(vertices=[self.neuronEnts[i].neuron.pos,self.poses[self.neuronEnts[i].neuron.connects[j]]],mode='line')))

    class NeuronEnt():
        neuron = 0
        ent = 0
        conLines = []
        def __init__(self,neuronArgs,entColor=color.blue):
            self.neuron = Network.Neuron(*neuronArgs)
            self.ent = Entity(model="sphere",color=entColor,scale=(0.1,0.1,0.1))
            self.ent.position = self.neuron.pos
            

    class Neuron():
        pos = np.zeros(3)
        ODval = 1
        connects = []
        #allPoses = []
        def __init__(self,pos,connects,OD):
            self.pos = pos
            self.connects = connects
            self.ODval = OD

def update():
    pass
    #time.sleep(1)

def initNetwork():
    poses = []
    for i in range(10):
        pos = []
        for j in range(3):
            pos.append(random.uniform(-1,1))
        poses.append(pos)
    connects = []
    for i in range(10):
        con = []
        for j in range(2):
            con.append(random.choice([k for k in range(0,10) if k is not i and k not in con]))
        connects.append(con)
    net = Network()
    for i in range(10):
        net.poses.append(poses[i])
        net.neuronEnts.append(Network.NeuronEnt((poses[i],connects[i],1)))
    
    net.poses.append(np.array([0,0,1]))
    net.poses.append(np.array([0,0,-1]))
    endNeurons = {}
    endNeurons['start'] = Network.NeuronEnt(([0,0,1],[],1),entColor=color.red)
    endNeurons['stop'] = Network.NeuronEnt(([0,0,-1],[],1),entColor=color.green)
    for k in endNeurons.keys():
        for i in range(5):
            endNeurons[k].neuron.connects.append(random.choice([j for j in range(0,10) if j not in endNeurons[k].neuron.connects]))
        net.neuronEnts.append(endNeurons[k])    
    net.initConnects()
    return net

def initCosmetics():
    fov = 40
    camera.orthographic =  True
    EditorCamera()
    
    PointLight(parent=Entity(),x=5,y=5,z=-5,shadows=True)
    #AmbientLight(shadows=True,color=(1,1,1,0.1))#PointLight(parent=Entity(),x=-1000,y=-1000,z=-500,shadows=True)

def startUp():
    network = initNetwork()
    app = Ursina()
    initCosmetics()
    app.run()

if __name__ == "__main__":
    startUp()