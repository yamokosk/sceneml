# SceneML 1.2 Notes #

## Change Log ##
  * SceneML C++ library
    * Can now change a body's parent on the fly.
    * Scale parameter for geometries now works.
  * Matlab interface
    * sceneRender plays nicely with other figure windows now.
    * New Matlab function, sceneSetParent, to allow changing a parent's body on the fly.
    * sceneSetVars completely re-written. Now accepts both structures and cell matrices. Type 'help sceneSetVars' in Matlab for more information.

## Porting notes ##

## Annotated roadmap ##
  * New mesh attribute
    * scale parameter which would allow you to scale up or down a pre-existing mesh. For instance to account for units mis-match
  * Matlab interface
    * Augment the sceneSetVars command to accept different types of data structures. I have two ideas for this.
      * Structures such as myvars.var1 = pi, myvars.var2 = 2\*pi, ...
      * Sparse arrays where the index of non-zero arrays correspond to the variable ID.