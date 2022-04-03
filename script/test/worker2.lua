function on_load()
    kot.log("worker2 on_load")
end

function on_message(msg)
    kot.log("worker2 on_message: ", msg)
    if (nil ~= msg["content"] and "stop" == msg["content"]["cmd"]) then
        kot.message({head={src="worker2", dst="worker1"}, content={cmd="stop"}})
        return false
    else
        return true
    end
    return true
end

function on_timer(id)
    
end