Properties = {
    UseTransformation = true
}

x = 0
y = 0
z = 0
h = 0
v = 0

move_speed = 60
look_speed = 55

function OnReady()
    CreateCamera("player_view", 0, -60, 0, 0, 90, 250)
    ConnectCamera("player_view")
    SetViewPortCamera("main_view", "player_view")

    CreateCamera("test_camera", 0, -40, 0, 0, 90, 90)
    CreateViewPort("test_view", 0, 0, 200, 200)
    SetViewPortCamera("test_view", "test_camera")

    CreateCamera("test_camera2", 0, -60, 0, 10, 90, 180)
    CreateViewPort("test_view2", 500, 0, 300, 300)
    SetViewPortCamera("test_view2", "test_camera2")

    -- x = -move_speed
    -- h = look_speed
end

function Event(e)
    if(e == "Start") then
        print("Level Loaded, Game Start")
    end
end

function KeyInput(key, action)
    -- 0 = Release
    -- 1 = Press
    -- 2 = Hold
    if(action == 1) then
        if(key == 256) then -- ESC
            Quit()
        end
        
        if(key == 87) then -- W
            y = move_speed
            --print("Forward")
        elseif(key == 83) then -- S
            y = -move_speed
            --print("Backward")
        end

        if(key == 44) then -- , <
            x = -move_speed
            --print("Strafe Left")
        elseif(key == 46) then -- . >
            x = move_speed
            --print("Strafe Right")
        end

        if(key == 81) then -- Q
            z = move_speed
            --print("Down")
        elseif(key == 69) then -- E
            z = -move_speed
            --print("Up")
        end

        if(key == 65) then -- A
            h = -look_speed
            --print("Look Left")
        elseif(key == 68) then -- D
            h = look_speed
            --print("Look Right")
        end

        if(key == 82) then -- R
            v = look_speed
            --print("Look Up")
        elseif(key == 70) then -- F
            v = -look_speed
            --print("Look Down")
        end

        if(key == 32) then -- SPACE
            Move(0, -40, 0, 0, 90)
            TriggerEvent("Reset")
            --print("Reset")
        end

    elseif(action == 0) then
        if((key == 87 and y > 0) or (key == 83 and y < 0)) then -- W or S
            y = 0
        elseif((key == 44 and x < 0) or (key == 46 and x > 0)) then -- , < or . >
            x = 0
        elseif((key == 65 and h < 0) or (key == 68 and h > 0)) then -- A or D
            h = 0
        elseif((key == 69 and z < 0) or (key == 81 and z > 0)) then -- Q or E
            z = 0
        elseif((key == 70 and v < 0) or (key == 82 and v > 0)) then -- R or F
            v = 0
        end
    end
end

function Logic()
    Push(x, y, z, h, v)
    PushCamera("test_camera2", 60, 0, 0, -55, 0)
end

function OnDelete()
    print("Player Deleted")
end