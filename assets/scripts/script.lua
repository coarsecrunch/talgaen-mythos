function makeBlock(x, y)
    GAME:manager():AddTexture("../assets/textures/testblock.png")
    local sprite = Sprite.new(GAME:manager():GetTexture("testblock.png"))
    local phys = RectCollider.new(64,64,x,y)

    local obj = GameObject.new(sprite, phys)
    obj:getCollider():setMass(40)
    print("jelly")
    GAME:addObj(obj)
end