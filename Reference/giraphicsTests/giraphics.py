from ursina import *
from anytree import Node
import os
from zipfile import ZipFile
import glob

os.chdir(os.path.dirname(__file__))

def recursive_search(folder,file_type, ignore=None):
    abspath = os.path.dirname(os.path.abspath(__file__))
    os.chdir(abspath)
    directory = "./"
    pathname = directory + "/**/*." + file_type
    files = glob.glob(pathname, recursive=True)
    files = [e for e in files if "./" + folder + "/" in e]
    files = ["/".join(".".join(e.split(".")[1:]).split("/")[1:]) for e in files]
    if ignore != None:
        files = [e for e in files if ignore not in e]
    return files

class subMesh():#burde have en enkelt root mesh, og så evt. nogle undermeshes som er med subMesh klassen
    rootMesh = 0
    subMeshes = []
    def __init__(self, rootMesh, submeshes):
        self.subMeshes = submeshes

def loadMultiMesh(path):
    with ZipFile(path, 'r') as zip:
        zip.printdir()
        zip.extractall()
        n = os.path.basename(path)
        n = n.split(".")[0:-1]
        subMeshPaths = []
        rootMeshPath = ""
        for p in os.listdir(n):
            if 
        return multMesh([load_model(name=os.path.join("bugObj","root","bug.obj"))])

ogMesh = loadMultiMesh(os.path.join(os.path.dirname(__file__),"bugObj.zip"))



ent = Entity(model=ogMesh,color=color.color(0, 0, 0.5))

def update():
    print("lmao")

app = Ursina()
app.run()