

let convert_to_png file = 
  let i_file = Unix.openfile file [Unix.O_RDONLY] 0 in
  let pid0 = Unix.create_process "convert" [|"convert"; "-"; "png:-"|] i_file Unix.stdout Unix.stderr in
    ignore(Unix.waitpid [] pid0)

let gocr file = 
  let i_file = Unix.openfile file [Unix.O_RDONLY] 0 in
  let gocr_in, jpg_out = Unix.pipe() in
  let result, gocr_out = Unix.pipe() in
  let pid0 = 
    Unix.create_process 
      "jpegtopnm" 
      [|"jpegtopnm"|] i_file jpg_out Unix.stderr in
    Unix.close jpg_out;
  let pid1 = 
    Unix.create_process 
      "gocr" 
      [|"gocr"; "-"|] gocr_in gocr_out Unix.stderr in
    Unix.close gocr_out;
    ignore(Unix.waitpid [] pid0);
    ignore(Unix.waitpid [] pid1);
    let buffer = Buffer.create 4096 in
    let read_buffer = String.create 512 in
    let r = ref true in
      while !r do
	let len = Unix.read result read_buffer 0 512 in
	  if len > 0 then
	    Buffer.add_substring buffer read_buffer 0 len
	  else
	    r := false
      done;
      Buffer.contents buffer

let process dbPool docid file = 
  prerr_endline ("Processing " ^ (Eliom_sessions.get_original_filename file));
  

