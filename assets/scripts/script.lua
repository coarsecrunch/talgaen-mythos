function makeBlock(x, y)
    sprite = Sprite.new(GAME:manager():GetTexture("testblock.png"))
    phys = RectCollider.new(64,64,x,y)

    obj = GameObject.new(sprite, phys)
    obj:getCollider():setMass(40)
    print("jelly")
    GAME:addObj(obj)
end