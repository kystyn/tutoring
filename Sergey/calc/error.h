#ifndef ERROR_H
#define ERROR_H

// TODO: separate errors and functional status
typedef enum {
    OK,
    NO_MEM,
    SYNTAX_ERR,
    MANY_ARG,
    WRONG_NAME,
    EMPTY_STR,
    STREAM_END,
    //PRINT CASES
    NORMAL,
    COMMENT,
    PARSER_ERR,
    //only one case - incorrect lexem
    INF2POL_ERR,
    //three cases
    I2P_NO_PREFIX,
    I2P_NO_SUFFIX,
    I2P_PROBLEM_BRACKETS,
    EVAL_ERR,
    //three cases but i am not sure about first
    INCORRECT_OPERATION,
    DEFAREA,
    EMPTY
} ERR_STATUS;

#endif /* ERROR_H */
