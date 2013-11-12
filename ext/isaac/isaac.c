#include "ruby.h"
#include "rand.h"
#include "rand4.h"

#ifndef min
# define min(a,b) (((a)<(b)) ? (a) : (b))
#endif /* min */

static VALUE
ISAAC_s_allocate(VALUE klass)
{
    randctx *ctx;

    return Data_Make_Struct(klass, randctx, NULL, NULL, ctx);
}

static VALUE
ISAAC4_s_allocate(VALUE klass)
{
    randctx4 *ctx;

    return Data_Make_Struct(klass, randctx4, NULL, NULL, ctx);
}

/*
 * Seed the generator with an array of up to ISAAC::RANDSIZ integers in the
 * range 0..2**32-1. More entries are ignored. Missing entries are treated
 * as 0. Returns +nil+.
 */
static VALUE
ISAAC_srand(VALUE self, VALUE ary)
{
    int i;
    randctx *ctx;

    Check_Type(ary, T_ARRAY);
    
    Data_Get_Struct(self, randctx, ctx);
    
    MEMZERO(ctx, randctx, 1);
    for (i=min(RANDSIZ, RARRAY_LEN(ary))-1; i>=0; i--) {
        ctx->randrsl[i] = NUM2UINT(RARRAY_PTR(ary)[i]);
    }
    isaac_init(ctx, 1);

    return Qnil;
}

/*
 * Seed the generator with an array of up to ISAAC::RANDSIZ integers in the
 * range 0..2**32-1. More entries are ignored. Missing entries are treated
 * as 0. Returns +nil+.
 */
static VALUE
ISAAC4_srand(VALUE self, VALUE ary)
{
    int i;
    randctx4 *ctx;

    Check_Type(ary, T_ARRAY);
    
    Data_Get_Struct(self, randctx4, ctx);
    
    MEMZERO(ctx, randctx4, 1);
    for (i=min(RANDSIZ4, RARRAY_LEN(ary))-1; i>=0; i--) {
        ctx->randrsl[i] = NUM2UINT(RARRAY_PTR(ary)[i]);
    }
    isaac_init4(ctx, 1);

    return Qnil;
}

/*
 * Return a random integer in the range 0..2**32-1.
 */
static VALUE
ISAAC_rand32(VALUE self)
{
    randctx *ctx;

    Data_Get_Struct(self, randctx, ctx);

    if (!ctx->randcnt--) {
        isaac_rand(ctx);
        ctx->randcnt=RANDSIZ-1;
    }
    
    return UINT2NUM(ctx->randrsl[ctx->randcnt]);
}

/*
 * Return a random integer in the range 0..2**32-1.
 */
static VALUE
ISAAC4_rand32(VALUE self)
{
    randctx4 *ctx;

    Data_Get_Struct(self, randctx4, ctx);

    if (!ctx->randcnt--) {
        isaac_rand4(ctx);
        ctx->randcnt=RANDSIZ4-1;
    }
    
    return UINT2NUM(ctx->randrsl[ctx->randcnt]);
}

/*
 * Return a random float in the range 0..1.
 */
static VALUE
ISAAC_rand(VALUE self)
{
    randctx *ctx;

    Data_Get_Struct(self, randctx, ctx);

    if (!ctx->randcnt--) {
        isaac_rand(ctx);
        ctx->randcnt=RANDSIZ-1;
    }
    
    return rb_float_new(ctx->randrsl[ctx->randcnt] / 4294967295.0);
}

/*
 * Return a random float in the range 0..1.
 */
static VALUE
ISAAC4_rand(VALUE self)
{
    randctx4 *ctx;

    Data_Get_Struct(self, randctx4, ctx);

    if (!ctx->randcnt--) {
        isaac_rand4(ctx);
        ctx->randcnt=RANDSIZ4-1;
    }
    
    return rb_float_new(ctx->randrsl[ctx->randcnt] / 4294967295.0);
}

