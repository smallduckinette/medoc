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

(** This module defines generic types into modules, and ways to make type tuples *)

(** This exception is thrown when the data does not correspond *)
exception XBadData
  
type data = Binary | Text
    
(** Base type signature *)
module type TypeT =
sig
  type t
  val from_string : bool -> string -> t
  val to_string : t -> string
  val data : data
end
  
(** Type Int *)
module Int : TypeT with type t = int

(** Type Bool *)
module Bool : TypeT with type t = bool

(** Type String *)
module String : TypeT with type t = string

(** Type Float *)
module Float : TypeT with type t = float

(** Type Option. Takes the underlying type as functor parameter *)
module Option (Underlying : TypeT) : TypeT with type t = Underlying.t option

(** Base type tuple signature *)
module type TypeTupleT =
sig
  type t
  val to_stringlist : t -> data list * string list
  val from_stringlist : (int -> bool) -> (int -> string) -> t
end

(** Empty *)
module Empty : TypeTupleT with type t = unit

(** Singleton *)
module Singleton
  (T1: TypeT) : TypeTupleT with type t = T1.t
  
(** Pair *)
module Pair 
  (T1 : TypeT) 
  (T2 : TypeT) : TypeTupleT with type t = T1.t * T2.t

(** Triplet *)
module Nuplet3 
  (T1 : TypeT) 
  (T2 : TypeT)
  (T3 : TypeT): TypeTupleT with type t = T1.t * T2.t * T3.t

(** Quadruplet *)
module Nuplet4
  (T1 : TypeT) 
  (T2 : TypeT)
  (T3 : TypeT)
  (T4 : TypeT): TypeTupleT with type t = T1.t * T2.t * T3.t * T4.t

(** Quintuplet *)
module Nuplet5
  (T1 : TypeT) 
  (T2 : TypeT)
  (T3 : TypeT) 
  (T4 : TypeT)
  (T5 : TypeT): TypeTupleT with type t = T1.t * T2.t * T3.t * T4.t * T5.t

(** Hexatuplet *)
module Nuplet6
  (T1 : TypeT)
  (T2 : TypeT)
  (T3 : TypeT)
  (T4 : TypeT)
  (T5 : TypeT)
  (T6 : TypeT): TypeTupleT with type t = T1.t * T2.t * T3.t * T4.t * T5.t * T6.t

(** Heptuplet *)
module Nuplet7
  (T1 : TypeT) 
  (T2 : TypeT)
  (T3 : TypeT) 
  (T4 : TypeT)
  (T5 : TypeT)
  (T6 : TypeT)
  (T7 : TypeT): TypeTupleT with type t = T1.t * T2.t * T3.t * T4.t * T5.t * T6.t * T7.t
