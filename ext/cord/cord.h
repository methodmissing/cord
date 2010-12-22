typedef struct {
    long len;
    short int depth;
    void *left, *right;
} RConcatCord;

typedef struct {
    long len;
    long depth;
    void *cord;
} RCord;

VALUE rb_cCord;

#define STRING_CORD_THRESHOLD 17
#define MAX_CORD_DEPTH        96

#define GetCord(obj)          (Check_Type(obj, T_DATA), (RCord*)DATA_PTR(obj))

#define EMPTY_CORD            NULL
#define STRING_CORD(c)        RSTRING(c)
#define CONCAT_CORD(c)        ((RConcatCord *)(c))
#define EMPTY_CORD_P(c)       (!(c))
#define STRING_CORD_P(c)      (BUILTIN_TYPE(c) == T_STRING)
#define CONCAT_CORD_P(c)      (!EMPTY_CORD_P(c) && !(STRING_CORD_P(c)))
#define CORD_LENGTH(c)        (STRING_CORD_P(c) ? RSTRING_LEN(c) : CONCAT_CORD(c)->len)
#define CORD_DEPTH(c)         (STRING_CORD_P(c) ? 1 : CONCAT_CORD(c)->depth)
#define CORD_WALK_GC(cond, iter, func, cast) \
    if (c == NULL) return; \
    if (CONCAT_CORD_P(c)){  \
      (iter)(CONCAT_CORD(c)->left); \
      (iter)(CONCAT_CORD(c)->right); \
    }else{ \
      if((cond)) (func)((cast)c); \
    }
