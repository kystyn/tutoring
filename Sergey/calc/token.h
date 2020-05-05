#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    OPERATOR,
    NUMBER
} token_type_t;

typedef enum {
    // binars
    PLUS,
    MINUS,
    MUL,
    DIV,
    POW,
    LBRACE,
    RBRACE,
    MOD,
    // unars
    FIRST_UNAR,
    UNAR_MINUS,
    SIN,
    COS,
    TG,
    CTG,
    ASIN,
    ACOS,
    ATG,
    ACTG,
    LN,
    SQRT,
    FLOOR,
    CEIL,
    NOT_EXISTS
} operator_t;

//#pragma pack(push, 4)
typedef struct tag_token_t {
  token_type_t type;
  union {
    double num;
    operator_t op;
  } value;
} token_t;
//#pragma pack(pop)

#endif /* TOKEN_H */
