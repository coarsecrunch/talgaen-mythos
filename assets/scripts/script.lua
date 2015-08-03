function makeBlock(x, y)
    GAME:manager():AddTexture("../assets/textures/testblock.png")
    sprite = Sprite.new(GAME:manager():GetTexture("testblock.png"))
    phys = RectCollider.new(64,64,x,y)

    obj = GameObject.new(sprite, phys)
    obj:getCollider():setMass(40)
    print("jelly")
    GAME:addObj(obj)
end