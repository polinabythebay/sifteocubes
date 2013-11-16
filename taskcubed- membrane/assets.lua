IconAssets = group{quality=9.95}
Icon = image{"icon.png"}

BootAssets = group{quality=8.90}
Title = image{"title.png", width=128, height=128}

GameAssets = group{quality=9.95}

Playfield = image{"playfield.png"}
StripesPlayfield = image{"stripes.png"}
Markers = image{"markers.png", width=16, height=16}
Brackets = image{"brackets.png"}

PlayfieldE = image{"playfield-e.png", width=96, height=24}
PlayfieldA = image{"playfield-a.png", width=24, height=96}
PlayfieldF = image{"playfield-f.png", width=96, height=24}
PlayfieldW = image{"playfield-w.png", width=24, height=96}

Particle0 = image{"particle-0.png", width=16, height=16, pinned=true}
Particle0Warp = image{"particle-0-warp.png", width=16, height=16, pinned=true}









--ALL OF THIS IS TAKEN FROM THE ASSETSLOT DEMO

-- Main Menu

MenuGroup = group{ quality=9.8 }

MenuStripe = image{ "images/stripes.png", pinned=1 }
MenuBg = image{ "images/bg.png", pinned=1 }
MenuFooter = image{ "images/footer.png" }

MenuLabelEmpty = image{ "images/labelEmpty.png" }
MenuIconSpinny = image{ "images/icon-spinny.png" }
MenuIconBall = image{ "images/icon-ball.png" }

-- Loading screen: Divided into two asset groups
LoadingBg = image{ "images/loading.png" }

-- Animation: "spinny".

function frames(fmt, count, pingPong)
    t = {}
    for i = 1, count do
        t[1+#t] = string.format(fmt, i)
    end
    if pingPong then
        for i = count-1, 2, -1 do
            t[1+#t] = string.format(fmt, i)
        end
    end
    return t
end

SpinnyGroup = group{ quality=9.5 }
Spinny = image{ frames("images/spinny/spinny-%d.png", 24, true) }

-- Animation: "ball"

BallGroup = group{ quality=10 }
Ball = image{ frames("images/ball/%04d.png", 29) }

