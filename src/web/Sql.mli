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

exception SqlError of string

val connect : string -> int -> string -> string -> string -> Postgresql.connection

val disconnect : Postgresql.connection -> unit

val exec_query : Postgresql.connection -> string -> string list -> bool list -> Postgresql.result

val exec_command : Postgresql.connection -> string -> unit

val copy_out : Postgresql.connection -> string -> string list list

val copy_in : Postgresql.connection -> string -> string list list -> unit

module SqlBool : Types.TypeT with type t = bool

module SqlDate : Types.TypeT with type t = CalendarLib.Date.t

module SqlTime : Types.TypeT with type t = CalendarLib.Time.t

module SqlDateTime : Types.TypeT with type t = CalendarLib.Calendar.t

module BinString : Types.TypeT with type t = string
