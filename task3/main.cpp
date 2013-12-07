/* Task3
 * 
 * Authors: Danika Suggs, Sydney Cusack, Polina Soshnin
 */

#include <sifteo.h>
#include <sifteo/menu.h>
#include "assets.gen.h"
#include <sifteo/motion.h>

using namespace Sifteo;

// METADATA

static const unsigned gNumCubes = 6;
//changed to three so I could see the demo on the siftulator

static Metadata M = Metadata()
    .title("Task 3")
    .package("edu.wellesley.cs.task3", "1.0.0")
    .icon(Icon)
    .cubeRange(gNumCubes);

static AssetSlot gMainSlot = AssetSlot::allocate()
    .bootstrap(BootstrapAssets);

// GLOBALS

static VideoBuffer vbuf[gNumCubes];
static CubeSet newCubes; // new cubes as a result of paint()
static CubeSet lostCubes; // lost cubes as a result of paint()
static CubeSet reconnectedCubes; // reconnected (lost->new) cubes as a result of paint()
static CubeSet dirtyCubes; // dirty cubes as a result of paint()
static CubeSet activeCubes; // cubes showing the active scene

//add MotionBuffer to detect motion


//static MotionBuffer<> mbuf0;
//static MotionBuffer<> mbuf1;
//static MotionBuffer<> mbuf2;




static AssetLoader loader; // global asset loader (each cube will have symmetric assets)
static AssetConfiguration<1> config; // global asset configuration (will just hold the bootstrap group)

static struct MenuItem cube0Items[] = {{&Task1_Icon, &LabelEmpty}, {&Task2_Icon, &LabelEmpty}, {&Task3_Icon, &LabelEmpty}, {&Task4_Icon, &LabelEmpty}, {&Task5_Icon, &LabelEmpty}, {&Task6_Icon, &LabelEmpty}, {NULL, NULL}};
static struct MenuItem cube1Items[] = {{&Task1_Icon, &LabelEmpty}, {&Task2_Icon, &LabelEmpty}, {&Task3_Icon, &LabelEmpty}, {&Task4_Icon, &LabelEmpty}, {&Task5_Icon, &LabelEmpty}, {&Task6_Icon, &LabelEmpty}, {NULL, NULL}};
static struct MenuItem cube2Items[] = {{&Task1_Icon, &LabelEmpty}, {&Task2_Icon, &LabelEmpty}, {&Task3_Icon, &LabelEmpty}, {&Task4_Icon, &LabelEmpty}, {&Task5_Icon, &LabelEmpty}, {&Task6_Icon, &LabelEmpty}, {NULL, NULL}};
static struct MenuItem cube3Items[] = {{&Task1_Icon, &LabelEmpty}, {&Task2_Icon, &LabelEmpty}, {&Task3_Icon, &LabelEmpty}, {&Task4_Icon, &LabelEmpty}, {&Task5_Icon, &LabelEmpty}, {&Task6_Icon, &LabelEmpty}, {NULL, NULL}};
static struct MenuItem cube4Items[] = {{&Task1_Icon, &LabelEmpty}, {&Task2_Icon, &LabelEmpty}, {&Task3_Icon, &LabelEmpty}, {&Task4_Icon, &LabelEmpty}, {&Task5_Icon, &LabelEmpty}, {&Task6_Icon, &LabelEmpty}, {NULL, NULL}};
static struct MenuItem cube5Items[] = {{&Task1_Icon, &LabelEmpty}, {&Task2_Icon, &LabelEmpty}, {&Task3_Icon, &LabelEmpty}, {&Task4_Icon, &LabelEmpty}, {&Task5_Icon, &LabelEmpty}, {&Task6_Icon, &LabelEmpty}, {NULL, NULL}};


static struct MenuAssets gAssets = {&BgTile, &Footer, &LabelEmpty, {NULL}};

struct CubeInfo {
    int task; //0, 1, 2, or 255 = menu
    int status; // 0 = red, 1 = yellow, 2 = blue, or 255 = nothing
};

static struct CubeInfo taskCubes[gNumCubes];


//Detecting Motion
//MotionBuffer<> buffer;
static TiltShakeRecognizer motion[gNumCubes];



// FUNCTIONS

