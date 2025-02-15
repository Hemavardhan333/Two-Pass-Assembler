ldc 0         ; Initialize A to 0 (sum accumulator)
stl 0         ; Store sum (initialize sum memory location)
ldl -1        ; Load n (count of numbers)
stl 1         ; Store n in memory location 1 (for division later)
ldc 0         ; Initialize A to 0 (index counter)
stl 2         ; Store index in memory location 2

loop:
    ldl 2         ; Load current index
    ldl -1        ; Load n
    sub           ; Check if index < n
    brz end       ; If index == n, end loop

    ldl 0         ; Load current sum
    ldnl 2        ; Load number at current index
    add           ; Add number to sum
    stl 0         ; Store new sum

    ldl 2         ; Load current index
    ldc 1         ; Increment index by 1
    add
    stl 2         ; Store incremented index

    br loop       ; Repeat loop

end:
    ldl 0         ; Load final sum
    ldl 1         ; Load n
    div           ; Divide sum by n to get mean
    stl 3         ; Store mean in memory location 3

HALT             ; End program
