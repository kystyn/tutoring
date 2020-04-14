#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    OPERATOR,
    NUMBER
} token_type_t;

//#pragma pack(push, 4)
typedef struct tag_token_t {
  token_type_t type;
  union {
      double num;
      char op;
  } value;
} token_t;
//#pragma pack(pop)

#endif /* TOKEN_H */
