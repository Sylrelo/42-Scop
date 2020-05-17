## 42 - Scop

### Goal
> - Your goal is to create a small program that will show a 3D object stored in a .obj file. You will be at least in charge of parsing to obtain the requested rendering.
> - In a window, your 3D object will be displayed in perspective, rotate on itself around its main symmetrical axis. 
> - By using various colors, it must be possible to distinguish the various sides. The object can be moved on three axis, in both directions.
> - A texture must be applicable simply on the object when we press a dedicated
key, and the same key allows to go back to the different colors. A soft transition between the two is requested.

### Constraints
- C Language
- Modern OpenGL (min v4)
- Shaders
- No external library other than libc and libm, except for Window and Events handling
- School norm

------------
Here's the list of whats working and what not.

**Bold** elements are mandatory.

## Whats working
- **Shader parsing and building**
- **Object parsing** (**vertices**, texture coords, normals and **face**)
- **Face format** (**v**, v/vt, v//vn, v/vt/vn)
- **Object display**
- **Events (translation, rotation)**
- Cross-platform binaries (Linux, Windows)
- Diffuse lightning (non-smooth)
- Polygon triangulation

## Todo
- **Object center calculation -> rotation around center**
- **Texture display** (with / **without** UV mapping)
- **Mac binary**
- MTL parsing
- Texture parsing
- Camera movement (not only object)
- UI menu buttons
- Shadows

## Screenshot
![](https://i.imgur.com/4vxMdYb.png)
