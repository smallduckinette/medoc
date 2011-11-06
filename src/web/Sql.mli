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
