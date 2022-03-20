local testdp = require('testdep.testdep')

function test(param1)
    kot.log("test param: ", param1)
    local tdp = testdp:new()
    tdp.data = "tdp data"
    tdp:test()

    kot.log("lua test:",{key1 = "value1", key2 = "value2"}, {1, 3, 4, 76})
end