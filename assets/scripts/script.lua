
local function onUnstaged(self)
    print("unstaged!")
end

local function onKeyDPressed(self, action)
    print("D pressed!")
end

local function onKeySpacePressed(self, action)
    print("Space pressed!")
end

local function onStaged(self)
    self:addKeyCallback("D", script.onKeyPressD)
    self:setCollisionType(2)
    self:addCollisionCallback(1, script.hitMapGeom)
    print("Talga has been staged")
end



local function onHitMapGeom(self)

end

script = 
{
    stagedFunc = onStaged,
    unstagedFunc = onUnstaged,
    updateFunc = onUpdate,
    onKeyPressD = onKeyDPressed,
    hitMapGeom = onHitMapGeom
}