COLL_PLAYER = 2

local function keyD(self, action)
    if action == TALGA_KEYPRESS then
        self.applyRight = true
        self:playAnimation("walkR", 500, true)
    elseif action == TALGA_KEYRELEASE then
        self.applyRight = false
        self:playAnimation("standR", 1000, true)
    end
end

local function keyA(self, action)
    if action == TALGA_KEYPRESS then
        self.applyLeft = true
        self:playAnimation("walkL", 500, true)
    elseif action == TALGA_KEYRELEASE then
        self.applyLeft = false
        self:playAnimation("standL", 1000, true)
    end
end

local function keyW(self, action)
    if action == TALGA_KEYPRESS then
        if self.hasJumped == false then
            self:getCollider():applyImpulseY(40000)
            self.hasJumped = true
        end
    end
end

local function updateFuncLoc(self, dt)
    if self.applyLeft and self:getCollider():getVx() > -150 then
        self:getCollider():applyImpulseX(-2000)
    end

    if self.applyRight and self:getCollider():getVx() < 150 then
        self:getCollider():applyImpulseX(2000)

    end

    GAME:camera():box():setX(self:getCollider():getX())
    GAME:camera():box():setY(-self:getCollider():getY())
end

local function onCollideWithGround(self)
    self.hasJumped = false;
end

local function stagedFuncLoc(self)
    local animation = {}
    animation["standL"] = { {0, 0, 64, 64}, {64, 0, 64, 64} };
    animation["standR"] = { {128, 0, 64, 64}, {192, 0, 64, 64} };
    animation["walkL"] = {}
    animation["walkR"] = {}
    self.applyLeft = false
    self.applyRight = false
    self.hasJumped = true

    for i=0,5 do
        animation.walkL[i+1] = {64 * i, 64,64,64}
    end

    for i=6,11 do
        animation.walkR[i-5] = {64 * i, 64,64,64}
    end

    GAME:manager():AddTexture("../assets/textures/talgasheet.png")
    spr = AnimSprite.new(GAME:manager():AddAnimationSet("talgaAnim", "talgasheet.png", animation))
    collider = RectCollider.new(64,64)

    self:setRenderable(spr)
    self:setCollider(collider)
    self:setCollisionType(COLL_PLAYER)
    self:playAnimation("standR", 1000, true)
    self:addKeyCallback("D", talga.keyDCback)
    self:addKeyCallback("A", talga.keyACback)
    self:addKeyCallback("W", talga.keyWCback)
   
    self:addCollisionCallback(COLL_MAPGEOM, talga.groundColl)

    self:getCollider():setMass(100)
    self:getCollider():setMoment(INFINITY)
    self:getCollider():setFriction(0.94)
    self:getCollider():setX(200)
end

talga =
{
    stagedFunc = stagedFuncLoc,
    updateFunc = updateFuncLoc,
    keyDCback = keyD,
    keyACback = keyA,
    keyWCback = keyW,
    groundColl = onCollideWithGround
}
