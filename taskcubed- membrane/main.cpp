/*
 * Sifteo SDK Example.
 */

//I like how this is organized 

#include "game.h"
#include "loader.h"
#include "sifteo/menu.h"
#include "assets.gen.h"

AssetSlot MainSlot = AssetSlot::allocate()
    .bootstrap(BootAssets);

//THIS IS FROM ASSET SLOT THINGY

static AssetSlot AnimationSlot = AssetSlot::allocate();
static const unsigned numCubes = 3;
static const CubeSet allCubes(0, numCubes);
static VideoBuffer vid[numCubes];
static MyLoader loader(allCubes, MainSlot, vid);

//ASSET SLOT THINGY 

static Metadata M = Metadata()
    .title("Membrane")
    .package("com.sifteo.sdk.membrane", "1.0")
    .icon(Icon)
    .cubeRange(NUM_CUBES);

void animation(const AssetImage &image)
{
    static Game game;

//after you pick an image, the game runs its course 
    while (1) {
        //LOG("I'm Here");
        
        game.title();
       // game.init();
        // game.run();
       //game.cleanup();
    }


    /*
    loader.load(image.assetGroup(), AnimationSlot);

    for (CubeID cube : allCubes) {
        vid[cube].initMode(BG0);
        vid[cube].attach(cube);
    }

    while (1) {
        unsigned frame = SystemTime::now().cycleFrame(2.0, image.numFrames());

        for (CubeID cube : allCubes) {
            vid[cube].bg0.image(vec(0,0), image, frame);
            if (cube.isTouching())
                return;
        }

        System::paint();
    }*/

}

void main()
{
    static Game game;
    //is this just like an infinite while loop?
    
    
    //it would be really cool if we could go from a menu item to this 
    //step
    
    /*
     while (1) {
        loader.load(MenuGroup, MainSlot);

        static MenuItem items[] = { {&MenuIconSpinny}, {&MenuIconBall}, {0} };
        static MenuAssets assets = { &MenuBg, &MenuFooter, &MenuLabelEmpty };

        Menu m(vid[0], &assets, items);
        MenuEvent e;

        for (CubeID cube : allCubes) {
            vid[cube].initMode(BG0);
            vid[cube].attach(cube);
            vid[cube].bg0.erase(MenuStripe);
        }

        while (m.pollEvent(&e))
            m.performDefault();

        switch (e.item) {
            case 0: animation(Spinny); break;
            case 1: animation(Ball); break;
        }

    }*/

    
    while (1) {
	    game.title();
	    game.init();
	    game.run();
	    game.cleanup();
	}
    
}
