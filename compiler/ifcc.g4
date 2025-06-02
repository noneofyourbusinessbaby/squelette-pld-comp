grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' core * return_stmt '}' ;

return_stmt: RETURN CONST ';' # return_const
            | RETURN VARIABLE ';' # return_var
            ; 

core : declaration # core_declaration
      | assignment # core_assignment
    ;

declaration: INT VARIABLE '=' CONST ';' # declare_const
            | INT VARIABLE '=' VARIABLE ';' # declare_var
            | INT VARIABLE ';' # declare_empty
            ;

assignment: VARIABLE '=' CONST ';' # assign_const
            | VARIABLE '=' VARIABLE ';' # assign_var
            ;

RETURN : 'return' ;
INT: 'int' ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
VARIABLE : [a-zA-Z_][a-zA-Z0-9_]* ;
