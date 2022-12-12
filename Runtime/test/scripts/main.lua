function OnReady()
    CreateCamera("main", 0, 0, 0, 0)
    BindToCamera("main")
    Move(0, -20, 0, 0)
end

x = 0
y = 0
z = 0
r = 0

function KeyInput(key, action)
    -- 0 = Release
    -- 1 = Press
    -- 2 = Hold
    if(action == 1) then
        if(key == 256) then -- ESC
            Quit()
        end
        
        if(key == 87) then -- W
            y = 0.01
            --print("Forward")
        elseif(key == 83) then -- S
            y = -0.01
            --print("Backward")
        end

        if(key == 44) then -- , <
            x = -0.01
            --print("Strafe Left")
        elseif(key == 46) then -- . >
            x = 0.01
            --print("Strafe Right")
        end

        if(key == 65) then -- A
            r = -0.02
            --print("Look Left")
        elseif(key == 68) then -- D
            r = 0.02
            --print("Look Right")
        end
    elseif(action == 0) then
        if((key == 87 and y > 0) or (key == 83 and y < 0)) then -- W or S
            y = 0
        elseif((key == 44 and x < 0) or (key == 46 and x > 0)) then -- , < or . >
            x = 0
        elseif((key == 65 and r < 0) or (key == 68 and r > 0)) then -- A or D
            r = 0
        end
    end
end

function Logic()
    Push(x, y, z, r)
end

function OnDelete()
    print("Deleted")
end
