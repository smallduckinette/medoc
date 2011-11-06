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

exception XBadData

type data = Binary | Text

module type TypeT =
sig
  type t
  val from_string : bool -> string -> t
  val to_string : t -> string
  val data : data
end

module Int =
struct
  type t = int
  let from_string _ = int_of_string
  let to_string = string_of_int
  let data = Text
end

module Bool =
struct
  type t = bool
  let from_string _ = bool_of_string
  let to_string = string_of_bool
  let data = Text
end

module String =
struct
  type t = string
  let from_string _ s = s
  let to_string s = s
  let data = Text
end

module Float =
struct
  type t = float
  let from_string _ = float_of_string
  let to_string = string_of_float
  let data = Text
end

module Option (Underlying : TypeT) =
struct
  type t = Underlying.t option
  let from_string b s = 
    if b 
    then 
      Some (Underlying.from_string b s) 
    else
      None
  let to_string t = match t with 
      None -> ""
    | Some e -> Underlying.to_string e
  let data = Underlying.data
end

module type TypeTupleT =
sig
  type t
  val to_stringlist : t -> data list * string list
  val from_stringlist : (int -> bool) -> (int -> string) -> t
end

module Empty =
struct
  type t = unit
  let to_stringlist _ = [], []
  let from_stringlist _ _ = ()
end

module Singleton =
  functor (T1: TypeT) -> 
struct
  type t = T1.t
  let to_stringlist t1 = [T1.data],[T1.to_string t1]
  let from_stringlist isnull getresult =
    try
      T1.from_string (isnull 0) (getresult 0)
    with
	_ -> raise XBadData
end

module Pair 
  (T1 : TypeT) 
  (T2 : TypeT) =
struct
  type t = T1.t * T2.t
  let to_stringlist (t1, t2) =
    [T1.data;
     T2.data],
    [T1.to_string t1;
     T2.to_string t2]
  let from_stringlist isnull getresult =
    try
      T1.from_string (isnull 0) (getresult 0),
      T2.from_string (isnull 1) (getresult 1)
    with
	_ -> raise XBadData
	  
end

module Nuplet3 
  (T1 : TypeT) 
  (T2 : TypeT)
  (T3 : TypeT) =
struct
  type t = T1.t * T2.t * T3.t
  let to_stringlist (t1, t2, t3) =
    [T1.data;
     T2.data;
     T3.data],
    [T1.to_string t1;
     T2.to_string t2;
     T3.to_string t3]	  
  let from_stringlist isnull getresult =
    try
      T1.from_string (isnull 0) (getresult 0),
      T2.from_string (isnull 1) (getresult 1),
      T3.from_string (isnull 2) (getresult 2)	  
    with
	_ -> raise XBadData
end

module Nuplet4 
  (T1 : TypeT) 
  (T2 : TypeT)
  (T3 : TypeT)
  (T4 : TypeT) =
struct
  type t = T1.t * T2.t * T3.t * T4.t
  let to_stringlist (t1, t2, t3, t4) =
    [T1.data;
     T2.data;
     T3.data;
     T4.data], 
    [T1.to_string t1;
     T2.to_string t2;
     T3.to_string t3;
     T4.to_string t4]	  
  let from_stringlist isnull getresult =
    try
      T1.from_string (isnull 0) (getresult 0),
      T2.from_string (isnull 1) (getresult 1),
      T3.from_string (isnull 2) (getresult 2),	  
      T4.from_string (isnull 3) (getresult 3)
    with
	_ -> raise XBadData
end

module Nuplet5
  (T1 : TypeT) 
  (T2 : TypeT)
  (T3 : TypeT) 
  (T4 : TypeT)
  (T5 : TypeT) =
struct
  type t = T1.t * T2.t * T3.t * T4.t * T5.t
  let to_stringlist (t1, t2, t3, t4, t5) =
    [T1.data;
     T2.data;
     T3.data;
     T4.data;
     T5.data],
    [T1.to_string t1;
     T2.to_string t2;
     T3.to_string t3;
     T4.to_string t4;
     T5.to_string t5]
  let from_stringlist isnull getresult =
    try
      T1.from_string (isnull 0) (getresult 0),
      T2.from_string (isnull 1) (getresult 1),
      T3.from_string (isnull 2) (getresult 2),	  
      T4.from_string (isnull 3) (getresult 3),
      T5.from_string (isnull 4) (getresult 4)
    with
	_ -> raise XBadData
end

module Nuplet6
  (T1 : TypeT) 
  (T2 : TypeT)
  (T3 : TypeT) 
  (T4 : TypeT)
  (T5 : TypeT)
  (T6 : TypeT) =
struct
  type t = T1.t * T2.t * T3.t * T4.t * T5.t * T6.t
  let to_stringlist (t1, t2, t3, t4, t5, t6) =
    [T1.data;
     T2.data;
     T3.data;
     T4.data;
     T5.data;
     T6.data],
    [T1.to_string t1;
     T2.to_string t2;
     T3.to_string t3;
     T4.to_string t4;
     T5.to_string t5;
     T6.to_string t6]
  let from_stringlist isnull getresult =
    try
      T1.from_string (isnull 0) (getresult 0),
      T2.from_string (isnull 1) (getresult 1),
      T3.from_string (isnull 2) (getresult 2),	  
      T4.from_string (isnull 3) (getresult 3),
      T5.from_string (isnull 4) (getresult 4),
      T6.from_string (isnull 5) (getresult 5)
    with
	_ -> raise XBadData
end

module Nuplet7
  (T1 : TypeT) 
  (T2 : TypeT)
  (T3 : TypeT) 
  (T4 : TypeT)
  (T5 : TypeT)
  (T6 : TypeT)
  (T7 : TypeT) =
struct
  type t = T1.t * T2.t * T3.t * T4.t * T5.t * T6.t * T7.t
  let to_stringlist (t1, t2, t3, t4, t5, t6, t7) =
    [T1.data;
     T2.data;
     T3.data;
     T4.data;
     T5.data;
     T6.data;
     T7.data],
    [T1.to_string t1;
     T2.to_string t2;
     T3.to_string t3;
     T4.to_string t4;
     T5.to_string t5;
     T6.to_string t6;
     T7.to_string t7]
  let from_stringlist isnull getresult =
    try
      T1.from_string (isnull 0) (getresult 0),
      T2.from_string (isnull 1) (getresult 1),
      T3.from_string (isnull 2) (getresult 2),	  
      T4.from_string (isnull 3) (getresult 3),
      T5.from_string (isnull 4) (getresult 4),
      T6.from_string (isnull 5) (getresult 5),
      T7.from_string (isnull 6) (getresult 6)
    with
	_ -> raise XBadData
end
