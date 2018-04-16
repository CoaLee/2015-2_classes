type treasure = Starbox | Namebox of string
type key = Bar | Node of key * key
type resolving_key = Barr 
                   | Unknown of string 
									 | Node of resolving_key * resolving_key 
									 | Freeset 
									 | Known of string * resolving_key
									 | Unknownn
type environment = (string * resolving_key) list

type map = End of treasure
         | Branch of map * map
         | Guide of string * map

exception IMPOSSIBLE
exception I

let rec implyingKey (e:map) (r:resolving_key) (env: enviornment) : (resolving_key * environment) = 
  if r = Freeset
    then
	  match e with
	  | End Starbox -> (Barr, env)
	  | End (Namebox s) -> (Unknown s, env)
	  | Guide (s, e1) -> (Node (Unknown s, fst (implyingKey e1 Freeset env)), snd (implyingKey e1 Freeset env))
	  | Branch (e1, e2) -> let (a, env') = fst (implyingKey e2 Freeset env) in
		                     let (k, env'') = fst (implyingKey e1 a env') in
												 match k with 
												 | Node (a, b) -> (b, env'')
												 | _ -> raise IMPOSSIBLE
  	else if r = (Node (a, b))
		  then
			match e with
			| End Starbox -> raise IMPOSSIBLE
			| End (Namebox s) -> (r, (s, r)::env)
			| Guide (s, e1) -> (
			| Branch (e1, e2) -> let a = (implyingKey e2 Freeset) in
													 let k = (implyingKey e1 (Node (a, r)) in
													 (match k with
													 | Node (a, b) -> if b = r then r else raise IMPOSSIBLE
													 | _ -> IMPOSSIBLE)
			else (*ASSERT: r = Barr | Unknown of string *)
			match e with
			| End Starbox -> raise IMPOSSIBLE
			| End (Namebox s) -> Known (s, Node (r, Unknownn))
			| Guide (s, e1) -> Node (Known (s, r), implyingKey e1 Freeset)
			| Branch (e1, e2) -> let a = (implyingKey e2 Freeset) in
													 let k = (implyingKey e1 (Node (a, r)) in 
													 (match k with
													 | Node (a, b) -> if b = r then r else raise IMPOSSIBLE
													 | _ -> IMPOSSIBLE)
(*
let rec getReady (m:map) :key list =
  let rec implying m = 
    match m with
	 | End t -> (match t with 
                | Starbox -> Bar 
		  			 | Namebox s -> implying TODO )
    | Guide (s, m') -> let b = implying m' in
	                    let a = s in
							  Node (a, b)
    | Branch (m1, m2) -> let k' = implying m1 in
	 							 if k' = Bar 
								   then raise IMPOSSIBLE
								   else match k' with Node (a, b) in
								 let c = implying m2 in
								 if a = c then b else raise IMPOSSIBLE


	                    
  match m with
  | End t -> (match t with 
                | Starbox -> Bar 
					 | Namebox of string -> )
  | Branch (m1, m2) ->  
  | Guide (s, m') ->  

  match m with 
  | End Starbox -> Bar
  | End (Namebox s) -> Bar
  | Branch (m1, m2) -> 
  | Guide (s, m') ->

  match m with
  | End 

  *)

let t0 = End Starbox
let t1 = End (Namebox "x")
let t2 = Guide ("x", t1)
let t3 = Branch (t2, t0)
let t4 = Branch (Guide ("x", Branch (t1, t1)), t0)
let t5 = Branch (t2, Branch (Guide ("y", End (Namebox "y")), t0))
let t6 = Branch (t2, Guide ("y", End (Namebox "y")))
let t7 = Branch (t1, t0)
