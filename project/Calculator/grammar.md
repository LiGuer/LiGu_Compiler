# Terminal Symbol

|Terminal Symbol|String|level|
|---|---|---|
| **object** |
| INT |
| REAL |
| VAR |
| MAT |
| ( |
| ) |
| **operator** |
| NEG | '-' | 0 |
| ADD | '+' | 0 |
| SUB | '-' | 0 |
| MUL | '*' | 0 |
| DIV | '/' | 0 |
| POWER | '^' | 0 |
| EQUAL | '==' | 0 |
| ASSIGN | '=' | 0 |
| FUCTION |  | 0 |
| **basic function** |
| SQRT | 'sqrt' | 0 |
| EXP | 'exp' | 0 |
| LOG | 'log' | 0 |
| ABS | 'abs' | 0 |
| SIN | 'sin' | 0 |
| COS | 'cos' | 0 |
| TAN | 'tan' | 0 |
| ARCSIN | 'arcsin' | 0 |
| ARCCOS | 'arccos' | 0 |
| ARCTAN | 'arctan' | 0 |
| **variable** |
| SUM | 'sum' |
| INTEGRAL | 'integral' |
| SOLVE | 'solve' |
| | |


# Non-Terminal Symbol

|Non-Terminal Symbol|
|---|
| statements |
| expr_level |
| assignment |

# Production Rule

* statements  
    $\quad \to$ statements , assignment  
    $\quad \to$ statements , expr_{maxlevel}  
    $\quad \to$ assignment  
    $\quad \to  expr_{maxlevel}$  

* $expr_4$   
    $\quad \to expr_4 + expr_3$  
    $\quad \to expr_4 - expr_3$  
    $\quad \to expr_3$  

* $expr_3$  
    $\quad \to expr_3 * expr_2$  
    $\quad \to expr_3 / expr_2$  
    $\quad \to expr_2$  

* $expr_2$  
    $\quad \to expr_2$ ^ $expr_1$  
    $\quad \to expr_1$  

* $expr_1$  
    $\quad \to - expr_0$  
    $\quad \to \text{FUNCTION } expr_0$  
    $\quad \to expr_0$  

* $expr_0$  
    $\quad \to$ INT  
    $\quad \to$ REAL  
    $\quad \to$ VAR  
    $\quad \to$ ( $expr_{maxlevel}$ )  

* assignment  
    $\quad \to$ VAR = INT  
    $\quad \to$ VAR = REAL  

# LR Parser
