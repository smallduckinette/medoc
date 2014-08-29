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

open Eliom_parameter

let medoc_empty = Eliom_service.Http.service ~path:["latest"] ~get_params:unit ()
let medoc_search = Eliom_service.Http.service  ~path:["search"] ~get_params:(string "term") ()
let medoc_view = Eliom_service.Http.service  ~path:["view"] ~get_params:(int "docid") ()
let medoc_view_tag = Eliom_service.Http.service ["tag"] (string "tag") ()
let medoc_login = Eliom_service.Http.post_service ~fallback:medoc_empty ~post_params:(string "login" ** string "password") ()
let medoc_delete_doc = Eliom_service.Http.service ~path:["delete"] ~get_params:(int "docId") ()
let medoc_delete_tag = Eliom_service.Http.service ~path:["deleteTag"] ~get_params:(int "docId" ** string "tag") ()
let medoc_add_tag = Eliom_service.Http.service ~path:["addTag"] ~get_params:(int "docId" ** string "tag") ()
  
let login_info = Eliom_reference.Volatile.eref Eliom_common.default_session_scope (None:((string * string) option))

