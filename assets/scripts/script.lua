local function onStaged(self)
    print("staged!")
end

local function onUnstaged(self)
    print("unstaged!")
end

script = 
{
    stagedFunc = onStaged;
    unstagedFunc = onUnstaged;
    updateFunc = onUpdate;
}