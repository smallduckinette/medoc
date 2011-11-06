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

open Eliom_parameters

let medoc_empty = Eliom_services.new_service ["latest"] unit ()
let medoc_search = Eliom_services.new_service ["search"] (string "term") ()
let medoc_view = Eliom_services.new_service ["view"] (int "docid") ()
let medoc_view_tag = Eliom_services.new_service ["tag"] (string "tag") ()
let medoc_login = Eliom_services.new_post_service ~fallback:medoc_empty ~post_params:(string "login" ** string "password") ()
let medoc_delete_doc = Eliom_services.new_service ["delete"](int "docId") ()
let medoc_delete_tag = Eliom_services.new_service ["deleteTag"](int "docId" ** string "tag") ()
let medoc_add_tag = Eliom_services.new_service ["addTag"](int "docId" ** string "tag") ()
