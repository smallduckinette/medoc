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