static VALUE
ISAAC_marshal_dump(VALUE self)
{
    randctx *ctx;
    int i;
    int ary_size = sizeof(randctx)/sizeof(ub4);
    VALUE ary;

    Data_Get_Struct(self, randctx, ctx);
    
    ary = rb_ary_new2(ary_size);
    for (i = 0; i < ary_size; i++) {
        rb_ary_push(ary, UINT2NUM(((ub4 *)ctx)[i]));
    }
    
    return ary;
}

static VALUE
ISAAC4_marshal_dump(VALUE self)
{
    randctx4 *ctx;
    int i;
    int ary_size = sizeof(randctx4)/sizeof(ub4);
    VALUE ary;

    Data_Get_Struct(self, randctx4, ctx);
    
    ary = rb_ary_new2(ary_size);
    for (i = 0; i < ary_size; i++) {
        rb_ary_push(ary, UINT2NUM(((ub4 *)ctx)[i]));
    }
    
    return ary;
}

static VALUE
ISAAC_marshal_load(VALUE self, VALUE ary)
{
    randctx *ctx;
    int i;
    int ary_size = sizeof(randctx)/sizeof(ub4);

    Data_Get_Struct(self, randctx, ctx);

    if (RARRAY_LEN(ary) != ary_size)
        rb_raise(rb_eArgError, "bad length in loaded ISAAC data");

    for (i = 0; i < ary_size; i++) {
        ((ub4 *)ctx)[i] = NUM2UINT(RARRAY_PTR(ary)[i]);
    }
    
    return self;
}

static VALUE
ISAAC4_marshal_load(VALUE self, VALUE ary)
{
    randctx4 *ctx;
    int i;
    int ary_size = sizeof(randctx4)/sizeof(ub4);

    Data_Get_Struct(self, randctx4, ctx);

    if (RARRAY_LEN(ary) != ary_size)
        rb_raise(rb_eArgError, "bad length in loaded ISAAC4 data");

    for (i = 0; i < ary_size; i++) {
        ((ub4 *)ctx)[i] = NUM2UINT(RARRAY_PTR(ary)[i]);
    }
    
    return self;
}

void
Init_isaac()
{
    VALUE ISAAC;
    VALUE ISAAC4;
    VALUE mPRNG;

    mPRNG = rb_define_module("PRNG");
    ISAAC = rb_define_class_under(mPRNG, "ISAAC", rb_cObject);
    ISAAC4 = rb_define_class_under(mPRNG, "ISAAC4", rb_cObject);
    rb_define_const(ISAAC, "VERSION", rb_str_new_cstr("0.1.2"));
        
    rb_define_alloc_func(ISAAC, ISAAC_s_allocate);
    rb_define_method(ISAAC, "srand", ISAAC_srand, 1);
    rb_define_method(ISAAC, "rand32", ISAAC_rand32, 0);
    rb_define_method(ISAAC, "rand", ISAAC_rand, 0);
    rb_define_method(ISAAC, "marshal_dump", ISAAC_marshal_dump, 0);
    rb_define_method(ISAAC, "marshal_load", ISAAC_marshal_load, 1);
    
    rb_define_alloc_func(ISAAC4, ISAAC4_s_allocate);
    rb_define_method(ISAAC4, "srand", ISAAC4_srand, 1);
    rb_define_method(ISAAC4, "rand32", ISAAC4_rand32, 0);
    rb_define_method(ISAAC4, "rand", ISAAC4_rand, 0);
    rb_define_method(ISAAC4, "marshal_dump", ISAAC4_marshal_dump, 0);
    rb_define_method(ISAAC4, "marshal_load", ISAAC4_marshal_load, 1);
    
    rb_const_set(ISAAC, rb_intern("RANDSIZ"), UINT2NUM(RANDSIZ));
    rb_const_set(ISAAC4, rb_intern("RANDSIZ"), UINT2NUM(RANDSIZ4));
}
