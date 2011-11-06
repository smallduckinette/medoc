(** This module provides request definitions *)

(** The request properties type *)
module type RequestPropertiesT =
sig
  val request_name : string
end

(** The request module *)
module Request
  (Input : Types.TypeTupleT)
  (Output : Types.TypeTupleT)
  (Properties : RequestPropertiesT)
  :
sig
  (** [request keeperid logger input f_output] executes the request with the [input] parameters. Errors and informations are dumped via [logger]. *)
  val request : 
    Postgresql.connection -> 
    (string -> unit) -> 
    Input.t ->
    Output.t list

  (** Identical to request, except that the output is Output.t instead of Output.t list. This is useful for requests that garantee one and only one returned record. *)
  val single_request : 
    Postgresql.connection -> 
    (string -> unit) -> 
    Input.t ->
    Output.t
end

module CopyTo (Input : Types.TypeTupleT) :
sig
  val copy : 
    Postgresql.connection -> 
    (string -> unit) -> 
    string -> 
    Input.t list -> 
    unit
end

module CopyFrom (Output : Types.TypeTupleT) :
sig
  val copy : 
    Postgresql.connection ->
    (string -> unit) -> 
    string -> 
    Output.t list
end
