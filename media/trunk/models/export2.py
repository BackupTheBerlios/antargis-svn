import Blender
import struct
from Blender import *
from Blender.Scene import *

print "Hello World: ", Blender

scn = Scene.GetCurrent()

filename="floor.ant2"

file=open(filename,"wb")
meshes=0
for obj in scn.getChildren():
	print obj.getType()
	if obj.getType()=="Mesh":
		meshes=meshes+1
file.write(struct.pack("H",meshes))
for obj in scn.getChildren():
	print obj.getType()
	if obj.getType()=="Mesh":
		nmesh=obj.getData()
		print "Mesh"
		i=0
		file.write(struct.pack("H",len(nmesh.faces)))
		for face in nmesh.faces:
			c=nmesh.materials[face.mat].rgbCol
			file.write(struct.pack("H",len(face.v)))
			for i in range(len(face.v)):
				v=face.v[i]
				uv=face.uv[i]
				file.write(struct.pack("fff",v.co[0],v.co[1],v.co[2])) # coords
				file.write(struct.pack("fff",v.no[0],v.no[1],v.no[2])) # normal
				file.write(struct.pack("fff",c[0],c[1],c[2])) # face color
				file.write(struct.pack("ff",uv[0],uv[1])) # uv coords
