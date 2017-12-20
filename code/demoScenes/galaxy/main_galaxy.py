import numpy as np
import math
import random
import json

def rotation_matrix(axis, theta):
    """
    Return the rotation matrix associated with counterclockwise rotation about
    the given axis by theta radians.
    """
    axis = np.asarray(axis)
    axis = axis/math.sqrt(np.dot(axis, axis))
    a = math.cos(theta/2.0)
    b, c, d = -axis*math.sin(theta/2.0)
    aa, bb, cc, dd = a*a, b*b, c*c, d*d
    bc, ad, ac, ab, bd, cd = b*c, a*d, a*c, a*b, b*d, c*d
    return np.array([[aa+bb-cc-dd, 2*(bc+ad), 2*(bd-ac)],
                     [2*(bc-ad), aa+cc-bb-dd, 2*(cd+ab)],
                     [2*(bd+ac), 2*(cd-ab), aa+dd-bb-cc]])


def getVelocity(centerMass, objectCoordinates):
    v_norm = (1. * centerMass / ((objectCoordinates[0]**2 + objectCoordinates[1]**2)**0.5))**0.5

    r_norm = (objectCoordinates[0]**2 + objectCoordinates[1]**2)**0.5
    x_comp = objectCoordinates[0]/r_norm
    y_comp = objectCoordinates[1]/r_norm

    return (-y_comp*v_norm, x_comp*v_norm, 0)


ATTRACTOR_MASS = 100.0
ATTRACTOR_RADIUS = 0.000001

OBJECT_MASS = 0.001
OBJECT_RADIUS = 0.1

FIRST_RING = 2.
OBJECTS_PER_RING = 15
INTERRING_DISTANCE = 0.5
RING_NUMBER = 50

START_ANGLE = 0.
RING_ANGLE_OFFSET = 5.

mainDict = {}
mainDict["name"] = "Galaxy"
mainDict["id"] = "S9"
mainDict["objects"] = []

sun = {}
sun["id"] = "0"
sun["size"] = ATTRACTOR_RADIUS
sun["type"] = "sun"
sun["texture"] = "sunmap.jpg"
sun["bumpmap"] = "earth_normalmap_1024x512.jpg"
sun["mass"] = ATTRACTOR_MASS
sun["position"] = {
    "x": 0.0,
    "y": 0.0,
    "z": 0.0
}
sun["ratio"] = 1.0
sun["scaling"] = 1.0

sun["linearVelocity"] = {
    "x": 0.0,
    "y": 0.0,
    "z": 0.0
}
sun["angularVelocity"] = {
    "x": 0.0,
    "y": 0.0,
    "z": 0.0
}

sun["force"] = {
    "x": 0.0,
    "y": 0.0,
    "z": 0.0
}

sun["torque"] = {
    "x": 0.0,
    "y": 0.0,
    "z": 0.0
}

mainDict["objects"].append(sun)

currentID = 1
for ringIdx in range(RING_NUMBER):
    objectAngle = START_ANGLE + ringIdx * RING_ANGLE_OFFSET
    angleOffset = 2 * math.pi / OBJECTS_PER_RING
    objectRadius = FIRST_RING + ringIdx * INTERRING_DISTANCE

    rotation = rotation_matrix([1, 0, 0], 1.2)

    for objectIdx in range(OBJECTS_PER_RING):
        position = (objectRadius * math.cos(objectAngle + objectIdx * angleOffset), objectRadius * math.sin(objectAngle + objectIdx * angleOffset), 0)
        velocity = getVelocity(ATTRACTOR_MASS + ringIdx * OBJECTS_PER_RING * OBJECT_MASS, position)

        position = np.dot(rotation, position)
        velocity = np.dot(rotation, velocity)

        object = {
                  "id" : currentID,
                  "size" : OBJECT_RADIUS,
                  "type" : "asteroid",
                  "mass" : OBJECT_MASS,
                  "position" : {
                      "x" : position[0],
                      "y" : position[1],
                      "z" : position[2]
                  },
                  "ratio": 1.0,
                  "scaling": 0.0001 * random.randint(1,20 + ringIdx),
                  "linearVelocity": {
                      "x": velocity[0],
                      "y": velocity[1],
                      "z": velocity[2]
                  },
                  "angularVelocity": {
                      "x": random.random(),
                      "y": random.random(),
                      "z": random.random()
                  },
                  "force": {
                      "x": 0.0,
                      "y": 0.0,
                      "z": 0.0
                  },
                  "torque": {
                      "x": 0.0,
                      "y": 0.0,
                      "z": 0.0
                  },
				  "obj": "asteroid" + str(random.randint(1,30)) + ".obj",
                  "texture": "Am" + str(random.randint(1,15)) + ".jpg"
                }

        currentID += 1
        mainDict["objects"].append(object)

with  open("galaxy.json","w") as output:
    output.write(json.dumps(mainDict, indent=4, sort_keys=True))
