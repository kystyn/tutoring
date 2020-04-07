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
    STREAM_END
} ERR_STATUS;

#endif /* ERROR_H */
