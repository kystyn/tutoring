#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    OPERATOR,
    NUMBER
} token_type_t;

typedef struct tag_token_t {
  token_type_t type;
  union {
      double num;
      char op;
  } value;
} token_t;

#endif /* TOKEN_H */
