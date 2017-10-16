#include <stdio.h>
#include <mpfr.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/fail.h>


static struct custom_operations mpfr_ops = {
  "github.com/thvnx/mlmpfr.1",
  custom_finalize_default,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
  custom_compare_ext_default
};

#define Mpfr_val(m) (*((mpfr_t *) Data_custom_val (m)))

int rounding_mode2mpfr_rnd_t (r)
{
  switch (r)
    {
    case 0: return MPFR_RNDN;
    case 1: return MPFR_RNDZ;
    case 2: return MPFR_RNDU;
    case 3: return MPFR_RNDD;
    case 4: return MPFR_RNDA;
    default:
      caml_failwith(__FUNCTION__);
    }
}

CAMLprim value mpfr_prec_min_ml ()
{
  CAMLparam0 ();
  CAMLreturn (Val_int (MPFR_PREC_MIN));
}

CAMLprim value mpfr_prec_max_ml ()
{
  CAMLparam0 ();
  CAMLreturn (Val_int (MPFR_PREC_MAX));
}

CAMLprim value mpfr_init2_ml (value prec)
{
  CAMLparam1 (prec);
  CAMLlocal1 (initialized_value);
  initialized_value = caml_alloc_custom (&mpfr_ops, sizeof (mpfr_t), 0, 1);
  mpfr_init2 (Mpfr_val (initialized_value), (mpfr_prec_t) Int_val (prec));
  CAMLreturn (initialized_value);
}

CAMLprim value mpfr_inits2_ml (value prec, value n)
{
  CAMLparam2 (prec, n);
  CAMLlocal2 (list, tmp);

  if (Int_val (n) <= 0) // if n is zero, return empty list
    CAMLreturn (Val_int (0));

  // build a list of size n
  list = caml_alloc (2, 0);
  Store_field (list, 0, mpfr_init2_ml (prec));
  Store_field (list, 1, Val_int(0));
  for (int i = 1; i < Int_val (n); i++)
    {
      tmp = caml_alloc (2, 0);
      Store_field (tmp, 0, mpfr_init2_ml (prec));
      Store_field (tmp, 1, list);
      list = tmp;
    }
  CAMLreturn (list);
}

CAMLprim value mpfr_clear_ml (value op)
{
  CAMLparam1 (op);
  mpfr_clear (Mpfr_val (op));
  CAMLreturn (Val_unit);
}

CAMLprim value mpfr_init_ml ()
{
  CAMLparam0 ();
  CAMLlocal1 (initialized_value);
  initialized_value = caml_alloc_custom (&mpfr_ops, sizeof (mpfr_t), 0, 1);
  mpfr_init (Mpfr_val (initialized_value));
  CAMLreturn (initialized_value);
}

CAMLprim value mpfr_inits_ml (value n)
{
  CAMLparam1 (n);
  CAMLlocal2 (list, tmp);

  if (Int_val (n) <= 0) // if n is zero, return empty list
    CAMLreturn (Val_int (0));

  // build a list of size n
  list = caml_alloc (2, 0);
  Store_field (list, 0, mpfr_init_ml ());
  Store_field (list, 1, Val_int(0));
  for (int i = 1; i < Int_val (n); i++)
    {
      tmp = caml_alloc (2, 0);
      Store_field (tmp, 0, mpfr_init_ml ());
      Store_field (tmp, 1, list);
      list = tmp;
    }
  CAMLreturn (list);
}

CAMLprim value mpfr_set_default_prec_ml (value prec)
{
  CAMLparam1 (prec);
  mpfr_set_default_prec (Int_val (prec));
  CAMLreturn (Val_unit);
}

CAMLprim value mpfr_get_default_prec_ml ()
{
  CAMLparam0 ();
  mpfr_prec_t prec = mpfr_get_default_prec ();
  CAMLreturn (Val_int (prec));
}

