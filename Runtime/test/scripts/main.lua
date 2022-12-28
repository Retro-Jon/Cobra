function OnReady()
    CreateObject("player.lua")
    TriggerEvent("Start")
end

function Event(e)
    if(e == "Reset") then
        print("Player has been reset.")
    end
end

function OnDelete()
    print("Game Over")
end
