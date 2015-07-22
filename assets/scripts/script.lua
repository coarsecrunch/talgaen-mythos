function doStuff(x, y)
    sprite = Sprite.new(GAME:manager():GetTexture("testblock.png"))
    phys = RectCollider.new(64,64,x,y)

    obj = GameObject.new(sprite, phys)
    GAME:addObj(obj)
end