CAMLprim value mpfr_set_prec_ml (value op, value prec)
{
  CAMLparam2 (op, prec);
  mpfr_set_prec (Mpfr_val (op), Int_val (prec));
  CAMLreturn (Val_unit);
}

CAMLprim value mpfr_get_prec_ml (value op)
{
  CAMLparam1 (op);
  CAMLreturn (Val_int (mpfr_get_prec (Mpfr_val (op))));
}

CAMLprim value mpfr_set_ml (value rop, value op, value rnd)
{
  CAMLparam3 (rop, op, rnd);
  CAMLreturn (Val_int (mpfr_set (Mpfr_val (rop), Mpfr_val (op),
				 rounding_mode2mpfr_rnd_t (Int_val (rnd)))));
}

CAMLprim value mpfr_set_si_ml (value rop, value op, value rnd)
{
  CAMLparam3 (rop, op, rnd);
  CAMLreturn (Val_int (mpfr_set_si (Mpfr_val (rop), Int_val (op),
				   rounding_mode2mpfr_rnd_t (Int_val (rnd)))));
}

CAMLprim value mpfr_set_d_ml (value rop, value op, value rnd)
{
  CAMLparam3 (rop, op, rnd);
  CAMLreturn (Val_int (mpfr_set_d (Mpfr_val (rop), Double_val (op),
				   rounding_mode2mpfr_rnd_t (Int_val (rnd)))));
}

CAMLprim value mpfr_set_str_ml (value rop, value op, value base, value rnd)
{
  CAMLparam4 (rop, op, base, rnd);
  CAMLreturn (Val_int (mpfr_set_str (Mpfr_val (rop), String_val (op),
				     Int_val (base), rounding_mode2mpfr_rnd_t (Int_val (rnd)))));
}

CAMLprim value mpfr_strtofr_ml (value rop, value op, value base, value rnd)
{
  CAMLparam4 (rop, op, base, rnd);
  CAMLreturn (Val_int (mpfr_strtofr (Mpfr_val (rop), String_val (op), NULL,
				     Int_val (base), rounding_mode2mpfr_rnd_t (Int_val (rnd)))));
}

CAMLprim value mpfr_set_nan_ml (value rop)
{
  CAMLparam1 (rop);
  mpfr_set_nan (Mpfr_val (rop));
  CAMLreturn (Val_unit);
}

CAMLprim value mpfr_set_inf_ml (value rop, value sign)
{
  CAMLparam2 (rop, sign);
  mpfr_set_inf (Mpfr_val (rop), Int_val (sign));
  CAMLreturn (Val_unit);
}

CAMLprim value mpfr_set_zero_ml (value rop, value sign)
{
  CAMLparam2 (rop, sign);
  mpfr_set_zero (Mpfr_val (rop), Int_val (sign));
  CAMLreturn (Val_unit);
}

CAMLprim value mpfr_swap_ml (value op1, value op2)
{
  CAMLparam2 (op1, op2);
  mpfr_swap (Mpfr_val (op1), Mpfr_val (op2));
  CAMLreturn (Val_unit);
}

CAMLprim value mpfr_init_set_ml (value op, value rnd)
{
  CAMLparam2 (op, rnd);
  CAMLlocal1 (initialized_value);

  int tv;
  value result;
  initialized_value = caml_alloc_custom (&mpfr_ops, sizeof (mpfr_t), 0, 1);

  tv = mpfr_init_set (Mpfr_val (initialized_value), Mpfr_val (op),
		      rounding_mode2mpfr_rnd_t (Int_val (rnd)));

  Store_field (result, 0, initialized_value);
  Store_field (result, 1, Val_int (tv));
  CAMLreturn (result);
}

CAMLprim value mpfr_init_set_si_ml (value op, value rnd)
{
  CAMLparam2 (op, rnd);
  CAMLlocal1 (initialized_value);

  int tv;
  value result;
  initialized_value = caml_alloc_custom (&mpfr_ops, sizeof (mpfr_t), 0, 1);

  tv = mpfr_init_set_si (Mpfr_val (initialized_value), Int_val (op),
		      rounding_mode2mpfr_rnd_t (Int_val (rnd)));

  Store_field (result, 0, initialized_value);
  Store_field (result, 1, Val_int (tv));
  CAMLreturn (result);
}