static Int2 getRestPosition(Side s) {
    // Look up the top-left pixel of the bar for the given side.
    // We use a switch so that the compiler can optimize this
    // however if feels is best.
    switch(s) {
    case TOP: return vec(64 - RedBars[0].pixelWidth()/2,0);
    case LEFT: return vec(0, 64 - RedBars[1].pixelHeight()/2);
    case BOTTOM: return vec(64 - RedBars[2].pixelWidth()/2, 128-RedBars[2].pixelHeight());
    case RIGHT: return vec(128-RedBars[3].pixelWidth(), 64 - RedBars[3].pixelHeight()/2);
    default: return vec(0,0);
    }
}

static int barSpriteCount(CubeID cid) {
    // how many bars are showing on this cube?
    ASSERT(activeCubes.test(cid));
    int result = 0;
    for(int i=0; i<4; ++i) {
        if (!vbuf[cid].sprites[i].isHidden()) {
            result++;
        }
    }
    return result;
}

static bool showSideBar(CubeID cid, CubeID nb, Side s) {
    // if cid is not showing a bar on side s, show it
    int stat1 = taskCubes[cid].status;
    int stat2 = taskCubes[nb].status;

    //To-Do: add logic for different colored bars based on the statuses of the cubes.
    ASSERT(activeCubes.test(cid));
    if (vbuf[cid].sprites[s].isHidden()) {
        switch (stat1 + stat2) {
            case 0: vbuf[cid].sprites[s].setImage(RedBars[s]); break; //2 red -> red
            case 1: vbuf[cid].sprites[s].setImage(OrangeBars[s]); break; //1 red, 1 yellow -> orange
            case 2:
                if (stat1 == 0 || stat1 == 2) { //1 red, 1 blue -> purple
                    vbuf[cid].sprites[s].setImage(PurpleBars[s]);
                    break;
                } else {
                    vbuf[cid].sprites[s].setImage(YellowBars[s]); //2 yellow -> yellow
                    break; 
                }
            case 3: vbuf[cid].sprites[s].setImage(GreenBars[s]); break; //1 yellow, 1 blue -> green
            case 4: vbuf[cid].sprites[s].setImage(BlueBars[s]); break; //2 blue -> blue
        }
        vbuf[cid].sprites[s].move(getRestPosition(s));
        return true;
    } else {
        return false;
    }
}

static bool hideSideBar(CubeID cid, Side s) {
    // if cid is showing a bar on side s, hide it
    ASSERT(activeCubes.test(cid));
    if (!vbuf[cid].sprites[s].isHidden()) {
        vbuf[cid].sprites[s].hide();
        return true;
    } else {
        return false;
    }
}

static void activateCube(CubeID cid, int task) {
    // mark cube as active and render its canvas
    activeCubes.mark(cid);
    vbuf[cid].initMode(BG0_SPR_BG1);

    //this is where the starting image is set (after a task is set and we exit the menu) 
    vbuf[cid].bg0.image(vec(0,0), TaskReds, task);

    auto neighbors = vbuf[cid].physicalNeighbors();
    for(int side=0; side<4; ++side) {
        if (neighbors.hasNeighborAt(Side(side))) {
            //showSideBar(cid, Side(side));
        } else {
            hideSideBar(cid, Side(side));
        }
    }
}

static void paintWrapper() {
    // clear the palette
    newCubes.clear();
    lostCubes.clear();
    reconnectedCubes.clear();
    dirtyCubes.clear();

    // fire events
    System::paint();

    // dynamically load assets just-in-time
    if (!(newCubes | reconnectedCubes).empty()) {
        loader.start(config);
        while(!loader.isComplete()) {
            for(CubeID cid : (newCubes | reconnectedCubes)) {
                vbuf[cid].bg0rom.hBargraph(
                    vec(0, 4), loader.cubeProgress(cid, 128), BG0ROMDrawable::ORANGE, 8
                );
            }
            // fire events while we wait
            System::paint();
        }
        loader.finish();
    }

    // repaint cubes
    for(CubeID cid : dirtyCubes) {
        activateCube(cid, taskCubes[cid].task);
    }
    
    // also, handle lost cubes, if you so desire :)
}

static void onCubeConnect(void* ctxt, unsigned cid) {
    // this cube is either new or reconnected
    if (lostCubes.test(cid)) {
        // this is a reconnected cube since it was already lost this paint()
        lostCubes.clear(cid);
        reconnectedCubes.mark(cid);
    } else {
        // this is a brand-spanking new cube
        newCubes.mark(cid);
    }
    // begin showing some loading art (have to use BG0ROM since we don't have assets)
    dirtyCubes.mark(cid);
    auto& g = vbuf[cid];
    g.attach(cid);
    g.initMode(BG0_ROM);
    g.bg0rom.fill(vec(0,0), vec(16,16), BG0ROMDrawable::SOLID_BG);
    g.bg0rom.text(vec(1,1), "Hold on!", BG0ROMDrawable::BLUE);
    g.bg0rom.text(vec(1,14), "Adding Cube...", BG0ROMDrawable::BLUE);
}

