(*
  Copyright 2010 small_duck
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*)

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
  prerr_endline ("Processing " ^ (Eliom_request_info.get_original_filename file));
  

