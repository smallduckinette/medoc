Lwt_preemptive.init 5 20 prerr_endline 

let dbPool = DbPool.init "localhost" 5432 "medoc" "medoc_user" "medoc_user" 5

let session_table:((string * string) Eliom_sessions.volatile_table) = Eliom_sessions.create_volatile_table ()