static void onCubeDisconnect(void* ctxt, unsigned cid) {
    // mark as lost and clear from other cube sets
    lostCubes.mark(cid);
    newCubes.clear(cid);
    reconnectedCubes.clear(cid);
    dirtyCubes.clear(cid);
    activeCubes.clear(cid);
}

static void onCubeRefresh(void* ctxt, unsigned cid) {
    // mark this cube for a future repaint
    dirtyCubes.mark(cid);
}

static bool isActive(NeighborID nid) {
    // Does this nid indicate an active cube?
    return nid.isCube() && activeCubes.test(nid);
}

static void onNeighborAdd(void* ctxt, unsigned cube0, unsigned side0, unsigned cube1, unsigned side1) {
    // update art on active cubes (not loading cubes or base)
    int stat1 = taskCubes[cube0].status;
    int stat2 = taskCubes[cube1].status;

    if (isActive(cube0)) {
        if (stat1 != 255 && stat2 != 255) { 
            showSideBar(cube0, cube1, Side(side0));
        }   
    }
    if (isActive(cube1)) {
        if (stat1 != 255 && stat2 != 255) { 
            showSideBar(cube1, cube0, Side(side1));
        }
    }
}

static void onNeighborRemove(void* ctxt, unsigned cube0, unsigned side0, unsigned cube1, unsigned side1) {
    // update art on active cubes (not loading cubes or base)
    if (isActive(cube0)) { 
        hideSideBar(cube0, Side(side0)); 
    }
    if (isActive(cube1)) { 
        hideSideBar(cube1, Side(side1)); 
    }
}

//Simulates a paper crumpling
//uses a 128x1024 png that cycles through 10 frames 
static void updateAnimation(unsigned cid){ 
    
    //change this increment to reflect a time frame we want
    //still need to tweak this 
    int counter= 0;
    while (counter<10){ //is not a bad frame 

    for (int i=0; i<12; i++){ //12 frames per count to slow down the animation
    //the cycleFrame reflects how fast the frames of the animation cycle through
    //unsigned frame = SystemTime::now().cycleFrame(2.0, PaperAnimation.numFrames());
    unsigned frame = counter;
    vbuf[cid].sprites[0].setImage(PaperAnimation, frame % PaperAnimation.numFrames());

    unsigned status1= frame % PaperAnimation.numFrames();
    //sprites[0] is highest priority (they go from 0-7)
    //vbuf[cid].sprites[0].setImage(PaperAnimation, frame);

    LOG("The frame is now %d.\n", status1); 
    System::paint();
    }
    counter= counter+1;
    }
   
}

static void onCubeTouch(void* ctxt, unsigned cid) {
    CubeID cube(cid);
    if (cube.isTouching() == true) { //only want touches, not untouches to trigger

        //depending on the status of the cube, we want to change the picture
        switch (taskCubes[cid].status) {
            case 0: //status is red, so change to yellow
                taskCubes[cid].status = 1;
                vbuf[cid].bg0.image(vec(0,0), TaskYellows, taskCubes[cid].task);
                break;
            case 1: //status is yellow, so change to blue
                taskCubes[cid].status = 2;
                vbuf[cid].bg0.image(vec(0,0), TaskBlues, taskCubes[cid].task);
                break;
            case 2: //status is blue, so return to menu
                updateAnimation(cid);
                taskCubes[cid].status = 3;
                taskCubes[cid].task = 255;
                break;
            case 3: //cube is having an animation
                taskCubes[cid].status = 255;
                break;
            case 255: //cube is being activated from menu
                taskCubes[cid].status = 0;
                break;
        }
        LOG("cube %d touched, status is now %d.\n", cid, taskCubes[cid].status); 
    }
}

static void begin() {
    // initialize asset configuration and loader
    config.append(gMainSlot, BootstrapAssets);
    loader.init();

    // subscribe to events
    Events::cubeConnect.set(onCubeConnect);
    Events::cubeDisconnect.set(onCubeDisconnect);
    Events::cubeRefresh.set(onCubeRefresh);
    Events::cubeTouch.set(onCubeTouch);

    Events::neighborAdd.set(onNeighborAdd);
    Events::neighborRemove.set(onNeighborRemove);

    // Blank screens, attach VideoBuffers
    for(CubeID cid : CubeSet::connected()) {
        auto &vid = vbuf[cid];
        vid.initMode(BG0);
        vid.attach(cid);
        vid.bg0.erase(StripeTile);
    }
}

