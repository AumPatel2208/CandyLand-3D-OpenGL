# Computer Graphics Coursework

## Questions to Ask

- [ ] Quad texture being flipped
- [ ] Quad textures for hud not rendering on top of each other

Talk to in private to fix it.







Marking: 
The coursework is marked out of 100%, with the final mark scaled to 75% for the module.

1. #### Deliverables -- Project report and source code (15%):
   
    -  Prepare a project report (maximum 15 pages) in Word or PDF format. (10%) 
        - Overview of your project; a description of your concept and game or application.
        - Include a prototype sketch or annotated screengrab showing a top-down view of your scene.
        - Provide a table to identify all user interface (keyboard / mouse) controls. 
        - For each section (Parts 2 - 5 below), list briefly the requirements that were implemented and relevant details on the implementation. If a requirement was skipped, so.
    -  List your scene’s assets, including any websites where objects were downloaded – URL, date of download, and license for use. Reference the source of all meshes used in your coursework, including original files you designed (if relevant). Document any process used to convert to the meshes in the final deliverable. Reference any external source code used.
    o Include a discussion section reflecting on the project. Consider the strengths and weaknesses of the game implemented and what you have accomplished in the time provided. Also discuss what would be required to expand the project into a more complete game.
    -  Source code to be commented and follow a logical design, organisation, and coding style (i.e., use of classes). (5%)
2. #### Route and camera (25%):
   
    -  Route (15%)
        - ~~Create a 3D non-linear centreline for your path, based on splines with C1 or higher continuity.~~ 
        - ~~Create primitives based on the centreline to generate a visualisation of the path/track on which the game or simulation will occur.~~ 
          - MAKE THE PRIMITIVES BE ALTERED BY TNB
        - ~~Ensure your primitives have correctly oriented normals and texture coordinates. Render the track with appropriate texturing and lighting.~~
        - ~~Note: the route must be generated programmatically, using OpenGL primitives. Building the path in 3D modelling software and loading it as a mesh is not permitted.~~
    -  ~~Camera / viewing (10%)~~ 
        - ~~The graphics template code allows the user to control camera movement in a “free view” mode. Retain this type of camera movement as an option (useful for debugging and visualisation).~~
        - ~~Implement at least two of the following camera modes:~~
           1. ~~First person: Add a keyboard control to place the camera so that it provides a first person view allowing the player to move along the route.~~ 
           2. ~~Third person: Add a keyboard control to place the camera either in or behind the player (rendered as a mesh).~~ 
           3. ~~Side view: Add a keyboard control to place the camera to the side of the player character (mesh) moving along the route.~~
           4. ~~Top view: Add a keyboard control to place the camera so that it provides a top view of your scene, but follows the player moving along the route.~~
              - ~~You will likely find it helpful to use a TNB frame for specifying the camera viewing geometr~~y.
3. #### Basic objects, meshes, and lighting (30%)
   
    -  Basic objects (10%)
        - ~~Create at least two different basic objects from primitives. Examples could include a cube, tetrahedron, torus, cylinder, cone, disk (other surfaces are possible). Render these objects using triangle primitive types (GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN). Note that the basic objects (plane, sphere) given in the template code do not count towards this requirement; nor do meshes loaded into the scene. However, shapes created in labs can count towards this section.~~
        -  ~~Apply appropriate texture coordinates and normals for your objects. Render these objects using texture mapping and lighting.~~ 
        -  ~~Transform the models to have an appropriate location, orientation and scale within the scene.~~
    -  ~~Meshes (10%)~~
        - ~~Load at least two different texture-mapped mesh models (with appropriate normals) to the scene using the model loader code provided in template code. Populate the scene with objects that match your theme. Note the mesh models (horse, barrel) already provided in the template do not count towards these four models.~~ 
        - ~~Transform the models to have an appropriate location, orientation and scale within the scene.~~
        - ~~Create parts of your scene using repeated mesh objects (e.g. rows of pickups, obstacles, etc.).~~ 
    -  Lighting (10%)
        - The OpenGL template has a general lighting for all objects. Implement at least one additional dynamic light in your scene. 
4. #### Head’s up display (HUD), gameplay, and advanced rendering (30%)
   
    -  HUD (5%)
       
        - Include a head’s up display to provide information to user (time, speed, score, fuel, place, etc.). 
    -  Gameplay (5%)
        - ~~Implement control of the player position on the track using the mouse or keyboard. Animate the player movement.~~
        - ~~Implement gameplay to make the game interesting to play. You’re free to design the gameplay as you wish, but it should not be trivial to play. Some suggestions include:~~
            - ~~Time-trial racing: The player seeks to get from the start to finish as fast as possible. Obstacles and/or speed-ups enhance gameplay.~~
            - ~~Points: The player seeks to achieve as high a score as possible, by collecting pick-ups on the track. Obstacles may cause damage or lower score.~~
            - ~~Shooter: The player defeats enemies by shooting at them.~~
        -  ~~Utilise basic collision detection using standard techniques (e.g., distance between player and object).~~
    -  Advanced rendering (20%)
        - Implement at least two advanced rendering techniques. Some ideas for this include the following:
            - Easier: 
                -  Blinking (e.g. Lab 1; or rendering/not rendering an object using a timer)
                -  Wobble
                -  ~~**Fog**~~
                -  Animation using discard in the fragment shader
                -  Toon shader
                -  Camera shake 
                -  Instanced rendering
                -  Multi-texturing
            - Harder:
                - Particle animation
                
                - Environment mapping
                
                - Bloom
                
                - **Blur** (regular, **radial**, motion)
	    
			    - Shadows 
			    
	    	    - Perlin (or related) noise
	    
			    - Bump mapping
	    
			    - Use of a geometry, tessellation, or compute shader
    
          

We will cover most of these in lecture / labs. Pick techniques that are interesting and feasible for your work. Nearly all of these methods are described in the reference textbooks for the module. If you have a different technique in mind, please speak to the module leader (Eddie).

-  In your report, be sure to describe the techniques implemented, and how they work.


5. #### Optional (unmarked)
   
    -  Include audio in your application to enhance the experience. 


### Presentation in Class (5 minutes)

-  You are to present your work during the lectures and lab sessions on Thursday 1st April. Clear explanations of your implementation will be expected.
-  The presentation is to be done in a Zoom class. This is your opportunity to show everyone what you have accomplished, and should be a fun experience for all!
-  The demo session will be marked on a similar marking schedule as the report except that section 1 will cover presentation (30%) instead of the report/code as there is no submission 
-  This will be marked out of 100% and scaled to 15% for the module.
-  You should aim to have all the deliverables for the coursework completed by the time of the demo.
-  Feedback will be provided rapidly and give an indication of progress at that point.
