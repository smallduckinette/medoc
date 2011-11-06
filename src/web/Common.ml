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

Lwt_preemptive.init 5 20 prerr_endline 

let dbPool = DbPool.init "localhost" 5432 "medoc" "medoc_user" "medoc_user" 5

let session_table:((string * string) Eliom_sessions.volatile_table) = Eliom_sessions.create_volatile_table ()
