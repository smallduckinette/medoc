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

type t = Condition.t * Mutex.t * Postgresql.connection Queue.t
  
let in_lock mutex f input = 
  Mutex.lock mutex;
  try
    let r = f mutex input in
      Mutex.unlock mutex;
      r
  with
      e -> (Mutex.unlock mutex; raise e)

let init host port dbname user password n = 
  let queue = Queue.create () in
    for i = 0 to n - 1 do
      Queue.push (Sql.connect host port dbname user password) queue;
    done;
    (Condition.create (), Mutex.create (), queue)


let get_connection signal mutex queue = 
  in_lock
    mutex
    (
      fun mutex (signal, queue) -> 
	while Queue.is_empty queue do
	  Condition.wait signal mutex;
	done;
	Queue.pop queue
    )
    (signal, queue)

let push_connection signal mutex queue dbConn = 
  in_lock
    mutex
    (
      fun _ (signal, queue, dbConn) -> Queue.push dbConn queue; Condition.signal signal
    )
    (signal, queue, dbConn)
    
let run (signal, mutex, queue) f =
  let dbConn = get_connection signal mutex queue in
    try
      let r = f dbConn in
	push_connection signal mutex queue dbConn;
	r
    with
	e -> (push_connection signal mutex queue dbConn; raise e)
