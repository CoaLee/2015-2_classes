module type KMMM =
  sig
   type exp =
     | Num of int
     | Bool of bool
     | Var of string
     | Add of exp * exp
     | Rev of exp
     | Less of exp * exp
     | Not of exp

   type com =
     | Skip
     | Assign of (Var of string) * exp
     | Seq of com * com
     | If of exp * com * com
     | While of exp * com
     | For of (Assign of (Var of string)) * exp * com
     | Read (Var of string)
     | Write exp

   type prog = com
