# **Compiler**

* Formal grammar
  - Define  
    Formal grammar is a 4-tuples
    $$G = <N, \Sigma, P>$$

    Symbols:
    - $N$: nonterminal symbol.   
      It is replaced by groups of terminal symbols according to the production rules.
    - $\Sigma$: terminal symbol.  
      It is the elementary symbols of the language defined by a formal grammar.  
      And the terminals and nonterminals of a particular grammar are two disjoint sets.
    - $P$: production rule, each rule of the form
      $$(\Sigma \cap N)^* N (\Sigma \cup N)^* \to (\Sigma \cup N)^*$$
      where $*$ is the Kleene star operator and $\cup$ denotes set union.  
      It is a rewrite rule specifying a symbol substitution that can be recursively performed to generate new symbol sequences.
    - $S \in N$: start symbol

  - Include
    * CFG, Context-free grammar
      - Define  
        A context-free grammar (CFG) is a formal grammar whose production rules are of the form
          $$A \to \alpha$$
        with $A$ a single nonterminal symbol, and $\alpha$ a string of terminals and/or nonterminals ($\alpha$ can be empty). 
        A formal grammar is "context free" if its production rules can be applied regardless of the context of a nonterminal. 

      - Include
        * Regular grammar
          - Define  
            They require that all production rules have at most one non-terminal symbol, and that symbol is either always at the end or always at the start of the rule's right-hand side.

    * CSG, Context-sensitive grammar
      - Define  
        A context-sensitive grammar is a formal grammar in which the left-hand sides and right-hand sides of any production rules may be surrounded by a context of terminal and nonterminal symbols.

# Lexical Analysis

# Parsing, Syntax Analysis

* Top-down parsing
  * LL(k) parsers

* Bottom-up parsing
  * LR parsers

# Semantic Analysis

