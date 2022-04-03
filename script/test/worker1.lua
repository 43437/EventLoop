local timer_id = nil

local nloop = 4

function on_load()
    kot.log("worker1 on_load")
    timer_id = kot.timer();
    kot.start_timer(timer_id, 1, 1, 1, 1);
end

function on_message(msg)
    kot.log("worker1 on_message: ", msg)
    if (nil ~= msg["content"] and "stop" == msg["content"]["cmd"]) then
        return false
    else
        return true
    end
end

function on_timer(id)
    kot.log("worker1 on_timer ", id)
    nloop = nloop - 1
    if (nloop < 0) then
        kot.stop_timer(timer_id)
        kot.message({head={src="worker1", dst="worker2"}, content={cmd="stop"}})
    else
        kot.message({head={src="worker1", dst="worker1"}})
        kot.message({head={src="worker1", dst="worker2"}})
    end
end