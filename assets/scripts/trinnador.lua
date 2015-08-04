
function init()
    tga = GameObject.new("../assets/scripts/talga.lua")

    GAME:addObj(tga)
    
    pos1 = vec2.new(2.0)
    move = vec2.new(2.0, 1.0)


    newpos = pos1 - move

    print("x: " .. newpos:x() .. "  y: " .. newpos:y())

	print("HIII, IT'S ME, FRANKLIN!!!!")
end