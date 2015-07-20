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
    print("Staged")
    self:addKeyCallback("D", script.onKeyPressD)
    self:setCollisionType(2)
    self:addCollisionCallback(1, script.hitMapGeom)
    self.canJump = "hello jump!"
end

local function onHitMapGeom(self)
    print("TALGA HIT THE GROUND")
    print("can jump: " .. self.canJump)
end

script = 
{
    stagedFunc = onStaged,
    unstagedFunc = onUnstaged,
    updateFunc = onUpdate,
    onKeyPressD = onKeyDPressed,
    hitMapGeom = onHitMapGeom
}