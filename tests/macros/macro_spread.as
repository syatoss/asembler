macro m1
    this line is form the macro
    this line is also from the macro
endmacro

this line is not from macro

macro m2
    this line is from macro 2
    should apear only once
endmacro

m1
m2

macro m3
    this line should not apear as a seperate one
endmacro

macro3: m3 m1

;m2
