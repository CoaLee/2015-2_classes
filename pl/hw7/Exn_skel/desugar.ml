(*
 * SNU 4190.310 Programming Languages 
 * Homework "Exceptions are sugar" Skeleton
 * Jaeseung Choi (jschoi@ropas.snu.ac.kr)
 *)

open Xexp

let pair : xexp -> xexp -> xexp = fun e1 e2 ->
  Fn ("x", If (Var "x", e1, e2))

let trueval : xexp = Equal (Num 1, Num 1) 
let falseval : xexp = Equal (Num 1, Num 0)

let pfst : xexp -> xexp = fun p ->
  App (Fn ("x", App (Var "x", trueval)), p)

let psnd : xexp -> xexp = fun p ->
  App (Fn ("x", App (Var "x", falseval)), p)

let removeExn : xexp -> xexp = fun xexpress -> 
  let rec removeexn : xexp*xexp -> xexp*xexp = fun xe ->
		let (ev, excpt) = xe in 
		match ev with
		| Num n -> xe 
		| Var s -> xe 
		| Fn (f, e) -> (Fn (f, fst (removeexn (e, excpt))), excpt)
		| App (e1, e2) -> (App (fst (removeexn (e1, excpt)), fst (removeexn (e2, excpt))), excpt)
		| If (e1, e2, e3) -> (If (fst (removeexn (e1, excpt)), fst (removeexn (e2, excpt)), fst (removeexn (e3, excpt))), excpt)
		| Equal (e1, e2) -> (Equal (fst (removeexn (e1, excpt)), fst (removeexn (e2, excpt))), excpt)
		| Raise e -> (App (excpt, fst (removeexn (e, excpt))), excpt)
		| Handle (e1, n, e2) -> 
		  let newExcpt = Fn ("x", If (Equal (Var "x", Num n), fst (removeexn (e2, excpt)), App (excpt, Var "x")))
			in
		  (fst (removeexn (e1, newExcpt)), excpt)
	in
	fst (removeexn (xexpress, (Fn ("x", Num 201511))))

(*
		let ev = pfst xe and excpt = psnd xe in (* ERROR TODO : it works after the evaluation *)
		match ev with
		| Num n -> pair ev excpt 
		| Var s -> pair ev excpt
		| Fn (f, e) -> pair (Fn (f, removeexn (pair e excpt))) excpt
		| App (e1, e2) -> pair (App (removeexn (pair e1 excpt), removeexn (pair e2 excpt))) excpt
		| If (e1, e2, e3) -> pair (If (removeexn (pair e1 excpt), removeexn (pair e2 excpt), removeexn (pair e3 excpt))) excpt
		| Equal (e1, e2) -> pair (Equal (removeexn (pair e1 excpt), removeexn (pair e2 excpt))) excpt
		| Raise e -> App (excpt, removeexn (pair e excpt))
		| Handle (e1, n, e2) -> 
		  let newExcpt = Fn ("x", If (Equal (Var "x", Num n), removeexn (pair e2 excpt), excpt))
			in
		  pair (removeexn (pair e1 newExcpt)) excpt
	in
	removeexn (pair xexpress (Fn ("x", Num 201511)))
	*)
