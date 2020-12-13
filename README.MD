# UnknownEngine

Unkown Engine is our project for "Engines" subject at CITM's videogames design & development. 
This time, we will be working on the creation of the core of a videogames engine, 
focusing on basic level stuff in order to understand how a high level engine works.

## Team

* Jordi Pardo Gutiérrez  https://github.com/Jordi-Pardo
* Eudald Garrofé Flix	 https://github.com/Hevne

## Controls

### FBX Loading

Simply drag and drop your fbx into the viewport and it will load with its texture if it does have one.

### PNG & DDS Loading

When a gameobject is selected via Hierarchy drag and drop a texture into the viewport and it will
be blaced on the selected gameobject.

##Unselecting gameobjects

* Press T to unselect the currently selected gameobject.

### Top menus

* File: For exiting application.
* Help: Github related stuff.
* Settings: OpenGL settings + hardware specs, input logs, console, windows settings, plots of fps...
* Primitives: Menu for creating sphere, cones, cylinders and cubes.

### Camera

* F when gameobject is selected: The camera focuses on the object.
* Right Click: Rotates the camera.
* Alt + Right Click: Orbitates the camera.
* WASD: To move around the viewport.
* Scroll Up/Down: Zoom
* Holding Shift: Camera moves twice as fast!

### Hierarchy

Each time a gameobject is added into the engine, a item is created on the hierarchy window, from here
you can see its children and select the object.

* Double click on an item: Selects the item.
* Single click on an item: Unfolds its children.


### Inspector

Here, you can check information related to the game object mesh, material and transformation.


#### Mesh

* Click on Active checkbox: Disables mesh.
* Click on Show Normals Vertices: Shows the normal vertices.


#### Material

* Click on Active checkbox: Disables material.
* Click on UseCheckers button: Applies or removes the checker texture from the current gameobject.





