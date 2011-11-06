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

type document_t = int * string * CalendarLib.Date.t * int list

val search : DbPool.t -> string -> string -> document_t list

val search_by_tag: DbPool.t -> string -> string -> document_t list

val search_by_latest : DbPool.t -> int -> string -> document_t list

val document : DbPool.t -> int -> string -> document_t

val login : DbPool.t -> string -> string -> bool

val create_document : DbPool.t -> string -> CalendarLib.Date.t -> string -> unit

val delete_document : DbPool.t -> int -> unit

val get_all_tags : DbPool.t -> (string * int) list

type document_tag = TagIn | TagOut

val get_tags_for_document : DbPool.t -> document_tag -> int -> string list

val delete_tag : DbPool.t -> int -> string -> unit

val add_tag : DbPool.t -> int -> string -> unit

val get_stats : DbPool.t -> (int * int * string)
