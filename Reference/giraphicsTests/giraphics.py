from ursina import *
from anytree import Node

class multMesh():
    subMeshes = []
    def __init__(self, submeshes):
        for m in submeshes:
            self.subMeshes.append(m)

ogMesh = load_model(name="bug.obj")

ent = Entity(model=ogMesh,texture="grass",color=color.color(0, 0, 0.5))

def update():
    print("lmao")

app = Ursina()
app.run()