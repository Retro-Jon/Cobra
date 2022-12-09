function OnReady()
    CreateCamera("main", 0, -25, 0, 0)
    BindToCamera("main")
end

function KeyInput(key, action)
    -- 0 = Release
    -- 1 = Press
    -- 2 = Hold
    if(action == 1 or action == 2) then
        if(key == 256) then -- ESC
            Quit()
        end
        
        if(key == 87) then -- W
            Push(0, 0.1, 0, 0)
            -- print("Forward")
        elseif(key == 83) then -- S
            Push(0, -0.1, 0, 0)
            -- print("Backward")
        end

        if(key == 44) then -- , <
            Push(-0.1, 0, 0, 0)
            -- print("Strafe Left")
        elseif(key == 46) then -- . >
            Push(0.1, 0, 0, 0)
            -- print("Strafe Right")
        end

        if(key == 65) then -- A
            Push(0, 0, 0, -1)
            -- print("Look Left")
        elseif(key == 68) then -- D
            Push(0, 0, 0, 1)
            -- print("Look Right")
        end
    end
end

hp = 10

function Logic()
    if(hp <= 0) then
        DestroyObject()
    end
end

function OnDelete()
    print("Deleted")
end
