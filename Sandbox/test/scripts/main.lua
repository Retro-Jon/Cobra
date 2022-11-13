function Ready()
    Test()
    print(ReturnTest())
end

a = 0

function Logic()
    print(a)
    a = a + 1

    if a >= 2 then
        Quit()
    end
end

function Close()
end
