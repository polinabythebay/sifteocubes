/*
 * Sifteo SDK Example.
 *I changed the file
 */

#include <sifteo.h>
#include <sifteo/menu.h>
#include "assets.gen.h"
using namespace Sifteo;

// Static Globals
static const unsigned gNumCubes = 3;
static VideoBuffer gVideo[gNumCubes];
static struct MenuItem gItems[] = { {&task1, NULL}, {&task2, NULL}, {&task3, NULL},{NULL, NULL} };
//{&IconChroma, &LabelChroma}, {&IconSandwich, &LabelSandwich}, {&IconPeano, &LabelPeano}, {&IconBuddy, &LabelBuddy}, {&IconChroma, NULL}, 

static struct MenuItem clickItems[] = { {&task1, NULL}, {&task2, NULL} , {&task3, NULL} };
static struct MenuItem selectItems[] = { {&task1_selected, NULL}, {&task2_selected, NULL}, {&task3_selected, NULL} };
static struct MenuItem completeItems[] = { {&task1_complete, NULL}, {&task2_complete, NULL}, {&task3_complete, NULL}};



static struct MenuAssets gAssets = {&BgTile, &Footer, &LabelEmpty, {&Tip0, &Tip1, &Tip2, NULL}};
//{&helloworld, NULL}
static AssetSlot MainSlot = AssetSlot::allocate()
    .bootstrap(BetterflowAssets);

static Metadata M = Metadata()
    .title("Menu SDK Demo")
    .package("com.sifteo.sdk.menudemo", "1.0.0")
    .icon(Icon)
    .cubeRange(gNumCubes);


static void begin() {
    // Blank screens, attach VideoBuffers
    for(CubeID cube = 0; cube != gNumCubes; ++cube) {
        auto &vid = gVideo[cube];
        vid.initMode(BG0);
        vid.attach(cube);
        vid.bg0.erase(StripeTile);
    }
}

