# Project: Asteroid Fields


## Getting Started


### Prerequisites

#### Open Scene Graph (OSG, Version > 3.2.1)
Windows:
* Download the libraries of OSG from here: http://objexx.com/OpenSceneGraph.html (debug and release if you want to build in both)
* Extract to `<path_to_osg>/` (I pasted release and debug in the same, since they use a "d" as postfix for each files => no problems with overwritting etc)
* Add path `<path_to_osg>/bin` to the PATH-environment-variable

Ubuntu:
* Install the libraries with "sudo apt-get install openscenegraph
 libopenscenegraph-dev libopenscenegraph100v5"
* For the examples one can use "sudo apt-get install openscenegraph-examples". These can then simply called by the command "osgteapot" for example.
* Includes should be at "/usr/include" and libraries at `/usr/lib/x86_64-linux-gnu` (at least for me.. ;-) )

### Boost (Version > 1.54)
Windows:
* Instructions: http://www.boost.org/doc/libs/1_65_1/more/getting_started/windows.html
* Set environment-variable `BOOST_ROOT` to `<path_to_boost>`

Ubuntu:
* Instructions: http://www.boost.org/doc/libs/1_65_1/more/getting_started/unix-variants.html
* Install the libraries with "sudo apt-get install libboost*" (except you know which you really need ;-) )

#### CGAL (Version 4.12-I-900 - Install from git-source)
Windows:
* Instructions: https://www.cgal.org/download/windows.html
* Download the installer `CGAL-4.11-Setup.exe` from here: https://github.com/CGAL/cgal/releases
* Install it to `<path_to_cgal>`
* Set environment-variable `CGAL_DIR` to `<path_to_cgal>`
* Add `<path_to_cgal>\auxiliary\gmp\lib` to the PATH-environment-variable 

Ubuntu:
* Instructions: https://www.cgal.org/download/linux.html
* Install the libraries with "sudo apt-get install libcgal*"
* If this works, fine, otherwise:
    * Download the source from git to `<path_to_cgal>`
    * Build it with make install in `<path_to_cgal>/build`

### Project-setup
I suggest to use the cmake-gui, since it most likely does not find the variables autoamtically, so it might be easier to set them correctly.

* Specify the location for source to "physSim/project/code" and for binaries to "physSim/project/code/build".
* Hit configure (Windows => make sure to select 64Bit-version)
* Verify or set these variables:
    * OPENSCENEGRAPH_INCLUDE_DIR:
        * Windows: `<path_to_osg>/include`
        * Ubuntu: `/usr/include`
    * OPENSCENEGRAPH_LIB_DIR:
        * Windows: `<path_to_osg>/lib`
        * Ubuntu: `/usr/lib/x86_64-linux-gnu` (for me)
    * BOOST_INCLUDE_DIR: (acvanced, when found => `Boost version: 1.65.1 Boost_FOUND=true.` is written in the output of cmake)
        * Windows: `<path_to_boost>`
        * Ubunutu: `/usr/include`
    * CGAL_DIR:
        * Windows: `<path_to_cgal>/build`
        * Ubuntu: `<path_to_cgal>/build`
* Generate

Open the project, build and run it and enjoy :-)
Verify that the path to the models in "config.h" is set to the correct folder (should already, but you never know).

## Structure 
I added two asteroid (3d obj models) and two planets (simple spheres). Both implemented in the classes Asteroid, and Planet respectively. So far it is really basic and I will continue with working on the look and feel of the whole scene => nicer background than violet and so on :-)

The structure is meant to be like this:
* Core: main loop, path config, ...
* Graphics: All graphics calculations (convex-hull, minkowski-sum, ...)
* OSG: Using and setup of OSG. All rendering-relevant parts are included here as well classes to work with OSG.
* OSG/Visitors: All visitors which are used for OSG to traverse the graph to make calculations/changes on the nodes
* Physics: All physics calculations (Collision-handling, NBody-handling, ...)
* Scene: Used to build and manage the scene and the objects which are currently on the scene.


<img src="version_1.png">