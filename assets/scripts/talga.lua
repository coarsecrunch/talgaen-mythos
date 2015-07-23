
local function keyD(self, action)

end

local function keyA(self, action)

end

local function updateFuncLoc(self, dt)
    
end

local function stagedFuncLoc(self)
    local animation = {}
    animation["standL"] = { {0, 0, 64, 64}, {64, 0, 64, 64} };
    animation["standR"] = { {128, 0, 64, 64}, {192, 0, 64, 64} };
    animation["walkL"] = {}
    animation["walkR"] = {}

    for i=0,5 do
        animation.walkL[i+1] = {64 * i, 64,64,64}
        print(i)
    end

    for i=6,11 do
        animation.walkR[i-5] = {64 * i, 64,64,64}
    end

    spr = AnimSprite.new(GAME:manager():AddAnimationSet("talgaAnim", "talgasheet.png", animation))
    collider = RectCollider.new(64,64)

    self:setRenderable(spr)
    self:setCollider(collider)
    self:playAnimation("walkR", 1000, true)
end

talga =
{
    stagedFunc = stagedFuncLoc,
    updateFunc = updateFuncLoc
}