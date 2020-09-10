#TEST_RETURN_CODE=DO_NOT_CHECK

$ 01                    #doit etre $01
- 5                     #doit etre -5
$01, $02                #doit etre $01 , $02
(equation)              #doit etre ( equation )
etiquette :             #doit etre etiquette:
. directive             #doit etre .directive
LW  $01 , 2             #doit etre LW $01 , 2
LW      $01 , 2         #doit etre LW $01 , 2
