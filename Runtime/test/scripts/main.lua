function Ready()
    print("Hello from LUA")
end

a = 0

function Logic()
    print(a)
    a = a + 1

    if a >= 2 then
        Quit()
    end
end

function Delete()
end
