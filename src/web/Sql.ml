type t = Postgresql.connection

exception SqlError of string

(* Split a string into its constituens *)
let split c s =
  if s = "" then [] else
	let accum = ref [] in
	let len = String.length s in
	let i = ref (len-1) in
	  try
		while true do
		  let nextc = String.rindex_from s !i c in
		  let s = if nextc < !i then String.sub s (nextc+1) (!i-nextc) else "" in
			accum := s::!accum;
			i := nextc-1;
		done;
		!accum
	  with
		  Not_found ->
			(String.sub s 0 (!i+1))::!accum 

(* Connects to the database *)
let connect host port dbname user password =
  let stringconnect = "host="^host^" port="^(string_of_int port)^" dbname="^dbname^" user="^user ^ " password=" ^ password in
  new Postgresql.connection ~conninfo:stringconnect ()


(* Disconnects from the database *)
let disconnect (c:t) =
  c#finish


(* Loads a table from a query *)
let exec_query (c:t) request params binary =
  try
	c#exec ~expect:[Postgresql.Tuples_ok] ~params:(Array.of_list params) ~binary_params:(Array.of_list binary) request
  with
	  Postgresql.Error e -> raise (SqlError (Postgresql.string_of_error e))
		
(* Execute a simple command *)
let exec_command (c:t) command = 
  try
	ignore(c#exec ~expect:[Postgresql.Command_ok] command)
  with
    Postgresql.Error e -> raise (SqlError (Postgresql.string_of_error e))

let exec_query_new (c:t) query = 
  try
	let result = c#exec ~expect:[Postgresql.Tuples_ok] query in
	  result#get_all
  with
	  Postgresql.Error e -> raise (SqlError (Postgresql.string_of_error e))
  

let copy_out (c:t) table = 
  try
	ignore(c#exec 
			 ~expect:[Postgresql.Copy_out] 
			 ("copy " ^ table ^ " to stdout"));
	let field_list = ref [] in
	let input str = 
	  field_list := (split '\t' str)::(!field_list)
	in
	  c#copy_out input;
	  !field_list
  with
	  Postgresql.Error e -> raise (SqlError (Postgresql.string_of_error e))

let copy_in (c:t) table data =
  try
	ignore(c#exec 
			 ~expect:[Postgresql.Copy_in] 
			 ("copy " ^ table ^ " from stdin"));
	List.iter
	  (fun fields -> c#putline ((String.concat "\t" fields)^"\n"))
	  data;
	c#endcopy
  with
	  Postgresql.Error e -> raise (SqlError (Postgresql.string_of_error e))  


module SqlBool =
struct
  type t = bool
  let from_string _ b = b = "t"
  let to_string b = if b then "t" else "f"
  let data = Types.Text
end

module SqlDate =
struct
  type t = CalendarLib.Date.t
  let from_string _ = CalendarLib.Printer.DatePrinter.from_string
  let to_string s = "'" ^ (CalendarLib.Printer.DatePrinter.to_string s) ^ "'"
  let data = Types.Text
end

let trim_milliseconds time = 
  let r = Str.regexp "[^\\.]*" in
	if Str.string_match r time 0 then
	  Str.matched_string time
	else
	  time

module SqlTime =
struct
  type t = CalendarLib.Time.t
  let from_string _ s = CalendarLib.Printer.TimePrinter.from_string(trim_milliseconds s)
  let to_string s = "'" ^ (CalendarLib.Printer.TimePrinter.to_string s) ^ "'"
  let data = Types.Text
end

module SqlDateTime =
struct
  type t = CalendarLib.Calendar.t
  let from_string _ s = CalendarLib.Printer.CalendarPrinter.from_string(trim_milliseconds s)
  let to_string s = "'" ^ (CalendarLib.Printer.CalendarPrinter.to_string s) ^ "'"
  let data = Types.Text
end

module BinString =
struct
  type t = string
  let from_string _ s = s
  let to_string s = s
  let data = Types.Binary
end
