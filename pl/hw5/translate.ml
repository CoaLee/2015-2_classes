(*
 * SNU 4190.310 Programming Languages 
 * K-- to SM5 translator skeleton code
 * Jaeseung Choi (jschoi@ropas.snu.ac.kr)
 *)

open K
open Sm5
module Translator = struct
  let rec trans : K.program -> Sm5.command = function
    | K.NUM i -> [Sm5.PUSH (Sm5.Val (Sm5.Z i))]
    | K.TRUE -> [Sm5.PUSH (Sm5.Val (Sm5.B true))]
    | K.FALSE -> [Sm5.PUSH (Sm5.Val (Sm5.B false))]
		| K.UNIT -> [Sm5.PUSH (Sm5.Val (Sm5.Unit))]
    | K.VAR x -> [Sm5.PUSH (Sm5.Id x); Sm5.LOAD] 
    | K.ADD (e1, e2) -> trans e1 @ trans e2 @ [Sm5.ADD]
		| K.SUB (e1, e2) -> trans e1 @ trans e2 @ [Sm5.SUB]
		| K.MUL (e1, e2) -> trans e1 @ trans e2 @ [Sm5.MUL]
		| K.DIV (e1, e2) -> trans e1 @ trans e2 @ [Sm5.DIV]
		| K.EQUAL (e1, e2) -> trans e1 @ trans e2 @ [Sm5.EQ]
		| K.LESS (e1, e2) -> trans e1 @ trans e2 @ [Sm5.LESS]
		| K.NOT e -> trans e @ [Sm5.NOT]
	  | K.ASSIGN (x, e) -> trans e @ [Sm5.BIND x]
(**)| K.SEQ (e1, e2) -> trans e1 @ trans e2
		| K.IF (e1, e2, e3) -> trans e1 @ [Sm5.JTR (trans e2, trans e3)]
(**)| K.WHILE (e1, e2) -> 
		  let cmdT = trans e2 @ trans (K.WHILE (e1, e2)) in 
			let cmdF = trans (K.UNIT) in
		  trans e1 @ [Sm5.JTR (cmdT, cmdF)]
(**)| K.FOR (x, e1, e2, e3) -> 
			let forT = trans (K.LETV (x, e1, e3)) in
			let forF = [Sm5.PUSH (Sm5.Val (Sm5.Unit))] in
			let newFor = trans (K.FOR (x, K.ADD (e1, (K.NUM 1)), e2, e3)) in
      trans e2 @ trans e1 @ [Sm5.LESS; Sm5.JTR (forF, forT @ newFor)]
    | K.LETV (x, e1, e2) ->
      trans e1 @ [Sm5.MALLOC; Sm5.BIND x; Sm5.PUSH (Sm5.Id x); Sm5.STORE] @
			trans e2 @ [Sm5.UNBIND; Sm5.POP]
(**)| K.LETF (f, x, e1, e2) ->  
      [Sm5.PUSH (Sm5.Fn (x, trans e1)); Sm5.BIND f] @ trans e2
(**)| K.CALLV (f, e) -> 
      [Sm5.PUSH (Sm5.Id f)] @ trans e @ [Sm5.MALLOC; Sm5.CALL]
(**)| K.CALLR (f, y) -> 
      [Sm5.PUSH (Sm5.Id f); Sm5.PUSH (Sm5.Id y); Sm5.MALLOC; Sm5.CALL]
    | K.READ x -> [Sm5.GET; Sm5.PUSH (Sm5.Id x); Sm5.STORE; Sm5.PUSH (Sm5.Id x); Sm5.LOAD]
(**)| K.WRITE e -> (trans e) @ [Sm5.PUT] (* @ Sm5.PUSH it? *)
    (*| _ -> failwith "Unimplemented"*)
end