CAMLprim value mpfr_init_set_d_ml (value op, value rnd)
{
  CAMLparam2 (op, rnd);
  CAMLlocal1 (initialized_value);

  int tv;
  value result;
  initialized_value = caml_alloc_custom (&mpfr_ops, sizeof (mpfr_t), 0, 1);

  tv = mpfr_init_set_d (Mpfr_val (initialized_value), Double_val (op),
		      rounding_mode2mpfr_rnd_t (Int_val (rnd)));

  Store_field (result, 0, initialized_value);
  Store_field (result, 1, Val_int (tv));
  CAMLreturn (result);
}

CAMLprim value mpfr_init_set_str_ml (value op, value base, value rnd)
{
  CAMLparam3 (op, base, rnd);
  CAMLlocal1 (initialized_value);

  int tv;
  value result;
  initialized_value = caml_alloc_custom (&mpfr_ops, sizeof (mpfr_t), 0, 1);

  tv = mpfr_init_set_str (Mpfr_val (initialized_value), String_val (op), Int_val (base),
		      rounding_mode2mpfr_rnd_t (Int_val (rnd)));

  Store_field (result, 0, initialized_value);
  Store_field (result, 1, Val_int (tv));
  CAMLreturn (result);
}

CAMLprim value mpfr_get_d_ml (value op, value rnd)
{
  CAMLparam2 (op, rnd);
  CAMLreturn (caml_copy_double (mpfr_get_d (Mpfr_val (op), rounding_mode2mpfr_rnd_t (rnd))));
}

CAMLprim value mpfr_get_si_ml (value op, value rnd)
{
  CAMLparam2 (op, rnd);
  CAMLreturn (Val_int (mpfr_get_si (Mpfr_val (op), rounding_mode2mpfr_rnd_t (rnd))));
}

CAMLprim value mpfr_get_d_2exp_ml (value op, value rnd)
{
  CAMLparam2 (op, rnd);
  CAMLlocal1 (result);

  long *exp;
  double dv = mpfr_get_d_2exp (exp, Mpfr_val (op), rounding_mode2mpfr_rnd_t (rnd));

  Store_field (result, 0, caml_copy_double (dv));
  Store_field (result, 1, Val_int (&exp));
  CAMLreturn (result);
}

CAMLprim value mpfr_frexp_ml (value op1, value op2, value rnd)
{
  CAMLparam3 (op1, op2, rnd);
  CAMLlocal1 (result);

  mpfr_exp_t *exp;
  mpfr_frexp (exp, Mpfr_val (op1), Mpfr_val (op2), rounding_mode2mpfr_rnd_t (rnd));
  
  Store_field (result, 0, Val_int (&exp));
  Store_field (result, 1, op1);
  CAMLreturn (result);
}

CAMLprim value mpfr_get_str_ml (value base, value n, value op, value rnd)
{
  CAMLparam4 (base, n, op, rnd);

  char *ret;
  mpfr_exp_t expptr;
  value result;

  ret = mpfr_get_str (NULL, &expptr, Int_val (base), Int_val (n), Mpfr_val (op),
		      rounding_mode2mpfr_rnd_t (Int_val (rnd)));

  result = caml_alloc_tuple (2);

  Store_field (result, 0, caml_copy_string (ret));
  Store_field (result, 1, Val_int (expptr));
  mpfr_free_str (ret);

  CAMLreturn (result);
}

CAMLprim value mpfr_fits_sint_p_ml (value op, value rnd)
{
  CAMLparam2 (op, rnd);
  CAMLreturn (Val_int (mpfr_fits_sint_p (Mpfr_val (op), rounding_mode2mpfr_rnd_t (rnd))));
}
