

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
end

script = 
{
    stagedFunc = onStaged;
    unstagedFunc = onUnstaged;
    updateFunc = onUpdate;
    onKeyPressD = onKeyDPressed
}