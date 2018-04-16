(*
 * SNU 4190.310 Programming Languages 2015 Fall
 *  K- Interpreter Skeleton Code
 * Jaeseung Choi (jschoi@ropas.snu.ac.kr)
 *)

(* Location Signature *)
module type LOC =
sig
  type t
  val base : t
  val equal : t -> t -> bool
  val diff : t -> t -> int
  val increase : t -> int -> t
end

module Loc : LOC =
struct
  type t = Location of int
  let base = Location(0)
  let equal (Location(a)) (Location(b)) = (a = b)
  let diff (Location(a)) (Location(b)) = a - b
  let increase (Location(base)) n = Location(base+n)
end

(* Memory Signature *)
module type MEM = 
sig
  type 'a t
  exception Not_allocated
  exception Not_initialized
  val empty : 'a t (* get empty memory *)
  val load : 'a t -> Loc.t  -> 'a (* load value : Mem.load mem loc => value *)
  val store : 'a t -> Loc.t -> 'a -> 'a t (* save value : Mem.store mem loc value => mem' *)
  val alloc : 'a t -> Loc.t * 'a t (* get fresh memory cell : Mem.alloc mem => (loc, mem') *)
end

(* Memory Implementation *)
module Mem : MEM =
struct
  exception Not_allocated
  exception Not_initialized
  type 'a content = V of 'a | U
  type 'a t = M of Loc.t * 'a content list
  let empty = M (Loc.base,[])

  let rec replace_nth = fun l n c -> 
    match l with
    | h::t -> if n = 1 then c :: t else h :: (replace_nth t (n - 1) c)
    | [] -> raise Not_allocated

  let load (M (boundary,storage)) loc =
    match (List.nth storage ((Loc.diff boundary loc) - 1)) with
    | V v -> v 
    | U -> raise Not_initialized

  let store (M (boundary,storage)) loc content =
    M (boundary, replace_nth storage (Loc.diff boundary loc) (V content))

  let alloc (M (boundary,storage)) = 
    (boundary, M (Loc.increase boundary 1, U :: storage))
end

(* Environment Signature *)
module type ENV =
sig
  type ('a, 'b) t
  exception Not_bound
  val empty : ('a, 'b) t (* get empty environment *)
  val lookup : ('a, 'b) t -> 'a -> 'b (* lookup environment : Env.lookup env key => content *)
  val bind : ('a, 'b) t -> 'a -> 'b -> ('a, 'b) t  (* id binding : Env.bind env key content => env'*)
end

(* Environment Implementation *) (* Env is a function *)
module Env : ENV=
struct
  exception Not_bound
  type ('a, 'b) t = E of ('a -> 'b)
  let empty = E (fun x -> raise Not_bound)
  let lookup (E (env)) id = env id
  let bind (E (env)) id loc = E (fun x -> if x = id then loc else env x)
end

(*
 * K- Interpreter
 *)
module type KMINUS =
sig
  exception Error of string
  type id = string
  type exp =
  | NUM of int | TRUE | FALSE | UNIT
  | VAR of id
  | ADD of exp * exp
  | SUB of exp * exp
  | MUL of exp * exp
  | DIV of exp * exp
  | EQUAL of exp * exp
  | LESS of exp * exp
  | NOT of exp
  | SEQ of exp * exp            (* sequence *)
  | IF of exp * exp * exp       (* if-then-else *)
  | WHILE of exp * exp          (* while loop *)
  | LETV of id * exp * exp      (* variable binding *)
  | LETF of id * id list * exp * exp (* procedure binding *)
  | CALLV of id * exp list      (* call by value *)
  | CALLR of id * id list       (* call by referenece *)
  | RECORD of (id * exp) list   (* record construction *)
  | FIELD of exp * id           (* access record field *)
  | ASSIGN of id * exp          (* assgin to variable *)
  | ASSIGNF of exp * id * exp   (* assign to record field *)
  | READ of id
  | WRITE of exp
    
  type program = exp
  type memory
  type env
  type value =
  | Num of int
  | Bool of bool
  | Unit
  | Record of (id -> Loc.t)
  val emptyMemory : memory
  val emptyEnv : env
  val run : memory * env * program -> value
end

module K : KMINUS =
struct
  exception Error of string

  type id = string
  type exp =
  | NUM of int | TRUE | FALSE | UNIT
  | VAR of id
  | ADD of exp * exp
  | SUB of exp * exp
  | MUL of exp * exp
  | DIV of exp * exp
  | EQUAL of exp * exp
  | LESS of exp * exp
  | NOT of exp
  | SEQ of exp * exp            (* sequence *)
  | IF of exp * exp * exp       (* if-then-else *)
  | WHILE of exp * exp          (* while loop *)
  | LETV of id * exp * exp      (* variable binding *)
  | LETF of id * id list * exp * exp (* procedure binding *)
  | CALLV of id * exp list      (* call by value *)
  | CALLR of id * id list       (* call by referenece *)
  | RECORD of (id * exp) list   (* record construction *)
  | FIELD of exp * id           (* access record field *)
  | ASSIGN of id * exp          (* assgin to variable *)
  | ASSIGNF of exp * id * exp   (* assign to record field *)
  | READ of id
  | WRITE of exp

  type program = exp

  type value =
  | Num of int
  | Bool of bool
  | Unit
  | Record of (id -> Loc.t)
    
  type memory = value Mem.t
  type env = (id, env_entry) Env.t
  and  env_entry = Addr of Loc.t | Proc of id list * exp * env

  let emptyMemory = Mem.empty
  let emptyEnv = Env.empty

  let value_int v =
    match v with
    | Num n -> n
    | _ -> raise (Error "TypeError : not int")

  let value_bool v =
    match v with
    | Bool b -> b
    | _ -> raise (Error "TypeError : not bool")

  let value_unit v =
      match v with
      | Unit -> ()
      | _ -> raise (Error "TypeError : not unit")

  let value_record v =
      match v with
      | Record r -> r
      | _ -> raise (Error "TypeError : not record")
  
  let lookup_env_loc e x =
    try
      (match Env.lookup e x with
      | Addr l -> l
      | Proc _ -> raise (Error "TypeError : not addr")) 
    with Env.Not_bound -> raise (Error "Unbound")

  let lookup_env_proc e f =
    try
      (match Env.lookup e f with
      | Addr _ -> raise (Error "TypeError : not proc") 
      | Proc (id, exp, env) -> (id, exp, env))
    with Env.Not_bound -> raise (Error "Unbound")

(* READ, WRITE, LETV, ASSIGN are by TA *)
  let rec eval mem env e =
    match e with
	 | NUM n -> (Num n, mem)
   | TRUE -> (Bool true, mem)
	 | FALSE -> (Bool false, mem)
	 | UNIT -> (Unit, mem)
	 | VAR x -> let v = (Mem.load mem (lookup_env_loc env x)) in (v, mem)
	 | ADD (e1, e2) -> 
	   let (n1, mem') = eval mem env e1 in
	   let (n2, mem'') = eval mem' env e2 in
	 	 (Num ((value_int n1) + (value_int n2)), mem'')
	 | SUB (e1, e2) ->
	   let (n1, mem') = eval mem env e1 in
		 let (n2, mem'') = eval mem' env e2 in
		 (Num ((value_int n1) - (value_int n2)), mem'')
	 | MUL (e1, e2) ->
	   let (n1, mem') = eval mem env e1 in
		 let (n2, mem'') = eval mem' env e2 in
		 (Num ((value_int n1) * (value_int n2)), mem'')
	 | DIV (e1, e2) ->
	   let (n1, mem') = eval mem env e1 in
		 let (n2, mem'') = eval mem' env e2 in
		 (Num ((value_int n1) / (value_int n2)), mem'')
	 | EQUAL (e1, e2) ->
	   let (v1, mem') = eval mem env e1 in
		 let (v2, mem'') = eval mem' env e2 in
		 let v = match v1, v2 with 
		   | Unit, Unit -> true
		   | Num x, Num y -> x=y
		   | Bool x, Bool y -> x=y
		   | _, _ -> false in
		 (Bool v, mem'')
    | LESS (e1, e2) ->
	    let (n1, mem') = eval mem env e1 in
		  let (n2, mem'') = eval mem' env e2 in
		  let v = (value_int n1) < (value_int n2) in
		  (Bool v, mem'')
	 | NOT e1 -> 
     let (b, mem') = eval mem env e1 in 
     (Bool (not (value_bool b)), mem')
	 | SEQ (e1, e2) ->
	   let (v1, mem') = eval mem env e1 in
		 let (v2, mem'') = eval mem' env e2 in
		 (v2, mem'')
	 | IF (e1, e2, e3) ->
	   let (b, mem') = eval mem env e1 in
		 if (value_bool b) 
		   then eval mem' env e2
		   else eval mem' env e3
    | WHILE (e1, e2) ->
	    let (b, mem') = eval mem env e1 in
		  if (value_bool b)
			  then let (v1, mem1) = eval mem' env e2 in
				  eval mem1 env (WHILE (e1, e2))
		    else (Unit, mem') 
    | LETV (x, e1, e2) ->
		  let (v, mem') = eval mem env e1 in
			let (l, memA) = Mem.alloc mem' in
			eval (Mem.store memA l v) (Env.bind env x (Addr l)) e2
		| LETF (f, vars, e1, e2) ->
		  eval mem (Env.bind env f (Proc (vars, e1, env))) e2  
		| CALLV (f, exps) ->
      let rec building_vals (es:exp list) (vs:value list) (m:memory):(value list * (value Mem.t)) =
			(match es with
			| [] -> (vs, m) 
			| h::t -> let (newV, newM) = eval m env h in 
			          building_vals t (vs@[newV]) newM) 
			in let (vals, memN) = building_vals exps [] mem in
			let (vars, e', env') = lookup_env_proc env f in
			let rec building_envs_mems (varl: id list) (vall: value list) (ms: memory) (es: env) :memory * env = 
			(match varl with
			| [] -> (ms, es)
			| varh::vart -> (match vall with [] -> raise (Error "error! call by value")
			                               | valh::valt -> let (newL, memA) = Mem.alloc ms in
			                building_envs_mems vart valt (Mem.store ms newL valh) (Env.bind es varh (Addr newL))))
(* 
			                if vall = [] then raise (Error "error! call by value") 
			                else let (valh::valt) = vall in 
			                let (newL, memA) = Mem.alloc ms in
			                building_envs_mems vart valt (Mem.store ms newL valh) (Env.bind es varh newL))*)
			in let (memfinal, envfinal) = building_envs_mems vars vals memN env' in
			eval memfinal (Env.bind envfinal f (Proc (vars, e', env'))) e'
	  | CALLR (f, refs) ->
		  let (vars, e, env') = lookup_env_proc env f in
			let rec building_envs (vs: id list) (rs: id list) (es: env) : env =
			(match vs with
			| [] -> es 
			| varh::vart -> (match rs with 
			                | [] -> raise (Error "error! call by reference") 
											| refh::reft -> building_envs vart reft (Env.bind es varh (Addr (lookup_env_loc env refh)))))
(*
			if rs = [] then raise (Error "error! call by reference")
			                else let (refh::reft) = rs in
											building_envs vart reft (Env.bind es varh (Addr (lookup_env_loc env refh)))) *)
			in let envfinal = building_envs vars refs env' in
			eval mem (Env.bind envfinal f (Proc (vars, e, env'))) e
		| RECORD l -> 
		  let exps = List.map (fun (a,b) -> b) l in
      let rec building_vals (es:exp list) (vs:value list) (m:memory):(value list * (value Mem.t)) =
			(match es with
			| [] -> (vs, m) 
			| h::t -> let (newV, newM) = eval m env h in 
			          building_vals t (vs@[newV]) newM)
			in let (vals, memN) = building_vals exps [] mem in (* vals = [v1;v2;...;vn] *)
		  let rec building_record (v:value) (ll:(id*exp) list) (vl:value list) (m:memory) : value * memory =
			(match v with
		  | Record r ->
				(match ll with
				| [] -> (Record r, m)
				| (idh, exph)::lt -> (match vl with
														 | [] -> raise (Error "error! building_record")
														 | vh::vt -> 
															 let (newL, m') = Mem.alloc m in
															 let newR = (fun x -> if x = idh then newL else r x) in
															 building_record (Record newR) lt vt (Mem.store m' newL vh)))

	(*			if vl = [] then raise (Error "ERROR! building_record")
														 else let (newL, m') = Mem.alloc m in 
														 let newR = (fun x -> if x = idh then newL else r x) in 
														 building_record (Record newR) lt (List.tl vl) m') *)
			| _ -> raise (Error "error! record"))
			in if l = [] then (Unit, mem) else building_record (Record (fun x -> Loc.base)) l vals memN
			
		| FIELD (e, x) -> 
		  let (v, m') = eval mem env e in 
			(match v with 
			 | Record r -> (Mem.load m' (r x), m')
			 | _ -> raise (Error "error! field"))
	  | ASSIGNF (e1, x, e2) -> 
      let (v, m1) = eval mem env e1 in  
			(match v with
			 | Record r -> let (v, m2) = eval m1 env e2 in (v, Mem.store m2 (r x) v)
			 | _ -> raise (Error "error! field assign"))

			
	 (*| CALLV (f, exps) ->
	   let (vars, e', env') = Env.lookup env f in
		let rec eval_k k : (value * mem) = if k = 1 then eval mem env (List.nth exps 1) 
		                                            else let (vk, memk_1) = eval_k (k-1) in eval memk_1 env (List.nth exps k) in
		let (vals, mems) = let rec evalbuild k = if k = 0 then ([], [])
		                                                  else evalbuild (k-1)
		match exps with
		| [] ->
		| h::t ->  
	 | CALLR (x, vars) 
	 | RECORD l -> 
	   let rec vk_memk k = if k = 0 then ([], [], []) else
	   match l with 
		| [] -> (Unit, mem)
		| (x, e1)::(y, e2)::[] -> 
		  let (v1, mem1) = eval mem env e1 in
		  let (v2, mem2) = eval mem1 env e2 in
		  let (l1, mem


	   | (xk, ek)::t -> let r id loc = fun x -> if x = id then loc else r x in Record r
      
	 | FIELD (e1, x) ->
	   let (r, mem') = eval mem env e1 in ((Mem.load mem' ) mem')

	 | ASSIGNF (e1, x, e2) ->
	 	let (r, mem1) = eval mem env e1 in
		let (v, mem2) = eval mem1 env e2 in
		(v, (Mem.store *)
    | ASSIGN (x, e) ->
      let (v, mem') = eval mem env e in
      let l = lookup_env_loc env x in
      (v, Mem.store mem' l v)
    | READ x -> 
      let v = Num (read_int()) in
      let l = lookup_env_loc env x in
      (v, Mem.store mem l v)
    | WRITE e ->
      let (v, mem') = eval mem env e in
      let n = value_int v in
      let _ = print_endline (string_of_int n) in
      (v, mem')

  let run (mem, env, pgm) = 
    let (v, _ ) = eval mem env pgm in
    v
end
