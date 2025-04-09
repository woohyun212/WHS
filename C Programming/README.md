JSON 특성상 재귀적으로 탐색하게 하는 것이 좋을 것 같아 DFS 추적하게끔 설계해보았습니다.
# 실행 결과
```
/WHS/Cprogramming/ast_parser ast.json
File Read Success, File size: 794323
Variable: FuncDecl exit
Variable: FuncDecl getchar
Variable: FuncDecl malloc
Variable: FuncDecl putchar
Variable: FuncDecl main1
Function: main() -> int
    Return: main1()
main 함수의 if조건 개수: 0

Function: my_realloc(*char old, int oldlen, int newlen) -> *char
    Variable: *char new
    Variable: int i
    While condition: (i <= (oldlen - 1))
    Return: new
my_realloc 함수의 if조건 개수: 0

Variable: int nextc
Variable: *char token
Variable: int token_size
Function: error() -> void
error 함수의 if조건 개수: 0

Variable: int i
Function: takechar() -> void
    If condition: (token_size <= (i + 1))
        Variable: int x
    Else:
takechar 함수의 if조건 개수: 1

Function: get_token() -> void
    Variable: int w
    While condition: w
        While condition: (((nextc == ' ') | (nextc == 9)) | (nextc == 10))
        While condition: (((('a' <= nextc) & (nextc <= 'z')) | (('0' <= nextc) & (nextc <= '9'))) | (nextc == '_'))
        If condition: (i == 0)
            While condition: ((((((nextc == '<') | (nextc == '=')) | (nextc == '>')) | (nextc == '|')) | (nextc == '&')) | (nextc == '!'))
        Else:
        If condition: (i == 0)
            If condition: (nextc == 39)
                While condition: (nextc != 39)
            Else:
                If condition: (nextc == '"')
                    While condition: (nextc != '"')
                Else:
                    If condition: (nextc == '/')
                        If condition: (nextc == '*')
                            While condition: (nextc != '/')
                                While condition: (nextc != '*')
                        Else:
                    Else:
                        If condition: (nextc != (0 - 1))
                        Else:
        Else:
get_token 함수의 if조건 개수: 7

Function: peek(*char s) -> int
    Variable: int i
    While condition: ((s[i] == token[i]) & (s[i] != 0))
    Return: (s[i] == token[i])
peek 함수의 if조건 개수: 0

Function: accept(*char s) -> int
    If condition: peek(s)
        Return: 1
    Else:
        Return: 0
accept 함수의 if조건 개수: 1

Function: expect(*char s) -> void
    If condition: (accept(s) == 0)
    Else:
expect 함수의 if조건 개수: 1

Variable: *char code
Variable: int code_size
Variable: int codepos
Variable: int code_offset
Function: save_int(*char p, int n) -> void
save_int 함수의 if조건 개수: 0

Function: load_int(*char p) -> int
    Return: ((((p[0] & 255) + ((p[1] & 255) << 8)) + ((p[2] & 255) << 16)) + ((p[3] & 255) << 24))
load_int 함수의 if조건 개수: 0

Function: emit(int n, *char s) -> void
    If condition: (code_size <= (codepos + n))
        Variable: int x
    Else:
    While condition: (i <= (n - 1))
emit 함수의 if조건 개수: 1

Function: be_push() -> void
be_push 함수의 if조건 개수: 0

Function: be_pop(int n) -> void
be_pop 함수의 if조건 개수: 0

Variable: *char table
Variable: int table_size
Variable: int table_pos
Variable: int stack_pos
Function: sym_lookup(*char s) -> int
    Variable: int t
    Variable: int current_symbol
    While condition: (t <= (table_pos - 1))
        While condition: ((s[i] == table[t]) & (s[i] != 0))
        If condition: (s[i] == table[t])
        Else:
        While condition: (table[t] != 0)
    Return: current_symbol
sym_lookup 함수의 if조건 개수: 1

Function: sym_declare(*char s, int type, int value) -> void
    Variable: int t
    While condition: (s[i] != 0)
        If condition: (table_size <= (t + 10))
            Variable: int x
        Else:
sym_declare 함수의 if조건 개수: 1

Function: sym_declare_global(*char s) -> int
    Variable: int current_symbol
    If condition: (current_symbol == 0)
    Else:
    Return: current_symbol
sym_declare_global 함수의 if조건 개수: 1

Function: sym_define_global(int current_symbol) -> void
    Variable: int i
    Variable: int j
    Variable: int t
    Variable: int v
    If condition: (table[(t + 1)] != 'U')
    Else:
    While condition: i
sym_define_global 함수의 if조건 개수: 1

Variable: int number_of_args
Function: sym_get_value(*char s) -> void
    Variable: int t
    If condition: (Assignment == 0)
    Else:
    If condition: (table[(t + 1)] == 'D')
    Else:
        If condition: (table[(t + 1)] == 'U')
        Else:
            If condition: (table[(t + 1)] == 'L')
                Variable: int k
            Else:
                If condition: (table[(t + 1)] == 'A')
                    Variable: int k
                Else:
sym_get_value 함수의 if조건 개수: 5

Function: be_start() -> void
be_start 함수의 if조건 개수: 0

Function: be_finish() -> void
    While condition: (i <= (codepos - 1))
be_finish 함수의 if조건 개수: 0

Function: promote(int type) -> void
    If condition: (type == 1)
    Else:
        If condition: (type == 2)
        Else:
promote 함수의 if조건 개수: 2

Variable: FuncDecl expression
Function: primary_expr() -> int
    Variable: int type
    If condition: (('0' <= token[0]) & (token[0] <= '9'))
        Variable: int n
        While condition: token[i]
    Else:
        If condition: (('a' <= token[0]) & (token[0] <= 'z'))
        Else:
            If condition: accept("(")
                If condition: (peek(")") == 0)
                Else:
            Else:
                If condition: ((((token[0] == 39) & (token[1] != 0)) & (token[2] == 39)) & (token[3] == 0))
                Else:
                    If condition: (token[0] == '"')
                        Variable: int i
                        Variable: int j
                        Variable: int k
                        While condition: (token[j] != '"')
                            If condition: ((token[j] == 92) & (token[(j + 1)] == 'x'))
                                If condition: (token[(j + 2)] <= '9')
                                Else:
                                If condition: (token[(j + 3)] <= '9')
                                Else:
                            Else:
                    Else:
    Return: type
primary_expr 함수의 if조건 개수: 9

Function: binary1(int type) -> void
binary1 함수의 if조건 개수: 0

Function: binary2(int type, int n, *char s) -> int
    Return: 3
binary2 함수의 if조건 개수: 0

Function: postfix_expr() -> int
    Variable: int type
    If condition: accept("[")
    Else:
        If condition: accept("(")
            Variable: int s
            If condition: (accept(")") == 0)
                While condition: accept(",")
            Else:
        Else:
    Return: type
postfix_expr 함수의 if조건 개수: 3

Function: additive_expr() -> int
    Variable: int type
    While condition: 1
        If condition: accept("+")
        Else:
            If condition: accept("-")
            Else:
                Return: type
additive_expr 함수의 if조건 개수: 2

Function: shift_expr() -> int
    Variable: int type
    While condition: 1
        If condition: accept("<<")
        Else:
            If condition: accept(">>")
            Else:
                Return: type
shift_expr 함수의 if조건 개수: 2

Function: relational_expr() -> int
    Variable: int type
    While condition: accept("<=")
    Return: type
relational_expr 함수의 if조건 개수: 0

Function: equality_expr() -> int
    Variable: int type
    While condition: 1
        If condition: accept("==")
        Else:
            If condition: accept("!=")
            Else:
                Return: type
equality_expr 함수의 if조건 개수: 2

Function: bitwise_and_expr() -> int
    Variable: int type
    While condition: accept("&")
    Return: type
bitwise_and_expr 함수의 if조건 개수: 0

Function: bitwise_or_expr() -> int
    Variable: int type
    While condition: accept("|")
    Return: type
bitwise_or_expr 함수의 if조건 개수: 0

Function: expression() -> int
    Variable: int type
    If condition: accept("=")
        If condition: (type == 2)
        Else:
    Else:
    Return: type
expression 함수의 if조건 개수: 2

Function: type_name() -> void
    While condition: accept("*")
type_name 함수의 if조건 개수: 0

Function: statement() -> void
    Variable: int p1
    Variable: int p2
    If condition: accept("{")
        Variable: int n
        Variable: int s
        While condition: (accept("}") == 0)
    Else:
        If condition: (peek("char") | peek("int"))
            If condition: accept("=")
            Else:
        Else:
            If condition: accept("if")
                If condition: accept("else")
                Else:
            Else:
                If condition: accept("while")
                Else:
                    If condition: accept("return")
                        If condition: (peek(";") == 0)
                        Else:
                    Else:
statement 함수의 if조건 개수: 8

Function: program() -> void
    Variable: int current_symbol
    While condition: token[0]
        If condition: accept(";")
        Else:
            If condition: accept("(")
                Variable: int n
                While condition: (accept(")") == 0)
                    If condition: (peek(")") == 0)
                    Else:
                If condition: (accept(";") == 0)
                Else:
            Else:
program 함수의 if조건 개수: 4

Function: main1() -> int
    Return: 0
main1 함수의 if조건 개수: 0


전체 함수의 개수: 36
전체 조건문의 개수: 54

종료 코드 0(으)로 완료된 프로세스
```
