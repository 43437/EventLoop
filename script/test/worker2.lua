function on_load()
    kot.log("worker2 on_load")
end

function on_message(msg)
    kot.log("worker2 on_message", msg)
    return true
end

function on_timer(id)
    
end