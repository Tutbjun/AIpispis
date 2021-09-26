import numpy as np
from ursina import *
import random
import time
import threading

#TODO: signal arraysne bliver inititaliseret på en måde hvor der kommer alt for mange? pls fix. man kan se det på entitiesne også


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
        signalEnts = []
        def __init__(self,neuronArgs,entColor=color.blue):
            self.neuron = Network.Neuron(*neuronArgs)
            self.ent = Entity(model="sphere",color=entColor,scale=(0.1,0.1,0.1))
            self.ent.position = self.neuron.pos
        
        def updateSignalEnts(self):
            for i,e in enumerate(self.signalEnts):
                self.signalEnts[i] = None
                del self.signalEnts[i]
            self.signalEnts = []
            for sp in self.neuron.signalProgri:
                self.signalEnts.append([])
                for i,s in enumerate(sp):
                    self.signalEnts[-1].append(Entity(model="sphere",color=color.yellow,scale=(0.01,0.01,0.01)))
                    self.signalEnts[-1][-1].position = self.neuron.pos
        
        def updateSignalPos(self,network):
            for i,sp in enumerate(self.neuron.signalProgri):
                for j,s in enumerate(sp):
                    p1 = (1-s)*np.array(self.neuron.pos)
                    p2 = s*np.array(network.poses[self.neuron.connects[j]])
                    try:#!yikes kode, pls fix (men ikke højesteprioritet)
                        self.signalEnts[i][j].position = p1 + p2
                    except IndexError:
                        pass

    class Neuron():
        pos = np.zeros(3)
        ODval = 1
        val = 0
        connects = []
        connectStrength = []
        signalProgri = []
        def __init__(self,pos,connects,connectStrength,OD):
            self.pos = pos
            self.connects = connects
            self.ODval = OD
            self.connectStrength = connectStrength

gpuThread = 0

def GPUmanager(network):
    neuronSpeed = 0.1
    dt = 0.1
    def updateNetwork():
        for n in network.neuronEnts:#!does not seem to want to for loop after first network itteration
            n.updateSignalPos(net)
            if n.neuron.val >= n.neuron.ODval:
                if len(n.neuron.connects) > 0:
                    n.neuron.signalProgri = np.append(np.array(n.neuron.signalProgri).flatten(),np.zeros(len(n.neuron.connects)),axis=0)
                    n.neuron.signalProgri = np.reshape(n.neuron.signalProgri,(n.neuron.signalProgri.size//len(n.neuron.connects),len(n.neuron.connects)))
                else:
                    print("signal!")
                n.neuron.val -= n.neuron.ODval
                n.updateSignalEnts()
            if len(n.neuron.signalProgri) != 0:
                val2Add = dt*neuronSpeed
                for i,_ in list(enumerate(n.neuron.signalProgri))[::-1]:
                    n.neuron.signalProgri[i] = n.neuron.signalProgri[i] + val2Add #TODO: take into account length of neuron
                    endedSigs = np.array(np.nonzero(n.neuron.signalProgri[i] >= 1)).flatten()
                    if len(endedSigs) > 0:
                        if len(endedSigs) == len(n.neuron.signalProgri[i]):
                            n.neuron.signalProgri = np.delete(n.neuron.signalProgri,i,axis=0)
                        else:
                            n.neuron.signalProgri[i][endedSigs] = None#!may give error, never tested
                        valsToAdd = n.neuron.connectStrength[endedSigs]
                        indecies = np.array(n.neuron.connects)[endedSigs]
                        for k,j in enumerate(indecies):
                            network.neuronEnts[j].neuron.val += valsToAdd[k]
    while True:
        time.sleep(0.001)
        updateNetwork()

keyHeld = False
net = Network()
def update():
    global keyHeld
    if held_keys['enter'] or held_keys['space'] and not keyHeld:
        print("ja")
        net.neuronEnts[10].neuron.val = net.neuronEnts[10].neuron.ODval
        keyHeld = True
    elif not held_keys['enter'] and not held_keys['space']:
        keyHeld = False

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
    
    for i in range(10):
        net.poses.append(poses[i])
        net.neuronEnts.append(Network.NeuronEnt((poses[i],connects[i],np.ones(len(connects[0])),1)))
    
    net.poses.append(np.array([0,0,1]))
    net.poses.append(np.array([0,0,-1]))

    startBoi = Network.NeuronEnt(([0,0,1],[],[],1),entColor=color.green)
    startBoi.neuron.connects = random.choices([i for i in range(0,10)],k=5)
    startBoi.neuron.connectStrength = np.ones(5)
    net.neuronEnts.append(startBoi)
    endBoi = Network.NeuronEnt(([0,0,-1],[],[],1),entColor=color.red)
    net.neuronEnts.append(endBoi)
    for i in random.choices([i for i in range(0,10)],k=5):
        net.neuronEnts[i].neuron.connects.append(len(net.neuronEnts)-1)
        net.neuronEnts[i].neuron.connectStrength = np.append(net.neuronEnts[i].neuron.connectStrength,1)
    
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
    global gpuThread
    gpuThread = threading.Thread(target=GPUmanager,args=[network])
    gpuThread.start()
    app.run()

if __name__ == "__main__":
    startUp()