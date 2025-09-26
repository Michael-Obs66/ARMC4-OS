target sim
load
break main
break kernel_panic
run
while 1
    if $pc == 0xFFFFFFFE
        echo "System halted"
        break
    end
    continue
end
info registers
x/10i $pc