void main()
{
    begin();


    //first cube menu
    Menu m0(gVideo[0], &gAssets, gItems);
    //second cube menu
    Menu m1(gVideo[1], &gAssets, gItems);
    //third cube menu
    Menu m2(gVideo[2], &gAssets, gItems);

    //m.anchor(2);


    int item_0_status= 0;
    int item_1_status= 0;
    int item_2_status=0;

    //indicates MenuEvent needs to ASSERT(e.type == MENU_EXIT);
    int assert_indicator=0;

    //NOTE: need to use menudemo.elf, it's the only one that changes--> need to figure out how to fix that


    struct MenuEvent e;
    struct MenuEvent e1;
    struct MenuEvent e2;


    uint8_t item; //item associated with first cube
    uint8_t item1; //item associated with second cube 

    while (1) {
        while ((m0.pollEvent(&e))&&(m1.pollEvent(&e1))&&(m2.pollEvent(&e2))) {

//////////////////////////////////////////////////////////////////////////////////////////////////
                //CUBE 0 EVENTS
 //////////////////////////////////////////////////////////////////////////////////////////////////           
            
            switch (e.type) {

                case MENU_ITEM_PRESS:
          
                    
                    if (e.item ==0){
                        //if this is the first time you've clicked the first item
                        if (item_0_status==0){
                        m0.replaceIcon(e.item, selectItems[0].icon, selectItems[0].label);
                        m0.anchor(e.item);
                        item_0_status=1;
                        //second time you've clicked first item
                        } else if (item_0_status==1) {
                            m0.replaceIcon(e.item, completeItems[0].icon, completeItems[0].label);
                            item_0_status=2;
                            m0.anchor(e.item);
                        //third time you've clicked first item
                        } else if (item_0_status==2){
                            m0.replaceIcon(e.item, clickItems[0].icon, clickItems[0].label);
                            item_0_status=0; //loop back to the first state
                            m0.anchor(e.item); 
                        }
                    }

                     if (e.item ==1){
                        //if this is the first time you've clicked the 2nd item
                        if (item_1_status==0){
                            m0.replaceIcon(e.item, selectItems[1].icon, selectItems[1].label);
                            m0.anchor(e.item);
                            item_1_status=1;
                        //second time you've clicked the 2nd item
                        } else if (item_1_status==1) {
                            m0.replaceIcon(e.item, completeItems[1].icon, completeItems[1].label);
                            m0.anchor(e.item);
                            item_1_status=2;
                        //third time you've clicked the 2nd item
                        } else if (item_1_status==2){
                            m0.replaceIcon(e.item, clickItems[1].icon, clickItems[1].label);
                            m0.anchor(e.item);
                            item_1_status=0; //it'll loop back to the first state
                        }
                    }

                     if (e.item ==2){
                        //if this is the first time you've clicked the 3rd item
                        if (item_2_status==0){
                            m0.replaceIcon(e.item, selectItems[2].icon, selectItems[2].label);
                            m0.anchor(e.item);
                            item_2_status=1;
                            //2nd time you've clicked the 3rd item
                        } else if (item_2_status==1) {
                            m0.replaceIcon(e.item, completeItems[2].icon, completeItems[2].label);
                            m0.anchor(e.item);
                            item_2_status=2;
                            //third time you've clicked the 3rd item
                        } else if (item_2_status==2){
                            m0.replaceIcon(e.item, clickItems[2].icon, clickItems[2].label);
                            m0.anchor(e.item);
                            item_2_status=0; //it'll loop back to the first state 
                        }
                    }

                   assert_indicator =1; 

                    break;

                case MENU_EXIT:
                    // this is not possible when pollEvent is used as the condition to the while loop.
                    // NOTE: this event should never have its default handler skipped.
                    ASSERT(false);
                    break;

                case MENU_NEIGHBOR_ADD:
                    LOG("E: found cube %d on side %d of menu (neighbor's %d side)\n",
                         e.neighbor.neighbor, e.neighbor.masterSide, e.neighbor.neighborSide);
                    break;

                case MENU_NEIGHBOR_REMOVE:
                    LOG("E: lost cube %d on side %d of menu (neighbor's %d side)\n",
                         e.neighbor.neighbor, e.neighbor.masterSide, e.neighbor.neighborSide);
                    break;

                case MENU_ITEM_ARRIVE:
                    LOG("E: arriving at menu item %d\n", e.item);
                    item = e.item;
                   // item1 = a.item;

                    break;

                case MENU_ITEM_DEPART:
                    LOG("E: departing from menu item %d, scrolling %s\n", item, e.direction > 0 ? "forward" : "backward");
                    break;

                case MENU_PREPAINT:
                    // do your implementation-specific drawing here
                    // NOTE: this event should never have its default handler skipped.
                    break;

                case MENU_UNEVENTFUL:
                    // this should never happen. if it does, it can/should be ignored.
                    ASSERT(false);
                    break;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////
                //CUBE 1 EVENTS
 //////////////////////////////////////////////////////////////////////////////////////////////////   

            switch (e1.type) {

                case MENU_ITEM_PRESS:
          
                    
                    if (e1.item ==0){
                        //if this is the first time you've clicked the first item
                        if (item_0_status==0){
                        m1.replaceIcon(e1.item, selectItems[0].icon, selectItems[0].label);
                        m1.anchor(e1.item);
                        item_0_status=1;
                        //second time you've clicked first item
                        } else if (item_0_status==1) {
                            m1.replaceIcon(e1.item, completeItems[0].icon, completeItems[0].label);
                            item_0_status=2;
                            m1.anchor(e1.item);
                        //third time you've clicked first item
                        } else if (item_0_status==2){
                            m1.replaceIcon(e1.item, clickItems[0].icon, clickItems[0].label);
                            item_0_status=0; //loop back to the first state
                            m1.anchor(e1.item); 
                        }
                    }

                     if (e1.item ==1){
                        //if this is the first time you've clicked the 2nd item
                        if (item_1_status==0){
                            m1.replaceIcon(e1.item, selectItems[1].icon, selectItems[1].label);
                            m1.anchor(e1.item);
                            item_1_status=1;
                        //second time you've clicked the 2nd item
                        } else if (item_1_status==1) {
                            m1.replaceIcon(e1.item, completeItems[1].icon, completeItems[1].label);
                            m1.anchor(e1.item);
                            item_1_status=2;
                        //third time you've clicked the 2nd item
                        } else if (item_1_status==2){
                            m1.replaceIcon(e1.item, clickItems[1].icon, clickItems[1].label);
                            m1.anchor(e1.item);
                            item_1_status=0; //it'll loop back to the first state
                        }
                    }

                     if (e1.item ==2){
                        //if this is the first time you've clicked the 3rd item
                        if (item_2_status==0){
                            m1.replaceIcon(e1.item, selectItems[2].icon, selectItems[2].label);
                            m1.anchor(e1.item);
                            item_2_status=1;
                            //2nd time you've clicked the 3rd item
                        } else if (item_2_status==1) {
                            m1.replaceIcon(e1.item, completeItems[2].icon, completeItems[2].label);
                            m1.anchor(e1.item);
                            item_2_status=2;
                            //third time you've clicked the 3rd item
                        } else if (item_2_status==2){
                            m1.replaceIcon(e1.item, clickItems[2].icon, clickItems[2].label);
                            m1.anchor(e1.item);
                            item_2_status=0; //it'll loop back to the first state 
                        }
                    }

                    assert_indicator =2;


                    break;

                case MENU_EXIT:
                    // this is not possible when pollEvent is used as the condition to the while loop.
                    // NOTE: this event should never have its default handler skipped.
                    ASSERT(false);
                    break;

                case MENU_NEIGHBOR_ADD:
                    LOG("E1: found cube %d on side %d of menu (neighbor's %d side)\n",
                         e1.neighbor.neighbor, e1.neighbor.masterSide, e1.neighbor.neighborSide);
                    break;

                case MENU_NEIGHBOR_REMOVE:
                    LOG("E1: lost cube %d on side %d of menu (neighbor's %d side)\n",
                         e1.neighbor.neighbor, e1.neighbor.masterSide, e1.neighbor.neighborSide);
                    break;

                case MENU_ITEM_ARRIVE:
                    LOG("E1: arriving at menu item %d\n", e1.item);
                    item = e1.item;
                   // item1 = a.item;

                    break;

                case MENU_ITEM_DEPART:
                    LOG("E1: departing from menu item %d, scrolling %s\n", item, e1.direction > 0 ? "forward" : "backward");
                    break;

                case MENU_PREPAINT:
                    // do your implementation-specific drawing here
                    // NOTE: this event should never have its default handler skipped.
                    break;

                case MENU_UNEVENTFUL:
                    // this should never happen. if it does, it can/should be ignored.
                    ASSERT(false);
                    break;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////
                //CUBE 2 EVENTS
 //////////////////////////////////////////////////////////////////////////////////////////////////   
            
            switch (e2.type) {

                case MENU_ITEM_PRESS:
          
                    
                    if (e2.item ==0){
                        //if this is the first time you've clicked the first item
                        if (item_0_status==0){
                        m2.replaceIcon(e2.item, selectItems[0].icon, selectItems[0].label);
                        m2.anchor(e2.item);
                        item_0_status=1;
                        //second time you've clicked first item
                        } else if (item_0_status==1) {
                            m2.replaceIcon(e2.item, completeItems[0].icon, completeItems[0].label);
                            item_0_status=2;
                            m2.anchor(e2.item);
                        //third time you've clicked first item
                        } else if (item_0_status==2){
                            m2.replaceIcon(e2.item, clickItems[0].icon, clickItems[0].label);
                            item_0_status=0; //loop back to the first state
                            m2.anchor(e2.item); 
                        }
                    }

                     if (e2.item ==1){
                        //if this is the first time you've clicked the 2nd item
                        if (item_1_status==0){
                            m2.replaceIcon(e2.item, selectItems[1].icon, selectItems[1].label);
                            m2.anchor(e2.item);
                            item_1_status=1;
                        //second time you've clicked the 2nd item
                        } else if (item_1_status==1) {
                            m2.replaceIcon(e2.item, completeItems[1].icon, completeItems[1].label);
                            m2.anchor(e2.item);
                            item_1_status=2;
                        //third time you've clicked the 2nd item
                        } else if (item_1_status==2){
                            m2.replaceIcon(e2.item, clickItems[1].icon, clickItems[1].label);
                            m2.anchor(e2.item);
                            item_1_status=0; //it'll loop back to the first state
                        }
                    }

                     if (e2.item ==2){
                        //if this is the first time you've clicked the 3rd item
                        if (item_2_status==0){
                            m2.replaceIcon(e2.item, selectItems[2].icon, selectItems[2].label);
                            m2.anchor(e2.item);
                            item_2_status=1;
                            //2nd time you've clicked the 3rd item
                        } else if (item_2_status==1) {
                            m2.replaceIcon(e2.item, completeItems[2].icon, completeItems[2].label);
                            m2.anchor(e2.item);
                            item_2_status=2;
                            //third time you've clicked the 3rd item
                        } else if (item_2_status==2){
                            m2.replaceIcon(e2.item, clickItems[2].icon, clickItems[2].label);
                            m2.anchor(e2.item);
                            item_2_status=0; //it'll loop back to the first state 
                        }
                    }

                    assert_indicator =3;


                    break;

                case MENU_EXIT:
                    // this is not possible when pollEvent is used as the condition to the while loop.
                    // NOTE: this event should never have its default handler skipped.
                    ASSERT(false);
                    break;

                case MENU_NEIGHBOR_ADD:
                    LOG("E2: found cube %d on side %d of menu (neighbor's %d side)\n",
                         e2.neighbor.neighbor, e2.neighbor.masterSide, e2.neighbor.neighborSide);
                    break;

                case MENU_NEIGHBOR_REMOVE:
                    LOG("E2: lost cube %d on side %d of menu (neighbor's %d side)\n",
                         e2.neighbor.neighbor, e2.neighbor.masterSide, e2.neighbor.neighborSide);
                    break;

                case MENU_ITEM_ARRIVE:
                    LOG("E2: arriving at menu item %d\n", e2.item);
                    item = e2.item;
                   // item1 = a.item;

                    break;

                case MENU_ITEM_DEPART:
                    LOG("E2: departing from menu item %d, scrolling %s\n", item, e2.direction > 0 ? "forward" : "backward");
                    break;

                case MENU_PREPAINT:
                    // do your implementation-specific drawing here
                    // NOTE: this event should never have its default handler skipped.
                    break;

                case MENU_UNEVENTFUL:
                    // this should never happen. if it does, it can/should be ignored.
                    ASSERT(false);
                    break;
            } 

//////////////////////////////////////////////////////////////////////////////////////////////////
                //ALL CUBE EVENTS
 ////////////////////////////////////////////////////////////////////////////////////////////////// 

            m0.performDefault();
            m1.performDefault();
            m2.performDefault();
            //LOG("I'm here and I'm not going anywhere \n");
        }

        // Handle the exit event (so we can re-enter the same Menu)
        //Need this for every distinct cube we have in order to handle click events 
        if (assert_indicator==1){
            ASSERT(e.type == MENU_EXIT);
        } 
        if (assert_indicator==2){
            ASSERT(e1.type == MENU_EXIT);
        }
        if (assert_indicator==3){
            ASSERT(e2.type == MENU_EXIT);
        }
        
        
        //need to be able to switch between these asserts 
        m0.performDefault();
        m1.performDefault();
        m2.performDefault();
        

        LOG("E: Selected Game: %d\n", e.item);
        LOG("E1: Selected Game: %d\n", e1.item);
        LOG("E1: Selected Game: %d\n", e2.item);
    }
}