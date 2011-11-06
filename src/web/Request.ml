module type RequestPropertiesT =
sig
  val request_name : string
end

module Request
  (Input : Types.TypeTupleT)
  (Output : Types.TypeTupleT)
  (Properties : RequestPropertiesT)
  =
struct
  let make_request input =
    let string_input = snd (Input.to_stringlist input) in
    let parameters =
      "("
      ^ (String.concat ", " string_input)
      ^ ")"
    in
      Properties.request_name ^ parameters

  let translate_multi_response result = 
    let array = 
      Array.init 
	result#ntuples
	(fun i -> Output.from_stringlist
	   (result#getisnull i)
	   (result#getvalue i))
    in
      Array.to_list array

  let translate_mono_response result = 
    if result#ntuples = 1 then
      Output.from_stringlist
	(result#getisnull 0)
	(result#getvalue 0)
    else
      raise Types.XBadData
	
  let do_request dbconn log input = 
    let t1 = Unix.gettimeofday () in
    let input_data = Input.to_stringlist input in
    let result = Sql.exec_query dbconn Properties.request_name (snd input_data) (List.map (function Types.Text -> false | Types.Binary -> true) (fst input_data)) in
    let t2 = Unix.gettimeofday () in
      log 
	("Request \"" 
	 ^ Properties.request_name
	 ^ "\" executed in " 
	 ^ (string_of_int (int_of_float((t2 -. t1)*.1000.))) 
	 ^ " ms");
      result
	

  let request dbconn log input = 
    translate_multi_response (do_request dbconn log input)

  let single_request dbconn log input = 
    translate_mono_response (do_request dbconn log input)

end

module CopyTo (Input : Types.TypeTupleT) =
struct
  let copy c log table input = 
    let t1 = Unix.gettimeofday () in
      Sql.copy_in c table (List.map (fun e -> snd(Input.to_stringlist e)) input);
      let t2 = Unix.gettimeofday () in
	log 
	  ("Copy to table \"" 
	   ^ table
	   ^ "\" executed in " 
	   ^ (string_of_int (int_of_float((t2 -. t1)*.1000.))) 
	   ^ " ms")

end

module CopyFrom (Output : Types.TypeTupleT) =
struct
  let copy c log table = 
    let t1 = Unix.gettimeofday () in
    let output = Sql.copy_out c table in		
    let result = 
      List.map 
	(fun e -> 
	   let tuple = Array.of_list e in
	     Output.from_stringlist
	       (fun _ -> true)
	       (Array.get tuple))
	output in
    let t2 = Unix.gettimeofday () in
      log 
	("Copy from table \"" 
	 ^ table
	 ^ "\" executed in " 
	 ^ (string_of_int (int_of_float((t2 -. t1)*.1000.))) 
	 ^ " ms");
      result
	
end
