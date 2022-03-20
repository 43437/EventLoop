local testdp = {}

function testdp:new(o)
    local o = o or {}
    setmetatable(o, {__index = self})
    return o
end

function testdp:test()
    kot.log('testdp:test: ', self.data)
end

return testdp;