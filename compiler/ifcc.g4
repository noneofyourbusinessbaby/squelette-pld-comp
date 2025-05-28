grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' core * return_stmt '}' ;

return_stmt: RETURN CONST ';'
            | RETURN VARIABLE ';'
            ;

core : declaration
    | assignment
    ;

declaration: INT VARIABLE '=' CONST ';'
            | INT VARIABLE '=' VARIABLE ';'
            | INT VARIABLE ';'
            ;

assignment: VARIABLE '=' CONST ';'
            | VARIABLE '=' VARIABLE ';'
            ;

RETURN : 'return' ;
INT: 'int' ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
VARIABLE : [a-zA-Z_][a-zA-Z0-9_]* ;
