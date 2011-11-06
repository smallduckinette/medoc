
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