void setTask(int task, CubeID cid) {
    taskCubes[cid].task = task; //update the task variable
    activateCube(cid, task); //change the background, etc.

}

void handleMenuEvents(Menu &m, int cid) {
    struct MenuEvent e;

    if (m.pollEvent(&e)) {
        switch (e.type) {
            case MENU_ITEM_PRESS: 
                m.anchor(e.item); //afraid this just makes the item "stick"
                break;

            //stuff we don't care about
            case MENU_EXIT: ASSERT(false); break;
            case MENU_NEIGHBOR_ADD: break;
            case MENU_NEIGHBOR_REMOVE: break;
            case MENU_ITEM_ARRIVE: break;
            case MENU_ITEM_DEPART: break;
            case MENU_PREPAINT: break;
            case MENU_UNEVENTFUL: ASSERT(false); break;
        }

        m.performDefault(); //animation

    } else {
         // Handle the exit event.
        ASSERT(e.type == MENU_EXIT);
        m.performDefault();
        LOG("item %d selected on cube %d.\n", e.item, cid);

        setTask(e.item, cid);
    }
}

void handleMotionEvents(int cid){
  //unassigned motion left to right in menu, add more functionality?  
    //updates the motion variables and booleans to current state
    if (taskCubes[cid].task!=255)
    {

        motion[cid].update();
    
//if cube tilted right, change image to duration of task
    if((motion[cid].tilt.x)==1){
        
        vbuf[cid].bg0.image(vec(3,3), TaskDurations, taskCubes[cid].task);
        
      //if cube tilted left, change image to Picture of task 
    }else if((motion[cid].tilt.x)==-1){
        
       vbuf[cid].bg0.image(vec(3,3), TaskPics, taskCubes[cid].task); 
     
     //if shaken, returns to Name and Status page  
    }else if (motion[cid].shake==true){
        
        if (taskCubes[cid].status==0){ //Not Started

           vbuf[cid].bg0.image(vec(0,0), TaskReds, taskCubes[cid].task);  
        
        }else if (taskCubes[cid].status ==1){ //In Progress

            vbuf[cid].bg0.image(vec(0,0), TaskYellows, taskCubes[cid].task); 

        }else if (taskCubes[cid].status==2){ //Completed

            vbuf[cid].bg0.image(vec(0,0), TaskBlues, taskCubes[cid].task); 
        
        }
    }
}
}



void main() {

    begin();

    //make CubeInfo structs
    for (int i = 0; i < gNumCubes; i++) {
        taskCubes[i].task = 255;
        taskCubes[i].status = 255;
    }

    //attach MotionBuffer to cubes
    for (int i = 0; i < gNumCubes; i++)
    {
        motion[i].attach(i);
    }

    //initialize menus
    Menu m0(vbuf[0], &gAssets, cube0Items);
    Menu m1(vbuf[1], &gAssets, cube1Items);
    Menu m2(vbuf[2], &gAssets, cube2Items);
    Menu m3(vbuf[3], &gAssets, cube3Items);
    Menu m4(vbuf[4], &gAssets, cube4Items);
    Menu m5(vbuf[5], &gAssets, cube5Items);
    

    while (1) { //forever
    

        if (taskCubes[0].task == 255) { //if cube 0 is in menu mode, handle menu events
            handleMenuEvents(m0, 0); 

        }
        
        if (taskCubes[1].task == 255) { //if cube 1 is in menu mode, handle menu events
            handleMenuEvents(m1, 1);            
        }
        
        if (taskCubes[2].task == 255) { //if cube 2 is in menu mode, handle menu events
            handleMenuEvents(m2, 2);                    
        } 
        
        if(taskCubes[3].task==255){
            handleMenuEvents(m3, 3);
        }

        if (taskCubes[4].task==255) {
            handleMenuEvents(m4, 4);
        }

        if (taskCubes[5].task==255) {
            handleMenuEvents(m5, 5);
        }
        
        //handle motion events once a task has been selected
        for (int i = 0; i < gNumCubes; i++)
        {            
            handleMotionEvents(i);
        }

       
        //handle events for connection
        paintWrapper();
    }

}
