
type t

val init : string -> int -> string -> string -> string -> int -> t

val run : t -> (Postgresql.connection -> 'a) -> 'a
