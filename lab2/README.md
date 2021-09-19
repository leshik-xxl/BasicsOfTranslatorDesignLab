# Bikerei KV-82 Variant 2

```
1. <signal-program> --> <program>
2. <program> --> PROGRAM <procedure-identifier> ;
<block>. |
PROCEDURE <procedure-identifier><parameters-list> ; <block> ;
3. <block> --> BEGIN <statements-list> END
4. <statements-list> --> <empty>
5. <parameters-list> --> ( <declarations-list> ) | <empty>
6. <declarations-list> --> <declaration><declarations-list> |
<empty>
7. <declaration> --><variable-identifier><identifiers-list>:<attribute><attributes-list> ;
8. <identifiers-list> --> , <variable-identifier> 
<identifiers-list> |
<empty>
9. <attributes-list> --> <attribute> <attributes-list> | <empty>
10. <attribute> --> SIGNAL |
COMPLEX |
INTEGER |
FLOAT |
BLOCKFLOAT |
EXT
11. <procedure-identifier> --> <identifier>
12. <variable-identifier> --> <identifier>
13. <identifier> --> <letter><string>
14. <string> --> <letter><string> | <digit><string> | <empty>
15. <digit> --> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |9
16. <letter> --> A | B | C | D | ... | Z
```
## Errors
1. Specified two or more base attributes the same time;
2. Base attribute already declared;
3. Variable with this name was already declared.
