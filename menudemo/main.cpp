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

    Menu m(gVideo[1], &gAssets, gItems);
    //m.anchor(2);


    int item_0_status= 0;
    int item_1_status= 0;
    int item_2_status=0;


    struct MenuEvent e;
    uint8_t item;

    while (1) {
        while (m.pollEvent(&e)) {

            switch (e.type) {

                case MENU_ITEM_PRESS:
          

                    if (e.item ==0){
                        //if this is the first time you've clicked the first item
                        if (item_0_status==0){
                        m.replaceIcon(e.item, selectItems[0].icon, selectItems[0].label);
                        m.anchor(e.item);
                        item_0_status=1;
                        //second time you've clicked first item
                        } else if (item_0_status==1) {
                            m.replaceIcon(e.item, completeItems[0].icon, completeItems[0].label);
                            item_0_status=2;
                            m.anchor(e.item);
                        //third time you've clicked first item
                        } else if (item_0_status==2){
                            m.replaceIcon(e.item, clickItems[0].icon, clickItems[0].label);
                            item_0_status=0; //loop back to the first state
                            m.anchor(e.item); 
                        }
                    }

                     if (e.item ==1){
                        //if this is the first time you've clicked the 2nd item
                        if (item_1_status==0){
                            m.replaceIcon(e.item, selectItems[1].icon, selectItems[1].label);
                            m.anchor(e.item);
                            item_1_status=1;
                        //second time you've clicked the 2nd item
                        } else if (item_1_status==1) {
                            m.replaceIcon(e.item, completeItems[1].icon, completeItems[1].label);
                            m.anchor(e.item);
                            item_1_status=2;
                        //third time you've clicked the 2nd item
                        } else if (item_1_status==2){
                            m.replaceIcon(e.item, clickItems[1].icon, clickItems[1].label);
                            m.anchor(e.item);
                            item_1_status=0; //it'll loop back to the first state
                        }
                    }

                     if (e.item ==2){
                        //if this is the first time you've clicked the 3rd item
                        if (item_2_status==0){
                            m.replaceIcon(e.item, selectItems[2].icon, selectItems[2].label);
                            m.anchor(e.item);
                            item_2_status=1;
                            //2nd time you've clicked the 3rd item
                        } else if (item_2_status==1) {
                            m.replaceIcon(e.item, completeItems[2].icon, completeItems[2].label);
                            m.anchor(e.item);
                            item_2_status=2;
                            //third time you've clicked the 3rd item
                        } else if (item_2_status==2){
                            m.replaceIcon(e.item, clickItems[2].icon, clickItems[2].label);
                            m.anchor(e.item);
                            item_2_status=0; //it'll loop back to the first state 
                        }
                    }

                   

                    break;

                case MENU_EXIT:
                    // this is not possible when pollEvent is used as the condition to the while loop.
                    // NOTE: this event should never have its default handler skipped.
                    ASSERT(false);
                    break;

                case MENU_NEIGHBOR_ADD:
                    LOG("found cube %d on side %d of menu (neighbor's %d side)\n",
                         e.neighbor.neighbor, e.neighbor.masterSide, e.neighbor.neighborSide);
                    break;

                case MENU_NEIGHBOR_REMOVE:
                    LOG("lost cube %d on side %d of menu (neighbor's %d side)\n",
                         e.neighbor.neighbor, e.neighbor.masterSide, e.neighbor.neighborSide);
                    break;

                case MENU_ITEM_ARRIVE:
                    LOG("arriving at menu item %d\n", e.item);
                    item = e.item;
                    break;

                case MENU_ITEM_DEPART:
                    LOG("departing from menu item %d, scrolling %s\n", item, e.direction > 0 ? "forward" : "backward");
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

            m.performDefault();
        }

        // Handle the exit event (so we can re-enter the same Menu)
        ASSERT(e.type == MENU_EXIT);
        m.performDefault();
        

        LOG("Selected Game: %d\n", e.item);
    }
}