#include <ruby.h>
#include "cord.h"

static void rb_mark_cord_i(void *c)
{
    CORD_WALK_GC(STRING_CORD_P(c), rb_mark_cord_i, rb_gc_mark_maybe, VALUE);
}

static void rb_mark_cord(RCord *c)
{
    if (c) rb_mark_cord_i(c->cord);
}

static void rb_free_cord_i(void *c)
{
    CORD_WALK_GC((!STRING_CORD_P(c)), rb_free_cord_i, xfree, void*);
}

static void rb_free_cord(RCord *c)
{
    if (c){
      rb_free_cord_i(c->cord);
      xfree(c);
    }
}

static VALUE rb_cord_alloc(VALUE klass)
{
    RCord *cs = NULL;
    VALUE c;
    c = Data_Make_Struct(klass, RCord, rb_mark_cord, rb_free_cord, cs);
    cs->len = 0;
    cs->depth = 0;
    cs->cord = EMPTY_CORD;
    return c;
}

static void *rb_cord_balance(void *cord)
{
    return cord;
}

static VALUE rb_merge_cords(void *left, void *right)
{
    char *cord = NULL;
    long left_len, right_len, len;
    left_len = CORD_LENGTH(left);
    right_len = CORD_LENGTH(right);
    len = left_len + right_len;
    cord = ALLOC_N(char, len + 1);
    memcpy(cord, RSTRING_PTR(left), left_len);
    memcpy(cord + left_len, RSTRING_PTR(right), right_len);
    cord[len] = '\0';
    return rb_str_new(cord, len);
}

static VALUE rb_new_flat_cord(void *left, void *right)
{
    return rb_str_new4(rb_merge_cords(left, right));
}

static void *rb_new_concat_cord(void * left, void * right){
    RConcatCord *cord = NULL;
    short int depth;
    cord = ALLOC(RConcatCord);
    cord->left = (void*)left;
    cord->right = (void*)right;
    cord->len = CORD_LENGTH(left) + RSTRING_LEN((VALUE)right);
    depth = (CORD_DEPTH(right) > CORD_DEPTH(left)) ? CORD_DEPTH(right) : CORD_DEPTH(left);
    cord->depth = depth + 1;
    return cord;
}

static void *rb_append_cords(void * left, void *right)
{
    RConcatCord *new_left = NULL;
    RConcatCord *new_right = NULL;
    if (CORD_LENGTH(left) + CORD_LENGTH(right) < STRING_CORD_THRESHOLD){
      return (void *)rb_new_flat_cord(left, right);
    }
    if (!CONCAT_CORD_P(left) && CONCAT_CORD_P(right)){
      new_right = CONCAT_CORD(right);
      if (CORD_LENGTH(left) + CORD_LENGTH(new_right->left) < STRING_CORD_THRESHOLD){
        return rb_new_concat_cord((void *)rb_new_flat_cord(left, new_right->left), new_right->right);
      }
    }
    if (!CONCAT_CORD_P(right) && CONCAT_CORD_P(left)){
      new_left = CONCAT_CORD(left);
      if (CORD_LENGTH(right) + CORD_LENGTH(new_left->right) < STRING_CORD_THRESHOLD){
        return rb_new_concat_cord(new_left->left, (void *)rb_new_flat_cord(new_left->right, right));
      }
    }
    return rb_new_concat_cord(left, right);
}

static void concat_cord_to_s(VALUE buffer, void * cord)
{
    if (EMPTY_CORD_P(cord) || STRING_CORD_P(cord)) return;
    concat_cord_to_s(buffer, CONCAT_CORD(cord)->left);
    concat_cord_to_s(buffer, CONCAT_CORD(cord)->right);
    if (STRING_CORD_P(CONCAT_CORD(cord)->left)) rb_str_cat(buffer, RSTRING_PTR(CONCAT_CORD(cord)->left), RSTRING_LEN(CONCAT_CORD(cord)->left));
    rb_str_cat(buffer, RSTRING_PTR(CONCAT_CORD(cord)->right), RSTRING_LEN(CONCAT_CORD(cord)->right));
}

static VALUE rb_cord_to_s(VALUE obj)
{
    VALUE buffer;
    RCord *c = GetCord(obj);
    if (EMPTY_CORD_P(c->cord)) return rb_str_new2("");
    if (STRING_CORD_P(c->cord)) return (VALUE)c->cord;
    if (CONCAT_CORD_P(c->cord)){
      buffer = rb_str_buf_new((long)CORD_LENGTH(c->cord));
      concat_cord_to_s(buffer, c->cord);
      return rb_str_new4(buffer);
    }
    return Qnil;
}

static VALUE rb_cord_append(VALUE obj, VALUE str)
{
    RCord *c = GetCord(obj);
    if (rb_obj_is_kind_of(str, rb_cCord)) str = rb_cord_to_s(str);
    Check_Type(str, T_STRING);
    if (RSTRING_LEN(str) == 0) return Qnil;
    str = rb_str_new4(str);
    if (EMPTY_CORD_P(c->cord)){
      c->cord = (void *)str;
    }else{
      c->cord = rb_cord_balance(rb_append_cords((void *)c->cord, (void *)str));
    }
    return obj;
}

static VALUE rb_cord_length(VALUE obj)
{
    RCord *c = GetCord(obj);
    if (EMPTY_CORD_P(c->cord)) return INT2FIX(0);
    return INT2FIX(CORD_LENGTH(c->cord));
}

static VALUE rb_cord_depth(VALUE obj)
{
    RCord *c = GetCord(obj);
    if (EMPTY_CORD_P(c->cord)) return INT2FIX(0);
    return INT2FIX(CORD_DEPTH(c->cord));
}

static VALUE rb_cord_initialize(int argc, VALUE *argv, VALUE obj)
{
    VALUE val;
    RCord *c = GetCord(obj);
    rb_scan_args(argc, argv, "01", &val);
    if (!NIL_P(val)) rb_cord_append(obj, val);
    return obj;
}

static VALUE
rb_cord_slice(int argc, VALUE *argv, VALUE obj)
{
    VALUE str;
    str = rb_cord_to_s(obj);
    return rb_funcall2(str, rb_intern("slice"), argc, argv);
}

void
Init_cord()
{
    rb_cCord = rb_define_class("Cord", rb_cObject);
    rb_define_alloc_func(rb_cCord, rb_cord_alloc);

    rb_define_method(rb_cCord, "initialize", rb_cord_initialize, -1);
    rb_define_method(rb_cCord, "<<", rb_cord_append, 1);
    rb_define_method(rb_cCord, "to_s", rb_cord_to_s, 0);
    rb_define_method(rb_cCord, "to_str", rb_cord_to_s, 0);
    rb_define_method(rb_cCord, "length", rb_cord_length, 0);
    rb_define_method(rb_cCord, "depth", rb_cord_depth, 0);
    rb_define_method(rb_cCord, "slice", rb_cord_slice, -1);
}