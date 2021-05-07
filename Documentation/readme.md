# Computer Graphics Coursework
Marking: 
The coursework is marked out of 100%, with the final mark scaled to 75% for the module.

1. #### Deliverables -- Project report and source code (15%):
   
    - [ ]  Prepare a project report (maximum 15 pages) in Word or PDF format. (10%) 
        - [ ] Overview of your project; a description of your concept and game or application.
        - [ ] Include a prototype sketch or annotated screengrab showing a top-down view of your scene.
        - [ ] Provide a table to identify all user interface (keyboard / mouse) controls. 
        - [ ] For each section (Parts 2 - 5 below), list briefly the requirements that were implemented and relevant details on the implementation. If a requirement was skipped, so.
    - [ ]  List your scene’s assets, including any websites where objects were downloaded – URL, date of download, and license for use. Reference the source of all meshes used in your coursework, including original files you designed (if relevant). Document any process used to convert to the meshes in the final deliverable. Reference any external source code used.
    o Include a discussion section reflecting on the project. Consider the strengths and weaknesses of the game implemented and what you have accomplished in the time provided. Also discuss what would be required to expand the project into a more complete game.
    - [ ]  Source code to be commented and follow a logical design, organisation, and coding style (i.e., use of classes). (5%)
2. #### Route and camera (25%):
   
    - [ ]  Route (15%)
    - [ ]  Camera / viewing (10%) 
    
3. #### Basic objects, meshes, and lighting (30%)
   
    - [ ]  Basic objects (10%)
        - Create at least two different basic objects from primitives. Examples could include a cube, tetrahedron, torus, cylinder, cone, disk (other surfaces are possible). Render these objects using triangle primitive types (GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN). Note that the basic objects (plane, sphere) given in the template code do not count towards this requirement; nor do meshes loaded into the scene. However, shapes created in labs can count towards this section.
        -  Apply appropriate texture coordinates and normal for your objects. Render these objects using texture mapping and lighting. 
        -  Transform the models to have an appropriate location, orientation and scale within the scene.
    - [ ]  Meshes (10%)
        - [ ] Load at least two different texture-mapped mesh models (with appropriate normals) to the scene using the model loader code provided in template code. Populate the scene with objects that match your theme. Note the mesh models (horse, barrel) already provided in the template do not count towards these four models. 
        - [ ] Transform the models to have an appropriate location, orientation and scale within the scene.
        - [ ] Create parts of your scene using repeated mesh objects (e.g. rows of pickups, obstacles, etc.). 
    - [ ]  Lighting (10%)
        - [ ] The OpenGL template has a general lighting for all objects. Implement at least one additional dynamic light in your scene. 
4. #### Head’s up display (HUD), gameplay, and advanced rendering (30%)

    - [ ]  HUD (5%)
      
        - [ ] Include a head’s up display to provide information to user (time, speed, score, fuel, place, etc.). 
    - [ ]  Gameplay (5%)
        - [ ] Implement control of the player position on the track using the mouse or keyboard. Animate the player movement.
            - [ ] ADD BOUNDARY LIMITS TO HOW FAR THE CAR CAN GO
        - [ ] Implement gameplay to make the game interesting to play. You’re free to design the gameplay as you wish, but it should not be trivial to play. Some suggestions include:
            - [ ] Time-trial racing: The player seeks to get from the start to finish as fast as possible. Obstacles and/or speed-ups enhance gameplay.
            - [ ] Points: The player seeks to achieve as high a score as possible, by collecting pick-ups on the track. Obstacles may cause damage or lower score.
            - [ ] Shooter: The player defeats enemies by shooting at them.
        - [ ]  Utilise basic collision detection using standard techniques (e.g., distance between player and object).
    - [ ] Advanced rendering (20%)
        - [ ] Implement at least two advanced rendering techniques. Some ideas for this include the following:
            - [ ] Easier: 
                - [ ] **Fog**
            - [ ] Harder:
                - [ ] **Blur** **radial**


          

We will cover most of these in lecture / labs. Pick techniques that are interesting and feasible for your work. Nearly all of these methods are described in the reference textbooks for the module. If you have a different technique in mind, please speak to the module leader (Eddie).

- [ ]  In your report, be sure to describe the techniques implemented, and how they work.


5. #### Optional (unmarked)
   
    -  Include audio in your application to enhance the experience. 


### Presentation in Class (5 minutes)

-  You are to present your work during the lectures and lab sessions on Thursday 1st April. Clear explanations of your implementation will be expected.
-  The presentation is to be done in a Zoom class. This is your opportunity to show everyone what you have accomplished, and should be a fun experience for all!
-  The demo session will be marked on a similar marking schedule as the report except that section 1 will cover presentation (30%) instead of the report/code as there is no submission 
-  This will be marked out of 100% and scaled to 15% for the module.
-  You should aim to have all the deliverables for the coursework completed by the time of the demo.
-  Feedback will be provided rapidly and give an indication of progress at that point.